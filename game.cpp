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
#include "template.h"

//Sound Lib
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>  // mciSendString() 
#include "Mmsystem.h"
#include "Digitalv.h"


#define MCI_DGV_PLAY_REPEAT                 0x00010000L

#define CONST_MAPX			500
#define CONST_MAPY			500

using namespace std;

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	Surface bgstage("assets/bgstage01.png");
	static Sprite mainUser(new Surface("assets/playership2_2.png"), 9);
	static Sprite mainEnemy(new Surface("assets/enemy_red.png"), 6);
	static Sprite secondtypeEnemy(new Surface("assets/aagun.tga"), 36);
	static Sprite thirdtypeEnemy(new Surface("assets/ctankbase.tga"), 16);
	static Sprite thirdtypeEnemygun(new Surface("assets/ctankgun.tga"), 16);
	static Sprite forthtypeEnemy(new Surface("assets/turret.tga"), 36);
	static Sprite bulletimg(new Surface("assets/bullet_plazma.png"), 5);
	static Sprite itemsheild(new Surface("assets/sheild.png"), 4);
	static Sprite itemsupporter(new Surface("assets/supporter.png"), 12);
	static Sprite supporter_bullet(new Surface("assets/supporter_bullet.png"), 2);
	//static Sprite bgstage01(new Surface("assets/bgstage01.png"), 16);
	Surface* img = new Surface("assets/bgstage01.png");
  

	jute::jValue json_stagedata;
	jute::jValue json_entitydata;
	int usermapX;
	int usermapY;
	int delay = 0;
	int debug_mode;
	int cool = 0;
	int cool_time = 10;
	int space=0;
	int enemy_total;
	int supporter_cool;
	string change_result;

	/*/
	//sound lib
	MCI_OPEN_PARMS m_mciOpenParms;
	MCI_PLAY_PARMS m_mciPlayParms;
	DWORD m_dwDeviceID;
	MCI_OPEN_PARMS mciOpen;
	MCI_PLAY_PARMS mciPlay;
	int dwID;
	*/

	void get_entity_type() {
		
	}
	class class_user {   
	public:
		int hp = 100;
		int damage = 100;
		int speed = 10;
		int x = 0;
		int y = 0;
		int r = 20;

		/*void attack(int num) {
			if (enemy[) {

			}
			
		}*/


		

		void Draw(Surface* gameScreen)
		{
			mainUser.SetFrame(0);
			mainUser.Draw(gameScreen, (int)x, (int)y);
		}

	};

	class_user user;

	class class_enemy {
	public:
		int respawn=0;
		int trigger_x;
		int direction;
		int hp;
		int damage;
		int type;
		int speed;
		int x;
		int y;
		int enable;
		int pattern[20000];
		int pattern_size;
		void attack() {
			user.hp -= damage;
		}

		void move() {
			x -= speed;
			//if (x <= 0) {
				//x = 850;
			//}
			
			if (delay % 8 == 0) {

				direction++;
				if (direction > 5)direction = 0;
			}
		x = x + pattern[((delay-trigger_x) % (pattern_size/2))*2];
		y = y + pattern[((delay-trigger_x) % (pattern_size/2))*2 + 1];
			
			//printf("x - %d y - %d point - %d \n", pattern[((delay - trigger_x) % (pattern_size / 2)) * 2], pattern[((delay - trigger_x) % (pattern_size / 2)) * 2 + 1], ((delay - trigger_x) % (pattern_size / 2)) * 2 + 1);
		}
		void Draw(Surface* gameScreen)
		{
			if (enable == 1){

				mainEnemy.SetFrame(direction);
					mainEnemy.Draw(gameScreen, (int)x, (int)y);
					//enable = 1;
			}
		}
	};

	
	class class_weapon {
	public:
		int frame;
		int speed = 60;
		int damage;
		int type = 0;
		int x;
		int y;
		int y_offset = 0;
		int enable = 0;
		int energy;

		void fire(int userx, int usery,int offset){
			y_offset = offset;
			frame = (energy / 20)-1;
			if (frame < 0)frame = 0;
			x = userx;
			y = usery+12;
			supporter_cool++;

		}
		void move() {
			x += speed;
			y += y_offset;
		}
	};

	class_weapon weapon[1000];

	class class_item {
	public:
		//int attached = 0;
		int damage;
		int type = 0;
		int deltaY;
		int deltaX;
		int speed = 5;
		int x=user.x+56;
		int y;
		int enable = 1;
		//int past_type;

		void attack() {

			for (int bullet = 0; bullet < 1000; bullet++) {
				if (type == 2){

					if (weapon[bullet].enable == 0) {
						weapon[bullet].energy = 0;
						weapon[bullet].fire(x-25, y-15,0);
						weapon[bullet].enable = 1;
						weapon[bullet].type = 1;
						weapon[bullet].speed = 20;
							break;
					}
				}
				else {

					if (weapon[bullet].enable == 0) {
						weapon[bullet].energy = 0;
						weapon[bullet].fire(x, y, 15);
						for (int bulletB = 0; bulletB < 1000; bulletB++) {
							if (weapon[bulletB].enable == 0){
								if (bulletB != bullet) {
									weapon[bulletB].enable = 1;
									weapon[bulletB].type = 0;
									weapon[bulletB].fire(x, y, -15);
									break;
								}
							}

						}
						for (int bulletB = 0; bulletB < 1000; bulletB++) {
							if (weapon[bulletB].enable == 0) {
								if (bulletB != bullet) {
									weapon[bulletB].enable = 1;
									weapon[bulletB].type = 0;
									weapon[bulletB].speed = 0;
									weapon[bulletB].fire(x, y, 60);
									break;
								}
							}

						}
						for (int bulletB = 0; bulletB < 1000; bulletB++) {
							if (weapon[bulletB].enable == 0) {
								if (bulletB != bullet) {
									weapon[bulletB].enable = 1;
									weapon[bulletB].type = 0;
									weapon[bulletB].speed = 0;
									weapon[bulletB].fire(x, y, -60);
									break;
								}
							}

						}
					
						weapon[bullet].enable = 1;
						weapon[bullet].type = 0;
						break;
					}
				}
				
			}
		}

		void move() {
			if (type == 0) {
				if (deltaX <= 0|| abs(user.x + 56 - x) < 20) {
					deltaX = (user.x + 56 - x) / 5;
					//deltaY = (user.y - y) / 5;

					x += deltaX;
					y = user.y;
					if (abs(user.x + 56 - x) < 20) {
						x = user.x + 56;
						//attached = 1;
					}
				}
				else {
					type = 1;
				}
				
				/*
				x = user.x + 56;
				y = user.y;
				*/
				
			}
			if (type == 1) {
				if (deltaX>=0|| (abs(user.x + 400 - x) < 60)) {
					deltaX = (user.x + 400 - x) / 5;
					deltaY = (user.y - y) / 5;
					x += deltaX;
					y += deltaY;
				}
				else {
					type = 0;
				}
				
				
			}
			printf("     %d", type);
			
			if (type == 2) {
				deltaX = (user.x+10 - x) / 3;
				deltaY = (user.y-40 - y) / 3;
				x += deltaX;
				y += deltaY;
			}
			//printf("x - %d y - %d point - %d \n", pattern[((delay - trigger_x) % (pattern_size / 2)) * 2], pattern[((delay - trigger_x) % (pattern_size / 2)) * 2 + 1], ((delay - trigger_x) % (pattern_size / 2)) * 2 + 1);
		}
		void Draw(Surface* gameScreen)
		{
			move();

			
			if (enable == 1) {
				if (type == 2) {
					itemsupporter.SetFrame(delay % 12);
					itemsupporter.Draw(gameScreen, x, y);
				}
				else {
					itemsheild.SetFrame(delay % 4);
					itemsheild.Draw(gameScreen, x, y);
				}
				//enable = 1;
			}
		}
	};

	class class_sound {
	public:
		//sound lib
		MCI_OPEN_PARMS m_mciOpenParms;
		MCI_PLAY_PARMS m_mciPlayParms;
		DWORD m_dwDeviceID;
		MCI_OPEN_PARMS mciOpen;
		MCI_PLAY_PARMS mciPlay;


		int dwID;
		void doSountInit() {

			/*
			mciOpen.lpstrElementName = "assets/bgm.mp3"; // 파일 경로 입력
			mciOpen.lpstrDeviceType = "mpegvideo";

			mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
				(DWORD)(LPVOID)&mciOpen);

			dwID = mciOpen.wDeviceID;

			mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
				(DWORD)(LPVOID)&m_mciPlayParms);


			PlaySoundW(mciOpen.lpstrElementName, NULL, SND_ASYNC);
			
			*/
			//mciSendString("assets/bgm.mp3", NULL, 0, NULL);//배경음 재생 
			mciSendString("play \"title.wav\"", NULL, 0, NULL);//배경음 재생 s
		}
		void fire() {
			PlaySound(TEXT("assets/wav/gamefire.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			//mciSendString("play \"gamefire.wav\"", NULL, 0, NULL);
		}
		void monster_death() {
			PlaySound(TEXT("assets/wav/gamedeath.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			//mciSendString("play \"gamedeath.wav\"", NULL, 0, NULL);
		}
	};
	class_item sheild;
	class_item supporter;
	class_sound sound;
	
	class_enemy enemy[100];

	
	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	int dx = 480, offsetx = 0;
	void doPlay(Surface* screen)
	{
		
		usermapX = offsetx / 1000;
		//usermapX = user.x / 40 + offsetx / 1000;
		//printf("%d", usermapX);
		//printf("x - %d y - %d", enemy[0].x,enemy[0].y);
		delay++;
		cool++;
		system("cls");
		//screen->Clear(0);
		if (cool_time < cool) {
			cool = cool_time;
		}
		printf("%d", space);
		
		//printf("%s", change_result);
		for (int amount = 0; amount < enemy_total; amount++) {
			if (enemy[amount].trigger_x <= usermapX) {
				if (enemy[amount].respawn == 0) {
					enemy[amount].enable = 1;
					enemy[amount].respawn = 1;
				}
			}
		}
		for (int bullet = 0; bullet < 1000; bullet++) {
			if (weapon[bullet].enable == 1) {
				if (weapon[bullet].x > 800||weapon[bullet].y>700||weapon[bullet].y<-10) {
					weapon[bullet].energy = 0;
					weapon[bullet].enable = 0;

					weapon[bullet].speed = 60;
				}

				for (int amount = 0; amount < enemy_total; amount++) {
					if (enemy[amount].enable == 1) {
						if ((enemy[amount].x <= weapon[bullet].x) && (weapon[bullet].x <= (enemy[amount].x + 80))) {
							if ((enemy[amount].y <= weapon[bullet].y) && (weapon[bullet].y <= (enemy[amount].y + 80))) {


								weapon[bullet].energy -= 10;
								if (weapon[bullet].energy < 0) {
									weapon[bullet].enable = 0;
									weapon[bullet].energy = 0;
								}
								enemy[amount].enable = 0;
								sound.monster_death();
								weapon[bullet].speed = 60;
							}

						}
					}
				}

				if (weapon[bullet].enable == 1) {
					weapon[bullet].move();
					if (weapon[bullet].type == 1) {

						supporter_bullet.SetFrame(weapon[bullet].frame);
						supporter_bullet.Draw(screen, weapon[bullet].x, weapon[bullet].y);
						if (delay % 4 == 0) {

							weapon[bullet].frame++;
							if (weapon[bullet].frame > 1) weapon[bullet].frame = 0;
						}
					}
					else {
						bulletimg.SetFrame(weapon[bullet].frame);
						printf("%d", weapon[bullet].energy);
						bulletimg.Draw(screen, weapon[bullet].x, weapon[bullet].y);
						
					}
					
				}
			}
			if (weapon[bullet].energy < 0) {
				weapon[bullet].enable = 0;
			}
		}
		
		if (sheild.enable == 1) {

			for (int amount = 0; amount < enemy_total; amount++) {
				if (enemy[amount].enable == 1) {
					if ((enemy[amount].x <= sheild.x) && (sheild.x <= (enemy[amount].x + 80))) {
						if ((enemy[amount].y <= sheild.y) && (sheild.y <= (enemy[amount].y + 80))) {

							enemy[amount].enable = 0;
							sound.monster_death();

						}

					}
				}
			}
		}
		
		if (debug_mode == 1) {
			int spWidth, spHeight;
			for (int amount = 0; amount < enemy_total; amount++) {
				if (enemy[amount].enable == 1) {
					spWidth = mainEnemy.GetWidth();
					spHeight = mainEnemy.GetHeight();
					screen->Line(enemy[amount].x, enemy[amount].y, enemy[amount].x, enemy[amount].y + spHeight, 0xffffff);
					screen->Line(enemy[amount].x, enemy[amount].y, enemy[amount].x + spWidth, enemy[amount].y, 0xffff00);
					screen->Line(enemy[amount].x + spWidth, enemy[amount].y, enemy[amount].x + spWidth, enemy[amount].y + spHeight, 0xff00ff);
					screen->Line(enemy[amount].x, enemy[amount].y + spHeight, enemy[amount].x + spWidth, enemy[amount].y + spHeight, 0x00ffff);

					enemy[amount].direction = ++enemy[amount].direction % 32;
				}
			}
			screen->Line(user.x, user.y, user.x, user.y + 40, 0xffffff);
			screen->Line(user.x, user.y, user.x + 40, user.y, 0xffffff);
			screen->Line(user.x + 40, user.y, user.x + 40, user.y + 40, 0xffffff);
			screen->Line(user.x, user.y + 40, user.x + 40, user.y + 40, 0xffffff);

		}

		for (int amount = 0; amount < enemy_total; amount++) {
			if (enemy[amount].enable == 1) {

				enemy[amount].Draw(screen);
				enemy[amount].move();
				if ((enemy[amount].x - 40 <= user.x) && (user.x <= (enemy[amount].x + 80))) {
					if ((enemy[amount].y - 40 <= user.y) && (user.y <= (enemy[amount].y + 80))) {
						enemy[amount].attack();
						enemy[amount].enable = 0;
					}
				}
			}


		}


		if (user.hp <= 0) {
			exit(0);
		}



		
		screen->Bar(100, 460, 700, 462, 0x0000cc);
		screen->Bar(100, 480, 700, 482, 0x0000cc);
		screen->Bar(100, 460, 102, 480, 0x0000cc);
		screen->Bar(700, 460, 702, 480, 0x0000cc);
		if (GetAsyncKeyState('W')) {
			user.y -= user.speed;
		}
		if (GetAsyncKeyState('D')) {
			user.x += user.speed;
		}
		if (GetAsyncKeyState('S')) {
			user.y += user.speed;;
		}
		if (GetAsyncKeyState('A')) {
			user.x -= user.speed;
		}

		if (GetAsyncKeyState('E')) {
			if (sheild.type == 0) sheild.type = 1;

			else sheild.type = 0;
		}
		if (GetAsyncKeyState('Z')) {
			if (debug_mode == 1) {
				debug_mode = 0;
			}
			else {
				debug_mode = 1;
			}


		}
		if (user.x < 0) {
			user.x = 0;
		}
		if (user.y < 0) {
			user.y = 0;
		}


		//sp  - user.attck(dlfk;sdlfk/sdlf;kds;l

		user.Draw(screen);
		sheild.Draw(screen);
		supporter.Draw(screen);
		supporter.type = 2;
		if (GetAsyncKeyState(VK_SPACE)) {
			//for test
			
			if (cool_time - cool == 0) {
				for (int bullet = 0; bullet < 1000; bullet++) {
					
					if (space == 1) {
						if (weapon[bullet].enable == 0) {
							weapon[bullet].energy += 3;
							if (weapon[bullet].energy > 100) {
								weapon[bullet].energy = 100;
							}
							break;
						}

					}
					else {
						if (weapon[bullet].enable == 0) {
							weapon[bullet].enable = 1;
							sheild.attack();
							if (supporter_cool % 4 == 0) {

								supporter.attack();
								weapon[bullet].fire(user.x, user.y,0);
								supporter_cool = 0;
							}
							
							weapon[bullet].fire(user.x, user.y,0);

							weapon[bullet].type = 0;
						//	weapon[bullet].energy = 0;
							sound.fire();
							break;
						}
					}

				}
			}
			space = 1;
		}
		
		else {
			for (int bullet = 0; bullet < 1000; bullet++) {
				if (space == 1) {
					if (weapon[bullet].enable == 0) {
						if (weapon[bullet].energy != 0) {
							weapon[bullet].enable = 1;
							sheild.attack();
							
							weapon[bullet].type = 0;
							sound.fire();
							for (int deletedata = 0; deletedata < 1000; deletedata++) {
								weapon[deletedata].energy = 0;
							}
							//exit(0);
							break;
						}
					}

				}
			}
			space = 0;
		}
		for (int bullet = 0; bullet < 1000; bullet++) {
			if (space == 1) {
				if (weapon[bullet].enable == 0) {
					if (weapon[bullet].energy != 0) {
						screen->Bar(100, 460, weapon[bullet].energy * 7, 480, 0x0000cc);
						break;
					}
				}

			}
		}
		//mainUser.Draw(screen, user.x, user.y);
		/*/
		for (int stage = 0; stage < 3; stage++) {
			cout << json_stagedata["game_data"]["stages"][stage]["stage_name"].as_string() <<user.x<<user.y<< endl;
		}*/
	}

	static char map[16][78] = {
		"jb fb fa fb fb fb fb fb fb fb fb fb fb fa fb fb kc kc kc kc kc kc kc kc kc kc",
		"jb fb fa fb fb fb fb fb fb fb fb fb fb fa fb fb kc kc kc kc kc kc kc kc kc kc",
		"jb fb fa fb fb fb fb fb fb fb fb fb fb fa fb fb kc kc kc kc kc kc kc kc kc kc",
		"jb fb ed fe fe fe fe fe fe fe fe fe fe fd fb fb kc kc kc kc kc kc kc kc kc kc",
		"jb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb kc kc kc kc kc kc kc ad kc kc",
		"jb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb kc kc kc kc kc kc kc cb kc kc",
		"jb fb kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc cb kc kc",
		"jb fb kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc cb kc kc",
		"jb fb kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc cb kc kc",
		"jb fb kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc cb kc kc",
		"jb fb kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc db kc kc",
		"jb fb kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc kc",
		"bd cd de cd cd cd cd cd cd cd cd cd de cd cd bc kc kc kc kc kc kc kc kc kc kc",
		"fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb",
		"fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb",
		"fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb"
	};


	void DrawTile(int tx, int ty, Surface* screen, int x, int y)
	{
		Pixel* src = bgstage.GetBuffer() + 1 + tx * 33 + (1 + ty * 33) * 595;
		Pixel* dst = screen->GetBuffer() + x + y * 800;

		for (int i = 0; i < 320; i++, src += 595, dst += 800)
			for (int j = 0; j < 320; j++)
				dst[j] = src[j];
	}

	//int dx = (int)(2.0f * 1024.0f);


	void doBackgroundRendering(Surface* screen)
	{
		system("cls");
		screen->Clear(0);
		/*
		for (int y = 0; y < 1; y++)
			for (int x = 0; x < 1; x++)
			{
				//int tx = map[y][x * 3] - 'a', ty = map[y][x * 3 + 1] - 'a';
				//DrawTile(tx, ty, screen, x * 32, y * 32);
				DrawTile(0, 0, screen, x * 32, y * 32);
			}
		*/

		Pixel* dst = screen->GetBuffer();
		Pixel* src = img->GetBuffer();
		int srcWidth = img->GetWidth();
		int srcHeight = img->GetHeight();
		for (int x = 0; x < ScreenWidth; x++)
			for (int y = 0; y < ScreenHeight; y++)
			{
				int readxpos = ((dx * x + offsetx) >> 10) % srcWidth;
				int readypos = ((dx * y) >> 10) % srcHeight;
				Pixel sample = src[readxpos + readypos * srcWidth];
				dst[x + y * ScreenWidth] = sample;
			}
		//dx = (dx * 1023) >> 10; // 1023 is the new 0.999
		offsetx += 1000;
		//cout << offsetx << endl;
	}

	void doSountTest() {
		//PlaySound(TEXT("CantinaBand3.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	/*/
	void doSountInit() {
		

		mciOpen.lpstrElementName = "assets/bgm.mp3"; // 파일 경로 입력
		mciOpen.lpstrDeviceType = "mpegvideo";

		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
			(DWORD)(LPVOID)&mciOpen);

		dwID = mciOpen.wDeviceID;

		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
			(DWORD)(LPVOID)&m_mciPlayParms);


		//PlaySoundW(mciOpen.lpstrElementName, NULL, SND_ASYNC);
		//PlaySound(TEXT("assets/wav/gamefire.wav"), NULL, SND_FILENAME | SND_ASYNC);
		
	}*/

	void Game::Init()
	{
		srand(time(NULL));
		debug_mode = 0;
		int enemy_type;
		//stage data
		ifstream in("stagedata.json");
		string str_stagedata = ""; 
		string tmp;
		while (getline(in, tmp)) str_stagedata += tmp;
		json_stagedata = jute::parser::parse(str_stagedata);
		
		//entity data
		ifstream into("entitydata.json");
		string str_entitydata = "";
		while (getline(into, tmp)) str_entitydata += tmp;
		json_entitydata = jute::parser::parse(str_entitydata);

		//cout << json_stagedata["game_data"]["stages"][stage]["stage_name"].as_string() <<user.x<<user.y<< endl;
		cout << json_stagedata["game_data"]["stages"][0]["enemy_total"].as_string() << endl;
		change_result = json_stagedata["game_data"]["stages"][0]["enemy_total"].as_string();
		
		enemy_total= json_stagedata["stage_data"]["stages"][0]["enemys"].size();
		enemy_type = json_entitydata["object_data"]["objects"].size();
		//enemy_total = stoi(json_stagedata["game_data"]["stages"][0]["enemy_total"].as_string());
	
		printf("Count : %d, %d \n", enemy_total, enemy_type);

		for (int amount = 0; amount < enemy_total; amount++) {
			for (int type_num = 0; type_num < enemy_type; type_num++) {

				//printf("Loop : %d, %d \n", amount, type_num);

				//printf("Loop : %s \n", json_stagedata["stage_data"][0]["stages"][amount]["enemys"]["object_name"].as_string());
				//printf("Loop : %s \n", json_entitydata["object_data"]["objects"][type_num]["object_name"].as_string());

				if (json_stagedata["stage_data"]["stages"][0]["enemys"][amount]["object_name"].as_string() == json_entitydata["object_data"]["objects"][type_num]["object_name"].as_string()) {

					enemy[amount].hp			= json_entitydata["object_data"]["objects"][type_num]["hp"].as_int();
					enemy[amount].speed			= json_entitydata["object_data"]["objects"][type_num]["speed"].as_int();
					enemy[amount].damage		= json_entitydata["object_data"]["objects"][type_num]["power"].as_int();

					enemy[amount].x				= json_stagedata["stage_data"]["stages"][0]["enemys"][amount]["x"].as_int();
					enemy[amount].y				= json_stagedata["stage_data"]["stages"][0]["enemys"][amount]["y"].as_int();
					enemy[amount].trigger_x		= json_stagedata["stage_data"]["stages"][0]["enemys"][amount]["trigerX"].as_int();

					enemy[amount].pattern_size  = json_entitydata["object_data"]["objects"][type_num]["patterns"].size();

					for (int pattern_loop =0; pattern_loop < enemy[amount].pattern_size; pattern_loop+=2) {
						enemy[amount].pattern[pattern_loop]= json_entitydata["object_data"]["objects"][type_num]["patterns"][pattern_loop].as_int();
						enemy[amount].pattern[pattern_loop+1]= json_entitydata["object_data"]["objects"][type_num]["patterns"][pattern_loop+1].as_int();
						//printf("Pattern Data ( %d, %d )\n", enemy[amount].pattern[pattern_loop], enemy[amount].pattern[pattern_loop + 1]);
					}
					
					//printf("size - %d", enemy[amount].pattern_size);
					//printf("triggerX- %d x,y - %d - %d\n", enemy[amount].trigger_x, enemy[amount].x, enemy[amount].y);
				  	//enemy[amount].Draw(screen);
					//enemy[amount].move();
					//enemy[amount].type = stoi(json_stagedata["game_data"]["stages"][0]["enemys"][0]["pattern"].as_string());;
					enemy[amount].enable = 0;
				}
			}
		}

		//user.x = 10;		 
		sound.doSountInit();
	}
	//printf("%d", json_entitydata["object_data"]["objects"][8]["patterns"][65].as_int());
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{		
		doBackgroundRendering(screen);
		doPlay(screen);				
	}
};



