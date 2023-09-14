#pragma once

#include <SFML/System.hpp>

sf::Vector2f vector2f_normalize(sf::Vector2f vec){
	float magnitude = vec.x * vec.x + vec.y * vec.y;
	if(magnitude > 0) vec /= magnitude;
	return vec;
}