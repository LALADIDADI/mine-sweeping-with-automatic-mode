#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<vector>
#include<queue>

#define EASY 30
#define NORMAL 50
#define HARD 100
#define HELL 200
#define PC_P 1000

void Gameinit(int,int);
void DrawMap(int,int);
int PlayGame();
void diffusion(int, int);
void GameStart();
void mapShow(int,int,int);
void Put_mine(int, int, int);
int Paint_loop(HWND);
void taunt();
int pc_play(int, int);
void pc_diffusion(int, int);
int is_mine(int, int);
void is_zero(int, int);
void is_true(int, int);
int is_guess(int, int);


