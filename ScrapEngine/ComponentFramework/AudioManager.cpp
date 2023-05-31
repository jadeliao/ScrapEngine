#include "AudioManager.h"

AudioManager* AudioManager::instance_(nullptr);

AudioManager* AudioManager::getInstance(){
	if (!instance_) {
		instance_ = new AudioManager();
	}
	return instance_;
}

bool AudioManager::OnCreate(){

	engine = createIrrKlangDevice();
	
    return true;
}

void AudioManager::OnDestroy(){
	if (engine) delete engine;
}

void AudioManager::Update(){

	if (backgroundMusic) {
		backgroundMusic->setVolume(masterVol * bgmVol);
	}

}

void AudioManager::PlayBGM(const char* fileName_){

	if (backgroundMusic) {
		delete backgroundMusic;
		backgroundMusic = nullptr;
	}
	//Make the isound with sound being paused
	backgroundMusic = engine->play2D(fileName_, true, true, true);
	//Set the volume, then start the sound
	backgroundMusic->setVolume(masterVol * bgmVol);
	backgroundMusic->setIsPaused(false);
}

void AudioManager::PlaySoundEffects(const char* fileName_, bool faded){

	ISound* sound_ = engine->play2D(fileName_, false, true, true);
	
	//Check if the sound is created
	if (!sound_) return;
	//Set the volume and play the sound
	sound_->setVolume(masterVol * sfxVol);
	sound_->setIsPaused(false);

	if (faded) {
		//Fading should use detach thread
		std::thread faded_thread(&AudioManager::PlayFadedSound, this, sound_);
		faded_thread.detach();
	}

}

void AudioManager::RestartBGM(){
	backgroundMusic->setPlayPosition(0.0f);
}

void AudioManager::PlayFadedSound(ISound* sound_){
	if (sound_) {
		for (float volume = 1.0f; volume > 0.0f; volume -= 0.01f) {
			sound_->setVolume(volume * masterVol * sfxVol);
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		delete sound_;
	}

}

void AudioManager::fadeBGM(){
	for (float volume = 1.0f; volume > 0.0f; volume -= 0.1f) {
		backgroundMusic->setVolume(volume * masterVol * bgmVol);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	PauseBGM(true);
}

void AudioManager::PauseBGM(bool paused_){
	if (!backgroundMusic) return;
	backgroundMusic->setVolume(masterVol * bgmVol);
	backgroundMusic->setIsPaused(paused_);
}

void AudioManager::FadeBGM(){
	//Fading needs to be detached thread
	std::thread faded_thread(&AudioManager::fadeBGM, this);
	faded_thread.detach();
}
