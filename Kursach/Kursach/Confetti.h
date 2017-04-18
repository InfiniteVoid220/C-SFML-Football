#include <SFML/Graphics.hpp>
class Confetti
{
	private:
		float x;
		float y;
		float size;
		float gravity;
		float rotation;
		float rotation_speed;
		Confetti(float X, float Y)
		{
			x = X;
			y = Y;
			size = (rand()*0.5+0.75)*15;
			gravity = (rand()*0.5 + 0.75) * 0.01;
			rotation = (3.14*2) * rand();
			rotation_speed = (3.14 * 2) * rand()*0.001;
		}
};