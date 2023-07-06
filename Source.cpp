#include<iostream>
#include<SFML/Graphics.hpp>
#include<time.h>
using namespace std;
using namespace sf;
float clip(float n, float lower, float upper)
{
	n = (n > lower) * n + !(n > lower) * lower;
	return (n < upper) * n + !(n < upper) * upper;
}
bool collision(sf::RectangleShape r, sf::CircleShape c)
{
	float closex = clip(c.getPosition().x, r.getPosition().x, r.getPosition().x + r.getSize().x);
	float closey = clip(c.getPosition().y, r.getPosition().y, r.getPosition().y + r.getSize().y);
	float distx = c.getPosition().x - closex;
	float disty = c.getPosition().y - closey;
	int distsq = (distx) * (distx)+(disty) * (disty);
	if (distsq < c.getRadius() * c.getRadius() && closey == c.getPosition().y)
	{
		return 1;
	}
	else if (distsq < c.getRadius() * c.getRadius() && closex != c.getPosition().x && closey != c.getPosition().y)
	{ 
		return 1;
	}
	else if (distsq < c.getRadius() * c.getRadius() && closex == c.getPosition().x)
	{
		return 1;
	}
}
class ball
{
private:
	float radius;
	sf::CircleShape c;
public:
	int x, y;
	float speedx;
	float speedy;
	ball(sf::RenderWindow& win)
	{
		this->radius = 20;
		this->x = 50;
		this->y = win.getSize().y / 3;
		this->speedx = 10;
		this->speedy = 7;
	}
	void draw(sf::RenderWindow& win)
	{
		c.setRadius(this->radius);
		c.setFillColor(sf::Color::Black);
		c.setPosition(this->x, this->y);
		win.draw(c);
	}
	void ballkomovekrrhyhain(sf::RenderWindow& win, sf::CircleShape s)
	{
		this->x += speedx;
		this->y += speedy;
	}
	sf::CircleShape getball()
	{
		return c;
	}
};
class paddle
{
public:
	sf::RectangleShape* r;
	int x, y;
	int speed;
	paddle(sf::RenderWindow& win)
	{
		this->x = win.getPosition().x;
		this->y = win.getPosition().y;
		this->speed = 30;
	}
	void draw(sf::RenderWindow& win, sf::Color color)
	{
		r = new sf::RectangleShape(sf::Vector2f(20, 110));
		r->setFillColor(color);
		r->setPosition(this->x, this->y);
		win.draw(*r);
	}
	sf::RectangleShape getrect()
	{
		return *this->r;
	}
};
void BOT(paddle*& racket, ball*& b, sf::RenderWindow& win)
{
	if (b->y > 0 && b->y < win.getSize().y)
	{

		racket->r->setPosition(racket->r->getPosition().x, b->getball().getPosition().y);

		if (rand() % 10 + 1 == 2)
		{
			if (collision(racket->getrect(), b->getball()) == 1)
			{
				srand(time(0));
				b->getball().setPosition(b->getball().getPosition().x + 120.f, b->getball().getPosition().y);
				if (b->speedx > 0) {
					b->speedx *= 1; b->speedx++;
					b->speedy = -b->speedy;
				}
				else
				{
					b->speedx *= -1;
					b->speedx--;
					b->speedy = -b->speedy;
				}
			}
		}
	}
}
int main()
{
	RenderWindow win(VideoMode(1366, 768), "pong", Style::Close);
	ball* B = new ball(win); sf::Event e;
	paddle* right = new paddle(win);
	paddle* left = new paddle(win);
	bool playauto = true;
	// basic collision detection 

	win.setFramerateLimit(60);
	// background 
	sf::Sprite bg; sf::Texture background; background.loadFromFile("b.jpg"); bg.setTexture(background);
	//=================
	// set paddle position
	right->x = win.getSize().x - 100.f;
	right->y = win.getSize().y / 2 - 50.f;
	right->draw(win, sf::Color::Black);
	// set paddle position
	left->x = 50.f;
	left->y = win.getSize().y / 2 - 50.f;
	left->draw(win, sf::Color::Black);

	//----------------------
	// Scores
	int sc1 = 0, sc2 = 0;
	Font f; f.loadFromFile("font1.ttf");
	Text S1("Score : " + std::to_string(sc1), f);
	Text S2("Score : " + std::to_string(sc2), f);
	S1.setCharacterSize(50); S1.setFillColor(sf::Color::Black);
	S1.setPosition(200, 100);
	S2.setCharacterSize(50); S2.setFillColor(sf::Color::Black);
	S2.setPosition(win.getSize().x - 400.f, 100);

	bool dead = false;
	while (win.isOpen())
	{
		while (win.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				win.close();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			right->r->move(0, -10.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			right->r->move(0, 10.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			left->r->move(0, -10.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			left->r->move(0, 10.f);
		}
		if (playauto)
		{
			BOT(left, B, win);
		}
		if (sc1 == 10 || sc2 == 10 || dead == true)
		{
			dead = true;
			win.clear();
			win.draw(bg);
			sf::Text End;
			End.setPosition(win.getSize().x / 2, win.getSize().y / 2);
			End.setFillColor(sf::Color::Red);
			End.setCharacterSize(60);
			End.setFont(f);
			if (sc1 > sc2)
				End.setString("Game Over!\n Player 1 Wins!");
			else 	End.setString("Game Over!\n Player 2 Wins!");
			win.draw(End);
		}
		if (B->x >= win.getSize().x)
		{
			sc1++; // left player's score
			S1.setString("Score = > " + std::to_string(sc1));
			B->x = win.getSize().x / 2;
			B->y = win.getSize().y / 2;
			B->speedx = -5;
		}
		if (B->x <= 0)
		{
			sc2++; // left player's score
			S2.setString("Score = > " + std::to_string(sc2));
			B->x = win.getSize().x / 2;
			B->y = win.getSize().y / 2;
			B->speedx = 5;
		}
		if (B->getball().getPosition().y >= win.getSize().y) // bottom
		{
			B->speedy = -B->speedy;
		}
		if (B->y <= 0)
		{
			B->speedy *= -1;
		}
		B->ballkomovekrrhyhain(win, B->getball());
		if (collision(*right->r, B->getball()) == 1)
		{
			B->getball().setPosition(right->x - 500.f, B->getball().getPosition().y);
			if (B->speedx > 0)
			{
				B->speedx *= -1;
				B->speedx--;
				B->speedy = -B->speedy;
			}
			else
			{
				B->speedx *= 1; B->speedx++;
				B->speedy = -B->speedy;
			}
		}
		if (collision(*left->r, B->getball()) == 1)
		{
			B->getball().setPosition(B->getball().getPosition().x + 120.f, B->getball().getPosition().y);
			if (B->speedx > 0) {
				B->speedx *= 1; B->speedx++;
				B->speedy = -B->speedy;
			}
			else
			{
				B->speedx *= -1;
				B->speedx--;
				B->speedy = -B->speedy;
			}
		}
		if (!dead)
		{
			win.draw(bg);
			win.draw(right->getrect());
			win.draw(left->getrect());
			win.draw(S1);
			win.draw(S2);
			B->draw(win);
		}
		win.display();
	}

}