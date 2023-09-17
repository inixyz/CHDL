#pragma once

#include <SFML/Graphics.hpp>

namespace digital_circuit_maker{
class Camera{
public:
	void create(const sf::Vector2u size, const float move_speed, 
		const float zoom_speed);
	void process_movement(const float delta_time);
	void process_zoom(const float delta_time);
	const sf::View& get_view() const;
	float get_zoom_factor() const;

private:
	sf::View view;
	float move_speed, zoom_speed; 
	float zoom_factor = 1;
};
}
