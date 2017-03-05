#pragma once
#include<exception>
typedef long double ld;

struct Point3 {
	ld x, y, z;
	ld len2() const;
};

Point3 operator*(Point3 a, Point3 b);

Point3 operator*(ld a, Point3 b);

ld operator^(Point3 a, Point3 b);

Point3 operator+(Point3 a, Point3 b);

Point3 operator-(Point3 a, Point3 b);

bool isZero(const ld z);

ld sign(const ld z);

bool areEqual(ld a, ld b);

bool areEqual(Point3 p1, Point3 p2);

bool areCollinear(Point3 p1, Point3 p2);

struct Line {
	Point3 a, b;
};

bool isOnLine(Line l, Point3 p1);

bool isOnSegment(Line l, Point3 p);

bool areLinesEqualOrPar(Line a, Line b);

Point3 findLineIntersection(Line a, Line b);

bool doesSegmentsInterceptLine(Line seg, Line l);

bool areSegmentsIntercept(Line s1, Line s2);