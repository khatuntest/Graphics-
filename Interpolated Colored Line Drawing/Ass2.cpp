#include <windows.h>
#include <cmath>
using namespace std;


int Round(double n) {
    return static_cast<int>(n + 0.5);
}

void GetCR(COLORREF c, int& r, int& g, int& b) { // Function to extract RGB components from COLORREF
    r = GetRValue(c);
    g = GetGValue(c);
    b = GetBValue(c);

}

void InterpolatedColoredLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c1, COLORREF c2) {
    int diffX = x2 - x1, diffY = y2 - y1, x, y, r1, g1, b1, r2, g2, b2;
    double step = 1.0 / max(abs(diffX), abs(diffY));// Calculate number of steps based on max difference

    GetCR(c1, r1, g1, b1);
    GetCR(c2, r2, g2, b2);
    for (double t = 0; t <= 1; t += step) {
        x = Round(diffX * t + x1);
        y = Round(diffY * t + y1);

        // Interpolating RGB values
        int r = Round(r1 + t * (r2 - r1));
        int g = Round(g1 + t * (g2 - g1));
        int b = Round(b1 + t * (b2 - b1));
        SetPixel(hdc, x, y, RGB(r, g, b));
    }

}
LRESULT WindowProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    static int x1, y1, x2, y2;
    static bool draw = false;
    if (m == WM_LBUTTONDOWN) { // compute first point
        x1 = LOWORD(lp);
        y1 = HIWORD(lp);
        draw = true;
    }
    else if (m == WM_LBUTTONUP) { // compute last point
        x2 = LOWORD(lp);
        y2 = HIWORD(lp);
        draw = false;
        hdc = GetDC(hwnd);
        InterpolatedColoredLine(hdc, x1, y1, x2, y2, RGB(0, 0, 0), RGB(255, 0, 255)); // red to blue
        ReleaseDC(hwnd, hdc);

    }
    else if (m == WM_DESTROY) {
        PostQuitMessage(0);
    }
    else {
        return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.lpszClassName = "LineWindowClass";
    wc.lpfnWndProc = WindowProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("LineWindowClass", "Interpolated Line Drawing", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 700, 700, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}