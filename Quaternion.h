#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <iostream>
#include <string>
#include"Novice.h"
class Quaternion {

public:
	float x, y, z, w;
};

inline Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	Quaternion result;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	return result;
}

inline Quaternion IdentityMultiply() {
	Quaternion result;
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;
	return result;
}

inline Quaternion Normalize(const Quaternion& q) {
	float length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	Quaternion result;
	result.x = q.x / length;
	result.y = q.y / length;
	result.z = q.z / length;
	result.w = q.w / length;
	return result;
}

inline Quaternion Conjugate(const Quaternion& q) {
	Quaternion result;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w = q.w;
	return result;
}


inline float Norm(const Quaternion& q) { return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w); }
inline Quaternion Inverse(const Quaternion& q) {

	Quaternion conj = Conjugate(q);
	float norm = Norm(q);
	Quaternion result;
	result.x = conj.x / (norm * norm);
	result.y = conj.y / (norm * norm);
	result.z = conj.z / (norm * norm);
	result.w = conj.w / (norm * norm);
	return result;
}

inline float kColumnWidth = 60.0f;

inline void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label) {

	Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
	Novice::ScreenPrintf(x +(int) kColumnWidth, y, "%.02f", quaternion.y);
	Novice::ScreenPrintf(x +(int) kColumnWidth * 2, y, "%.02f", quaternion.z);
	Novice::ScreenPrintf(x +(int) kColumnWidth * 3, y, "%.02f", quaternion.w);
	Novice::ScreenPrintf(x +(int) kColumnWidth * 4, y, "%s", label);
}
