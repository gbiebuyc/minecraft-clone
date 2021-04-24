#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>

HWND hwnd;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0,  1);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2i(-1, -1);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2i(1, -1);
    glEnd();
	HDC hdc = GetDC(hwnd);
    SwapBuffers(hdc);
	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
		    PostQuitMessage(0);
		    return 0;
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) {
			    PostQuitMessage(0);
			    return 0;
			}
	    case WM_PAINT: {
	    	PAINTSTRUCT ps;
			display();
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			return 0;
	    }
	    case WM_SIZE:
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
			PostMessage(hwnd, WM_PAINT, 0, 0);
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	WNDCLASS wc = {0};
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInst;
	wc.lpszClassName = "MyWindowClass";
	RegisterClass(&wc);

	hwnd = CreateWindowEx(0, wc.lpszClassName, "OpenGL test", WS_OVERLAPPEDWINDOW,
	    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	    NULL, NULL, hInst, NULL);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC hdc = GetDC(hwnd);
	int iPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, iPixelFormat, &pfd);
	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
	ReleaseDC(hwnd, hdc);

	ShowWindow(hwnd, cmdshow);

	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}

	// Cleanup
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}
