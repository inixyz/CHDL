#pragma once

#include <vector>
#include <list>

namespace dcm{
	class Map{
	private:
		struct Position{
			unsigned int x, y;
		};

		struct WireGroup{
			unsigned int state;
			std::vector<Position> connected_inputs;
		};

	public:
		struct Cell{
			enum Type{
				EMPTY, WIRE, JUNCTION, AND_GATE, NAND_GATE, OR_GATE, NOR_GATE, 
				XOR_GATE, XNOR_GATE, NOT_GATE, YES_GATE
			}type;

			union{
				WireGroup* wire_group;
				struct{
					bool last_out;
					void (*behaviour)();
				};
			};
		};

	public:
		size_t width, height;
		std::vector<std::vector<Cell>> cells;
		std::list<WireGroup> wire_groups;

	public:
		void reset(size_t width, size_t height);
	};

	extern Map map;
}