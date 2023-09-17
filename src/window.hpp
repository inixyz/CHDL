#pragma once

#include <SFML/Graphics.hpp>

namespace digital_circuit_maker{
class Window{
public:
	void create(const sf::Vector2u size, const bool fullscreen, 
		const unsigned int fps_limit, const bool vsync);
	void handle_events();
	void refresh_delta_time();
	sf::RenderWindow& get_render_window();
	float get_delta_time() const;

private:
	sf::RenderWindow render_window;
	sf::Event event;
	sf::Clock clock;
	float delta_time;
};
}