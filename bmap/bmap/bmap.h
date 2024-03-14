// 2018 ReVoL Primer Template
// bmap.h
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"
#define MF_W 796
#define MF_H 431
int BMPX, BMPY;
int ScaleBlt = 8;
HINSTANCE hInst;
HWND hWndMain;
HWND hWndBMP;
HWND hWndEMF;
HBITMAP HBMP;
HDC hDCMain;
HDC DCBMP;
HDC CDC;
HDC DCEMF;
int loaded = 0;
int gde_w = 0, gde_h = 0;
LRESULT CALLBACK BMPProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    if (!loaded) return DefWindowProc(hWnd, message, wParam, lParam);
    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        // код для рисования
        //BitBlt(DCBMP, 0, 0, BMPX, BMPY, CDC, 0, 0, SRCCOPY);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK EMFProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    if (!loaded) return DefWindowProc(hWnd, message, wParam, lParam);
    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // код для рисования
        EndPaint(hWnd, &ps);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmIdent, wmEvent;
    switch (message) {
    case WM_COMMAND:
        wmIdent = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        switch (wmIdent) {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
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
ATOM RegisterClasses(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BMAP));
    wcex.hCursor = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_BMAP);
    wcex.lpszClassName = "BMAPAPP";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    if (!RegisterClassEx(&wcex)) return 0;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)BMPProc;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = "BMAPBMP";
    wcex.hIconSm = 0;
    if (!RegisterClassEx(&wcex)) return 0;
    wcex.lpfnWndProc = (WNDPROC)EMFProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
    wcex.lpszClassName = "BMAPEMF";
    return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    int SW = GetSystemMetrics(SM_CXSCREEN);
    int SH = GetSystemMetrics(SM_CYSCREEN);
    int cx = (SW - MF_W) / 2;
    int cy = (SH - MF_H) / 2;
    RECT rc;
    hInst = hInstance;
    hWndMain = CreateWindow("BMAPAPP", "Bitmaps Primer App", 0x80c80000, cx, cy, MF_W, MF_H, 0, 0, hInstance, 0);
    if (!hWndMain) return FALSE;
    GetClientRect(hWndMain, &rc);
    gde_w = (rc.right) / 2;
    gde_h = (rc.bottom - 2);
    hDCMain = GetDC(hWndMain);
    hWndBMP = CreateWindow("BMAPBMP", 0, 0x50810000, 1, 1, gde_w, gde_h, hWndMain, 0, hInst, 0);
    DCBMP = GetDC(hWndBMP);
    CDC = CreateCompatibleDC(DCBMP);
    hWndEMF = CreateWindow("BMAPEMF", 0, 0x50810000, gde_w - 1, 1, gde_w, gde_h, hWndMain, 0, hInst, 0);
    DCEMF = GetDC(hWndEMF);
    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);
    loaded = 1;
    return TRUE;
}
void bitmap(void);
void metafile(void);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPTSTR lpCmd, int nCmdShow) {
    RegisterClasses(hInstance);
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;
    bitmap();
    metafile();
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) {
        if (msg.message == WM_KEYDOWN && msg.wParam == 27) {
            SendMessage(hWndMain, WM_DESTROY, 0, 0);
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}
