#include "framework.h"
#include "wt.h"
#include <stdio.h>
#include <windowsx.h>

HINSTANCE hInst;
HWND hWnd;
RECT memRect;
HDC memDC = NULL;
HBITMAP memBM = NULL;
HBRUSH hbBlack = NULL;
POINT mouse;
HBRUSH whiteBrush = NULL;

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	{
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);

			char s[64];
			sprintf_s(s, "[%u x %u]", pt.x, pt.y);
			SetWindowText(hWnd, s);

			if (memBM != NULL)
			{
//				SetPixel(memDC, pt.x, pt.y, RGB(255, 0, 0));
				LineTo(memDC, pt.x, pt.y);
				InvalidateRect(hWnd, NULL, false);
			}
		}
		break;

	case WM_PAINT:
		{
			RECT cr;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			if (hdc != NULL)
			{
				GetClientRect(hWnd, &cr);

				if (whiteBrush == NULL)
					whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

				if (memBM != NULL)
				{
					if (cr.right != memRect.right || cr.bottom != memRect.bottom)
					{
						DeleteObject(memBM);
						DeleteDC(memDC);

						memDC = NULL;
						memBM = NULL;
					}
				}

				if (memDC == NULL)
					memDC = CreateCompatibleDC(hdc);

				if (memDC != NULL)
				{
					if (memBM == NULL)
					{
						memRect = cr;

						memBM = CreateCompatibleBitmap(hdc, memRect.right, memRect.bottom);
						SelectObject(memDC, memBM);

						FillRect(memDC, &memRect, whiteBrush);
					}

					if (memBM != NULL)
					{
						BitBlt(hdc, 0, 0, memRect.right, memRect.bottom, memDC, 0, 0, SRCCOPY);
					}
				}
			}

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

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX rc;

	hInst = hInstance;

	memset(&rc, 0, sizeof rc);

	rc.cbSize = sizeof rc;
	rc.style = CS_HREDRAW | CS_VREDRAW;
	rc.lpfnWndProc = WndProc;
	rc.cbClsExtra = 0;
	rc.cbWndExtra = 0;
	rc.hInstance = hInst;
	rc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_WT));
	rc.hCursor = LoadCursor(NULL, IDC_ARROW);
	rc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	rc.lpszMenuName = MAKEINTRESOURCE(IDC_WT);
	rc.lpszClassName = "MY_CLASS";
	rc.hIconSm = LoadIcon(rc.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&rc);

	hWnd = CreateWindow("MY_CLASS", "My Window Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	HACCEL hAccelTable = LoadAcceleratorsA(hInst, MAKEINTRESOURCE(IDC_WT));

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

