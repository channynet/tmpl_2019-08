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
#include <vector>
#include "jute/jute.h"

#define CONST_MAPX			500
#define CONST_MAPY			500

using namespace std;

class class_enemy {
public:
	int direction;
	int hp;
	int damage;
	int speed;
	int x;
	int y;
	int enable;
	void attack(int damage) {

	}

	void move(int speed) {

	}
	void spawn(int x, int y,class class_enemy enemy) {
		enemy.x = x;

	}
};

class class_user {
public:
	int hp=100;
	int damage=100;
	int speed=10;
	int x=0;
	int y=0;

	void attack(int damage,int r,int stage, class_enemy* enemy) {
		for (int x = -r; x < r;x++) {
			for (int y = 0; x < r;x++) {
				
				if (enemy->enable == 1) {
					if (enemy->x==x&&enemy->y==y) {
						enemy->hp -= damage;
						if (enemy->direction == 0) {
							enemy->y += 5;
						}
						if (enemy->direction == 1) {
							enemy->x -= 5;
						}
						if (enemy->direction == 2) {
							enemy->y -= 5;
						}
						if (enemy->direction == 3) {
							enemy->x += 5;
						}
					}
				}
				
			}
		}
	}

	void move(int speed,int diraction,class_user user) {
		if (diraction == 0) {
			printf("aaaaasdfafs");
			user.y -= speed;
			printf("%d",user.y);
		}
		if (diraction == 1) {
			user.x += speed;
		}
		if (diraction == 2) {
			user.y += speed;
		}
		if (diraction == 3) {
			user.x -= speed;
		}
		
		if (user.y <= 0) {
			user.y = 0;
		}
		if (user.x <= 0) {
			user.x = 0;
		}
		if (user.y >= CONST_MAPY) {
			user.y = CONST_MAPY;
		}
		if (user.x >= CONST_MAPX) {
			user.x = CONST_MAPX;
		}
		printf("%d", user.x);
	}

	void keypress(int r , int stage) {
		printf("aaaaaaaaaaaa");
		if (GetAsyncKeyState(VK_SPACE)) {
			//user.attack(damage,r,stage, &enemy);
		}
		if (GetAsyncKeyState('W')) {
			printf("aaaaaaaaaaaaaaaaaaaaaaaaaa");
			user->move(speed, 0, &user);
		}
		if (GetAsyncKeyState('D')) {
			user->move(speed, 1, &user);
		}
		if (GetAsyncKeyState('S')) {
			user->move(speed, 2, &user);
		}
		if (GetAsyncKeyState('A')) {
			user->move(speed, 3, &user);
		}
	}
};

class class_sum {
public:
	class_sum(int x, int y) {

	}

	void sumA(int x,int y) {
		//return x+y;
	}
};
class class_tool {
public:
	int damage;
	int longness;
	int enable;
};


class class_user user;
class class_tool tool;
class class_enemy enemy[100];
class class_sum sum(0,0);
namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------


	
	void Game::Init()
	{
		
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	Surface tiles("assets/nc2tiles.png"); 
	static Sprite mainUser(new Surface("assets/playership.png"),9);
	static Sprite mainEnemy(new Surface("assets/biggun.tga"), 32);
	static Sprite secondtypeEnemy(new Surface("assets/aagun.tga"), 36);
	static Sprite thirdtypeEnemy(new Surface("assets/ctankbase.tga"), 16);
	static Sprite thirdtypeEnemygun(new Surface("assets/ctankgun.tga"), 16);
	static Sprite forthtypeEnemy(new Surface("assets/turret.tga"), 36);
	static Sprite bulletimg(new Surface("assets/phaser.tga"), 16);

	

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	

	void Game::Tick(float deltaTime)
	{
		
		system("cls");
		screen->Clear(0);
		ifstream in("data.json");
		string str = "";
		string tmp;
		//user.x = 10;
		while (getline(in, tmp)) str += tmp;
		jute::jValue v = jute::parser::parse(str);


		user.keypress(100,3,5,&enemy[0]);

		mainUser.Draw(screen, user.x, user.y);

		for (int stage = 0; stage < 3; stage++) {
			cout << v["game_data"]["stages"][stage]["stage_name"].as_string() <<user.x<<user.y<< endl;
		}
	}
};



