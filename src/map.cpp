#include "map.hpp"

void Map::reset(unsigned int width, unsigned int height){
	this->width = width;
	this->height = height;

	cells = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height));
}