#ifndef UBOSTRUCT_H
#define UBOSTRUCT_H

#include "Matrix.h"

using namespace MATH;

#define LIGHT_NUM 3

struct GlobalLight {
	Vec4 position[LIGHT_NUM];
	Vec4 diffuse[LIGHT_NUM];
};

struct CameraMatrices {
	Matrix4 view;
	Matrix4 proj;
};

#endif
