#pragma once

#include <SFML/Graphics.hpp>

namespace dcm{
	class Window{
	public:
		sf::RenderWindow render_window;

	private:
		sf::Event event;

	public:
		void setup();
		void update();
	};

	extern Window window;
}