#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <math.h>
#include <Windows.h>
#include<conio.h>
//#include <synchapi.h>
//#include <consoleapi2.h>

#define PI 3.14159265
#define CONST_GRASS				0
#define CONST_ROAD				1
#define CONST_TREE				2
#define CONST_PLANT				3
#define CONST_TANK				4
#define CONST_TANKED			5
#define CONST_ENEMY				6
#define CONST_SPAWNING_ENEMY	7
#define CONST_BULLET			8

#define CONST_MAP_RATIO			1
#define CONST_MAP_SIZE			400


struct struct_attack_machine {
	int damage;
	int distance;
	int x;
	int y;
	int lv;
	int upgrade_cost;
	int cost;
	int type;
	int frame=0;
};

struct struct_monster {
	int speed;
	int hp;
	int x;
	int y;
	int type=0;
	int max_hp;
	int frame=0;
};
struct struct_tower {
	int hp=100;
	int money=1000;
	char moneyC[20];
	char hpC[3];
};
struct struct_bullet
{
	int damage;
	int speed;
	int x;
	int y;


};
struct struct_path {
	int x;
	int y;
	int frame;
};
//HANDLE hConsole;
int map[400][400] = { 0 },monstercount=0;
struct struct_attack_machine tank[20];
struct struct_monster mob[3];
struct struct_tower user;
struct struct_bullet	bullet;
struct struct_path		basic_path[20];
int delay;
namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		
		mob[0].x = 10;
		mob[0].y = 20;
		mob[1].x = 50;
		mob[1].y = 100;
		mob[2].x = 70;
		mob[2].y = 10;

		tank[0].x = 200;
		tank[0].y = 400;

		bullet.x = 100;
		bullet.y = 0;

		for (int i = 1; i < CONST_MAP_SIZE - 1; i++) {
			map[0][i] = CONST_ROAD;
			map[i][0] = CONST_ROAD;
		}
		map[0][0] = CONST_ROAD;
		map[CONST_MAP_SIZE - 1][1] = CONST_SPAWNING_ENEMY;
		map[CONST_MAP_SIZE - 1][CONST_MAP_SIZE - 1] = CONST_TANKED;
		//map[10][12] = CONST_TANK;
		//map[6][18] = CONST_TANKED;
		//map[6][6] = CONST_TANKED;


		basic_path[0].x = 300;
		basic_path[0].y = 200;
		basic_path[0].frame = 5;
		basic_path[1].x = -100;
		basic_path[1].y = 100;
		basic_path[1].frame = 10;
		basic_path[2].x = 400;
		basic_path[2].y = 250;
		basic_path[2].frame = 15;
		basic_path[3].x = 0;
		basic_path[3].y = 0;
		basic_path[3].frame = 0;
		
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}
	

	static Sprite rotatingGun(new Surface("assets/aagun.tga"), 36);
	static Sprite rotatingBiggun(new Surface("assets/biggun.tga"), 32);
	static int frame = 0;

	void spawn_enemy(int x,int y) {
		map[x][y-1] = CONST_ENEMY;
		monstercount++;
	}
	void spawn_bullet(int x,int y) {
		map[x][y - 1] = CONST_BULLET;
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		system("cls");
		mob[0].speed = 2;
		delay++;
		int i, j,mis=0;
		char n=' ',a;
		screen->Clear(0);
		itoa(user.hp,user.hpC,10);
		itoa(user.money, user.moneyC, 10);
		//tank[0].x = 20;
		//tank[0].y = 20;
		tank[0].type = 1;
		screen->Print(user.hpC, CONST_MAP_SIZE * CONST_MAP_RATIO+10, 40, 0xffffff);
		screen->Print(user.moneyC, CONST_MAP_SIZE * CONST_MAP_RATIO+10, 20, 0xffffff);


		//±«¹°À» 3°³¸¦ ¸¸µì½Ã´Ù. ÁÂ·á ÀÔ·Â
		
		//±«¹° 3°³¸¦ ÂïÀ¾½Ã´Ù.
	
		

		if (_kbhit()) {
			n = getch();
		}

		for (i = 0; i < 3; i++) {
			rotatingBiggun.Draw(screen, mob[i].x, mob[i].y);
		}

		//³ª¸¦ ¸¸µç´Ù.
		if (n == 'd') {
			if (tank[0].x < 390) {
				tank[0].x += 10;
					
			}
		}

		else if (n == 'a') {
		if (tank[0].x > 10) {
				tank[0].x -= 10;
			}
		}

		else if (n == 'p') {
			if (bullet.y <= 0) {
				bullet.y = 400;
				bullet.x = tank[0].x+20;
			}
		}

		rotatingGun.Draw(screen, tank[0].x, tank[0].y);

		if (bullet.y > 0) {
			screen->Print("A", bullet.x, bullet.y, 0xffffff);
			bullet.y -= 20;
			//screen->Print("A", 10,10, 0xffffff);
			//rotatingGun.Draw(screen, bullet.x, bullet.y);
		}
		else {
			bullet.y = 0;
		}
		
		for (i = 0; i < 3; i++) {
			if ( (mob[i].x <= bullet.x) && (bullet.x <= (mob[i].x + 80)) ) {
				if( (mob[i].y <= bullet.y )&& (bullet.y <= (mob[i].y + 80)) ){
					bullet.y = 0;
					mob[i].x = 1000;
				}
			}
			if ((mob[i].x <= tank[0].x) && (tank[0].x <= (mob[i].x + 80))) {
				if ((mob[i].y <= tank[0].y) && (tank[0].y <= (mob[i].y + 80))) {
					
					exit(0);
				}
			}
		}

		for (i = 0; i < 3; i++) {
			if (mob[i].y == tank[0].y+150) {
				mob[i].y = 1;
			}
			else {
				mob[i].y += 1;
			}
		}
		

		/*
		for (i = 0; i < CONST_MAP_SIZE; i++) {
			for (j = 0; j < CONST_MAP_SIZE; j++) {
				if (CONST_TANKED == map[i][j]) {

					rotatingGun.SetFrame(1);
					//rotatingGun.Draw(screen, tank[0].x * 20, tank[0].y * 20);
					rotatingGun.Draw(screen, i * CONST_MAP_RATIO, j * CONST_MAP_RATIO);
				}
				if (CONST_TANK == map[i][j]) {
					//printf("a");
					rotatingGun.SetFrame(1);
					//rotatingGun.Draw(screen, tank[0].x * 20, tank[0].y * 20);
					rotatingGun.Draw(screen, i * CONST_MAP_RATIO, j * CONST_MAP_RATIO);
					if (delay % 30 == 0) {
						spawn_bullet(i, j);
					}
				}
				if (CONST_BULLET == map[i][j]) {

					screen->Print("A", i * CONST_MAP_RATIO, j * CONST_MAP_RATIO, 0xffffff);
					if (map[i][j - 1] == CONST_GRASS) {
						map[i][j - 1] = CONST_BULLET;
						map[i][j] = CONST_GRASS;
					}
					else if(map[i][j - 1] == CONST_ENEMY) {
						map[i][j - 1] = CONST_ROAD;
					}
					else {
						map[i][j] = CONST_GRASS;
					}
					

					
				}
				if (map[i][j] == CONST_SPAWNING_ENEMY) {
					screen->Print("D", i * CONST_MAP_RATIO, j * CONST_MAP_RATIO, 0xffffff);
					if (delay % 100 == 0) {
						spawn_enemy(i, j);
					}
					//printf("b");

				}
				if (CONST_ENEMY == map[i][j]) {
					
					rotatingBiggun.Draw(screen, i * CONST_MAP_RATIO, j * CONST_MAP_RATIO);
					if (delay % 2== 0) {
						if (map[i - mob[0].speed][j] == CONST_ROAD) {
							map[i - mob[0].speed][j] = CONST_ENEMY;
							map[i][j] = CONST_ROAD;
						}
						else if (map[i][j + mob[0].speed] == CONST_ROAD) {
							map[i][j + mob[0].speed] = CONST_ENEMY;
							map[i][j] = CONST_ROAD;
						}
					}
					
				}
				if (CONST_ROAD == map[i][j]) {

					screen->Print("R", i * CONST_MAP_RATIO, j * CONST_MAP_RATIO, 0xffffff);
					

				}
			}
		}
		if (_kbhit()) {
			n = getch();
		}
		
		for (i = 0; i < CONST_MAP_SIZE; i++) {
			for (j = 0; j < CONST_MAP_SIZE; j++) {
				if (CONST_TANKED == map[i][j]) {
					if (n == 'w') {
						if (j >1) {
							map[i][j-1] = CONST_TANKED;
							map[i][j] = 0;
						}
						
					}
					else if (n == 's') {
						if (j < CONST_MAP_SIZE - 1) {
							map[i][j+1] = CONST_TANKED;
							map[i][j] = 0;
						}
						
					}
					else if (n == 'd') {
						if (i < CONST_MAP_SIZE - 1) {
							//printf("ads");
							map[i+1][j] = CONST_TANKED;
							map[i][j] = 0;
						}
			
						
					}
					else if (n == 'a') {
						if (i >1) {
							map[i-1][j] = CONST_TANKED;
							map[i][j] = 0;
						}
						
					}
					else if (n == 'z') {
						if (i > 0) {
							
							map[i][j] = CONST_TANK;
						}

					}
					mis = 1;
					n = ' ';
				}
				else if (i == CONST_MAP_SIZE - 1 && j == CONST_MAP_SIZE - 1 &&mis==0) {
					if (n == 'p') {
						map[CONST_MAP_SIZE - 1][CONST_MAP_SIZE - 1] = CONST_TANKED;
					}
				}
			}
		}
		*/
		//mob[0].x = 200;
		//mob[0].y = 700;

		/*
		tank[0].frame = atan2(tank[0].y-mob[0].x, -mob[0].x) * (18/PI);
		if (tank[0].frame < 0) {
			tank[0].frame=36 + tank[0].frame; 
		}
		*/
		//mob[0].frame = 27;

		// clear the graphics window

		// print something in the graphics window
		//screen->Print("12345678910111213141516171819202122232425262728293031323334353637383940", 2, 2, 0xffffff);
		// print something to the text window
		//printf("%d\n",tank[0].frame);
		// draw a sprite
		/*
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				if (map[i][j] == CONST_GRASS) {
					for (int k = 0; k < 80; k++) {
						for (int l = 0; l < 80; l++) {
							//screen->Print("A", i * 20 + k, j * 20 + l, 0xffffff);
						}
					}
				}
				else if (map[i][j] == CONST_SPAWNING_ENEMY) {
					for (int k = 0; k < 80; k++) {
						for (int l = 0; l < 80; l++) {
							screen->Print("A", i * 20 + k, j * 20 + l, 0xffffff);
						}
					}
					if (monstercount < 40) {
						spawn_enemy(i, j);
					}
					

				}
			
				
			}
		}
		*/
		/*for (i = 0; i < 50; i++) {
			rotatingBiggun.Draw(screen, mob[i].x, mob[i].y);
		}*/
		//rotatingGun.SetFrame(tank[0].frame);
		//rotatingGun.Draw(screen, tank[0].x , tank[0].y );
		/*
		for (int i = 0; i < 20; i++) {
			if (tank[i].type == 0) {
				break;
			}
			else {
				if (tank[i].type == 1) {
					rotatingGun.SetFrame(tank[0].frame);
					rotatingGun.Draw(screen, tank[0].x*20, tank[0].y*20);
				}
			}
		}
		*/
		//rotatingBiggun.SetFrame(mob[0].frame);
		
		


		//if (++frame >= 32) frame = 0;
		//Sleep(100);
	}
};