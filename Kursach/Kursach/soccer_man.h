#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
using namespace sf;
class soccer_man
{
	//Поля
private:
	float x;
	float y;
	float speed;
	float stamina;
	float dX;
	float dY;
	float anim;
	float goalkeeper_anim;
	float anim_speed;
	float pause_timer;
	float button_time;
	float button_timer;
	float control_time;
	float control_timer;
	static float pause_time;
	static int chosen_player;
	static int active_index1;
	static int active_index2;
	int team_n;
	int back_point_x;
	int back_point_y;
	int w1;
	int role;
	int	h1;
	int dir;
	int nearest_enemy ;
	int manual_pass ;
	bool stuc, moved;
	bool go_back;
	bool AI;
	bool manual;
	bool snipe_move;
	bool moving;
	Font font;
	Text text;
	Texture uniform;
	Sprite fotsprite;
	Texture player_pointer;
	Sprite player_pointer_sprite;
	Texture player_info;
	Sprite player_info_sprite;
	Sprite player_stamina_sprite;
	Sprite player2_stamina_sprite;
	String name;
	String role_name[6];
	SoundBuffer whistle;
	Sound referee_whistle;
public:
	//-------------------------------------------------Конструктор-------------------------------------------------
	soccer_man(bool ai = true, float Y = 0, float X = 300, int h = 89, char n[] = "Vasia", float s = 0.1, float a_s = 0.005, String f = "Sprites/Footbalers.png", int w = 142){
		x = X;
		y = Y;
		h1 = h;
		w1 = w;
		stamina = 200;
		manual = false;
		snipe_move = false;
		moving = false;
		pause_timer = 0;
		button_time = 0;
		nearest_enemy = 0;
		button_timer = 0;
		control_time = 0;
		manual_pass = 0;
		control_timer = 0;
		role_name[0] = "CP";
		role_name[1] = "LG";
		role_name[2] = "RG";
		role_name[3] = "LC";
		role_name[4] = "RC";
		role_name[5] = "GK";
		back_point_x = 601;
		back_point_y = 250;
		name = n;
		anim_speed = a_s;
		speed = s;
		anim = 0;
		chosen_player = 0;
		goalkeeper_anim = 0;
		uniform.loadFromFile(f);
		fotsprite.setTexture(uniform);
		fotsprite.setTextureRect(IntRect(w1, h1, 19, 39));
		fotsprite.setPosition(x, y);
		fotsprite.setOrigin(19 / 2, 35);
		stuc = false;
		moved = false;
		go_back = false;
		AI = ai;
		player_pointer.loadFromFile("Sprites/payer_pointer.png");
		player_pointer_sprite.setTexture(player_pointer);
		player_pointer_sprite.setOrigin(0, 13);
		player_info.loadFromFile("Sprites/Stamina_bar.png");
		player_info_sprite.setTexture(player_info);
		player_stamina_sprite.setTexture(player_info);
		player2_stamina_sprite.setTexture(player_info);
		player_info_sprite.setTextureRect(IntRect(0, 9, 200, 32));
		player_stamina_sprite.setTextureRect(IntRect(0, 0, stamina, 9));
		player2_stamina_sprite.setTextureRect(IntRect(0, 0, stamina, 9));
		font.loadFromFile("Sprites/Times_New_Roman.ttf");
		text = Text("", font, 20);
		text.setColor(Color::White);
		whistle.loadFromFile("Sounds/referee_whistle.ogg");
		referee_whistle.setBuffer(whistle);
		referee_whistle.setVolume(1);
	}
	//-------------------------------------------------Движение-----------------------------------------------------
	void movement(int current_team, int team, float time, RenderWindow &window, bool pass, bool control, Keyboard::Key **KeyKode, bool sniping, int w = 0, float ball_x = 0, float ball_y = 0, int index = 0, int nearest = 0){
		switch (dir)
		{
		case 1:{fotsprite.setTextureRect(IntRect(w1 + w + 25, h1, 19, 39)); break; }
		case 2:{fotsprite.setTextureRect(IntRect(w1 + w + 20 + 25, h1, -19, 39)); break; }
		case 3:{fotsprite.setTextureRect(IntRect(w1 + w + 3 + 25, h1 + 240, 19, 39)); break; }
		case 4:{fotsprite.setTextureRect(IntRect(w1 + w + 3 + 25, h1 + 281, 19, 39)); break; }
		}
		if (pause_timer >999) referee_whistle.play();
		if (pause_time > 0){
			pause_timer += time;
		}
		if (pause_timer >= pause_time){
			pause_timer = 0;
			pause_time = 0;
			if (!AI){
				if (current_team == team && control)
					speed = 0.08;
				else speed = 0.1;
				anim_speed = 0.005;
				if (team == 0) active_index1 = index;
				else active_index2 = index;
				if (Keyboard::isKeyPressed(KeyKode[team][7]) && moving == true) {
					if (stamina > 10) { stamina -= 0.07*time; speed+=0.04; anim_speed = 0.007; }
				}
				else  { if (stamina < 200)stamina += 0.01*time; }
				if (snipe_move == false){
					moving = false;
					player_pointer_sprite.setPosition(x - 2, y - 36);
					if (team == 0) player_pointer_sprite.setColor(Color::Cyan);
					else player_pointer_sprite.setColor(Color::Red);
					window.draw(player_pointer_sprite);
					if (!stuc){
						if (Keyboard::isKeyPressed(KeyKode[team][0])) {
							moving = true;
							anim += float(anim_speed*time);
							if (anim > 4) anim = 0;
							if (Keyboard::isKeyPressed(KeyKode[team][2]))
								fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1 + 39, 19, 39));
							else
								fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1, 19, 39));
							if (Keyboard::isKeyPressed(KeyKode[team][1])) anim = 1;
							x -= speed*time;
							if (x<89){
								x += (speed*time) + 0.1f; anim = 1;
							}
							fotsprite.setPosition(x, y);
							dir = 1;
						}
						if (Keyboard::isKeyPressed(KeyKode[team][1])) {
							moving = true;
							anim += float(anim_speed*time);
							if (anim > 3) anim = 0;
							fotsprite.setTextureRect(IntRect(w1 + w + 20 + (25 * int(anim)), h1, -19, 39));
							x += speed*time;
							if (Keyboard::isKeyPressed(KeyKode[team][0])) anim = 1;
							if (x>1106){
								x -= (speed*time) + 0.1f; anim = 1;
							}
							fotsprite.setPosition(x, y);
							dir = 2;
						}
						if (Keyboard::isKeyPressed(KeyKode[team][2])) {
							moving = true;
							if (anim > 3) anim = 0;
							if (Keyboard::isKeyPressed(KeyKode[team][0])){
								fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1 + 39, 19, 39)); dir = 5;
							}
							else
							if (Keyboard::isKeyPressed(KeyKode[team][1])){
								fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), 251, 19, 39)); dir = 6;
							}
							else {
								fotsprite.setTextureRect(IntRect(w1 + w + 3 + (25 * int(anim)), h1 + 240, 19, 39));
								anim += float(anim_speed*time); dir = 3;
							}
							if (!Keyboard::isKeyPressed(KeyKode[team][3])){
								y -= speed*time;
								if (y<227){
									y += (speed*time) + 0.1f; anim = 1;
								}
								fotsprite.setPosition(x, y);
							}
							else anim = 1;
						}
						if(Keyboard::isKeyPressed(KeyKode[team][3])) {
							moving = true;
							if (anim > 3) anim = 0;
							if (Keyboard::isKeyPressed(KeyKode[team][0])) {
								fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1 + 80, 19, 39)); dir = 7;
							}
							else
							if (Keyboard::isKeyPressed(KeyKode[team][1])) {
								fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), 290, 19, 39)); dir = 8;
							}
							else {
								fotsprite.setTextureRect(IntRect(w1 + w + 3 + (25 * int(anim)), h1 + 281, 19, 39));
								anim += float(anim_speed*time); dir = 4;
							}
							if (!Keyboard::isKeyPressed(KeyKode[team][2])){
								y += speed*time;
								if (y>1506){
									y -= (speed*time) + 0.1f; anim = 1;
								}
								fotsprite.setPosition(x, y);
							}
							else anim = 1;
						}
					}

				}
				else
				{
					if (moving){
						anim += float(anim_speed*time);
						if (anim > 3) anim = 0;
						switch (dir)
						{
						case 1:{x -= (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1, 19, 39));
							break; }
						case 2:{x += (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + 20 + (25 * int(anim)), h1, -19, 39));
							break; }
						case 3:{y -= (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + 3 + (25 * int(anim)), h1 + 240, 19, 39));
							break; }
						case 4:{y += (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + 3 + (25 * int(anim)), h1 + 281, 19, 39));
							break; }
						case 5:{x -= (speed*time); y -= (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1 + 39, 19, 39));
							break; }
						case 6:{x += (speed*time); y -= (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), 251, 19, 39));
							break; }
						case 7:{x -= (speed*time); y += (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1 + 80, 19, 39));
							break; }
						case 8:{x += (speed*time); y += (speed*time);
							fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), 290, 19, 39));
							break; }
						}
						fotsprite.setPosition(x, y);
					}
				}
			}
			else
			{
				if (ball_x != 599 && ball_y != 866){
					speed=0.1;
					int last_x = x, last_y = y;
					if (stamina < 200)stamina += 0.01*time;
					if (index != 5){
						dX = ball_x - x;
						dY = ball_y - y;
						float distance = sqrt((dX)*(dX)+(dY)*(dY));
						anim_speed = 0.005;
						if (distance > 1)
						{
							if ((chosen_player == index && pass && current_team == team) || (!control && index == nearest && !pass))
							{
								x += speed*time*(dX) / (distance);
								y += speed*time*(dY) / (distance);
							}
							else{
								if (role == 3 || role == 4 || role == 1 || role == 2 || role == 0){
									int zone_x;
									int zone_y;
									int dist;
									int dynamic_zone;
									if (current_team != team && (role == 4 || role == 3))
										dynamic_zone = 850;
									else dynamic_zone = 550;
									if (role == 2 || role == 4) {
										zone_x = 900;
									}
									else { zone_x = 250; }
									if (role == 0) zone_x = 600;
									if (role == 4 || role == 3 || role == 0) dist = 130;
									else dist = 300;
									if (role == 2 || role == 1) zone_y = 600 + 600 * team;
									if ((role == 4 || role == 3 || role == 0) && team == 0) zone_y = 1500;
									if ((role == 4 || role == 3 || role == 0) && team == 1) zone_y = 200;
									if ((y > zone_y && team == 0) || (team == 1 && y < zone_y) || ((((role == 4 || role == 3) && team == 1)) && y>1000) || ((role == 4 || role == 3) && (y < zone_y - dynamic_zone))){
										if ((team == 1 && y < zone_y) || ((role == 4 || role == 3) && y < zone_y - dynamic_zone)){
											if (((role == 4 || role == 3) && team == 1) && y>1000)
											{
												y -= 0.1f*time; x -= 0.1f*time;
											}
											else { y += 0.1f*time; x -= 0.1f*time; }
										}
										else { y -= 0.1f*time; x -= 0.1f*time; }
									}
									else{
										if (abs(dY)<dist && abs(dY)>150) { y -= speed*time*(dY) / (distance * 3); }
										if (abs(dY)<110 && current_team != team) y += speed*time*(dY) / distance;
										if (abs(dY)>dist) y += speed*time*(dY) / distance;
									}
									if ((x <= 600 && role == 2) || (x <= 100 && role == 1)){
										x += 0.1f*time;
									}
									else if ((x >= 1100 && (role == 2 || role == 3)) || (x >= 600 && (role == 1 || role == 3))){
										x -= 0.1f*time;
									}
									else
									if (current_team != team && (ball_x > 570 && (role == 2 || role == 4) || ball_x<610 && (role == 1 || role == 3) || (ball_x>500 && ball_x<700) && role == 0)) x += speed*time*(dX) / (distance / 1.3);
									else{
										if (abs(dY) - dist>1) anim_speed = 0.005;
										else anim_speed = 0;
										if (x<zone_x + 10 && !pass)	{
											x += 0.1f*time;
										}
										if (x>zone_x - 10 && !pass){
											x -= 0.1f*time;
										}
									}
									if ((y > zone_y - 0.1 && team == 0) || (team == 1 && y < zone_y + 0.1)) anim = 1;
								}
							}
							if (x<89 || x>1106){
								x = last_x;
								y = last_y;
								window.draw(fotsprite);
								return;
							}
							if (dY > 0) dir = 4;
							else dir = 3;
							float angle = (atan2(dY, dX)) * 180 / 3.14159265;
							if (angle > -110 && angle<-80) dir = 3;
							if (angle>80 && angle<110) dir = 4;
							if (angle>-10 && angle<10) dir = 2;
							if ((angle>-180 && angle<-170) || (angle>170 && angle<180)) dir = 1;
							if (angle>10 && angle < 80)
								dir = 8;
							if (angle>110 && angle < 170)
								dir = 7;
							if (angle>-170 && angle < -110)
								dir = 5;
							if (angle>-80 && angle < -10)
								dir = 6;
						}
						anim += float(anim_speed*time);
						if (anim > 3) anim = 0;
						if ((dY > -25 && dY < 25) && control) anim = 1;
						switch (dir)
						{
						case 1:{fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1, 19, 39)); break; }
						case 2:{fotsprite.setTextureRect(IntRect(w1 + w + 20 + (25 * int(anim)), h1, -19, 39)); break; }
						case 3:{fotsprite.setTextureRect(IntRect(w1 + w + 3 + (25 * int(anim)), h1 + 240, 19, 39)); break; }
						case 4:{fotsprite.setTextureRect(IntRect(w1 + w + 3 + (25 * int(anim)), h1 + 281, 19, 39)); break; }
						case 5:{fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1 + 39, 19, 39)); break; }
						case 6:{fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), 251, 19, 39)); break; }
						case 7:{fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), h1 + 80, 19, 39)); break; }
						case 8:{fotsprite.setTextureRect(IntRect(w1 + w + (25 * int(anim)), 290, 19, 39)); break; }
						}
						fotsprite.setPosition(x, y);
					}
				}
			}
		}
		window.draw(fotsprite);
	}
	//-------------------------------------------------Движение голкипера---------------------------------------
	void golie_movement(Keyboard::Key **KeyKode, float time, RenderWindow &window, float ball_x, float ball_y, int player_number, bool control, bool stop, bool pass, int g_y = 284, int j = 2, int team = 0)
	{
		dX = ball_x - x;
		dY = ball_y - y;
		int sprite = -143;
		if (j == 3) sprite = 143;
		float speed_g = 0.1;
		int s_p = 0;
		if (j == 3) s_p = 289;
		fotsprite.setTextureRect(IntRect(32 + s_p, 410, 24, 39));
		float distance = sqrt((dX)*(dX)+(dY)*(dY));
		int zone_x1 = 673, zone_x2 = 517;
		if ((dX <= 80 && dX >= -80) && (dY <= 60 && dY >= -60))
		{
			speed_g = 0.4;
			if (control==false && !stop){
				if (ball_x - x > 0) fotsprite.setTextureRect(IntRect(0 + s_p, 51, 42, 22));
				else fotsprite.setTextureRect(IntRect(42 + s_p, 73, -42, -22));
				}
		}
		if (ball_x <= 724 && ball_x >= 472 && ball_y <= 284){ zone_x1 = 724; zone_x2 = 472; }
		if (abs(distance)<160 && stop == true && control == false) { zone_x1 = 2000, zone_x2 = 0; g_y = g_y == 1428 ? 248 : 1428; }
		if ((player_number == 5 && control == false) || ((x > zone_x1 || x < zone_x2 || y > g_y) && player_number != 5 && j == 2) || ((x > zone_x1 || x < zone_x2 || y < g_y) && player_number != 5 && j == 3)){
			if (j == 3){
				back_point_x = 601;
				back_point_y = 1485;
			}
			go_back = true;
		}
		if (!go_back){
			if (player_number != 5){
				if (x <= zone_x1 && x >= zone_x2){
					if (x >= zone_x1 - 2) {
						x -= speed_g*time + 0.1f; goalkeeper_anim = 0;
					}
					if (x <= zone_x2 + 2) {
						x += speed_g*time + 0.1f; goalkeeper_anim = 0;
					}
					if (distance > 1){
						x += speed_g*time*(dX) / distance;
						if ((dX > 30 || dX < -30)){
							goalkeeper_anim += float(anim_speed*time);
							if (goalkeeper_anim > 2) goalkeeper_anim = 0;
							fotsprite.setTextureRect(IntRect(32 + s_p + (24 * int(goalkeeper_anim)), 410, 24, 39));
						}
					}
				}
				if (j== 3){
					if (ball_y >= g_y){
						if (distance > 1)
							y += speed_g*time*(dY) / distance;
					}
				}
				else if (ball_y <= g_y){
					if (distance > 1)
						y += speed_g*time*(dY) / distance;

				}
			}
			else movement(team, team, time, window, 0, 1, KeyKode, 0, sprite, 0, 0, 5);
		}
		else {
			distance = sqrt((back_point_x - x)*(back_point_x - x) + (back_point_y - y)*(back_point_y - y));
			if (distance > 1)
			{
				goalkeeper_anim += float(anim_speed*time);
				if (goalkeeper_anim > 3) goalkeeper_anim = 0;
				fotsprite.setTextureRect(IntRect(3 + s_p + (24 * int(goalkeeper_anim)), 330, 17, 39));
				x += speed*time*(back_point_x - x) / distance;
				y += speed*time*(back_point_y - y) / distance;
			}
			if (distance < 1 || (chosen_player == 5 && pass)) go_back = false;
		}
		fotsprite.setPosition(x, y);
		window.draw(fotsprite);
	}
//------------------------------Переключение между игроками для паса--------------------------------
	void players_switching(Keyboard::Key **KeyKode, RenderWindow &window, soccer_man **p, const int n, int p_number, int team, float time, int &slow, bool control){
		float d[6];
		float d_e[6];
		float players[5];
		int k = 0;
		for (int j = 0; j < n; j++)
		{
			if (p_number == j){ d[j] = 5000; continue; }
			players[k] = j;
			k++;
			d[j] = sqrt((p[team][j].x - p[team][p_number].x)*(p[team][j].x - p[team][p_number].x) + (p[team][j].y - p[team][p_number].y)*(p[team][j].y - p[team][p_number].y));
			switch (dir)
			{
			case 5:
			case 7:
			case 1:{if (x - p[team][j].x < 0) { d[j] = 2000; } break; }
			case 6:
			case 8:
			case 2:{if (x - p[team][j].x > 0) { d[j] = 2000; } break; }
			case 3:{if (y - p[team][j].y < 0) { d[j] = 2000; } break; }
			case 4:{if (y - p[team][j].y > 0) { d[j] = 2000; } break; }
			}
		}
		int enemy_team = team == 1 ? 0 : 1;
		for (int j = 0; j < n - 1; j++)
		{
			d_e[j] = sqrt((x - p[enemy_team][j].get_x())*(x - p[enemy_team][j].get_x()) + (y - p[enemy_team][j].get_y())*(y - p[enemy_team][j].get_y()));
		}
		if (control_time > 0)
			control_timer += time;
		if (control_timer >= control_time){
			control_timer = 0;
			control_time = 0;
			for (int j = 0; j < n - 1; j++)
			{
				if (d_e[j] < d_e[nearest_enemy]) nearest_enemy = j;
			}
			for (int i = 0; i < n; i++) p[enemy_team][i].set_AI(true);
			if (nearest_enemy != p_number) control_time = 800;
			p[enemy_team][nearest_enemy].set_AI(false);
		}
		if (button_time > 0)
			button_timer += time;
		if (button_timer >= button_time){
			button_timer = 0;
			button_time = 0;
			if (Keyboard::isKeyPressed(KeyKode[team][4]) && control){
				manual = true;
				if (manual){
					slow = 2600;
					button_time = 100;
					if (manual_pass > 3) manual_pass = 0;
					chosen_player = players[manual_pass];
					manual_pass++;
				}
			}
		}
		if (manual == false)
		for (int j = 0; j < n; j++)
		{
			if (d[j] < d[chosen_player]) chosen_player = j;
		}
		player_pointer_sprite.setPosition(p[team][chosen_player].x - 2,
			p[team][chosen_player].y - 36);
		if (team == 0) player_pointer_sprite.setColor(Color(24, 211, 5));
		else  player_pointer_sprite.setColor(Color::Yellow);
		window.draw(player_pointer_sprite);
	}
	//----------------------Вивод на екран виносливости и ынформации про футболиста--------------
	void Stamina(RenderWindow &window, View &view, int team = 0){
		std::ostringstream game;
		int z;
		for (int i = 0; i <2; i++)
			game.put(role_name[role][i]);
		game << "    ";
		z = name.getSize();
		for (int i = 0; i < z; i++)
			game.put(name[i]);
		text.setString(game.str());
		text.setColor(Color::White);
		if (team == 1){
			player_stamina_sprite.setTextureRect(IntRect(0, 0, stamina, 9));
			player_info_sprite.setPosition(view.getCenter().x + view.getCenter().x - 210, view.getCenter().y + view.getSize().y / 2 - 50);
			player_stamina_sprite.setPosition(view.getCenter().x + view.getCenter().x - 210, view.getCenter().y + view.getSize().y / 2 - 59);
			text.setPosition(view.getCenter().x + view.getCenter().x - 203, view.getCenter().y + view.getSize().y / 2 - 50);
			window.draw(player_stamina_sprite);
		}
		else {
			player2_stamina_sprite.setTextureRect(IntRect(0, 0, stamina, 9));
			player_info_sprite.setPosition(view.getCenter().x - view.getCenter().x + 10, view.getCenter().y + view.getSize().y / 2 - 50);
			player2_stamina_sprite.setPosition(view.getCenter().x - view.getCenter().x + 10, view.getCenter().y + view.getSize().y / 2 - 59);
			text.setPosition(view.getCenter().x - view.getCenter().x + 17, view.getCenter().y + view.getSize().y / 2 - 50);
			window.draw(player2_stamina_sprite);
		}
		window.draw(player_info_sprite);
		window.draw(text);
	}
	//-------------------------------------------Методы получения значения-----------------------------------
	float& get_x(){ return x; }
	float& get_y(){ return y; }
	float& get_speed(){ return speed; }
	float& get_anim_speed(){ return anim_speed; }
	int& get_chosen_player(){ return chosen_player; }
	int& get_w1(){ return w1; }
	int& get_active_index1(){ return active_index1; }
	int& get_active_index2(){ return active_index2; }
	int& get_h1(){ return h1; }
	int& get_dir(){ return dir; }
	bool& get_stuc(){ return stuc; }
	bool get_manual(){ return manual; }
	Sprite& get_fotsprite(){ return fotsprite; }
	//-----------------------------------------------Методи установления значений---------------------------
	void set_stuc(bool s){ stuc = s; }
	void set_snipe_move(bool s){ snipe_move = s; }
	void set_manual(bool m){ manual = m; }
	void set_footbaler(int w){
		w1 = w;
		fotsprite.setTextureRect(IntRect(w1 + 25, h1, 19, 39));
	}
	void set_sprite(float X, float Y){ fotsprite.setPosition(x = X, y = Y); }
	void set_x(float X){ x = X; }
	void set_y(float Y){ y = Y; }
	void set_name(String n){
		int z = n.getSize();
		name = n;
	}
	void set_dir(int d){ dir = d; }
	void set_start(float X, float Y){
		x = X;
		y = Y;
		fotsprite.setPosition(x, y);
	}
	void set_AI(bool ai){ AI = ai; }
	void set_button_time(int t){ button_time = t; }
	//-----------------------------------------------Другие мотоди-------------------------------------------------
	void unstuc(){
		if (x > 400) x -= 35;
		if (x < 400) x += 35;
		stuc = false;
		fotsprite.setPosition(x, y);
	}

	static void repositioning(soccer_man **p, int n){
		for (int i = 0; i < n - 1; i++){
			p[1][i].set_footbaler(437);
			p[1][i].set_dir(3);
			p[0][i].set_dir(4);
		}
		for (int i = 0; i < 2; i++)
		for (int j = 0; j < n; j++){
			p[i][j].set_AI(true);
			if (i == 1) p[i][j].set_dir(3);
		}
		p[0][0].set_AI(false);
		p[1][0].set_AI(false);
		p[0][0].set_start(599, 750);
		p[0][1].set_start(221, 680);
		p[0][2].set_start(963, 680);
		p[0][3].set_start(423, 422);
		p[0][4].set_start(778, 422);
		p[0][5].set_start(601, 250);
		p[1][0].set_start(599, 976);
		p[1][1].set_start(221, 1054);
		p[1][2].set_start(963, 1054);
		p[1][3].set_start(423, 1254);
		p[1][4].set_start(778, 1254);
		p[1][5].set_start(601, 1449);
		pause_time = 1000;
	}
	static void set_standart_names(soccer_man **p){
		p[0][0].set_name("Vasia");  p[0][0].role = 0;
		p[0][1].set_name("Saha");   p[0][1].role = 3;
		p[0][2].set_name("Vova");   p[0][2].role = 4;
		p[0][3].set_name("Dima");   p[0][3].role = 1;
		p[0][4].set_name("Denis");  p[0][4].role = 2;
		p[0][5].set_name("Andrey"); p[0][5].role = 5;
		p[1][0].set_name("Albert"); p[1][0].role = 0;
		p[1][1].set_name("Kostia"); p[1][1].role = 3;
		p[1][2].set_name("Bob");    p[1][2].role = 4;
		p[1][3].set_name("Oleg");   p[1][3].role = 1;
		p[1][4].set_name("Taras");  p[1][4].role = 2;
		p[1][5].set_name("Slavik"); p[1][5].role = 5;
	}
	static void coner_repositioning_R(soccer_man **p, int n){
		for (int i = 0; i < 2; i++)
		for (int j = 0; j < n; j++){
			p[i][j].set_AI(true);
		}
		p[0][0].set_start(600, 1336);
		p[0][1].set_start(380, 1408);
		p[0][2].set_start(771, 1373);
		p[0][3].set_start(423, 422);
		p[0][4].set_start(778, 422);
		p[0][5].set_start(601, 250);
		p[1][0].set_start(599, 976);
		p[1][1].set_start(221, 1054);
		p[1][2].set_start(963, 1054);
		p[1][3].set_start(438, 1387);
		p[1][4].set_start(784, 1387);
		p[1][5].set_start(601, 1449);
		pause_time = 1000;
	}
	static void coner_repositioning_L(soccer_man **p, int n){
		for (int i = 0; i < 2; i++)
		for (int j = 0; j < n; j++){
			p[i][j].set_AI(true);
		}
		p[0][0].set_start(600, 1336);
		p[0][1].set_start(380, 1408);
		p[0][2].set_start(771, 1373);
		p[0][3].set_start(460, 387);
		p[0][4].set_start(778, 387);
		p[0][5].set_start(601, 250);
		p[1][0].set_start(599, 400);
		p[1][1].set_start(221, 370);
		p[1][2].set_start(963, 375);
		p[1][3].set_start(423, 1254);
		p[1][4].set_start(778, 1254);
		p[1][5].set_start(601, 1449);
		pause_time = 1000;
	}
};

