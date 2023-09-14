#pragma once

#include <SFML/System.hpp>
#include <vector>
#include <list>

class Map{
private:
	struct WireGroup{
		unsigned int state = 0;
		std::vector<sf::Vector2u> connected_inputs;
	};

public:
	struct Cell{
		enum Type{
			EMPTY, WIRE, JUNCTION, AND_GATE, NAND_GATE, OR_GATE, NOR_GATE, 
			XOR_GATE, XNOR_GATE, NOT_GATE, BUFFER_GATE
		}type = EMPTY;

		WireGroup* wire_group;

		sf::Vector2i direction = sf::Vector2i(0, 0);
		bool last_out = false;
		void (*behaviour)() = NULL;
	};

public:
	unsigned int width, height;
	std::list<WireGroup> wire_groups;
	std::vector<std::vector<Cell>> cells;

public:
	void reset(unsigned int width, unsigned int height);
};
