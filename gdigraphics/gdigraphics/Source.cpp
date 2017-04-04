//#include <stdafx.h>
#include"thread_pool.h"
#include"scene.h"
#include"Triangle.h"
#include"surfacelib.h"
#include"AmbientLighter.h"
#include"PointLighter.h"

extern template class ThreadPool<void>;
ThreadPool<void> threadPool(3); //Num of threads

std::vector<Object3*> piramid(Point3 loc, size_t cnt, ld edge, std::vector<Object3*>& lst) {
	if (cnt >= 5) {
		edge = edge / 2.;
		piramid(loc, cnt / 5, edge, lst);
		piramid(loc + Point3{ edge, 0., 0. }, cnt / 5, edge, lst);
		piramid(loc + Point3{ 0., edge, 0. }, cnt / 5, edge,lst);
		piramid(loc + Point3{ edge, edge, 0. }, cnt / 5, edge,lst);
		piramid(loc + Point3{ edge / 2., edge / 2., edge }, cnt / 5, edge,lst);
	} else {
		lst.push_back(new Triangle(loc, { edge,0.,0. }, { edge/2,edge/2,edge }, { 0.,-1.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc, { 0.,edge,0. }, { edge / 2,edge / 2,edge }, { -1.,0.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc + Point3{ edge,edge,0 }, { -edge,0.,0. }, { -edge / 2,-edge / 2,edge }, { 0.,1.,1. }, Red, Blue));
		lst.push_back(new Triangle(loc + Point3{ edge, edge, 0 }, { 0.,-edge,0. } ,{ -edge / 2,-edge / 2,edge }, { 1.,0.,0.5 }, Red, Green));
	}
	return lst;
}

Scene construct(int sx, int sy) {
	Triangle* triag1 = new Triangle({ 25.,10.,0. }, { 0.0,0.,200. }, { -50.,0.,0. }, { 0.,-1.,0. }, whMirror, Red);
	Triangle* triagf1 = new Triangle({ 14.,3.,0. }, { -5.,5.,0. }, { 0.,0.,5. }, { -1.,-1.,0. }, transGreen, White);
	Triangle* triagf2 = new Triangle({ -10.,-10.,0. }, { 1000.,0.,0. }, { 0.,1000.,0. }, { 0.,0.,1. }, White, White);
	size_t n = 1500;
	AmbientLighter* amb = new AmbientLighter({ 0,0,0 }, AmbL);
	PointLighter* pwh1 = new PointLighter({ 12, 8, 5 }, PointLWhite);
	PointLighter* pwh2 = new PointLighter({ 20, 5, 5 }, PointLWhite);
	std::vector<Object3*> vect;
	piramid(Point3{ 16.,0.,0. }, n, 10.,vect);
	vect.push_back(triagf1);
	vect.push_back(triag1);
	vect.push_back(triagf2);
	return Scene(std::move(vect), {amb, pwh1, pwh2});
}

VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	int sy = 1000;
	int sx = 1000;
	Scene scen(construct(sx, sy));
	Viewer cam({ 0., -12., 5. }, { 0., 0., 0. });
	Screen scr({ 10., -10, 10. }, { -1. ,1. ,0. }, { 0., 0., -1. }, 10., 10., sx, sy);
	scen.prepareScene();
	auto arr = scen.render(scr, cam);
	Bitmap myBitmap(sx, sy, sx * 4, PixelFormat32bppARGB, (BYTE*)arr);
	graphics.DrawImage(&myBitmap, 0, 0, sx, sy);
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

	switch (message)
	{
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