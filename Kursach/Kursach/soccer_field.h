//Клас для работи с камерой, картой и текстом который выводится на экран (время игры и тд.)
#pragma once
#include <SFML\Graphics.hpp>
#include <sstream>
using namespace sf;
class soccer_field{
	//Поля
private:
	//Камери
	View view;
	View minimap;
	//Игровоэ время в минутах
	int minutes = 0;
	//Задержка нажатия клавиш
	float pause_timer = 0;
	float pause_time = 0;
	//Работа с миникартой
	bool visible = true;
	bool models = false;
	bool end_of_time = false;
	float capacity_mini = 80;
	//Кружочки для отображения игровых объектов на миникарте
	CircleShape players;
	CircleShape ball;
	//Текстуры 
	Texture field;
	Texture cursor;
	Texture gate;
	Texture info_tab;
	Texture mini;
	//Спрайты
	Sprite field_sprite;
	Sprite cursor_sprite;
	Sprite gate_sprite;
	Sprite Info_tab_sprite;
	Sprite minimap_sprite;
	//Текст и шрифт для него
	Font font;
	Text text;
public:
	//-------------------------------------------------Конструктор---------------------------------------------------------------
	soccer_field(int h = 5, int w = 7, int x = 0, int y = 400, String f = "Sprites/Field.png")
	{
		field.loadFromFile(f);
		field.setSmooth(true);
		field_sprite.setTexture(field);
		font.loadFromFile("Sprites/Times_New_Roman.ttf");
		cursor.loadFromFile("Sprites/Cursor.png");
		cursor_sprite.setTexture(cursor);
		cursor_sprite.setOrigin(6.25, 6.25);
		gate.loadFromFile("Sprites/Down_gate.png");
		gate_sprite.setTexture(gate);
		gate_sprite.setPosition(526, 1470);
		info_tab.loadFromFile("Sprites/info_tab.png");
		Info_tab_sprite.setTexture(info_tab);
		mini.loadFromFile("Sprites/Minimap.png");
		Info_tab_sprite.setOrigin(245, 0);
		minimap_sprite.setTexture(mini);
		text = Text("", font, 20);
		players.setRadius(20);
		players.setOutlineThickness(8);
		players.setOrigin(20 / 2, 20);
		ball.setRadius(10);
		ball.setOutlineThickness(3);
		ball.setOutlineColor(sf::Color::Black);
		ball.setOrigin(20 / 2, 20 / 2);
		ball.setFillColor(Color(255, 255, 255));
		text.Bold;
	}
	//------------------------------------Работа текстом в верху екрана(время,голы и тд.)-----------------
	void Show_game_Information(float &time, RenderWindow &window, soccer_ball &b)
	{
		text = Text("", font, 20);
		Info_tab_sprite.setPosition(view.getCenter().x + view.getCenter().x, view.getCenter().y - view.getSize().y / 2);
		window.draw(Info_tab_sprite);
		if (time >= 60) minutes++;
		text.setColor(Color::Black);
		std::ostringstream game_time, goal, team, Frame_rate;
		game_time << minutes << ":" << int(time);
		text.setString(game_time.str());
		text.setColor(Color::White);
		text.setPosition(view.getCenter().x + view.getCenter().x - 164, view.getCenter().y - view.getSize().y / 2 + 1);
		window.draw(text);
		goal << "\n" << b.get_goal_b() << ":" << b.get_goal_r();
		text.setString(goal.str());
		text.setPosition(view.getCenter().x + view.getCenter().x - 137, view.getCenter().y - view.getSize().y / 2 + 3);
		window.draw(text);
		team << "\n       Red                     " << "Blue";
		text.setString(team.str());
		text.setColor(Color::Black);
		text.setPosition(view.getCenter().x + view.getCenter().x - 243, view.getCenter().y - view.getSize().y / 2 + 3);
		window.draw(text);
		if (minutes > 5){ end_of_time = true; }
	}
	//-------------------------------------------------Работа с камерой вида-------------------------------------
	void changeview(Event &event){//Зум на колесико миши (пока не испопользуется)
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.delta == 1)
				view.zoom(1.006f);
			if (event.mouseWheelScroll.delta == -1)
				view.zoom(0.995f);
		}
	}
	View Cam(soccer_ball &b, int x, soccer_man **p){
		float camY = b.get_y();
		if (b.get_control()) camY = p[b.get_team()][b.get_player_number()].get_y();
		if (camY<view.getSize().y / 2){ return view; }
		if (camY>(field.getSize().y - view.getSize().y / 2)){ return view; }
		view.setCenter(view.getSize().x / 2, camY);
		return view;
	}
	//-------------------------------------------------Работа с миникартой---------------------------------------
	View minimap_draw(RenderWindow &window, soccer_man **p, float time, soccer_ball &b){
		minimap.setSize(window.getSize().x * 4, window.getSize().y * 4);
		minimap.setCenter(window.getSize().x*2.03, window.getSize().y*2.15);
		window.setView(minimap);
		minimap_sprite.setPosition(70, 181);
		if (pause_time> 0)
			pause_timer += time;
		if (pause_timer >= pause_time){
			pause_timer = 0;
			pause_time = 0;
			pause_time = 150;
			if (Keyboard::isKeyPressed(Keyboard::F3))
				visible = visible == true ? false : true;
			if (Keyboard::isKeyPressed(Keyboard::BackSlash))
				models = models == true ? false : true;
		}
		if (visible){
			window.draw(minimap_sprite);
			if (Keyboard::isKeyPressed(Keyboard::F2))
				capacity_mini += 0.2;
			if (Keyboard::isKeyPressed(Keyboard::F1))
				capacity_mini -= 0.2;
			minimap_sprite.setColor(Color(255, 255, 255, capacity_mini));
			for (int i = 0; i < 2; i++){
				if (i == 0)players.setFillColor(sf::Color::Blue);
				else players.setFillColor(sf::Color::Red);
				for (int j = 0; j < 6; j++){
					if (models == false){
						players.setPosition(p[i][j].get_x(), p[i][j].get_y());
						if (i == b.get_team() && j == b.get_player_number())
							players.setOutlineColor(sf::Color::Green);
						else players.setOutlineColor(sf::Color::Black);
						window.draw(players);
					}
					else window.draw(p[i][j].get_fotsprite());
				}
			}

			ball.setPosition(b.get_x(), b.get_y());
			window.draw(ball);
		}
		window.setView(view);
		return view;
	}
	//-------------------------------------------------Конец времени---------------------------------------
	void End_of_Time(float &time, RenderWindow &window, soccer_ball &b)
	{
		/*RectangleShape pause_m;
		pause_m.setFillColor(Color(13, 45, 150, 1));
		pause_m.setSize(Vector2f(window.getSize().x, window.getSize().y));
		pause_m.setOrigin(window.getSize().x / 2, window.getSize().y / 2);
		pause_m.setPosition(view.getCenter().x,view.getCenter().y);
		window.draw(pause_m);*/
		text = Text("", font, 40);
		if (time >= 60) minutes++;
		text.setColor(Color::Black);
		std::ostringstream goal;
		goal << "GOAL";
		text.setString(goal.str());
		text.setPosition(view.getCenter().x, view.getCenter().y);
		window.draw(text);
	}
	//-------------------------------------------------Методы получения значений-----------------------------
	Sprite& get_field_sprite(){ return field_sprite; }
	Sprite& get_cursor_sprite(){ return cursor_sprite; }
	Sprite& get_gate_sprite(){ return gate_sprite; }
	Texture& get_field(){ return field; }
	View& get_view(){ return view; }
	View& get_minimap(){ return minimap; }
	int get_minutes(){ return minutes; }
	//-------------------------------------------------Методы измeнения значений----------------------------
	void set_minutes(int m){ minutes = 0; }
};
