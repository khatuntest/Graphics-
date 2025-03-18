#include <windows.h>
#include <math.h>
using namespace std;

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - y, yc - x, c);
    SetPixel(hdc, xc + y, yc - x, c);
}

void DrawCircleBresenham(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0, y = R, d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
    Draw8Points(hdc, xc, yc, x, y, c);
    while (x <= y) {
        if (d < 0) {
            d += d1;
            d2 += 2;
        }
        else {
            d += d2;
            d2 += 4;
            y--;
        }
        d1 += 2;
        x++;
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}

LRESULT WindowProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp){
    HDC hdc;
    int xc, yc;
    int x, y;
    if (m == WM_LBUTTONDOWN) {
        xc = LOWORD(lp);
        yc = HIWORD(lp);
    }
    else if (m == WM_LBUTTONUP) {
        x = LOWORD(lp);
        y = HIWORD(lp);
        int r = (int)sqrt((x - xc) * (x - xc) + (y - yc) * (y - yc));
        hdc = GetDC(hwnd);
        DrawCircleBresenham(hdc, xc, yc, r, RGB(255, 0, 0));
    }
    else if (m == WM_DESTROY) {
        PostQuitMessage(0);
    }
    else{
        return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.lpszClassName = "MyClass";
    wc.lpfnWndProc = WindowProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Circle Drawer", WS_OVERLAPPEDWINDOW,
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