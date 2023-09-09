#pragma once

#include <SFML/Graphics.hpp>

namespace dcm{
	class Render{
	private:
		const size_t CELL_SIZE = 11;
		sf::VertexArray outline;

	public:
		void update_outline_position();
		void setup();
		void update();
	};

	extern Render render;
}