//#include <stdafx.h>
#include"thread_pool.h"
#include"scene.h"
#include"Triangle.h"
#include"surfacelib.h"
#include"AmbientLighter.h"
#include"PointLighter.h"
#include"Sphere.h"
#include"Quadrangle.h"
#include"TexturedTriangle.h"

using namespace cimg_library;

extern template class ThreadPool<void>;
ThreadPool<void> threadPool(7); //Num of threads

std::vector<Object3*> piramid(Point3 loc, int cnt, ld edge) {
	std::vector<Object3*> lst;
	if (cnt > 5) {
		edge = edge / 2.;
		std::vector<Object3*> vect(piramid(loc, cnt / 5, edge));
		for (size_t i = 0; i < vect.size(); ++i)
			lst.push_back(vect[i]);
		vect = std::move(piramid(loc + Point3{ edge, 0., 0. }, cnt / 5, edge));
		for (size_t i = 0; i < vect.size(); ++i)
			lst.push_back(vect[i]);
		vect = std::move(piramid(loc + Point3{ 0., edge, 0. }, cnt / 5, edge));
		for (size_t i = 0; i < vect.size(); ++i)
			lst.push_back(vect[i]);
		vect = std::move(piramid(loc + Point3{ edge, edge, 0. }, cnt / 5, edge));
		for (size_t i = 0; i < vect.size(); ++i)
			lst.push_back(vect[i]);
		vect = std::move(piramid(loc + Point3{ edge / 2., edge / 2., edge }, cnt / 5, edge));
		for (size_t i = 0; i < vect.size(); ++i)
			lst.push_back(vect[i]);
	}
	else {
		lst.push_back(new Triangle(loc, { edge,0.,0. }, { edge / 2,edge / 2,edge }, { 0.,-1.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc, { 0.,edge,0. }, { edge / 2,edge / 2,edge }, { -1.,0.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc + Point3{ edge,edge,0 }, { -edge,0.,0. }, { -edge / 2,-edge / 2,edge }, { 0.,1.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc + Point3{ edge, edge, 0 }, { 0.,-edge,0. }, { -edge / 2,-edge / 2,edge }, { 1.,0.,0.5 }, Red, Blue));
	}
	return lst;
}

class Envy {
public:
	Envy()
		:scene(std::vector<Object3*>(), std::vector<Lighter*>()){}

	Scene scene;
	Screen screen;
	Viewer viewer;
	int sy;
	int sx;

	void buildEnvy() {
		CImg<unsigned char>* image = new CImg<unsigned char>("D:\\Repos\\workf\\gdigraphics\\gdigraphics\\x64\\Release\\texture.bmp");
		Triangle* triag1 = new Triangle({ 25.,10.,0. }, { 0.0,0.,200. }, { -50.,0.,0. }, { 0.,-1.,0. }, whMirror, Red);
		Triangle* triagf1 = new Triangle({ 14.,3.,0. }, { -5.,5.,0. }, { 0.,0.,5. }, { -1.,-1.,0. }, transGreen, transGreen);
		Triangle* triagf2 = new TexturedTriangle({ -10.,-10.,0. }, { 1000.,0.,0. }, { 0.,1000.,0. }, { 0.,0.,1. }, image, White, White);
		size_t n = 1500;
		AmbientLighter* amb = new AmbientLighter({ 0,0,0 }, AmbL);
		PointLighter* pwh1 = new PointLighter({ 6, 4, 5 }, PointLWhite);
		PointLighter* pwh2 = new PointLighter({ 20, 5, 5 }, PointLWhite);
		Sphere* sp = new Sphere(Point3{ 20, 5, 3 }, 5, transGreen, innertransGreen);
		Quadrangle* q = new Quadrangle({ 17,-5,0 }, { 0,0,4 }, { 0,5,4 }, { 0,5,0 }, { -1,0,0 }, Red, Red);
		std::vector<Object3*> vect(piramid(Point3{ 16.,0.,0. }, n, 10.));
		vect.push_back(triagf1);
		vect.push_back(triag1);
		vect.push_back(triagf2);
		vect.push_back(sp);
		vect.push_back(q);
		scene.setObjects({ amb, pwh1, pwh2}, std::move(vect));
		viewer = Viewer({ 2., -7., 5. }, { 0., 0., 0. });
		screen = Screen({ 10., -7, 10. }, { -1. ,1. ,0. }, { 0., 0., -1. }, 18., 10., sx, sy);
		scene.prepareScene();
	}

	Color* renderEnvy() {
		return scene.render(screen, viewer, 3);
	}
	void moveFrwd() {
		Point3 loc = screen.getLocation();
		loc.x += 5;
		screen.setLocation(loc);
		loc = viewer.getLocation();
		loc.x += 5;
		viewer.setLocation(loc);
	};
	void moveBack() {

	}
	void moveRight() {

	}
	void moveLeft() {

	}
	void turnRight() {

	}
	void turnLeft() {

	}
} envy;

VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	auto arr = envy.renderEnvy();
	Bitmap myBitmap(envy.sx, envy.sy, envy.sx * 4, PixelFormat32bppARGB, (BYTE*)arr);
	graphics.DrawImage(&myBitmap, 0, 0, envy.sx, envy.sy);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters
	envy.sy = 1000;
	envy.sx = 1800;
	envy.buildEnvy();
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;
	int key;
	switch (message)
	{
	case WM_KEYDOWN:
		key = wParam; //Получаем код нажатой клавиши
		switch (key) {
			case VK_UP: envy.moveFrwd(); break;
			case VK_DOWN: envy.moveBack(); break;
			case VK_RIGHT: envy.moveRight(); break;
			case VK_LEFT: envy.moveLeft(); break;
			case VK_NUMPAD4: envy.turnLeft(); break;
			case VK_NUMPAD6: envy.turnRight(); break;
		}
		InvalidateRect(hWnd, 0, TRUE);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc