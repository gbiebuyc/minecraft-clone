#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>

HWND hwnd;
double seconds;
double rotx, roty;
double posx, posy;

void draw_block(int top, int bottom, int front, int back, int left, int right) {
 
   // Render a color-cube consisting of 6 quads with different colors
   glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen

	glColor3f(1.0f, 1.0f, 1.0f); // White

    double x;
    double y;

   glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      x = top % 16;
      y = top / 16;
      glTexCoord2d((x+1)/16.0, (y+0)/16.0);
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2d((x+0)/16.0, (y+0)/16.0);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2d((x+0)/16.0, (y+1)/16.0);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2d((x+1)/16.0, (y+1)/16.0);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
      x = bottom % 16;
      y = bottom / 16;
      glTexCoord2d((x+1)/16.0, (y+0)/16.0);
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2d((x+0)/16.0, (y+0)/16.0);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2d((x+0)/16.0, (y+1)/16.0);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2d((x+1)/16.0, (y+1)/16.0);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 
      x = front % 16;
      y = front / 16;
      glTexCoord2d((x+1)/16.0, (y+0)/16.0);
      glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2d((x+0)/16.0, (y+0)/16.0);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2d((x+0)/16.0, (y+1)/16.0);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2d((x+1)/16.0, (y+1)/16.0);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      x = back % 16;
      y = back / 16;
      glTexCoord2d((x+1)/16.0, (y+0)/16.0);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2d((x+0)/16.0, (y+0)/16.0);
      glVertex3f( 1.0f,  1.0f, -1.0f);
      glTexCoord2d((x+0)/16.0, (y+1)/16.0);
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2d((x+1)/16.0, (y+1)/16.0);
      glVertex3f(-1.0f, -1.0f, -1.0f);
 
      x = left % 16;
      y = left / 16;
      glTexCoord2d((x+1)/16.0, (y+0)/16.0);
      glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2d((x+0)/16.0, (y+0)/16.0);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2d((x+0)/16.0, (y+1)/16.0);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2d((x+1)/16.0, (y+1)/16.0);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      x = right % 16;
      y = right / 16;
      glTexCoord2d((x+1)/16.0, (y+0)/16.0);
      glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2d((x+0)/16.0, (y+0)/16.0);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2d((x+0)/16.0, (y+1)/16.0);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2d((x+1)/16.0, (y+1)/16.0);
      glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();  // End of drawing color-cube
}

void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotated(rotx, 1, 1, 0);
   glRotated(roty, 0, 1, 0);
   glTranslated(posx, 0, posy);

   draw_block(0, 2, 3, 3, 3, 3);

   // Render a pyramid consists of 4 triangles
   glTranslatef(-1.5f, 0.0f, -6.0f);  // Move left and into the screen
 
   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
 
      // Right
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
 
      // Back
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f, -1.0f, -1.0f);
 
      // Left
      glColor3f(1.0f,0.0f,0.0f);       // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f,0.0f,1.0f);       // Blue
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glColor3f(0.0f,1.0f,0.0f);       // Green
      glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid
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
			}
			return 0;
	    case WM_SIZE: {
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			if (height == 0) height = 1; // To prevent divide by 0
			GLfloat aspect = (GLfloat)width / (GLfloat)height;
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, aspect, 0.1f, 100.0f);
			return 0;
		}
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

	// https://minecraft.fandom.com/wiki/Terrain.png
	char *filename = "./terrain.bmp";
	int fd;
	if ((fd = open(filename, O_RDONLY|O_BINARY)) < 0) {
		fprintf(stderr, "open: %s: %s\n", strerror(errno), filename);
		exit(EXIT_FAILURE);
	}
	uint8_t *tmp = malloc(256*256*4);
	uint8_t *pixels = malloc(256*256*4);
	lseek(fd, 0x8a, SEEK_SET);
	read(fd, tmp, 256*256*4);
	for (int y=0; y<256; y++) {
		for (int x=0; x<256; x++) {
			uint8_t *src = tmp + (255-y)*256*4 + x*4;
			uint8_t *dst = pixels + y*256*4 + x*4;
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = src[3];
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	ShowWindow(hwnd, cmdshow);

	ShowCursor(false);
	RECT rect;
	GetWindowRect(hwnd, &rect);
	int center_x = rect.left + (rect.right - rect.left) / 2;
	int center_y = rect.top + (rect.bottom - rect.top) / 2;
	SetCursorPos(center_x, center_y);

	MSG msg = {0};
	while (true) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				goto End;
			}
		}

		GetWindowRect(hwnd, &rect);
		int center_x = rect.left + (rect.right - rect.left) / 2;
		int center_y = rect.top + (rect.bottom - rect.top) / 2;
		POINT p;
		GetCursorPos(&p);
		#define ROT_SPEED 0.1
		roty += (p.x - center_x) * ROT_SPEED;
		rotx += (p.y - center_y) * ROT_SPEED;
		SetCursorPos(center_x, center_y);

		#define MOVE_SPEED 0.3
		if (GetAsyncKeyState('Z')) {
			posx -= sin(roty * M_PI / 180) * MOVE_SPEED;
			posy += cos(roty * M_PI / 180) * MOVE_SPEED;
		}
		if (GetAsyncKeyState('S')) {
			posx += sin(roty * M_PI / 180) * MOVE_SPEED;
			posy -= cos(roty * M_PI / 180) * MOVE_SPEED;
		}
		if (GetAsyncKeyState('Q')) {
			posx += cos(roty * M_PI / 180) * MOVE_SPEED;
			posy += sin(roty * M_PI / 180) * MOVE_SPEED;
		}
		if (GetAsyncKeyState('D')) {
			posx -= cos(roty * M_PI / 180) * MOVE_SPEED;
			posy -= sin(roty * M_PI / 180) * MOVE_SPEED;
		}

		display();
		PostMessage(hwnd, WM_PAINT, 0, 0);
		Sleep(16);
		seconds += 0.016;
	}

End:
	// Cleanup
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}
