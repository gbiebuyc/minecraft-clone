#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>

#define FOV 70
#define MAP_DIMENSION 64
#define REACH_DISTANCE 8
#define COLLISION_DIST 0.4
#define CAM_HEIGHT 1.5
#define JUMP_SPEED 0.18
enum {
	BLOCK_EMPTY,
	BLOCK_STONE,
	BLOCK_DIRT,
	BLOCK_GRASS,
	BLOCK_PLANK,
	BLOCK_ROSE,
	BLOCK_DANDELION,
	NUM_BLOCKS,
};
bool isSolid[NUM_BLOCKS] = {
	[BLOCK_STONE] = true,
	[BLOCK_DIRT] = true,
	[BLOCK_GRASS] = true,
	[BLOCK_PLANK] = true,
};
HWND hwnd;
double seconds;
double rotX, rotY;
double posX, posY, posZ;
double speedY;
bool isTouchingTheGround;
uint8_t map[MAP_DIMENSION][MAP_DIMENSION][MAP_DIMENSION];
int screenWidth, screenHeight;
double aspectRatio;
int targetX, targetY, targetZ;
int targetPlaceX, targetPlaceY, targetPlaceZ;
bool isBlockSelected;
int placeBlockType = BLOCK_STONE;

bool isInsideMap(int x, int y, int z) {
	return (x>=0 && x<MAP_DIMENSION &&
		y>=0 && y<MAP_DIMENSION &&
		z>=0 && z<MAP_DIMENSION);
}

void draw_block_faces(int top, int bottom, int front, int back, int left, int right) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	double x, y;
	glBegin(GL_QUADS);

	x = top % 16;
	y = top / 16;
	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(1, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(0, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(0, 1, 1);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(1, 1, 1);

	x = bottom % 16;
	y = bottom / 16;
	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(1, 0, 1);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(0, 0, 1);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(0, 0, 0);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(1, 0, 0);

	x = front % 16;
	y = front / 16;
	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(1, 1, 1);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(0, 1, 1);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(0, 0, 1);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(1, 0, 1);

	x = back % 16;
	y = back / 16;
	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(0, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(1, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(1, 0, 0);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(0, 0, 0);

	x = left % 16;
	y = left / 16;
	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(0, 1, 1);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(0, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(0, 0, 0);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(0, 0, 1);

	x = right % 16;
	y = right / 16;
	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(1, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(1, 1, 1);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(1, 0, 1);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(1, 0, 0);

	glEnd();
}

void draw_small_plant(int texture) {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	double x, y;
	glBegin(GL_QUADS);

	x = texture % 16;
	y = texture / 16;
	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(1, 1, 1);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(0, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(0, 0, 0);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(1, 0, 1);

	glTexCoord2d((x+1)/16.0, (y+0)/16.0);
	glVertex3f(0, 1, 1);
	glTexCoord2d((x+0)/16.0, (y+0)/16.0);
	glVertex3f(1, 1, 0);
	glTexCoord2d((x+0)/16.0, (y+1)/16.0);
	glVertex3f(1, 0, 0);
	glTexCoord2d((x+1)/16.0, (y+1)/16.0);
	glVertex3f(0, 0, 1);

	glEnd();
}

void draw_block(uint8_t block) {
	glColor3f(1, 1, 1); // White
	glEnable(GL_TEXTURE_2D);
	switch (block) {
	case BLOCK_STONE: draw_block_faces(1, 1, 1, 1, 1, 1); break;
	case BLOCK_DIRT:  draw_block_faces(2, 2, 2, 2, 2, 2); break;
	case BLOCK_GRASS: draw_block_faces(0, 2, 3, 3, 3, 3); break;
	case BLOCK_PLANK: draw_block_faces(4, 4, 4, 4, 4, 4); break;
	case BLOCK_ROSE:      draw_small_plant(12); break;
	case BLOCK_DANDELION: draw_small_plant(13); break;
	}
}

void draw_block_outline() {
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glColor4f(0, 0, 0, 0.7); // Black
	glPolygonMode(GL_FRONT, GL_LINE);
	draw_block_faces(0, 0, 0, 0, 0, 0);
	glPolygonMode(GL_FRONT, GL_FILL);
}

void draw_crosshair() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScaled(1.0/screenWidth, 1.0/screenHeight, 1);
	glColor4f(0, 0, 0, 0.7); // Black
	glBegin(GL_QUADS);

	glVertex2d(-16,  2);
	glVertex2d(-16, -2);
	glVertex2d( 16, -2);
	glVertex2d( 16,  2);

	glVertex2d(-2,  16);
	glVertex2d(-2, -16);
	glVertex2d( 2, -16);
	glVertex2d( 2,  16);

	glEnd();
}

void draw_block_ui_icon() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-aspectRatio, aspectRatio, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(aspectRatio-0.2, -0.8, 0);
	#define SCALE 0.15
	glScaled(SCALE, SCALE, SCALE);
	glRotated(20, 1, 0, 0);
	glRotated(45, 0, 1, 0);
	glTranslated(-0.5, -0.5, -0.5);
	draw_block(placeBlockType);
}

void raycast() {
	double deltaDistX = fabs(1.0 / cos(rotX) / sin(rotY));
	double deltaDistY = fabs(1.0 / sin(rotX));
	double deltaDistZ = fabs(1.0 / cos(rotX) / cos(rotY));
	// printf("%.1f %.1f %.1f  ", deltaDistX, deltaDistY, deltaDistZ);
	targetX = (int)posX;
	targetY = (int)posY;
	targetZ = (int)posZ;
	isBlockSelected = false;
	double sideDistX, sideDistY, sideDistZ;
	int stepX, stepY, stepZ;
	int side;
	if (sin(rotY) < 0) {
		stepX = -1;
		sideDistX = (posX - targetX) * deltaDistX;
	} else {
		stepX = 1;
		sideDistX = (targetX + 1.0 - posX) * deltaDistX;
	}
	if (sin(rotX) > 0) {
		stepY = -1;
		sideDistY = (posY - targetY) * deltaDistY;
	} else {
		stepY = 1;
		sideDistY = (targetY + 1.0 - posY) * deltaDistY;
	}
	if (cos(rotY) > 0) {
		stepZ = -1;
		sideDistZ = (posZ - targetZ) * deltaDistZ;
	} else {
		stepZ = 1;
		sideDistZ = (targetZ + 1.0 - posZ) * deltaDistZ;
	}
	while (true) {
		double smallestSideDist = fmin(fmin(sideDistX, sideDistY), sideDistZ);
		if (smallestSideDist > REACH_DISTANCE ||
			!isInsideMap(targetX, targetY, targetZ))
			break;
		if (map[targetZ][targetY][targetX] != BLOCK_EMPTY) {
			// Ray has hit a block
			isBlockSelected = true;
			targetPlaceX = targetX + ((side==0) ? -stepX : 0);
			targetPlaceY = targetY + ((side==1) ? -stepY : 0);
			targetPlaceZ = targetZ + ((side==2) ? -stepZ : 0);
			break;
		}
		if (smallestSideDist == sideDistX) {
			sideDistX += deltaDistX;
			targetX += stepX;
			side = 0;
		} else if (smallestSideDist == sideDistY) {
			sideDistY += deltaDistY;
			targetY += stepY;
			side = 1;
		} else if (smallestSideDist == sideDistZ) {
			sideDistZ += deltaDistZ;
			targetZ += stepZ;
			side = 2;
		}
		// printf("%d %d %d, ", targetX, targetY, targetZ);
	}
	// printf("\n"); fflush(stdout);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, aspectRatio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(rotX * 180 / M_PI, 1, 0, 0);
	glRotated(rotY * 180 / M_PI, 0, 1, 0);
	glTranslated(-posX, -posY, -posZ);
	for (int z=0; z<MAP_DIMENSION; z++) {
		for (int y=0; y<MAP_DIMENSION; y++) {
			for (int x=0; x<MAP_DIMENSION; x++) {
				if (map[z][y][x] == BLOCK_EMPTY)
					continue;
				glPushMatrix();
				glTranslated(x, y, z);
				draw_block(map[z][y][x]);
				if (isBlockSelected && x==targetX && y==targetY && z==targetZ)
					draw_block_outline();
				glPopMatrix();
			}
		}
	}
	draw_crosshair();
	draw_block_ui_icon();
	HDC hdc = GetDC(hwnd);
	SwapBuffers(hdc);
	ReleaseDC(hwnd, hdc);
}

// Horizontal distance
double dist_to_block(int x, int z) {
	double closestX = fmin(fmax(posX, x), x+1);
	double closestZ = fmin(fmax(posZ, z), z+1);
	double distX = posX - closestX;
	double distZ = posZ - closestZ;
	return sqrt(distX*distX + distZ*distZ);
}

void collision() {
	// Check direct adjacents before diagonals to avoid false collisions
	// with corners when sliding against a straight line of blocks.
	const int adjacentsX[8] = {-1, 1, 0, 0,  -1,-1, 1, 1};
	const int adjacentsZ[8] = { 0, 0,-1, 1,  -1, 1,-1, 1};
	const double heights[2] = {0, -CAM_HEIGHT};
	for (int i=0; i<8; i++) {
		for (int h=0; h<2; h++) {
			int x = posX + adjacentsX[i];
			int z = posZ + adjacentsZ[i];
			int y = posY + heights[h];
			if (!isInsideMap(x, y, z))
				continue;
			if (!isSolid[map[z][y][x]])
				continue;
			double closestX = fmin(fmax(posX, x), x+1);
			double closestZ = fmin(fmax(posZ, z), z+1);
			double distX = posX - closestX;
			double distZ = posZ - closestZ;
			double dist = sqrt(distX*distX + distZ*distZ);
			if (dist < COLLISION_DIST) {
				posX = closestX + distX * COLLISION_DIST / dist;
				posZ = closestZ + distZ * COLLISION_DIST / dist;
			}
		}
	}
}

void collision_vertical() {
	int y = posY + speedY + ((speedY<0) ? -CAM_HEIGHT : 0);
	// printf("posY=%f y=%d\n", posY, y); fflush(stdout);
	for (int dx=-1; dx<=1; dx++) {
		for (int dz=-1; dz<=1; dz++) {
			int x = posX + dx;
			int z = posZ + dz;
			if (!isInsideMap(x, y, z))
				continue;
			if (!isSolid[map[z][y][x]])
				continue;
			if (dist_to_block(x, z) < COLLISION_DIST-0.1) {
				isTouchingTheGround = true;
				speedY = 0;
				return;
			}
		}
	}
	isTouchingTheGround = false;
	posY += speedY;
}

void get_window_center(POINT *center) {
	RECT rect;
	GetWindowRect(hwnd, &rect);
	center->x = (rect.left + rect.right) / 2;
	center->y = (rect.top + rect.bottom) / 2;
}

void handle_input() {
	if (GetForegroundWindow() != hwnd)
		return;
	// Mouse
	POINT center, cursor;
	get_window_center(&center);
	GetCursorPos(&cursor);
	#define ROT_SPEED 0.002
	rotY += (cursor.x - center.x) * ROT_SPEED;
	rotX += (cursor.y - center.y) * ROT_SPEED;
	rotX = fmin(fmax(rotX, -1.5), 1.5);
	SetCursorPos(center.x, center.y);
	// Keyboard
	#define MOVE_SPEED 0.1
	if (GetAsyncKeyState('Z') || GetAsyncKeyState('W')) {
		posX += sin(rotY) * MOVE_SPEED;
		posZ -= cos(rotY) * MOVE_SPEED;
	}
	if (GetAsyncKeyState('S')) {
		posX -= sin(rotY) * MOVE_SPEED;
		posZ += cos(rotY) * MOVE_SPEED;
	}
	if (GetAsyncKeyState('Q') || GetAsyncKeyState('A')) {
		posX -= cos(rotY) * MOVE_SPEED;
		posZ -= sin(rotY) * MOVE_SPEED;
	}
	if (GetAsyncKeyState('D')) {
		posX += cos(rotY) * MOVE_SPEED;
		posZ += sin(rotY) * MOVE_SPEED;
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_SPACE:
				if (isTouchingTheGround)
					speedY = JUMP_SPEED;
				break;
			}
			break;
		case WM_SETFOCUS: {
			POINT center;
			get_window_center(&center);
			SetCursorPos(center.x, center.y);
			ShowCursor(false);
			break;
		}
		case WM_KILLFOCUS: {
			ShowCursor(true);
			break;
		}
		case WM_LBUTTONDOWN:
			if (isBlockSelected) {
				// Destroy a block
				map[targetZ][targetY][targetX] = BLOCK_EMPTY;
			}
			break;
		case WM_RBUTTONDOWN:
			if (isBlockSelected &&
				(dist_to_block(targetPlaceX, targetPlaceZ) > COLLISION_DIST-0.1 ||
				 (targetPlaceY != (int)posY &&
				  targetPlaceY != (int)(posY-CAM_HEIGHT))))
			{
				// Place a block
				map[targetPlaceZ][targetPlaceY][targetPlaceX] = placeBlockType;
			}
			break;
		case WM_MOUSEWHEEL: {
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta == 0)
				break;
			delta = delta > 0 ? -1 : 1;
			do {
				placeBlockType += delta;
				if (placeBlockType < 0) {
					placeBlockType += NUM_BLOCKS;
				} else if (placeBlockType >= NUM_BLOCKS) {
					placeBlockType -= NUM_BLOCKS;
				}
			} while (placeBlockType == BLOCK_EMPTY);
			break;
		}
		case WM_SIZE: {
			screenWidth = LOWORD(lParam);
			screenHeight = HIWORD(lParam);
			if (screenHeight == 0) screenHeight = 1; // To prevent divide by 0
			aspectRatio = (double)screenWidth / (double)screenHeight;
			glViewport(0, 0, screenWidth, screenHeight);
			break;
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

	ShowWindow(hwnd, cmdshow);

	memset(map, BLOCK_EMPTY, sizeof(map));
	for (int x=0; x<MAP_DIMENSION; x++) {
		for (int z=0; z<MAP_DIMENSION; z++) {
			map[z][0][x] = BLOCK_STONE;
			map[z][1][x] = BLOCK_DIRT;
			map[z][2][x] = BLOCK_GRASS;
			switch (rand() % 40) {
			case 0: map[z][3][x] = BLOCK_ROSE; break;
			case 1: map[z][3][x] = BLOCK_DANDELION; break;
			}
		}
	}

	posX = MAP_DIMENSION / 2;
	posZ = MAP_DIMENSION / 2;
	posY = 0;
	while (posY<MAP_DIMENSION &&
		map[(int)posZ][(int)posY][(int)posX] != BLOCK_EMPTY)
	{
		posY++;
	}
	posY += CAM_HEIGHT + 0.01;

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

		handle_input();

		speedY -= 0.01;
		collision();
		collision_vertical();

		raycast();

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
