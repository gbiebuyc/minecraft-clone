#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>

#define MAP_DIMENSION 64
enum {
	BLOCK_EMPTY,
	BLOCK_STONE,
	BLOCK_DIRT,
	BLOCK_GRASS,
};
HWND hwnd;
double seconds;
double rotx, roty;
double posx, posy, posz;
uint8_t map[MAP_DIMENSION][MAP_DIMENSION][MAP_DIMENSION];
int screenWidth, screenHeight;

void draw_block(int top, int bottom, int front, int back, int left, int right) {
	double x, y;
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glBegin(GL_QUADS);

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

	glEnd();
}

void draw_crosshair() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScaled(1.0/screenWidth, 1.0/screenHeight, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.7); // Black
	glBegin(GL_QUADS);

	glVertex2d(-16,  2);
	glVertex2d( 16,  2);
	glVertex2d( 16, -2);
	glVertex2d(-16, -2);

	glVertex2d(-2,  16);
	glVertex2d( 2,  16);
	glVertex2d( 2, -16);
	glVertex2d(-2, -16);

	glEnd();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(rotx, 1, 0, 0);
	glRotated(roty, 0, 1, 0);
	glTranslated(-posx, -posy, -posz);
	for (int z=0; z<MAP_DIMENSION; z++) {
		for (int y=0; y<MAP_DIMENSION; y++) {
			for (int x=0; x<MAP_DIMENSION; x++) {
				glPushMatrix();
				glTranslated(x, y, z);
				if (map[z][y][x] != BLOCK_EMPTY) {
					draw_block(0, 2, 3, 3, 3, 3);
				}
				glPopMatrix();
			}
		}
	}
	draw_crosshair();
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
			screenWidth = LOWORD(lParam);
			screenHeight = HIWORD(lParam);
			if (screenHeight == 0) screenHeight = 1; // To prevent divide by 0
			GLfloat aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
			glViewport(0, 0, screenWidth, screenHeight);
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

	memset(map, BLOCK_EMPTY, sizeof(map));
	for (int x=0; x<MAP_DIMENSION; x++) {
		for (int z=0; z<MAP_DIMENSION; z++) {
			map[z][0][x] = BLOCK_STONE;
			map[z][1][x] = BLOCK_DIRT;
			map[z][2][x] = BLOCK_GRASS;
		}
	}

	posx = MAP_DIMENSION / 2;
	posz = MAP_DIMENSION / 2;
	posy = 0;
	while (posy<MAP_DIMENSION &&
		map[(int)posz][(int)posy][(int)posx] != BLOCK_EMPTY)
	{
		posy++;
	}
	posy += 2;

	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);

	MSG msg = {0};
	while (true) {
		QueryPerformanceCounter(&start);
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
		if (GetAsyncKeyState('Z') || GetAsyncKeyState('W')) {
			posx += sin(roty * M_PI / 180) * MOVE_SPEED;
			posz -= cos(roty * M_PI / 180) * MOVE_SPEED;
		}
		if (GetAsyncKeyState('S')) {
			posx -= sin(roty * M_PI / 180) * MOVE_SPEED;
			posz += cos(roty * M_PI / 180) * MOVE_SPEED;
		}
		if (GetAsyncKeyState('Q') || GetAsyncKeyState('A')) {
			posx -= cos(roty * M_PI / 180) * MOVE_SPEED;
			posz -= sin(roty * M_PI / 180) * MOVE_SPEED;
		}
		if (GetAsyncKeyState('D')) {
			posx += cos(roty * M_PI / 180) * MOVE_SPEED;
			posz += sin(roty * M_PI / 180) * MOVE_SPEED;
		}

		if (posx<0 || posx>=MAP_DIMENSION ||
			posy<0 || posy>=MAP_DIMENSION ||
			posz<0 || posz>=MAP_DIMENSION ||
			map[(int)posz][(int)posy-2][(int)posx] == BLOCK_EMPTY) {
			posy -= MOVE_SPEED;
		}

		draw();
		PostMessage(hwnd, WM_PAINT, 0, 0);

		QueryPerformanceCounter(&end);
		double elapsed_sec = ((double)(end.QuadPart - start.QuadPart)) / freq.QuadPart;
		int sleep_millisec = (1.0/60 - elapsed_sec) * 1000;
		if (sleep_millisec > 0) {
			Sleep(sleep_millisec);
		}
		seconds += 1.0/60;
	}

End:
	// Cleanup
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}
