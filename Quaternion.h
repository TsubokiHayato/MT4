#pragma once
#include "Matrix4x4.h"
#include "Novice.h"
#include "Vector3.h"
#include <cmath>
#include <iostream>
#include <string>
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
	Novice::ScreenPrintf(x + (int)kColumnWidth, y, "%.02f", quaternion.y);
	Novice::ScreenPrintf(x + (int)kColumnWidth * 2, y, "%.02f", quaternion.z);
	Novice::ScreenPrintf(x + (int)kColumnWidth * 3, y, "%.02f", quaternion.w);
	Novice::ScreenPrintf(x + (int)kColumnWidth * 4, y, "%s", label);
}

inline Quaternion MakeRotateAxisQuaternion(const Vector3& axis, float radian) {
	Quaternion result;
	float halfAngle = radian * 0.5f;
	float sinHalfAngle = sinf(halfAngle);

	Vector3 normalizedAxis = Normalize(axis);
	result.x = normalizedAxis.x * sinHalfAngle;
	result.y = normalizedAxis.y * sinHalfAngle;
	result.z = normalizedAxis.z * sinHalfAngle;
	result.w = cosf(halfAngle);
	return result;
}

inline Vector3 RotateQuaternion(const Vector3& v, const Quaternion& q) {
	Quaternion p = {v.x, v.y, v.z, 0.0f};
	Quaternion qInv = Inverse(q);
	Quaternion qv = Multiply(Multiply(q, p), qInv);
	Vector3 result = {qv.x, qv.y, qv.z};
	return result;
}

inline Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
	Matrix4x4 result;

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	result.m[0][0] = 1.0f - 2.0f * (yy + zz);
	result.m[0][1] = 2.0f * (xy + wz);
	result.m[0][2] = 2.0f * (xz - wy);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (xy - wz);
	result.m[1][1] = 1.0f - 2.0f * (xx + zz);
	result.m[1][2] = 2.0f * (yz + wx);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (xz + wy);
	result.m[2][1] = 2.0f * (yz - wx);
	result.m[2][2] = 1.0f - 2.0f * (xx + yy);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

inline Vector3 Transform(const Matrix4x4& matrix, const Vector3& vector) {
	Vector3 result;
	result.x = matrix.m[0][0] * vector.x + matrix.m[0][1] * vector.y + matrix.m[0][2] * vector.z + matrix.m[0][3];
	result.y = matrix.m[1][0] * vector.x + matrix.m[1][1] * vector.y + matrix.m[1][2] * vector.z + matrix.m[1][3];
	result.z = matrix.m[2][0] * vector.x + matrix.m[2][1] * vector.y + matrix.m[2][2] * vector.z + matrix.m[2][3];
	return result;
}

inline Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	Quaternion q2Copy = q2;
	float dot = q1.x * q2Copy.x + q1.y * q2Copy.y + q1.z * q2Copy.z + q1.w * q2Copy.w;
	if (dot < 0.0f) {
		q2Copy.x = -q2Copy.x;
		q2Copy.y = -q2Copy.y;
		q2Copy.z = -q2Copy.z;
		q2Copy.w = -q2Copy.w;
		dot = -dot;
	}
	const float kThreshold = 0.9995f;
	if (dot > kThreshold) {
		Quaternion result;
		result.x = q1.x + t * (q2Copy.x - q1.x);
		result.y = q1.y + t * (q2Copy.y - q1.y);
		result.z = q1.z + t * (q2Copy.z - q1.z);
		result.w = q1.w + t * (q2Copy.w - q1.w);
		return Normalize(result);
	}
	float theta = acosf(dot);
	float sinTheta = sinf(theta);
	float sinOneMinusTheta = sinf((1.0f - t) * theta);
	float sinTTheta = sinf(t * theta);
	Quaternion result;
	result.x = (q1.x * sinOneMinusTheta + q2Copy.x * sinTTheta) / sinTheta;
	result.y = (q1.y * sinOneMinusTheta + q2Copy.y * sinTTheta) / sinTheta;
	result.z = (q1.z * sinOneMinusTheta + q2Copy.z * sinTTheta) / sinTheta;
	result.w = (q1.w * sinOneMinusTheta + q2Copy.w * sinTTheta) / sinTheta;
	return result;
}