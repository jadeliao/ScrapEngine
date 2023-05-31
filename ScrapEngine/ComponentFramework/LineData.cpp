#include "LineData.h"

LineData::~LineData() {
	OnDestroy();
}


void LineData::OnDestroy() {
	isDestroyed = true;
}

void LineData::Update(const float deltaTime) {

	lifeTime -= deltaTime;
	if (lifeTime < 0.0f) {
		OnDestroy();
	}
}
