#pragma once
#include"Located.h"

class Screen : public Located {
public:
	Screen();
	Screen(Point3 loc, Point3 xAx, Point3 yAx, ld width, ld heigth, int pixX, int pixY);

	Point3 getYAxis() const;
	Point3 getXAxis() const;
	ld getWidth() const;
	ld getHeigth() const;
	int getPixX() const;
	int getPixY() const;
	Point3 getPixel(int x, int y) const;

	void setYAxis(Point3 y);
	void setXAxis(Point3 x);
	void setWidth(ld w);
	void setHeigth(ld h);
	void setPixX(int pix);
	void setPixY(int pix);

	~Screen();
private:
	Point3 _yAxis;
	Point3 _xAxis;
	ld _width;
	ld _heigth;
	int _pixX;
	int _pixY;
	Point3 _deltX;
	Point3 _deltY;

	void recalcDelt();
};

