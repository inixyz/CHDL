#pragma once

#include <SFML/Graphics.hpp>
#include "window.hpp"

class Camera{
public:
	sf::View view;

private:
	float move_speed, zoom_speed;
	float zoom_factor = 1;

public:
	void create(unsigned int width, unsigned int height, float move_speed, float zoom_speed);
	void update(Window& window);
};