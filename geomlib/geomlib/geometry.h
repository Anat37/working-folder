#pragma once

template<typename T>
struct Point {
	T x, y;
	T len2() const {
		return x*x + y*y;
	}
};

template<typename T>
T operator*(Point<T> a, Point<T> b) {
	return a.x*b.y - a.y*b.x;
}

template<typename T>
T operator^(Point<T> a, Point<T> b) {
	return a.x*b.x + a.y*b.y;
}

template<typename T>
Point<T> operator+(Point<T> a, Point<T> b) {
	return {a.x + b.x, a.y + b.y};
}

template<typename T>
Point<T> operator-(Point<T> a, Point<T> b) {
	return{ a.x - b.x, a.y - b.y };
}

template<typename T>
struct Polygon {
	std::vector<T> points;
};

template<typename T>
bool isPointInPolygon(Point<T> p, Polygon<T> polyg) {

}
