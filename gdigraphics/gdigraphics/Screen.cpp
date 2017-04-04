#pragma once
#include "Screen.h"


Screen::Screen() {}

Point3 Screen::getXAxis() const {
	return _xAxis;
}

Point3 Screen::getYAxis() const {
	return _yAxis;
}

ld Screen::getWidth() const {
	return _width;
}

ld Screen::getHeigth() const {
	return _heigth;
}

int Screen::getPixX() const {
	return _pixX;
}

int Screen::getPixY() const {
	return _pixY;
}

Point3 Screen::getPixel(int x, int y) const {
	return _location + x*_deltX + y*_deltY;
}

void Screen::setXAxis(Point3 x) {
	_xAxis = x;
}

void Screen::setYAxis(Point3 y) {
	_yAxis = y;
}

void Screen::setWidth(ld w) {
	_width = w;
}

void Screen::setHeigth(ld h) {
	_heigth = h;
}

void Screen::setPixX(int pix) {
	_pixX = pix;
}

void Screen::setPixY(int pix) {
	_pixY = pix;
}

Screen::Screen(Point3 loc, Point3 xAx, Point3 yAx, ld width, ld heigth, int pixX, int pixY)
				:Located(loc)
				,_xAxis(xAx)
				,_yAxis(yAx)
				,_width(width)
				,_heigth(heigth)
				,_pixX(pixX)
				,_pixY(pixY) {
	recalcDelt();
}


Screen::~Screen() {}

void Screen::recalcDelt() {
	_deltY = sqrtl((_heigth * _heigth) / (_yAxis.len2() * _pixY * _pixY))*_yAxis;
	_deltX = sqrtl((_width * _width) / (_xAxis.len2() * _pixX * _pixX))*_xAxis;
}
