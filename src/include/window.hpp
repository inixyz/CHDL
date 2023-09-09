#pragma once

#include <SFML/Graphics.hpp>

namespace dcm{
	class Window{
	public:
		sf::RenderWindow render_window;
		float delta_time;

	private:
		sf::Event event;
		sf::Clock delta_time_clock;

	public:
		void setup();
		void update();
	};

	extern Window window;
}