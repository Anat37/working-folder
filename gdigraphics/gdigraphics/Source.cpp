//#include <stdafx.h>
#include"glibs.h"
#include"scene.h"
#include"Triangle.h"
#include"surfacelib.h"
#include"AmbientLighter.h"
#include"PointLighter.h"

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
	} else {
		lst.push_back(new Triangle(loc, { edge,0.,0. }, { edge/2,edge/2,edge }, { 0.,-1.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc, { 0.,edge,0. }, { edge / 2,edge / 2,edge }, { -1.,0.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc + Point3{ edge,edge,0 }, { -edge,0.,0. }, { -edge / 2,-edge / 2,edge }, { 0.,1.,0.5 }, Red, Blue));
		lst.push_back(new Triangle(loc + Point3{ edge, edge, 0 }, { 0.,-edge,0. } ,{ -edge / 2,-edge / 2,edge }, { 1.,0.,0.5 }, Red, Blue));
	}
	return lst;
}

Scene construct(int sx, int sy) {
	//Triangle* triag1 = new Triangle({ 15.,3.,0. }, { 0.0,0.,5. }, { 0.,5.,0. }, { -1.,0.,0. }, Red, Blue);
	//Triangle* triagf1 = new Triangle({ 0.,3.,0. }, { 100.,-100.,0. }, { 100.,100.,0. }, { 0.,0.,1. }, White, White);
	size_t n = 150026;
	AmbientLighter* amb = new AmbientLighter({ 0,0,0 }, AmbL);
	PointLighter* pwh1 = new PointLighter({ 10, 5, 5 }, PointLWhite);
	PointLighter* pwh2 = new PointLighter({ 20, 5, 5 }, PointLWhite);
	return Scene(piramid(Point3{ 15.,0.,0. }, n, 10.), { amb, pwh1, pwh2 });
}

VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen      pen(Color(255, 0, 0, 255));
	int sy = 500;
	int sx = 500;
	Scene scen = construct(sx, sy);
	Viewer cam({ 0., -12., 5. }, { 0., 0., 0. });
	Screen scr({ 10., -10, 10. }, { -1. ,1. ,0. }, { 0., 0., -1. }, 10., 10., sx, sy);
	auto arr = scen.render(scr, cam);
	Bitmap myBitmap(sx, sy, sx * 4, PixelFormat32bppARGB, (BYTE*)arr);
	/*for (int i = 0; i < sy; ++i)
		for (int j = 0; j < sx; ++j)
			myBitmap.SetPixel(j, i, arr[i][j]);
	*/
	// Draw the bitmap.
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