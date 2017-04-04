#pragma once
#include<exception>
#include<math.h>
typedef long double ld;

struct Point3 {
	ld x, y, z;
	ld len2() const;
	Point3 inverse() const;
};

Point3 operator*(const Point3& a, const Point3& b);

Point3 operator*(const ld a, const Point3& b);

ld operator^(const Point3& a, const Point3& b);

Point3 operator+(const Point3& a, const Point3& b);

Point3 operator-(const Point3& a, const Point3& b);

bool isZero(const ld z);

ld sign(const ld z);

bool areEqual(const ld a, const ld b);

bool areEqual(const Point3& p1, const Point3& p2);

bool areCollinear(const Point3& p1, const Point3& p2);

struct Line {
	Point3 a, b;
};

bool isZeroPoint(const Point3& p);

bool isZeroRay(const Line& ray);

bool isOnLine(const Line& l, const Point3& p1);

bool isOnSegment(const Line& l, const Point3& p);

bool areLinesEqualOrPar(const Line& a, const Line& b);

Point3 findLineIntersection(const Line& a, const Line& b);

bool doesSegmentsInterceptLine(const Line& seg, const Line& l);

bool areSegmentsIntercept(const Line& s1, const Line& s2);

ld cosBetween(const Point3& s1, const Point3& s2);

Point3 safeInverse(const Point3& p);