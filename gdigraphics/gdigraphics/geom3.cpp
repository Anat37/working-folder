#pragma once
#include<exception>
#include"geom3.h"

const long double eps = 1e-10;

ld Point3::len2() const {
	return x*x + y*y + z*z;
}

Point3 operator*(Point3 a, Point3 b) {
	return{ a.y*b.z - a.z*b.y, a.x*b.z - a.z*b.x, a.x*b.y - a.y*b.x };
}

Point3 operator*(ld a, Point3 b) {
	return { b.x * a, b.y * a, b.z * a };
}

ld operator^(Point3 a, Point3 b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Point3 operator+(Point3 a, Point3 b) {
	return{ a.x + b.x, a.y + b.y, a.z + b.z };
}

Point3 operator-(Point3 a, Point3 b) {
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

bool areEqual(ld a, ld b) {
	return sign(a - b) >= 0;
}

bool areEqual(Point3 p1, Point3 p2) {
	if (areEqual(p1.x, p2.x) && areEqual(p1.z, p2.z))
		return areEqual(p1.y, p2.y);
	else
		return false;
}

bool areCollinear(Point3 p1, Point3 p2) {
	return isZero((p1*p2).len2());
}

bool isOnLine(Line l, Point3 p1) {
	return areCollinear(l.a - p1, l.b - p1);
}

bool isOnSegment(Line l, Point3 p) {
	return isOnLine(l, p) && sign((l.a - p) ^ (l.b - p)) <= 0;
}

bool areLinesEqualOrPar(Line a, Line b) {
	return areCollinear(a.a - a.b, b.a - b.b);
}

Point3 findLineIntersection(Line a, Line b) {
	if (areLinesEqualOrPar(a, b))
		throw 5;
	ld t = sqrtl(((a.a - b.a)*(b.b - b.a)).len2() / ((a.b - a.a)*(b.b - b.a)).len2());
	Point3 tr = a.a + t*(a.b - a.a);
	if (isOnLine(b, tr))
		return tr;
	else
		return a.a - t*(a.b - a.a);
}


bool doesSegmentsInterceptLine(Line seg, Line l) {
	if (areLinesEqualOrPar(seg, l) && !isOnLine(l, seg.a))
		return false;
	return isOnSegment(seg, findLineIntersection(seg, l));
}

bool areSegmentsIntercept(Line s1, Line s2) {
	if (areLinesEqualOrPar(s1, s2))
		return isOnSegment(s1, s2.a) || isOnSegment(s1, s2.b) || isOnSegment(s2, s1.a);
	else
		return doesSegmentsInterceptLine(s1, s2) && doesSegmentsInterceptLine(s2, s1);
}