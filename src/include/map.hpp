#pragma once

#include <vector>
#include <list>
#include <SFML/System.hpp>

namespace digital_circuit_maker{
class Map{
public:
	struct WireGroup{
		unsigned int state = 0;
		std::vector<sf::Vector2u> connected_inputs;
	};
	struct Cell{
		enum Type{
			EMPTY, WIRE, JUNCTION, AND_GATE, NAND_GATE, OR_GATE, NOR_GATE, 
			XOR_GATE, XNOR_GATE, NOT_GATE, BUFFER_GATE
		}type = EMPTY;

		WireGroup* wire_group = NULL;
		void (*behaviour)() = NULL;
		sf::Vector2i direction = sf::Vector2i(0, 0);
		bool last_out = false;
	};

	std::vector<std::vector<Cell>> cells;
	std::list<WireGroup> wire_groups;

	void reset(const sf::Vector2u size);
	sf::Vector2u get_size() const;

private:
	sf::Vector2u size;
};
}