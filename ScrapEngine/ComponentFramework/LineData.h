#ifndef LINEDATA_H
#define LINEDATA_H

#include "Vector.h"
#include "VMath.h"

using namespace MATH;

typedef unsigned int GLuint;

class LineData {
private:
	Vec3 start;
	Vec3 end;
	float lifeTime;
	bool isDestroyed;
	GLuint vao, vbo;
public:
	LineData() :start(Vec3(0.0f, 0.0f, 0.0f)), end(Vec3(0.0f, 0.0f, 0.0f)), lifeTime(0.0f), isDestroyed(false) {}
	LineData(Vec3 start_, Vec3 end_, float lifeTime_=1.0f):start(start_), end(end_),lifeTime(lifeTime_), isDestroyed(false) {}
	~LineData();
	void Update(const float deltaTime);
	void OnDestroy();
	Vec3 getStart() { return start; }
	Vec3 getEnd() { return end; }
	float getLifeTime() { return lifeTime; }
	bool getIsDestroyed() { return isDestroyed; }
	void setVao(GLuint vao_) { vao = vao_; }
	void setVbo(GLuint vbo_) { vbo = vbo_; }
	GLuint* getVao() { return &vao; }
	GLuint* getVbo() { return &vbo; }

};

#endif