#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include "form.h"

#define SPACEBAR 32
#define LEFT 75
#define RIGHT 77
#define DOWN 80

clock_t startT, endT, groundT;
int M_x=4, M_y=0;

int BlockType_R;
int BlockRotate_R;

void Init();
void DrawMap();
void CreatePattern();
void CreateBlock();
void DropDawn();
void BlockToSpace();
void InputKey();
void RemoveLine();
void GameOver();
bool CheckCollision(int x, int y);

void gotoxy(int x, int y);

int main() {
	Init();
	CreatePattern();
	while (1) {			
		DrawMap();
		CreateBlock();
		DropDawn();
		BlockToSpace();
		RemoveLine();
		InputKey();		
		GameOver();
	}
}

void Init() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	srand(time(NULL));
}

void DrawMap() {
	for (int x = 0; x < Space_X; x++) {
		for (int y = 0; y < Space_Y; y++) {
			if (TetrisSpace[y][x] == 1) {
				gotoxy(x, y);
				puts("¬");
			}
			else if (TetrisSpace[y][x] == 2) {
				gotoxy(x, y);
				puts("＝");
			}
		}
	}
}

void CreatePattern() {
	BlockType_R = rand() % (TypeForm - 1);
	BlockRotate_R = rand() % (RotateForm - 1);	
}

void CreateBlock() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (PatternForm[BlockType_R][BlockRotate_R][y][x] == 1) {
				gotoxy(M_x + x, M_y + y);
				puts("＝");
			}
		}
	}
}

void DropDawn() {
	endT = clock();
	if((float)(endT - startT) >= 700){
	if (CheckCollision(M_x,M_y+1)) return;
	M_y++;
	startT = clock();
	system("cls");	
	}

}

void BlockToSpace() {
	if (CheckCollision(M_x, M_y + 1)) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (PatternForm[BlockType_R][BlockRotate_R][j][i] == 1)
					TetrisSpace[M_y + j][M_x + i] = 2;
			}
		}
		M_x = 4;
		M_y = 0;
		CreatePattern();
	}
}

bool CheckCollision(int x,int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (PatternForm[BlockType_R][BlockRotate_R][j][i] == 1) {
				int ts = TetrisSpace[y + j][x + i];
				if (ts == 1 || ts == 2)
					return true;
			}
		}
	}
	return false;
}

void InputKey() {
	if (_kbhit()) {
		switch (_getch()) {
		case SPACEBAR:
			if (!CheckCollision(M_x, M_y) && !CheckCollision(M_x-1, M_y) && !CheckCollision(M_x+1, M_y)) {
				BlockRotate_R = (BlockRotate_R +1) % RotateForm;
			}
			break;
		case DOWN:
			if (!CheckCollision(M_x, M_y+1)) {
				M_y++;
			}
			break;
		case LEFT:
			if (!CheckCollision(M_x-1, M_y)) {
				M_x--;
			}
			break;
		case RIGHT:
			if (!CheckCollision(M_x+1, M_y)) {
				M_x++;
			}
			break;
		}
		system("cls");
	}
}

void RemoveLine() {
	int count;
	for (int y = Space_Y - 1; y >=0 ; y--) {
		count = 0;
		for (int x = 1; x < Space_X - 1; x++) { // 1~10
			if (TetrisSpace[y][x] == 2)
				count++;			
		}
		if (count >= 10) {				
			for (int cy = y; cy >= 1; cy--) {
				for (int c = 1; c < Space_X - 1; c++)
					TetrisSpace[cy][c] = TetrisSpace[cy - 1][c];				
			}
		}
	}
}

void GameOver() {
	for (int i = 1; i < Space_X-1; i++) {
		if (TetrisSpace[0][i] == 2)
			while (1) {
				puts("蟹亜 せせせせせせせせせせせせせせせせせせせせせせせせせせせせせ");
				Sleep(0.5);
			}
				
	}
}

void gotoxy(int x, int y) {
	COORD pos = { x * 2,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}