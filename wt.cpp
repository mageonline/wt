#include "framework.h"
#include "wt.h"

HINSTANCE hInst;
HWND hWnd;

HBRUSH hbBlack = NULL;

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
	
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			if (hbBlack == NULL)
				hbBlack = CreateSolidBrush((0, 0, 0));

			RECT r;

			r.left = 10;
			r.top = 10;
			r.right = 70;
			r.bottom = 30;

			FillRect(hdc, &r, hbBlack);

			r.left = 100;
			r.top = 100;
			r.right = 1000;
			r.bottom = 200;

			DrawText(hdc, "blabla", 6, &r, 0);

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

