#pragma once
#include<exception>
#include"geom3.h"

const long double eps = 1e-12;

ld Point3::len2() const {
	return x*x + y*y + z*z;
}

Point3 Point3::inverse() const{
	return{ -x, -y, -z };
}

Point3 operator*(const Point3& a, const Point3& b) {
	return{ a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}

Point3 operator*(const ld a, const Point3& b) {
	return { b.x * a, b.y * a, b.z * a };
}

ld operator^(const Point3& a, const Point3& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Point3 operator+(const Point3& a, const Point3& b) {
	return{ a.x + b.x, a.y + b.y, a.z + b.z };
}

Point3 operator-(const Point3& a, const Point3& b) {
	return{ a.x - b.x, a.y - b.y, a.z - b.z };
}

bool isZero(const ld z) {
	return fabs(z) < eps;
}

ld sign(const ld z) {
	if (isZero(z))
		return 0;
	else
		return (z > 0) ? 1 : -1;
}

bool areEqual(const ld a, const ld b) {
	return sign(a - b) >= 0;
}

bool areEqual(const Point3& p1, const Point3& p2) {
	if (areEqual(p1.x, p2.x) && areEqual(p1.z, p2.z))
		return areEqual(p1.y, p2.y);
	else
		return false;
}

bool areCollinear(const Point3& p1, const Point3& p2) {
	return isZero((p1*p2).len2());
}

bool isOnLine(const Line& l, const Point3& p1) {
	return areCollinear(l.a - p1, l.b - p1);
}

bool isOnSegment(const Line& l, const Point3& p) {
	return isOnLine(l, p) && sign((l.a - p) ^ (l.b - p)) <= 0;
}

bool areLinesEqualOrPar(const Line& a, const Line& b) {
	return areCollinear(a.a - a.b, b.a - b.b);
}

Point3 findLineIntersection(const Line& a, const Line& b) {
	if (areLinesEqualOrPar(a, b))
		throw 5;
	ld t = sqrtl(((a.a - b.a)*(b.b - b.a)).len2() / ((a.b - a.a)*(b.b - b.a)).len2());
	Point3 tr = a.a + t*(a.b - a.a);
	if (isOnLine(b, tr))
		return tr;
	else
		return a.a - t*(a.b - a.a);
}


bool doesSegmentsInterceptLine(const Line& seg, const Line& l) {
	if (areLinesEqualOrPar(seg, l) && !isOnLine(l, seg.a))
		return false;
	return isOnSegment(seg, findLineIntersection(seg, l));
}

bool areSegmentsIntercept(const Line& s1, const Line& s2) {
	if (areLinesEqualOrPar(s1, s2))
		return isOnSegment(s1, s2.a) || isOnSegment(s1, s2.b) || isOnSegment(s2, s1.a);
	else
		return doesSegmentsInterceptLine(s1, s2) && doesSegmentsInterceptLine(s2, s1);
}

bool isZeroPoint(const Point3& p) {
	if (isZero(p.x) && isZero(p.y) && isZero(p.z))
		return true;
	else 
		return false;
}

bool isZeroRay(const Line& ray) {
	if (isZeroPoint(ray.a) && isZeroPoint(ray.b))
		return true;
	else 
		return false;
}

ld cosBetween(const Point3& s1, const Point3& s2) {
	ld scal = (s1^s2);
	return sign(scal) * scal * scal / (s1.len2()*s2.len2());
}

Point3 safeInverse(const Point3& p) {
	ld x = 1. / p.x;
	ld y = 1. / p.y;
	ld z = 1. / p.z;
	return { x, y, z };
}