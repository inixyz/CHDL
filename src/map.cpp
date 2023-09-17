#include "map.hpp"

namespace digital_circuit_maker{
void Map::reset(const sf::Vector2u size){
	this->size = size;

	cells = std::vector<std::vector<Cell>>(size.x, std::vector<Cell>(size.y));
}

sf::Vector2u Map::get_size() const{
	return size;
}
}