#pragma once

#include <SFML/Graphics.hpp>

class Window{
public:
	sf::RenderWindow render_window;
	float delta_time;

private:
	sf::Event event;
	sf::Clock clock;

public:
	void create(unsigned int width, unsigned int height, unsigned int frame_rate_limit, bool vsync);
	void update();
};