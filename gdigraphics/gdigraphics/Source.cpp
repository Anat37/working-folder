//#include <stdafx.h>
#include"glibs.h"
#include"scene.h"
#include"Triangle.h"
#include"surfacelib.h"
#include"AmbientLighter.h"

Scene construct(int sx, int sy) {
	Viewer cam({ 0., 5., 5. }, { 0., 0., 0. });
	Screen scr({ 10., 0., 10. }, { 0. ,1. ,0. }, { 0., 0., -1. }, 10., 10., sx, sy);
	Triangle* triag1 = new Triangle({ 18.,3.,5. }, { 0.,0.,5. }, { 0.,5.,0. }, { -1.,0.,0. }, Red, Blue);
	//Triangle* triagf1 = new Triangle({ 0.,0.,0. }, { 0.,0.,0. }, { 100.,-100.,0. }, { 100.,100.,0. }, Color(255, 255, 255, 255));
	AmbientLighter* amb = new AmbientLighter({ 0,0,0 }, AmbL);
	return Scene({ triag1}, {amb});
}

VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen      pen(Color(255, 0, 0, 255));
	int sy = 900;
	int sx = 1000;
	Viewer cam({ 0., 5., 5. }, { 0., 0., 0. });
	Screen scr({ 10., 0., 10. }, { 0. ,1. ,0. }, { 0., 0., -1. }, 10., 10., sx, sy);
	Scene scen = construct(sx, sy);
	auto arr = scen.render(scr, cam);
	Bitmap myBitmap(sx, sy, sx * 4, PixelFormat32bppARGB, (BYTE*)arr);
	/*for (int i = 0; i < sy; ++i)
		for (int j = 0; j < sx; ++j)
			myBitmap.SetPixel(j, i, arr[i][j]);
	*/
	// Draw the bitmap.
	graphics.DrawImage(&myBitmap, 0, 0);
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