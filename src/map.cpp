#include "map.hpp"

using namespace dcm;

Map dcm::map;

void Map::create(size_t width, size_t height){
	data = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height));
}