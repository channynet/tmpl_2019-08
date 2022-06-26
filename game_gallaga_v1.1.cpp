#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <math.h>
#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <iostream>
#include "jute.h"
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
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
#define CONST_MAX_BULLET		1000
#define CONST_MAX_ENEMY_BULLET	1000
#define CONST_MOB_COUNT			16
#define CONST_ITEM_COUNT		10


struct struct_attack_machine {
	int damage;
	int distance;
	int x;
	int y;

	int damage_lv;
	int bullut_speed_lv;
	int tank_speed_lv;
	int guided_lv;

	int upgrader = 0;
	int upgrade_xp;
	int xp;
	int type;
	int frame = 0;
	int delay_time = 0;
};

struct struct_monster {
	int speed;
	int hp;
	int x;
	int y;
	int type = 0;
	int max_hp;
	int frame = 0;
	int path = 0;
	int enable = 1;
};
struct struct_tower {
	int hp = 100;
	int money = 1000;
	char moneyC[20];
	char hpC[3];
	int score = 0;
	char scoreC[20];
	
};
struct deadexplosion {
	int x;
	int y;
	int frame;
	int enable = 0;
};
struct struct_bullet
{
	int damage;
	int speed;
	int x;
	int y;
	int enable;
	int xoffset;
};
struct struct_path {
	int x;
	int y;
	int frame;
};
struct struct_item {
	int x;
	int y;
	int type;
	int enable;
};
//HANDLE hConsole;
int gamemap[400][400] = { 0 }, monstercount = 0;
struct struct_attack_machine tank[20];
struct struct_monster mob[CONST_MOB_COUNT];
struct struct_tower user;
struct struct_bullet	bullet[CONST_MAX_BULLET];
struct struct_bullet	enemy_bullet[CONST_MAX_ENEMY_BULLET];
struct struct_path		basic_path[8];
struct struct_item		item[CONST_ITEM_COUNT];
struct deadexplosion explode[CONST_MOB_COUNT];
int delay, bullet_index = 0,reload_delay=0;
int printer=0;

using namespace std;


namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		for (int i = 0; i < CONST_MOB_COUNT; i++) {
			mob[i].hp = 5;
			mob[i].path = 0;
		}
		for (int i = 0; i < 4; i++) {
			mob[i].y = 20;
		}
		for (int i = 4; i < 10; i++) {
			mob[i].y = 60;
		}
		for (int i = 10; i < 16; i++) {
			mob[i].y = 100;
		}
		mob[0].x = 180;
		mob[1].x = 200;
		mob[2].x = 220;
		mob[3].x = 240;
		mob[4].x = 160;
		mob[5].x = 180;
		mob[6].x = 200;
		mob[7].x = 220;
		mob[8].x = 240;
		mob[9].x = 260;
		mob[10].x = 160;
		mob[11].x = 180;
		mob[12].x = 200;
		mob[13].x = 220;
		mob[14].x = 240;
		mob[15].x = 260;
		
		tank[0].upgrader = 0;

		tank[0].x = 200;
		tank[0].y = 450;


		for (int i = 0; i < CONST_MAX_BULLET; i++) {
			bullet[i].damage = 5;
			bullet[i].x = 100;
			bullet[i].y = 0;
			bullet[i].enable = 0;
			bullet[i].y = 0;
		}
		for (int i = 1; i < CONST_MAP_SIZE - 1; i++) {
			gamemap[0][i] = CONST_ROAD;
			gamemap[i][0] = CONST_ROAD;
		}
		gamemap[0][0] = CONST_ROAD;
		gamemap[CONST_MAP_SIZE - 1][1] = CONST_SPAWNING_ENEMY;
		gamemap[CONST_MAP_SIZE - 1][CONST_MAP_SIZE - 1] = CONST_TANKED;
		//gamemap[10][12] = CONST_TANK;
		//gamemap[6][18] = CONST_TANKED;
		//gamemap[6][6] = CONST_TANKED;
		tank[0].bullut_speed_lv = 0;
		tank[0].tank_speed_lv = 0;
		tank[0].damage_lv = 0;
		tank[0].guided_lv = 0;
		tank[0].upgrade_xp = 30;
		srand(time(NULL));
		basic_path[0].x = 10;
		basic_path[0].y = 7;
		basic_path[1].x = 10;
		basic_path[1].y = 7;
		basic_path[2].x = 10;
		basic_path[2].y = 7;
		basic_path[3].x = 10;
		basic_path[3].y = 7;
		basic_path[4].x = -10;
		basic_path[4].y = 7;
		basic_path[5].x = -10;
		basic_path[5].y = 7;
		basic_path[6].x = -10;
		basic_path[6].y = 7;
		basic_path[7].x = -10;
		basic_path[7].y = 7;
		basic_path[0].frame = 5;		
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}


	static Sprite rotatingGun(new Surface("assets/aagun.tga"), 36);
	static Sprite rotatingBiggun(new Surface("assets/biggun.tga"), 32);
	static Sprite explosion(new Surface("assets/smoke.tga"), 10);
	static int frame = 0;

	void spawn_enemy(int x, int y) {
		gamemap[x][y - 1] = CONST_ENEMY;
		monstercount++;
	}
	void spawn_bullet(int x, int y) {
		gamemap[x][y - 1] = CONST_BULLET;
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------


	void Game::Tick(float deltaTime)
	{
		
		if (tank[0].upgrader >= 0 && tank[0].upgrader <= 3) {
			tank[0].delay_time = 12;
		}
		if (tank[0].upgrader >= 4 && tank[0].upgrader <= 8) {
			tank[0].delay_time = 9;
		}
		reload_delay++;
		if (reload_delay > tank[0].delay_time-tank[0].bullut_speed_lv) {
			reload_delay = tank[0].delay_time - tank[0].bullut_speed_lv;
		}
		
		system("cls");
		
		mob[0].speed = 2;
		delay++;
		int i, j, mis = 0;
		char n = ' ', a;
		screen->Clear(0);
		screen->Line(500, 0, 500, 512, 0xffffff);
		itoa(user.score, user.scoreC, 10);
		printf("%d    ",reload_delay);
		//tank[0].x = 20;
		//tank[0].y = 20;
		tank[0].type = 1;
		
		screen->Print("score", 520, 40, 0xffffff);
		screen->Print(user.scoreC, 570, 40, 0xffffff);
		screen->Print("bullet damage", 520, 70, 0xffffff);
		screen->Print("bullet reload", 520, 90, 0xffffff);
		screen->Print("bullet guided", 520, 110, 0xffffff);
		screen->Print("tank movement", 520, 130, 0xffffff);
		if (delay % 1 == 0) {
			for (int ei = 0; ei < CONST_MOB_COUNT; ei++) {
				if (explode[ei].enable == 1) {
					
					explode[ei].frame++;
					if (explode[ei].frame >= 10) {
						explode[ei].enable = 0;
					}
				}
			}
		}
		
		for (int ei = 0; ei < CONST_MOB_COUNT; ei++) {
			if (explode[ei].enable == 1) {
				explosion.SetFrame(explode[ei].frame);
				
				
				explosion.Draw(screen, explode[ei].x+12, explode[ei].y+25);
				explosion.Draw(screen, explode[ei].x + 12, explode[ei].y + 15);
				explosion.Draw(screen, explode[ei].x + 12, explode[ei].y + 35);
				explosion.Draw(screen, explode[ei].x + 22, explode[ei].y + 25);
				explosion.Draw(screen, explode[ei].x + 2, explode[ei].y + 25);
				//printf("a!!!!!!!!!!!!!!!!!!!!!!");
				
				
			}
		}
		


		for (int j = 0; j < 4; j++) {
			if (j == 0) {
				for (int i = 0; i < tank[0].damage_lv; i++) {

					screen->Bar(610 + i * 10, j * 20 + 70, 617 + i * 10, j * 20 + 73, 0xffffff);
				}
			}
			if (j == 1) {
				for (int i = 0; i < tank[0].bullut_speed_lv; i++) {

					screen->Bar(610 + i * 10, j * 20 + 70, 617 + i * 10, j * 20 + 73, 0xffffff);
				}
			}
			if (j == 2) {
				for (int i = 0; i < tank[0].guided_lv; i++) {

					screen->Bar(610 + i * 10, j * 20 + 70, 617 + i * 10, j * 20 + 73, 0xffffff);
				}
			}
			if (j == 3) {
				for (int i = 0; i < tank[0].tank_speed_lv; i++) {

					screen->Bar(610 + i * 10, j * 20 + 70, 617 + i * 10, j * 20 + 73, 0xffffff);
				}
			}

		}


		//±«¹°À» 3°³¸¦ ¸¸µì½Ã´Ù. ÁÂ·áÇ¥ ÀÔ·Â

		//±«¹° 3°³¸¦ ÂïÀ¾½Ã´Ù.

		for (int itemC = 0; itemC< CONST_ITEM_COUNT; itemC++) {
			if (item[itemC].y > 500) {
				item[itemC].enable = 0;
				item[itemC].y = 10;
			}
			if (item[itemC].enable == 1) {
				if (item[itemC].type == 0) {
					screen->Print("Upgrader", item[itemC].x, item[itemC].y, 0xffffff);
				}
				if (item[itemC].type == 1) {
					screen->Print("Twin", item[itemC].x, item[itemC].y, 0xffffff);
				}

				if (item[itemC].type == 2) {
					screen->Print("Machinegun", item[itemC].x, item[itemC].y, 0xffffff);
				}

				screen->Print("T", item[itemC].x , item[itemC].y, 0xffffff);
				item[itemC].y += 10;
				if ((tank[0].x-40 <=item[itemC].x) && (item[itemC].x <= (tank[0].x + 40))) {
					if ((tank[0].y <= item[itemC].y) && (tank[0].y <= (tank[itemC].y + 40))) {

						item[itemC].y = 10;
						item[itemC].enable = 0;
						if (item[itemC].type == 0) {
							if (tank[0].upgrader >= 1 && tank[0].upgrader < 3 || tank[0].upgrader >= 4 && tank[0].upgrader < 6) {
								tank[0].upgrader++;
							}
						}
						if (item[itemC].type == 1) {
							tank[0].upgrader = 1;
						}
						if (item[itemC].type == 2) {
							tank[0].upgrader = 4;
						}
					
					}
				}
			}
		}
		printf("upgrade - %d", tank[0].upgrader);
		
		printf("           %d         ", delay);




		for (i = 0; i < CONST_MOB_COUNT; i++) {
			if (mob[i].enable == 1) {
				//rotatingBiggun.Draw(screen, mob[i].x, mob[i].y);
				rotatingBiggun.DrawScaled(mob[i].x, mob[i].y, 50, 50, screen);
				if (mob[i].hp <= 0) {

					mob[i].enable = 0;
					user.score += 200;

					if (user.score % 1000 == 0) {
						tank[0].upgrade_xp++;
					}

					for (int ei = 0; ei < CONST_MOB_COUNT; ei++) {
						if (explode[ei].enable == 0) {
							explode[ei].x = mob[i].x;
							explode[ei].y = mob[i].y;
							explode[ei].frame = 0;
							explode[ei].enable = 1;
							break;
						}
					}
					
				}
			}
		}

		if (GetAsyncKeyState(VK_RIGHT)) {

			if (tank[0].x < 460) {
				tank[0].x += 10 + tank[0].tank_speed_lv * 5;

			}

		}
		if (GetAsyncKeyState(VK_LEFT)) {

			if (tank[0].x > 10) {
				tank[0].x -= 10 + tank[0].tank_speed_lv * 5;
			}

		}
		if (GetAsyncKeyState(VK_SPACE)) {
			int random;


			if (tank[0].upgrader == 0) {

				if (reload_delay % (12 - tank[0].bullut_speed_lv) == 0) {
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							bullet[bi].xoffset = 0;
							reload_delay = 0;
							break;
							
						}

					}
				}
			}
			else if (tank[0].upgrader == 1) {
				if (reload_delay % (12 - tank[0].bullut_speed_lv) == 0) {
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 16;
							bullet[bi].xoffset = 0;
							reload_delay = 0;
							break;
						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 24;
							bullet[bi].xoffset = 0;
							
							break;
						}

					}
				}
			}
			else if (tank[0].upgrader == 2) {
				if (reload_delay % (12 - tank[0].bullut_speed_lv) == 0) {
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 12;
							bullet[bi].xoffset = 0;
							reload_delay = 0;
							break;
						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 28;
							bullet[bi].xoffset = 0;
							break;
						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							bullet[bi].xoffset = 0;
							break;
						}

					}
				}
			}
			else if (tank[0].upgrader == 3) {
				if (reload_delay % (12 - tank[0].bullut_speed_lv) == 0) {
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							bullet[bi].xoffset = -2;
							reload_delay = 0;
							break;
						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							bullet[bi].xoffset = 2;
							break;
						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							bullet[bi].xoffset = 0;
							break;
						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							bullet[bi].xoffset = -4;
							break;
						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							bullet[bi].xoffset = 4;
							break;
						}

					}
				}
			}
			if (tank[0].upgrader == 4) {

				if (reload_delay % (9 - tank[0].bullut_speed_lv) == 0) {
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							random = rand() % 7 + 0;
							random = random - 4;
							bullet[bi].xoffset = random;
							
							reload_delay = 0;
							break;

						}

					}
				}
			}
			if (tank[0].upgrader == 5) {

				if (reload_delay % (9 - tank[0].bullut_speed_lv) == 0) {
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							random = rand() % 7 + 0;
							random = random - 4;
							bullet[bi].xoffset = random;

							reload_delay = 0;
							break;

						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							random = rand() % 7 + 0;
							random = random - 4;
							bullet[bi].xoffset = random;

							reload_delay = 0;
							break;

						}

					}
				}
			}
			if (tank[0].upgrader == 6) {

				if (reload_delay % (9 - tank[0].bullut_speed_lv) == 0) {
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							random = rand() % 7 + 0;
							random = random - 4;
							bullet[bi].xoffset = random;

							reload_delay = 0;
							break;

						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							random = rand() % 7 + 0;
							random = random - 4;
							bullet[bi].xoffset = random;

							reload_delay = 0;
							break;

						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							
							bullet[bi].xoffset = 0;

							reload_delay = 0;
							break;

						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							random = rand() % 7 + 0;
							random = random - 4;
							bullet[bi].xoffset = random;

							reload_delay = 0;
							break;

						}

					}
					for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {

						if (bullet[bi].enable == 0) {

							bullet[bi].enable = 1;
							bullet[bi].y = 450;
							bullet[bi].x = tank[0].x + 20;
							random = rand() % 7 + 0;
							random = random - 4;
							bullet[bi].xoffset = random;

							reload_delay = 0;
							break;

						}

					}

				}
			}

		}
		if (GetAsyncKeyState('Z')) {
			if (tank[0].upgrade_xp != 0) {
				if (tank[0].damage_lv < 7) {
					tank[0].upgrade_xp--;
					tank[0].damage_lv++;
				}
			}
		}

		if (GetAsyncKeyState('X')) {

			if (tank[0].upgrade_xp != 0) {
				if (tank[0].bullut_speed_lv < 7) {
					tank[0].upgrade_xp--;
					tank[0].bullut_speed_lv++;
				}
			}

		}
		if (GetAsyncKeyState('C')) {

			if (tank[0].upgrade_xp != 0) {
				if (tank[0].guided_lv < 7) {
					tank[0].upgrade_xp--;
					tank[0].guided_lv++;
				}
			}

		}
		if (GetAsyncKeyState('V')) {

			if (tank[0].upgrade_xp != 0) {
				if (tank[0].tank_speed_lv < 7) {
					tank[0].upgrade_xp--;
					tank[0].tank_speed_lv++;
				}
			}

		}
		for (int ebi = 0; ebi < CONST_MAX_ENEMY_BULLET;ebi++) {
			if (enemy_bullet[ebi].y > 500) {
				//printf("[ebi]= %d\n", ebi);
				enemy_bullet[ebi].enable = 0;
			}
			
			if (enemy_bullet[ebi].enable == 1) {
				screen->Print("V", enemy_bullet[ebi].x, enemy_bullet[ebi].y, 0xffffff);
				enemy_bullet[ebi].y += 10;

				
				if ((tank[0].x - 20 <= enemy_bullet[ebi].x) && (enemy_bullet[ebi].x <= (tank[0].x + 20))) {
					if ((tank[0].y <= enemy_bullet[ebi].y) && (tank[0].y <= (enemy_bullet[ebi].y + 20))) {
						exit(0);
					}
				}
				
			}
			
		}
		/*
		if (_kbhit()) {
			n = getch();
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
				bullet.x = tank[0].x + 20;
			}
		}
		*/
		rotatingGun.Draw(screen, tank[0].x, tank[0].y);

		for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {
			if (bullet[bi].enable == 1) {
				if (bullet[bi].y <= 0) {
					bullet[bi].y = 0;
					bullet[bi].enable = 0;
				}
				else
				{
					screen->Print("*", bullet[bi].x, bullet[bi].y, 0xffffff);
					bullet[bi].y -= 20;
					bullet[bi].x += bullet[bi].xoffset;

				}

				for (i = CONST_MOB_COUNT; i >=0; i--) {
					if (bullet[bi].y > mob[i].y) {
						if (mob[i].enable == 1) {
							
							if (mob[i].x - bullet[bi].y < 0) {
								bullet[bi].x -= tank[0].guided_lv;
							}
							if (mob[i].x - bullet[bi].y > 0) {
								bullet[bi].x += tank[0].guided_lv;
							}
							
						}
						
					}
				}
			}

			//screen->Print("A", 10,10, 0xffffff);
			//rotatingGun.Draw(screen, bullet.x, bullet.y);
			else {
				bullet[bi].y = 0;
			}
		}
		char hpChar[20];
		for (int i = 0; i < CONST_MOB_COUNT; i++) {
			printf("%d ", mob[i].y);
			if (mob[i].enable == 1) {
				if (mob[i].y - 1 > 0 && mob[i].y < 500 && mob[i].hp>0) {
					//itoa(mob[i].hp, hpChar, 10);
					//screen->Print(hpChar, mob[i].x + 20, mob[i].y - 1, 0xffffff);
				}
			}

		}
		
		for (int bi = 0; bi < CONST_MAX_BULLET; bi++) {
			if (bullet[bi].enable == 1) {
				for (i = 0; i < CONST_MOB_COUNT; i++) {
					if (mob[i].enable == 1) { 
						if ((mob[i].x <= bullet[bi].x) && (bullet[bi].x <= (mob[i].x + 40))) {
							if ((mob[i].y <= bullet[bi].y) && (bullet[bi].y <= (mob[i].y + 40))) {
								
								int random = rand() % 20 + 1;
								printer=random;
								if (random == 2) {
									if (tank[0].upgrader == 0) {
										random = rand() % 2 + 1;
											if (random == 1) {
												item[0].enable = 1;
												item[0].x = mob[i].x;
												item[0].y = mob[i].y;
												item[0].type = 1;
											}
											else {
												item[0].enable = 1;
												item[0].x = mob[i].x;
												item[0].y = mob[i].y;
												item[0].type = 2;
											}
										
										
									}
									else {
										item[0].enable = 1;
										item[0].x = mob[i].x;
										item[0].y = mob[i].y;
										item[0].type = 0;

									}
								}
								
								bullet[bi].y = 0;
								bullet[bi].enable = 0;

								mob[i].hp -= bullet[bi].damage + tank[0].damage_lv * 20;
							}
						}
						if ((mob[i].x <= tank[0].x) && (tank[0].x <= (mob[i].x + 40))) {
							if ((mob[i].y <= tank[0].y) && (tank[0].y <= (mob[i].y + 40))) {

								exit(0);
							}
						}
					}
				}
			}
		}
		printf("ad- %d", printer);

		int delayPer;
		printf("%d %d %d", mob[0].hp, mob[1].hp, mob[2].hp);
		for (int i = 0; i < CONST_MOB_COUNT; i++) {
			if (mob[i].enable == 1) {
				if (mob[i].y > 550) {
					mob[i].y = 0;
				}
				/*
				if (mob[i].path == 0) {
					delayPer = delay % 64;
					
					if (delayPer < 32) {
						mob[i].x -= 1;
					}
					if (delayPer >= 32) {
						mob[i].x += 1;
					}
					if (delayPer == 32) {
						mob[i].y+=5;
					}
					if (delayPer == 64) {
						mob[i].y+=5;
					}


				}*/
				if (mob[i].path == 1) {
					delayPer = delay % 8;
					mob[i].x += basic_path[delayPer].x;
					mob[i].y += basic_path[delayPer].y;


				}
			}
		}
		for (int i = 0; i < CONST_MOB_COUNT;i++) {
			int random = rand() % 40 + 1;
			if (random == 1) {
				for (int ebi = 0; ebi < CONST_MAX_ENEMY_BULLET; ebi++) {
					if (enemy_bullet[ebi].enable == 0) {
						if (mob[i].enable == 1) {
							enemy_bullet[ebi].y = mob[i].y + 20;
							enemy_bullet[ebi].x = mob[i].x + 5;
							enemy_bullet[ebi].enable = 1;
						}
						

					}
				
				}

			}
			
			
		}
		
		/*
		for (i = 0; i < CONST_MAP_SIZE; i++) {
			for (j = 0; j < CONST_MAP_SIZE; j++) {
				if (CONST_TANKED == gamemap[i][j]) {

					rotatingGun.SetFrame(1);
					//rotatingGun.Draw(screen, tank[0].x * 20, tank[0].y * 20);
					rotatingGun.Draw(screen, i * CONST_MAP_RATIO, j * CONST_MAP_RATIO);
				}
				if (CONST_TANK == gamemap[i][j]) {
					//printf("a");
					rotatingGun.SetFrame(1);
					//rotatingGun.Draw(screen, tank[0].x * 20, tank[0].y * 20);
					rotatingGun.Draw(screen, i * CONST_MAP_RATIO, j * CONST_MAP_RATIO);
					if (delay % 30 == 0) {
						spawn_bullet(i, j);
					}
				}
				if (CONST_BULLET == gamemap[i][j]) {

					screen->Print("A", i * CONST_MAP_RATIO, j * CONST_MAP_RATIO, 0xffffff);
					if (gamemap[i][j - 1] == CONST_GRASS) {
						gamemap[i][j - 1] = CONST_BULLET;
						gamemap[i][j] = CONST_GRASS;
					}
					else if(gamemap[i][j - 1] == CONST_ENEMY) {
						gamemap[i][j - 1] = CONST_ROAD;
					}
					else {
						gamemap[i][j] = CONST_GRASS;
					}



				}
				if (gamemap[i][j] == CONST_SPAWNING_ENEMY) {
					screen->Print("D", i * CONST_MAP_RATIO, j * CONST_MAP_RATIO, 0xffffff);
					if (delay % 100 == 0) {
						spawn_enemy(i, j);
					}
					//printf("b");

				}
				if (CONST_ENEMY == gamemap[i][j]) {

					rotatingBiggun.Draw(screen, i * CONST_MAP_RATIO, j * CONST_MAP_RATIO);
					if (delay % 2== 0) {
						if (gamemap[i - mob[0].speed][j] == CONST_ROAD) {
							gamemap[i - mob[0].speed][j] = CONST_ENEMY;
							gamemap[i][j] = CONST_ROAD;
						}
						else if (gamemap[i][j + mob[0].speed] == CONST_ROAD) {
							gamemap[i][j + mob[0].speed] = CONST_ENEMY;
							gamemap[i][j] = CONST_ROAD;
						}
					}

				}
				if (CONST_ROAD == gamemap[i][j]) {

					screen->Print("R", i * CONST_MAP_RATIO, j * CONST_MAP_RATIO, 0xffffff);


				}
			}
		}
		if (_kbhit()) {
			n = getch();
		}

		for (i = 0; i < CONST_MAP_SIZE; i++) {
			for (j = 0; j < CONST_MAP_SIZE; j++) {
				if (CONST_TANKED == gamemap[i][j]) {
					if (n == 'w') {
						if (j >1) {
							gamemap[i][j-1] = CONST_TANKED;
							gamemap[i][j] = 0;
						}

					}
					else if (n == 's') {
						if (j < CONST_MAP_SIZE - 1) {
							gamemap[i][j+1] = CONST_TANKED;
							gamemap[i][j] = 0;
						}

					}
					else if (n == 'd') {
						if (i < CONST_MAP_SIZE - 1) {
							//printf("ads");
							gamemap[i+1][j] = CONST_TANKED;
							gamemap[i][j] = 0;
						}


					}
					else if (n == 'a') {
						if (i >1) {
							gamemap[i-1][j] = CONST_TANKED;
							gamemap[i][j] = 0;
						}

					}
					else if (n == 'z') {
						if (i > 0) {

							gamemap[i][j] = CONST_TANK;
						}

					}
					mis = 1;
					n = ' ';
				}
				else if (i == CONST_MAP_SIZE - 1 && j == CONST_MAP_SIZE - 1 &&mis==0) {
					if (n == 'p') {
						gamemap[CONST_MAP_SIZE - 1][CONST_MAP_SIZE - 1] = CONST_TANKED;
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
				if (gamemap[i][j] == CONST_GRASS) {
					for (int k = 0; k < 80; k++) {
						for (int l = 0; l < 80; l++) {
							//screen->Print("A", i * 20 + k, j * 20 + l, 0xffffff);
						}
					}
				}
				else if (gamemap[i][j] == CONST_SPAWNING_ENEMY) {
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




