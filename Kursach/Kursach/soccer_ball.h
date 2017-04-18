#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
using namespace sf;  

class soccer_ball
{
	//Поля
private:
	//Координаты
	Vector2f pos;
	Vector2f las_pos;
	Vector2f target;
	Image screen;
	double sum=0;
	double friction= 0;
	float x;
	float y;
	float dX, dY;
	float distance;
	float Start_distance=0;
	//Анимация и скорость
	float speed_b;
	float anim = 0;
	float b_anim = 0;
	float rotation;
	float anim_speed;
	//Логика мяча
	bool pass = false;
	bool control = false, move = false, out = false;
	bool sniping = false;
	bool to_target = false;
	bool stop = false;
	bool goal_pause = false;
	int goal_red = 0, goal_blue = 0, lost = 0, player_number = 0;
	int team = 0;
	int nearest = 0;
	int imune;
	float imune_timer;
	//Текстуры и спрайты
	Texture tx;
	Sprite ballsprite;
	Texture goal_tx;
	Sprite goal_sprite;
	//Звук
	SoundBuffer kick;
	SoundBuffer whistle;
	SoundBuffer g;
	Sound kick_sound;
	Sound referee_whistle;
	Sound goal;
public:
	//-------------------------------------------------Конструктор-------------------------------------------------
	soccer_ball(String f = "Sprites/ball.png", float X = 599, float Y = 866, float s = 0.4, float a_s = 0.005){
		x = X;
		anim_speed = a_s;
		y = Y;
		speed_b = s;
		anim = 0;
		tx.loadFromFile(f);
		ballsprite.setTexture(tx);
		goal_tx.loadFromFile("Sprites/Goal.png");
		goal_sprite.setTexture(goal_tx);
		goal_sprite.setTextureRect(IntRect(0, 0, 300, 140));
		ballsprite.setTextureRect(IntRect(0, 0, 17, 16));
		ballsprite.setOrigin(17 / 2, 16 / 2);
		ballsprite.setPosition(x, y);
		kick.loadFromFile("Sounds/kick.ogg");
		kick_sound.setBuffer(kick);
		kick_sound.setVolume(100);
		whistle.loadFromFile("Sounds/referee_whistle.ogg");
		referee_whistle.setBuffer(whistle);
		referee_whistle.setVolume(10);
		g.loadFromFile("Sounds/goal.ogg");
		goal.setBuffer(g);
		goal.setVolume(10);
		goal.setMinDistance(6);
		las_pos.x = 599; las_pos.y = 866;
	}
	//-------------------------------------------------Движение мяча----------------------------------------------
	void ball_physics(Keyboard::Key **KeyKode, soccer_man **p, const int n, RenderWindow &window, float &t, Sprite &c, View &view, bool pause, int &slow){
		float last_x = x;
		float last_y= y;
		window.setKeyRepeatEnabled(false);
		//Обнаружениe гола и работа с ним
		//for (int j = 0; j < n - 1; j++)
		if ((y<227 || y>1506) && (x > 527 && x < 669))
		{
			if (goal_pause){
				soccer_man::repositioning(p, n);
				sniping = false;
				p[team][player_number].set_snipe_move(false);
				slow = 800;
				control = false;
				move = false;
				if (y > 400)
					goal_red++;
				else goal_blue++;
				ballsprite.setPosition(x = 599, y = 866);
				goal_pause = false; return;
			}
			goal_pause = true; goal.play();
			window.draw(ballsprite);
			return;
		}
		if (lost > 0)
			anim += t;
		if (anim >= lost){
			lost = 0;
			anim = 0;
		}
		//Одержание контроля над мячом
		if (imune> 0)
			imune_timer += t;
		if (imune_timer >= imune){
			imune_timer = 0;
			imune = 0;
			for (int i = 0; i < 2; i++)
			for (int j = 0; j <n; j++)
			{
				if ((p[i][j].get_x() <(x + 10) && p[i][j].get_x()>(x - 10)) && (p[i][j].get_y()<(y + 10) && p[i][j].get_y()>(y - 10)))
				{
					control = true;
					move = false;
					to_target = false;
					pass = false;
					if (player_number != j || (player_number == j && team != i)) { slow = 800; sniping = false; p[team][player_number].set_snipe_move(false); imune = 300; lost = 100; }
					for (int j = 0; j < n; j++)p[i][j].set_AI(true);
					player_number = j;
					team = i;
					p[i][player_number].set_AI(false);
				}
			}
		}
		//Условие мотери мяча если дотронулись до линии аута
		if ((y<227 || y>1506) || (x<89 || x>1106) && p[team][player_number].get_stuc() == false)
		{
			control = false; move = true;
		}
		//Контроль мяча
		if (control){
			//установка мяча в соотвецтвии с направлением игрока
			switch (p[team][player_number].get_dir())
			{
			case 1:{ballsprite.setPosition(x = p[team][player_number].get_x() - 15, y = p[team][player_number].get_y() - 4); if (x != las_pos.x)ballsprite.rotate(-(0.5*t)); break; }
			case 2:{ballsprite.setPosition(x = p[team][player_number].get_x() + 15, y = p[team][player_number].get_y() - 4); if (x != las_pos.x)ballsprite.rotate((0.5*t));  break; }
			case 3:{ballsprite.setPosition(x = p[team][player_number].get_x() - 1, y = p[team][player_number].get_y() - 10);
				ballsprite.setRotation(0); 
				if (y != las_pos.y){
			    if (b_anim > 7) b_anim = 0;
				b_anim += 0.03*t; ballsprite.setTextureRect(IntRect(18 * (int)b_anim, 0, 17, 16)); }break;
			}
			case 4:{ballsprite.setPosition(x = p[team][player_number].get_x() - 1, y = p[team][player_number].get_y() + 10); 
				ballsprite.setRotation(0);
				if (y != las_pos.y){
					   if (b_anim < 1) b_anim = 8;
					   b_anim -= 0.03*t; ballsprite.setTextureRect(IntRect(18 * (int)b_anim, 0, 17, 16));
			    } break; }
			case 5:{ballsprite.setPosition(x = p[team][player_number].get_x() - 13, y = p[team][player_number].get_y() - 10); if (x != las_pos.x)ballsprite.rotate(-(0.5*t));  break; }
			case 6:{ballsprite.setPosition(x = p[team][player_number].get_x() + 15, y = p[team][player_number].get_y() - 10); if (x != las_pos.x)ballsprite.rotate((0.5*t)); break; }
			case 8:{ballsprite.setPosition(x = p[team][player_number].get_x() + 15, y = p[team][player_number].get_y()); if (x != las_pos.x)ballsprite.rotate((0.5*t));  break;  }
			case 7:{ballsprite.setPosition(x = p[team][player_number].get_x() - 15, y = p[team][player_number].get_y()); if (x != las_pos.x)ballsprite.rotate(-(0.5*t)); break; }
			}
			//Удар.
			if (anim >= lost)
			if (Keyboard::isKeyPressed(KeyKode[team][5])){
				slow = 2800;
				lost = 100;
				p[team][player_number].set_manual(false);
				if (!sniping) c.setPosition(p[team][player_number].get_x(), p[team][player_number].get_y());
				if ((p[team][player_number].get_stuc() && (c.getPosition().x > 1106 || c.getPosition().x < 89))) sniping = true;
				else sniping = sniping ? false : true;
				p[team][player_number].set_snipe_move(true);
				if (!sniping){
					slow = 800;
					p[team][player_number].set_snipe_move(false);
					target = c.getPosition();
					imune = 100;
					kick_sound.play();
					ballsprite.setRotation(0);
					move = true;
					control = false;
					to_target = true;
					if (p[team][player_number].get_stuc()){
						p[team][player_number].unstuc();
					}
				}
			}
			if (sniping && control){
				if (Keyboard::isKeyPressed(KeyKode[team][0]))
					c.move(-0.8*t, 0);
				if (Keyboard::isKeyPressed(KeyKode[team][1]))
					c.move(0.8*t, 0);
				if (Keyboard::isKeyPressed(KeyKode[team][2]))
					c.move(0, -0.8*t);
				if (Keyboard::isKeyPressed(KeyKode[team][3]))
					c.move(0, 0.8*t);
				window.draw(c);
			}
		}
		//Пас
		if (anim >= lost)
		if ((Keyboard::isKeyPressed(KeyKode[team][6]) && !move && (x != 599 && y != 866)))
		{
			ballsprite.setRotation(0);
			slow = 800;
			p[team][player_number].set_manual(false);
			lost = 400;
			imune = 200;
			kick_sound.play();
			move = true;
			control = false;
			sniping = false;
			p[team][player_number].set_snipe_move(false);
			pass = true;
			if (p[team][player_number].get_stuc()) p[team][player_number].unstuc();
		}
		if (Keyboard::isKeyPressed(KeyKode[team][4])){
			sniping = false;
			p[team][player_number].set_snipe_move(false);
		}
		//Движение после удара и взаэмодействие с аутом
		if (move){
			float d[6];
			if (lost > 0)
				anim += t;
			if (anim >= lost){
				if (x<89 || x>1106){
					sniping = false;
					p[team][player_number].set_snipe_move(false);
					slow = 800;
					anim = 0;
					team = team == 1 ? 0 : 1;
					lost = 300;
					for (int j = 0; j < n - 1; j++)
					{
						d[j] = sqrt((x - p[team][j].get_x())*(x - p[team][j].get_x()) + (y - p[team][j].get_y())*(y - p[team][j].get_y()));
					}
					for (int j = 0; j < n - 1; j++)
					{
						if (d[j] < d[nearest]) nearest = j;
					}
					referee_whistle.play();
					if (x < 400) {
						p[team][nearest].set_x(x=x - 30);
						p[team][nearest].set_y(y);
						p[team][nearest].set_dir(2);
					}
					if (x > 400) {
						p[team][nearest].set_x(x = x + 30);
						p[team][nearest].set_y(y);
						p[team][nearest].set_dir(1);
					}
					p[team][nearest].set_start(p[team][nearest].get_x(), p[team][nearest].get_y());
					p[team][nearest].set_stuc(true);
					move = false;
				}
				if (y<227 || y>1506){
					sniping = false;
					p[team][player_number].set_snipe_move(false);
					slow = 800;
					referee_whistle.play();
					lost = 0;
					anim = 0;
					team = team == 1 ? 0 : 1;
					lost = 300;
					for (int j = 0; j < n - 1; j++)
					{
						d[j] = sqrt((x - p[team][j].get_x())*(x - p[team][j].get_x()) + (y - p[team][j].get_y())*(y - p[team][j].get_y()));
					}
					for (int j = 0; j < n - 1; j++)
					{
						if (d[j] < d[nearest]) nearest = j;
					}
					if (y < 800 && team != 0){
						soccer_man::coner_repositioning_L(p, n);
						if (x < 601) {
							p[team][nearest].set_x(60);
							p[team][nearest].set_y(235);
							p[team][nearest].set_dir(2);
						}
						else{
							p[team][nearest].set_x(1135);
							p[team][nearest].set_y(235);
							p[team][nearest].set_dir(1);
						}

					}
					if (y > 800 && team != 1){
						soccer_man::coner_repositioning_R(p, n);
						if (x < 601) {
							p[team][nearest].set_x(60);
							p[team][nearest].set_y(1505);
							p[team][nearest].set_dir(2);
						}
						else{
							p[team][nearest].set_x(1135);
							p[team][nearest].set_y(1505);
							p[team][nearest].set_dir(1);
						}
					}
					move = false;
					if (y > 800 && team != 1 || y < 800 && team != 0){
						control = false;
						p[team][nearest].set_start(p[team][nearest].get_x(), p[team][nearest].get_y());
						p[team][nearest].set_stuc(true);
						ballsprite.setPosition(x = p[team][nearest].get_x(), y = p[team][nearest].get_y());
					}
					else
					{
						control = false;
						ballsprite.setPosition(x = p[team][5].get_x(), y = p[team][5].get_y());
					}
				}
			}
			if (pass)
			{
				pos = Vector2f(p[team][p[team][player_number].get_chosen_player()].get_x(), p[team][p[team][player_number].get_chosen_player()].get_y());
				distance = sqrt((x - pos.x)*(x - pos.x) + (y - pos.y)*(y - pos.y));
				c.setPosition(pos.x, pos.y);
			}
			if (to_target) pos = target;
			if (!pass) distance = sqrt((pos.x - x)*(pos.x - x) + (pos.y - y)*(pos.y - y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
			if (distance>2){//этим условием убираем дергание во время конечной позиции спрайта
				if (Start_distance == 0) 
				{
					Start_distance = sqrt((x - pos.x)*(x - pos.x) + (y - pos.y)*(y - pos.y));
				}
				double f = (Start_distance- distance)*(speed_b /Start_distance);
				double h = 0;
				if (pass) h= 0.05;
				double s = std::fabs(speed_b+h- f);
				if ((s > speed_b) || (c.getPosition().y<227 || c.getPosition().y>1506)) s = speed_b;
				if ((y - pos.y) > 0){
					if (b_anim > 7) b_anim = 0;
					b_anim += s;
				}
				if ((y - pos.y) < 0){
					if (b_anim < 1) b_anim = 8;
					b_anim -= s;
				}
				ballsprite.setTextureRect(IntRect(18*(int)b_anim, 0, 17, 16));
				x += (s)*t*(pos.x - x) / distance;//идем по x с помощью вектора нормали
				y += (s)*t*(pos.y - y) / distance;//идем по y так же
				std::cout << "distance: " << y - pos.y << std::endl;
				ballsprite.setPosition(x, y);
				window.draw(c);
				for (int j = 0; j < n; j++)
				{
					d[j] = sqrt((x - p[team][j].get_x())*(x - p[team][j].get_x()) + (y - p[team][j].get_y())*(y - p[team][j].get_y()));
				}
				for (int j = 0; j < n; j++){
					if (d[j] < d[nearest]) nearest = j;
				}
			}
			else {
				Start_distance = 0;
				for (int j = 0; j < n; j++)
				{
					d[j] = sqrt((x - p[team][j].get_x())*(x - p[team][j].get_x()) + (y - p[team][j].get_y())*(y - p[team][j].get_y()));
				}
				for (int j = 0; j < n; j++){
					if (d[j] < d[nearest]) nearest = j;
				}
			}
		}
		window.draw(ballsprite);
		if (last_x == x || last_y == y) stop = true;
		else stop = false;
		las_pos.x = ballsprite.getPosition().x;
		las_pos.y = ballsprite.getPosition().y;
	}
	//-------------------------------------------Методы получения значения-----------------------------------
	float& get_x(){ return x; }
	float& get_y(){ return y; }
	bool& get_control(){ return control; }
	bool& get_move(){ return move; }
	bool& get_stop(){ return stop; }
	bool& get_pass(){ return pass; }
	bool& get_sniping(){ return sniping; }
	float& get_speed(){ return speed_b; }
	float& get_anim_speed(){ return anim_speed; }
	Sprite& get_ballsprite(){ return ballsprite; }
	int get_goal_b(){ return goal_blue; }
	int get_goal_r(){ return goal_red; }
	int get_nearest(){ return nearest; }
	int get_player_number(){ return player_number; }
	int get_team(){ return team; }
	bool get_goal_pause(){ return goal_pause; }
	//------------------------------------Методи установления значений--------------------------------------
	void set_lost(int l){ lost = l; }
	void set_ballstart(float X, float Y){ ballsprite.setPosition(x = X, y = Y); }
	void set_screen(Image i){ screen=i; }
	//--------------------------------------------Другое----------------------------------------------------
	void restart(){
		pass = false;
		control = false;
		move = false;
		out = false;
		sniping = false;
		to_target = false;
		goal_blue = 0;
		goal_red = 0;
		ballsprite.setPosition(x = 599, y = 866);
	}
};
