#include "soccer_man.h"
#include "soccer_ball.h"
#include "soccer_field.h"
#include "menu.h"
#include <windows.h> 
using namespace sf;
using namespace std;
int soccer_man::active_index1 = 0;
int soccer_man::active_index2 = 0;
int soccer_man::chosen_player = 0;
float soccer_man::pause_time = 0;
void main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	Keyboard::Key **KeyKode;
	Texture goal_tx;
	Sprite goal_sprite;
	goal_tx.loadFromFile("Sprites/Goal.png");
	goal_sprite.setTexture(goal_tx);
	goal_sprite.setTextureRect(IntRect(0, 0, 300, 140));
	KeyKode = new Keyboard::Key *[2];
	for (int i = 0; i < 2; i++){
		KeyKode[i] = new Keyboard::Key[7];
	}
	Music stadium;
	stadium.openFromFile("Sounds/stadium.ogg");
	stadium.setVolume(10);
	RenderWindow window(VideoMode(700, 600), "Soccer");
	Vector2i POSITION(window.getPosition());
	Clock tempore;//время
	int slow = 800;
	float pause_timer = 0;
	float pause_time = 0;
	float menu_anim = 0;
	float game_time = 0;
	int w = 0;
	const int n = 6;
	soccer_man **p;
	p = new soccer_man *[2];
	for (int i = 0; i < 2; i++){
		p[i] = new soccer_man[n];
	}
	soccer_man::repositioning(p, n);
	soccer_man::set_standart_names(p);
	soccer_ball b;
	soccer_field a;
	Menu mn;
	mn.load_binds(KeyKode);
	while (window.isOpen()){
		a.Cam(b, window.getSize().x, p);
		float time = tempore.getElapsedTime().asMicroseconds();
		if (game_time >= 60) game_time = 0;
		tempore.restart();
		time = time / slow;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::LostFocus && mn.get_play())
				mn.set_pause(true);
		}
		if (pause_time> 0)
			pause_timer += time;
		if (pause_timer >= pause_time){
			pause_timer = 0;
			pause_time = 0;
			if (Keyboard::isKeyPressed(Keyboard::Escape)){
				if (!mn.get_play()) mn.set_rbind(false);
				else{
					mn.set_pause(mn.get_pause() == true ? false : true);
					window.setMouseCursorVisible(mn.get_pause());
				}
				pause_time = 350;
			}
		}
		if (mn.get_play() && !mn.get_pause()){
			if (stadium.getStatus() != sf::Music::Status::Playing)
				stadium.play();
			game_time += time / 1300;
			window.setView(a.get_view());
			window.clear();
			window.draw(a.get_field_sprite());
			b.ball_physics(KeyKode, p, n, window, time, a.get_cursor_sprite(), a.get_view(), mn.get_pause(), slow);
			window.setView(a.get_view());
			for (int j = 0; j < n - 1; j++)
				p[0][j].movement(b.get_team(), 0, time, window, b.get_pass(), b.get_control(), KeyKode, b.get_sniping(),0, b.get_x(), b.get_y(), j, b.get_nearest());
			for (int j = 0; j < n - 1; j++)
				p[1][j].movement(b.get_team(), 1, time, window, b.get_pass(), b.get_control(), KeyKode, b.get_sniping(),0, b.get_x(), b.get_y(), j, b.get_nearest());
			p[0][n - 1].golie_movement(KeyKode, time, window, b.get_x(), b.get_y(), b.get_player_number(), b.get_control(), b.get_stop(), p[0][0].get_chosen_player());
			p[1][n - 1].golie_movement(KeyKode, time, window, b.get_x(), b.get_y(), b.get_player_number(), b.get_control(), b.get_stop(), p[1][0].get_chosen_player(), 1428, 3, b.get_team());
			if (!b.get_pass() && b.get_team() == 0) p[0][b.get_player_number()].players_switching(KeyKode, window, p, n, b.get_player_number(), b.get_team(), time, slow, b.get_control());
			if (!b.get_pass() && b.get_team() == 1) p[1][b.get_player_number()].players_switching(KeyKode, window, p, n, b.get_player_number(), b.get_team(), time, slow, b.get_control());
			window.draw(a.get_gate_sprite());
			a.Show_game_Information(game_time, window, b);
			p[0][p[0][2].get_active_index1()].Stamina(window, a.get_view());
			p[1][p[1][0].get_active_index2()].Stamina(window, a.get_view(), 1);
			if (b.get_goal_pause()) 
			{
				goal_sprite.setPosition(a.get_view().getCenter().x-139, a.get_view().getCenter().y-68);
				window.draw(goal_sprite);
				a.minimap_draw(window, p, time, b);
				window.display();
				Sleep(2000);
			}
		}
		a.minimap_draw(window, p, time, b);
		if (!mn.get_play()){
			mn.menu_show(KeyKode, window, event, a, time);
			mn.cursor_in_window(window);
		}
		if (mn.get_pause()){
			mn.Pause(window, a, b, p, n, game_time, POSITION, stadium);
		}
		window.display();
	}
	return;
}
