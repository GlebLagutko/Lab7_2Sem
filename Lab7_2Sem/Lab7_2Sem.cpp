#include "stdafx.h"
#include "resource.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>


//===============================================================
//===============================================================

const int N = 5;
const int M = 2;

namespace Colors
{
	static COLORREF FromHex(int hex)
	{
		auto r = (hex & 0xFF0000) >> 16;
		auto g = (hex & 0x00FF00) >> 8;
		auto b = (hex & 0x0000FF) >> 0;
		return RGB(r, g, b);
	}

	static const COLORREF Firebrick = FromHex(0xB22222);
	static const COLORREF Goldenrod1 = FromHex(0xFFC125);
	static const COLORREF SpringGreen3 = FromHex(0x00CD66);
	static const COLORREF Black = FromHex(0x000000);
	static const COLORREF White = FromHex(0xFFFFFF);
	static const COLORREF SteelBlue2 = FromHex(0x5CACEE);
	static const COLORREF SlateBlue = FromHex(0x6A5ACD);
	static const COLORREF Sienna1 = FromHex(0xFF8247);
	static const COLORREF Turquoise3 = FromHex(0x00C5CD);
}

class MathToHdcConverter
{
	HWND _hWnd;

public:
	explicit MathToHdcConverter(HWND hWnd) : _hWnd(hWnd) {  }

	int GetX(const double x) const
	{
		return GetCenterX() + x * GetScale();
	}

	int GetY(const double y) const
	{
		return GetCenterY() - y * GetScale();
	}

	int GetSize(const double size) const
	{
		return size * GetScale();
	}

private:
	int GetCenterX() const
	{
		return GetClientRect().right / 2;
	}

	int GetCenterY() const
	{
		return GetClientRect().bottom / 2;
	}

	RECT GetClientRect() const
	{
		RECT clientRect;
		::GetClientRect(_hWnd, &clientRect);
		return clientRect;
	}

	double GetScale() const
	{
		auto windowSize = GetClientRect();
		return min(windowSize.right, windowSize.bottom) / 2;
	}
};

class Program
{
public:
	static void OnDraw(HDC hdc, HWND hWnd)
	{
		auto penBrush = LOGBRUSH{ 0,255,2 };
		auto pen1 = ExtCreatePen(PS_GEOMETRIC | PS_DASH, 5, &penBrush, 0, nullptr);
		auto pen2 = CreatePen(PS_SOLID, 2, RGB(0,0,255));
		auto pen3 = CreatePen(PS_NULL, 1, RGB(0, 0, 0));

		
		penBrush.lbColor = Colors::Turquoise3;
		auto pen4 = ExtCreatePen(PS_GEOMETRIC | PS_DASH, 3, &penBrush, 0, nullptr);

		

		auto converter = MathToHdcConverter(hWnd);


		SelectObject(hdc, pen1);

		Ellipse(hdc, converter.GetX(-1), converter.GetY(1), converter.GetX(1), converter.GetY(-1));

		MoveToEx(hdc, converter.GetX(0), converter.GetY(0), nullptr);

		SelectObject(hdc, pen3);
		auto r = 1;
		double phi = 0;
		LineTo(hdc, converter.GetX(PolarToX(r, phi)), converter.GetY(PolarToY(r, phi)));
		// Star

		for(int i = 0 ; i < N + 1 ; i++)
		{
			SelectObject(hdc, pen2);
			LineTo(hdc, converter.GetX(PolarToX(r, phi)), converter.GetY(PolarToY(r, phi)));
			phi += M_PI * 2 * M / N;
		}

		for(int i = 0 ; i < N + 1 ; i++) 
		{
			SelectObject(hdc, pen2);
			LineTo(hdc, converter.GetX(PolarToX(r, phi)), converter.GetY(PolarToY(r, phi)));
			phi += M_PI * 2 / N;
		}


		DeleteObject(pen1);
		DeleteObject(pen2);
		DeleteObject(pen3);
		DeleteObject(pen4);
	}

private:
	static double PolarToX(double r, double phi)
	{
		return r * cos(phi);; // TODO.
	}

	static double PolarToY(double r, double phi)
	{
		return r * sin(phi); // TODO.
	}
};

//===============================================================
//===============================================================

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING] = L"Drawing";
WCHAR szWindowClass[MAX_LOADSTRING] = L"Drawing_App";

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDRAWING));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Program::OnDraw(hdc, hWnd);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
