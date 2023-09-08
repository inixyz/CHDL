#pragma once

#include <vector>

namespace dcm{
	class Map{
	public:
		struct Position{
			unsigned int x, y;
		};

		struct WireGroup{
			bool state;
			std::vector<Position> connected_inputs;
		};

		struct Cell{
			enum Type{
				EMPTY, WIRE, GATE
			}type;

			union{
				WireGroup* wire_group;
				void (*behaviour)();
			};
		};

	public:
		std::vector<std::vector<Cell>> data;

	public:
		void create(size_t width, size_t height);
	};

	extern Map map;
}