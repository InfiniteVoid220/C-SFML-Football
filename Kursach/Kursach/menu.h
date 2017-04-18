#include <SFML/Graphics.hpp>
#include <iostream>
#include "soccer_field.h"
#include <sstream>
#include <fstream>
using namespace std;
using namespace sf;
class Menu{
	//Текстуры
	Texture button_control_t;
	Texture button_save_t;
	Texture button_start_t;
	Texture info_t;
	Texture control_t;
	Texture m;
	Texture c;
	Texture close, resume;
	Texture to_m;
	//Спрайты
	Sprite button_start;
	Sprite button_quit;
	Sprite button_info;
	Sprite button_back;
	Sprite button_team;
	Sprite button_ctrl;
	Sprite button_control[2][8];
	Sprite button_save;
	Sprite Autor;
	Sprite control_sprite;
	Sprite menu;
	Sprite cursor;
	Sprite to_menu;
	Sprite close_sprite, resume_sprite;
	//Схема меню
	RenderTexture rt;
	Image menu_layout;
	//Логика кнопок
	bool ctrl = false;
	bool play = false;
	bool pause = false;
	bool info = false;
	bool rbind = false;
	//Преренастройка управления
	int re_player;
	int re_button;
	//Затемнение для паузы
	RectangleShape pause_m;
	//Текст
	Text Keys;
	Font font;
	//Звук
	SoundBuffer cl;
	Sound click;
	Music menu_them;
public:
	//-------------------------------------------------Конструктор-------------------------------------------------
	Menu(){
		font.loadFromFile("Sprites/Times_New_Roman.ttf");
		Keys = Text("", font, 42);
		m.loadFromFile("Sprites/menu.png");
		menu.setTexture(m);
		int y_map = m.getSize().y;
		menu.setTextureRect(IntRect(0, y_map, m.getSize().x, -y_map));
		rt.create(m.getSize().x, m.getSize().y);
		rt.draw(menu);
		menu.setTextureRect(IntRect(0, 0, m.getSize().x, y_map));
		menu_layout = rt.getTexture().copyToImage();
		button_start_t.loadFromFile("Sprites/no_pres.png");
		button_start.setTexture(button_start_t);
		button_start.setPosition(759, 518);
		button_quit.setTexture(button_start_t);
		button_quit.setPosition(647, 822);
		button_info.setTexture(button_start_t);
		button_info.setPosition(729, 250);
		button_back.setTexture(button_start_t);
		button_back.setPosition(50, 900);
		button_team.setTexture(button_start_t);
		button_team.setPosition(669, 55);
		button_ctrl.setTexture(button_start_t);
		button_ctrl.setPosition(736, 675);
		close.loadFromFile("Sprites/close_button.png");
		resume.loadFromFile("Sprites/resume_button.png");
		close_sprite.setTexture(close);
		resume_sprite.setTexture(resume);
		c.loadFromFile("Sprites/cur.png");
		to_m.loadFromFile("Sprites/To_menu.png");
		cursor.setTexture(c);
		info_t.loadFromFile("Sprites/info.png");
		Autor.setTexture(info_t);
		pause_m.setFillColor(Color(13, 45, 150, 1));
		cl.loadFromFile("Sounds/click.ogg");
		click.setBuffer(cl);
		menu_them.openFromFile("Sounds/Menu.ogg");
		menu_them.setVolume(30);
		to_menu.setTexture(to_m);
		control_t.loadFromFile("Sprites/control_tab.png");
		control_sprite.setTexture(control_t);
		button_control_t.loadFromFile("Sprites/control_bt.png");
		for (int i = 0; i < 2; i++)
		for (int j = 0; j < 8; j++){
			button_control[i][j].setTexture(button_control_t);
		}
		button_save_t.loadFromFile("Sprites/save_button.png");
		button_save.setTexture(button_save_t);
		button_save.setPosition(900, 950);
	}
	//-------------------------------------------------Меню----------------------------------------------------------
	void menu_show(Keyboard::Key **KeyKode, RenderWindow &window, Event &event, soccer_field &a, float t){
		if (menu_them.getStatus() != sf::Music::Status::Playing)
			menu_them.play();
		if (!rbind) window.clear();
		if (event.type == Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();
		a.get_view().reset(sf::FloatRect(0, 0, 1300, 1147));
		window.setView(a.get_view());
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		if (info){
			window.draw(Autor);
			button_back.setTextureRect(IntRect(546, 1683, 200, 200));
			if (button_back.getGlobalBounds().contains(pos.x, pos.y)){
				button_back.setTextureRect(IntRect(546, 1883, 200, 210));
				if (Mouse::isButtonPressed(Mouse::Left)) { info = false; click.play(); }
			}
			window.draw(button_back);
		}
		if (ctrl){
			window.draw(control_sprite);
			std::ostringstream CONTROL;
			for (int j = 0; j < 8; j++)
				CONTROL << Convert_To_Text(KeyKode[0][j]) << "\n";
			for (int i = 0; i < 2; i++)
			for (int j = 0; j < 8; j++){
				button_control[i][j].setPosition(a.get_view().getCenter().x - 174 * (1 - i) + (i * 240), a.get_view().getCenter().y - 48 * (1 - j));
				if (button_control[i][j].getGlobalBounds().contains(cursor.getPosition().x, cursor.getPosition().y) && !rbind){
					button_control[i][j].setTextureRect(IntRect(0, 64, 383, 48));
						if (event.type == Event::MouseButtonPressed){
							rbind = true;
							re_player = i;
							re_button = j;
							click.play();
						}
				}
				else 	button_control[i][j].setTextureRect(IntRect(0, 0, 383, 48));
				window.draw(button_control[i][j]);
			}
			Keys.setString(CONTROL.str());
			Keys.setColor(Color::Black);
			Keys.setPosition(a.get_view().getCenter().x, a.get_view().getCenter().y - 50);
			window.draw(Keys);
			CONTROL.str("");
			for (int j = 0; j < 8; j++)
				CONTROL << Convert_To_Text(KeyKode[1][j]) << "\n";
			Keys.setString(CONTROL.str());
			Keys.setColor(Color::Black);
			Keys.setPosition(a.get_view().getCenter().x + 400, a.get_view().getCenter().y - 50);
			window.draw(Keys);
			if (button_save.getGlobalBounds().contains(pos.x, pos.y) && !rbind){
				button_save.setTextureRect(IntRect(0, 117, 372, 117));
				if (event.type == Event::MouseButtonReleased) { save_binds(KeyKode);  click.play(); }
			}
			else button_save.setTextureRect(IntRect(0, 0, 372, 117));
			window.draw(button_save);
			if (button_back.getGlobalBounds().contains(pos.x, pos.y) && !rbind){
				button_back.setTextureRect(IntRect(546, 1883, 200, 210));
				if (event.type == Event::MouseButtonReleased) {ctrl = false; click.play();}
			}
			else button_back.setTextureRect(IntRect(546, 1683, 200, 200));
			window.draw(button_back);
			if (rbind)	rebinding(KeyKode, re_player, re_button, event, window, a);
		}
		if (!info && !ctrl){
			if (button_start.getGlobalBounds().contains(pos)) {
				button_start.setTextureRect(IntRect(10, 480, 525, 155));
				if (event.type == Event::MouseButtonReleased){
					click.play();
					play = true;
					a.set_minutes(0);
					menu_them.stop();
					window.create(VideoMode(1920,1080), "Soccer", Style::Fullscreen); //Style::Fullscreen
					a.get_view().reset(sf::FloatRect(0, 0, a.get_field().getSize().x, window.getSize().x / 2));
				}
			}
			else button_start.setTextureRect(IntRect(10, 480 * 3 + 81, 525, 155));
			if (menu_layout.getPixel(cursor.getPosition().x, cursor.getPosition().y) == Color(251, 251, 251))

			{
				button_quit.setTextureRect(IntRect(24, 720, 308, 320));
				if (event.type == Event::MouseButtonReleased) { click.play(); window.close(); }
			}
			else button_quit.setTextureRect(IntRect(24, 1760, 308, 320));
			if (menu_layout.getPixel(cursor.getPosition().x, cursor.getPosition().y) == Color(254, 254, 254)) {
				button_info.setTextureRect(IntRect(506, 3, 451, 252));
				if (event.type == Event::MouseButtonReleased) { info = true; click.play(); }
			}
			else button_info.setTextureRect(IntRect(506, 1040, 451, 252));

			if (menu_layout.getPixel(cursor.getPosition().x, cursor.getPosition().y) == Color(253, 253, 253)) {
				button_team.setTextureRect(IntRect(56, 5, 303, 318));
				if (event.type == Event::MouseButtonReleased)  { click.play(); }
			}
			else button_team.setTextureRect(IntRect(56, 1042, 303, 318));
			if (menu_layout.getPixel(cursor.getPosition().x, cursor.getPosition().y) == Color(252, 252, 252)) {
				button_ctrl.setTextureRect(IntRect(560, 434, 439, 270));
				if (event.type == Event::MouseButtonReleased) { click.play();ctrl = true;}
			}
			else button_ctrl.setTextureRect(IntRect(547, 1339, 436, 270));
			window.draw(menu);
			window.draw(button_start);
			window.draw(button_quit);
			window.draw(button_info);
			window.draw(button_team);
			window.draw(button_ctrl);
		}
	}
	//-------------------------------------------------Пауза----------------------------------------------------------
	void Pause(RenderWindow &window, soccer_field &a, soccer_ball &b, soccer_man **p, int n, float &time, Vector2i POSITION, Music& stadium){
		window.setMouseCursorVisible(true);
		pause_m.setFillColor(Color(13, 45, 150, 1));
		pause_m.setSize(Vector2f(window.getSize().x, window.getSize().y));
		pause_m.setOrigin(window.getSize().x / 2, window.getSize().y / 2);
		std::ostringstream pause_text;
		pause_text << " PAUSE";
		pause_m.setPosition(a.get_view().getCenter().x, a.get_view().getCenter().y);
		window.draw(pause_m); 
		Font font;
		font.loadFromFile("Sprites/Times_New_Roman.ttf");
		Text text("", font, 45);
		text.setString(pause_text.str());
		text.setColor(Color::Black);
		text.setPosition(a.get_view().getCenter().x - 75, a.get_view().getCenter().y - 100);
		window.draw(text);
		resume_sprite.setPosition(a.get_view().getCenter().x + 70, a.get_view().getCenter().y);
		to_menu.setPosition(a.get_view().getCenter().x - 150, a.get_view().getCenter().y);
		close_sprite.setPosition(a.get_view().getCenter().x - 40, a.get_view().getCenter().y);
		resume_sprite.setTextureRect(IntRect(0, 90, 90, 90));
		close_sprite.setTextureRect(IntRect(0, 90, 90, 90));
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		if (resume_sprite.getGlobalBounds().contains(pos)){
			resume_sprite.setTextureRect(IntRect(0, 92, 90, 90));
			if (Mouse::isButtonPressed(Mouse::Left)) { click.play(); pause = false; window.setMouseCursorVisible(false); }
		}
		else resume_sprite.setTextureRect(IntRect(0, 0, 92, 90));
		if (close_sprite.getGlobalBounds().contains(pos)){
			close_sprite.setTextureRect(IntRect(0, 90, 90, 90));
			if (Mouse::isButtonPressed(Mouse::Left)){ click.play(); window.close(); }
		}
		else close_sprite.setTextureRect(IntRect(0, 0, 90, 90));
		if (to_menu.getGlobalBounds().contains(pos)){
			to_menu.setTextureRect(IntRect(0, 0, 90, 90));
			if (Mouse::isButtonPressed(Mouse::Left)){
				click.play();
				pause = false;
				play = false;
				b.restart();
				stadium.stop();
				time = 0;
				soccer_man::repositioning(p, n);
				window.create(VideoMode(700, 600), "Soccer", sf::Style::Default);
			}
		}
		else to_menu.setTextureRect(IntRect(0, 90, 90, 90));
		window.draw(resume_sprite);
		window.draw(close_sprite);
		window.draw(to_menu);
	}
	//-------------------------------------------------Вывод видоизмененного курсора----------------------
	void cursor_in_window(RenderWindow &window){
		window.setMouseCursorVisible(false);
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		if (!(pixelPos.x > window.getSize().x || pixelPos.x<0 || pixelPos.y>window.getSize().y - 2 || pixelPos.y <0)){ cursor.setPosition(pos); }
		else window.setMouseCursorVisible(true);
		window.draw(cursor);
	}
	//-------------------------------------------------Переназначение клавиш-----------------------------------
	void rebinding(Keyboard::Key **KeyKode, int i, int j, Event &event, RenderWindow &window, soccer_field &a){
		std::ostringstream pause_text;
		Text text;
		text = Text("", font, 70);
		text.setColor(Color::Green);
		pause_text << " Click on the key that you want";
		text.setString(pause_text.str());
		text.setPosition(a.get_view().getCenter().x - 450, a.get_view().getCenter().y - 100);
		pause_m.setFillColor(Color(0, 0, 0, 100));
		rbind = true;
		pause_m.setSize(Vector2f(window.getSize().x * 2, window.getSize().y * 2));
		pause_m.setOrigin(pause_m.getSize().x / 2, pause_m.getSize().y / 2);
		pause_m.setPosition(a.get_view().getCenter().x, a.get_view().getCenter().y);
		window.draw(pause_m);
		window.draw(text);
		if (event.type == Event::KeyReleased){
			if (event.key.code == 85 || event.key.code == 86 || event.key.code == 87) { rbind = false; return; }
			for (int r = 0; r < 2; r++)
			for (int t = 0; t < 8; t++){
				if (event.key.code == KeyKode[r][t]){
					KeyKode[r][t] = KeyKode[i][j];
					KeyKode[i][j] = event.key.code;
					rbind = false;
				}
			}
			if (rbind) { KeyKode[i][j] = event.key.code; rbind = false; }
		}
	}
	//-------------------------------------------------Загрузка клавиш с файла----------------------------------
	void load_binds(Keyboard::Key **KeyKode){
		char **KEYS_p1;
		char **KEYS_p2;
		KEYS_p1 = new char *[8];
		KEYS_p2 = new char *[8];
		for (int i = 0; i < 8; i++){
			KEYS_p1[i] = new char[4];
			KEYS_p2[i] = new char[4];
		}
		ifstream fin("Binds.txt");
		if (!fin) return;
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 4; j++){
				fin.get(KEYS_p1[i][j]);
				if (KEYS_p1[i][j] == '\n') { KEYS_p1[i][j] = '\0'; break; }

			}
		}
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 4; j++){
				fin.get(KEYS_p2[i][j]);
				if (KEYS_p2[i][j] == '\n') { KEYS_p2[i][j] = '\0'; break; }
			}
		}
		fin.close();
		for (int i = 0; i < 8; i++){
			KeyKode[0][i] = Keyboard::Key(atoi(KEYS_p1[i]));
			KeyKode[1][i] = Keyboard::Key(atoi(KEYS_p2[i]));
		}
	}
	//-------------------------------------------------Сохранение клавиш в файл-------------------------------
	void save_binds(Keyboard::Key **KeyKode){
		ofstream outfile("Binds.txt");
		for (int i = 0; i < 2; i++)
		for (int j = 0; j < 8; j++)
			outfile << KeyKode[i][j] << "\n";
		outfile.close();
	}
	void set_rbind(bool r){ rbind = r; }
	bool get_play(){ return play; }
	bool get_pause(){ return pause; }
	void set_pause(bool p){ pause = p; }
	//-------------------------------------------------Перевод кода кнопки в текст-----------------------------
	char* Convert_To_Text(Keyboard::Key key){
		switch (key)
		{
		case 0:{char *rez = "A"; return rez; }  case 14:{char *rez = "O"; return rez; }
		case 28:{char *rez = "2"; return rez; } case 43:{char *rez = "SPACE"; return rez; }
		case 46:{char *rez = "["; return rez; }  case 1:{char *rez = "B"; return rez; }
		case 15:{char *rez = "P"; return rez; } case 29:{char *rez = "3"; return rez; }
		case 41:{char *rez = "RALT"; return rez; }	case 47:{char *rez = "]"; return rez; }
		case 2:{char *rez = "C"; return rez; }   case 16:{char *rez = "Q"; return rez; }
			   case 30 : {char *rez = "4"; return rez; }case 45:{char *rez = "A"; return rez; }
			   case 53:{char *rez = "\\"; return rez; } case 3:{char *rez = "D"; return rez; }
			   case 17:{char *rez = "R"; return rez; } case 31:{char *rez = "5"; return rez; }
			   case 65:{char *rez = "INS"; return rez; }case 48:{char *rez = ";"; return rez; }
			   case 4:{char *rez = "E"; return rez; }  case 18:{char *rez = "S"; return rez; }
			   case 32:{char *rez = "6"; return rez; }case 42:{char *rez = "RShift"; return rez; }
			   case 51:{char *rez = "'"; return rez; }  case 5:{char *rez = "F"; return rez; }
			   case 19:{char *rez = "T"; return rez; }case 33:{char *rez = "7"; return rez; }
			   case 63:{char *rez = "End"; return rez; }case 85:{char *rez = "F1"; return rez; }
			   case 6:{char *rez = "G"; return rez; }  case 20:{char *rez = "U"; return rez; }
			   case 34:{char *rez = "8"; return rez; }case 61:{char *rez = "PgUp"; return rez; }
			   case 86:{char *rez = "F2"; return rez; } case 7:{char *rez = "H"; return rez; }
			   case 21:{char *rez = "V"; return rez; }  case 35:{char *rez = "9"; return rez; }
			   case 62:{char *rez = "PgDp"; return rez; }case 87:{char *rez = "F3"; return rez; }
			   case 8:{char *rez = "I"; return rez; }  case 22:{char *rez = "W"; return rez; }
			   case 36:{char *rez = "A"; return rez; }case 59:{char *rez = "Back"; return rez; }
			   case 88:{char *rez = "F4"; return rez; }case 9:{char *rez = "J"; return rez; }
			   case 23:{char *rez = "X"; return rez; }  case 37:{char *rez = "Ctrl"; return rez; }
			   case 55:{char *rez = "+"; return rez; }case 89:{char *rez = "F5"; return rez; }
			   case 10:{char *rez = "K"; return rez; }  case 24:{char *rez = "Y"; return rez; }
			   case 38:{char *rez = "LShift"; return rez; }	case 56:{char *rez = "-"; return rez; }
			   case 90:{char *rez = "F6"; return rez; }case 11:{char *rez = "L"; return rez; }
			   case 25:{char *rez = "Z"; return rez; }  case 39:{char *rez = "LAlt"; return rez; }
			   case 49:{char *rez = ","; return rez; }case 91:{char *rez = "F7"; return rez; }
			   case 12:{char *rez = "M"; return rez; } case 26:{char *rez = "0"; return rez; }
			   case 54:{char *rez = "~"; return rez; }case 50:{char *rez = "."; return rez; }
			   case 92:{char *rez = "F8"; return rez; }case 13:{char *rez = "N"; return rez; }
			   case 27:{char *rez = "1"; return rez; }  case 60:{char *rez = "TAB"; return rez; }
			   case 52:{char *rez = "/"; return rez; }case 93:{char *rez = "F9"; return rez; }
			   case 94:{char *rez = "F10"; return rez; }   case 81:{char *rez = "Num 6"; return rez; }
			   case 95:{char *rez = "F11"; return rez; }   case 82:{char *rez = "Num 7"; return rez; }
			   case 96:{char *rez = "F12"; return rez; }   case 83:{char *rez = "Num 8"; return rez; }
			   case 69:{char *rez = "*"; return rez; }     case 84:{char *rez = "Num 9"; return rez; }
			   case 70:{char *rez = "/"; return rez; }     case 73:{char *rez = "Up"; return rez; }
			   case 68:{char *rez = "-"; return rez; }	    case 74:{char *rez = "Down"; return rez; }
			   case 67:{char *rez = "+"; return rez; }     case 71:{char *rez = "Left"; return rez; }
			   case 75:{char *rez = "0"; return rez; }     case 72:{char *rez = "Right"; return rez; }
			   case 76:{char *rez = "Num 1"; return rez; }
			   case 77:{char *rez = "Num 2"; return rez; }
			   case 78:{char *rez = "Num 3"; return rez; }
			   case 79:{char *rez = "Num 4"; return rez; }
			   case 80:{char *rez = "Num 5"; return rez; }
			   default:{char *rez = "Bad key"; return rez; }
		}
	}
};
