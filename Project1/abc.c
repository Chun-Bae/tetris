#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>

#define BLOCK_SIZE 4
#define BLOCK_COUNT 7

#define CENTER_X 5
#define CENTER_Y 3

#define SPACEBAR 32
#define LEFT 75
#define RIGHT 77
#define DOWN 80

int space[22][12] = {
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1},
};

typedef struct point {
	int x;
	int y;
}Point;


typedef struct block {
	Point pos[BLOCK_SIZE];
	bool collision;
	int current_shape;
	int diretion;
	int dawn_y;
	int transit_x;
}Block;

bool addblock_is = false;


void gotoxy(int x, int y);
void start_block();
Block block_make(Block block);
void wall();
void addblock(Block block);
Block block_autodown(Block block);
Block block_spin(Block block);
Block key_down(Block block);
Block block_move_L(Block block);
Block block_move_D(Block block);
Block block_move_R(Block block);
Block block_collision(Block block);

const static Point BlockShape[BLOCK_COUNT][BLOCK_SIZE][BLOCK_SIZE] = {
	//бсбсбсбс
	{
		{{9,4}, {10,4}, {11,4}, {12,4}},
		{{10,4}, {10,3}, {10,2}, {10,1}},
		{{9,4}, {10,4}, {11,4}, {12,4}},
		{{10,4}, {10,3}, {10,2}, {10,1}}
	},
	//бсбс
	//бсбс
	{
		{{10,2}, {11,2}, {10,3}, {11,3}},
		{{10,2}, {11,2}, {10,3}, {11,3}},
		{{10,2}, {11,2}, {10,3}, {11,3}},
		{{10,2}, {11,2}, {10,3}, {11,3}},
	},	
	//брбсбр
	//бсбсбс
	{
		{{9,4}, {10,4}, {11,4}, {10,3}},
		{{11,4}, {11,3}, {11,2}, {10,3}},
		{{9,2}, {10,2}, {11,2}, {10,3}},
		{{9,2}, {9,3}, {9,4}, {10,3}},
	},
	//бсбрбр     
	//бсбсбс
	{
		{{9,2}, {9,3}, {10,3}, {11,3}},
		{{9,4}, {10,4}, {10,3}, {10,2}},
		{{9,3}, {10,3}, {11,3}, {11,4}},
		{{10,4}, {10,3}, {10,2}, {11,2}},
	},
	//брбрбс    
	//бсбсбс
	{
		{{9,2}, {9,3}, {10,3}, {11,3}},
		{{9,4}, {10,4}, {10,3}, {10,2}},
		{{9,3}, {10,3}, {11,3}, {11,4}},
		{{10,4}, {10,3}, {10,2}, {11,2}},
	},
	//брбсбс    
	//бсбсбр
	{
		{{10,3}, {11,3}, {11,2}, {12,2}},
		{{10,1}, {10,2}, {11,2}, {11,3}},
		{{10,3}, {11,3}, {11,2}, {12,2}},
		{{10,1}, {10,2}, {11,2}, {11,3}},
	},
	//бсбсбр    
	//брбсбс
	{
		{{9,2}, {10,2}, {10,3}, {11,3}},
		{{10,3}, {10,2}, {11,2}, {11,1}},
		{{9,2}, {10,2}, {10,3}, {11,3}},
		{{10,3}, {10,2}, {11,2}, {11,1}},
	},
};

void main() {
	wall();
	start_block();	
}

void start_block() {
	Block block;	
	while(TRUE){			
		block.collision = false;
		block.transit_x = 0;
		block.dawn_y = 0;
		block = block_make(block);
		while (!block.collision) {		
			block = key_down(block);
			block = block_autodown(block);			
			system("cls");
			wall();
			for (int i = 0; i < BLOCK_SIZE; i++) {
				gotoxy(block.pos[i].x, block.pos[i].y);
				puts("бс");
			}
			if (addblock_is) {
				addblock(block); 
				addblock_is = false;
			}
		}		
	}
}

Block key_down(Block block) {
	clock_t startT = clock();
	clock_t endT = clock();
	
	while ((float)(endT - startT) < 700) {
		Sleep(150);
		if (_kbhit()) {
			switch (_getch()) {
			case SPACEBAR:				
				block = block_spin(block);
				system("cls");
				wall();
				for (int i = 0; i < BLOCK_SIZE; i++) {
					gotoxy(block.pos[i].x, block.pos[i].y);
					puts("бс");
				}
					break;
			case LEFT:
				block = block_move_L(block);
				system("cls");
				wall();
				for (int i = 0; i < BLOCK_SIZE; i++) {
					gotoxy(block.pos[i].x, block.pos[i].y);
					puts("бс");
				}
				break;
			case DOWN:
				block = block_move_D(block);
				system("cls");
				wall();
				for (int i = 0; i < BLOCK_SIZE; i++) {
					gotoxy(block.pos[i].x, block.pos[i].y);
					puts("бс");
				}
				break;
			case RIGHT:
				block = block_move_R(block);
				system("cls");
				wall();
				for (int i = 0; i < BLOCK_SIZE; i++) {
					gotoxy(block.pos[i].x, block.pos[i].y);
					puts("бс");
				}
				break;
				
			}
		}
		endT = clock();		
	}
	return block;
}
Block block_move_L(Block block) {
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if (space[block.pos[i].y][block.pos[i].x - CENTER_X - 1] == 1 || space[block.pos[i].y][block.pos[i].x - CENTER_X - 1] == 2)
			return block;
		else
			block.pos[i].x--;				
	}
	block.transit_x--;
	return block;
}
Block block_move_D(Block block) {
	bool exist[4];
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if (space[block.pos[i].y][block.pos[i].x - CENTER_X + 1] == 1 || space[block.pos[i].y][block.pos[i].x - CENTER_X + 1] == 2)
			exist[i] = true;
		else
			exist[i] = false;
	}
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if (exist[0] && exist[1] && exist[2] && exist[3]) {
			addblock_is = true;
			block.collision = true;
			return block;
		}
		else {
			block.pos[i].y++;
			block.dawn_y = block.pos[i].y;
		}		
	}
	return block;
}
Block block_move_R(Block block) {
	bool exist[4];
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if (space[block.pos[i].y][block.pos[i].x - CENTER_X + 1] == 1 || space[block.pos[i].y][block.pos[i].x - CENTER_X + 1] == 2)
			exist[i] = false;
		else
			exist[i] = true;
	}
	if (exist[0] && exist[1] && exist[2] && exist[3]){
		for (int i = 0; i < BLOCK_SIZE; i++) {
			block.pos[i].x++;			
		}
		block.transit_x++;
	}
	else 
		return block;
	return block;
}

Block block_collision(Block block) {	
	for (int i = 0; i < 4; i++) {
		if (space[block.pos[i].y][block.pos[i].x - CENTER_X + 1] == 1 || space[block.pos[i].y][block.pos[i].x - CENTER_X + 1] == 2) {
			addblock_is = true;
			block.collision = true;
		}
	}
	return block;
}

Block block_autodown(Block block) {		
	block = block_collision(block);
	
	for (int i = 0; i < 4; i++) {
		block.pos[i].y++;
	}
	block.dawn_y++;

	return block;
}

Block block_spin(Block block) {
	block.diretion= (block.diretion + 1) % 4;
	int bpx, bpy;
	for (int i = 0; i < BLOCK_SIZE; i++) {
		bpx = BlockShape[block.current_shape][block.diretion][i].x;
		bpy = BlockShape[block.current_shape][block.diretion][i].y;	
		bpx += block.transit_x;
		bpy += block.dawn_y;
		if (space[bpy][bpx - CENTER_X] == 1 || space[bpy][bpx - CENTER_X] == 2)
			return block;
		else {
			block.pos[i].x = bpx;
			block.pos[i].y = bpy;
		}
	}	
	
	return block;
}

void addblock(Block block) {
	for (int i = 0; i < BLOCK_SIZE; i++) {
		space[block.pos[i].y-2][block.pos[i].x - 5] = 2;
	}
}

Block block_make(Block block) {
	block.current_shape = rand() % BLOCK_COUNT;
	block.diretion = rand() % BLOCK_SIZE;

	for (int i = 0; i < BLOCK_SIZE; i++) {
		block.pos[i].x = BlockShape[block.current_shape][block.diretion][i].x;
		block.pos[i].y = BlockShape[block.current_shape][block.diretion][i].y;
		gotoxy(block.pos[i].x, block.pos[i].y);
		puts("бс");
	}
	return block;
}

void wall() {		
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 22; j++) {
			if (space[j][i] == 1) {
				gotoxy(CENTER_X + i, CENTER_Y + j);
				puts("в╠");
			}
			else if (space[j][i] == 2) {
				gotoxy(CENTER_X + i, CENTER_Y + j);
				puts("бс");
			}			
		}
	}	
}

void gotoxy(int x, int y) {
	COORD pos = { x*2,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}