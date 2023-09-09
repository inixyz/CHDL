#include "render.hpp"
#include "window.hpp"
#include "map.hpp"

using namespace dcm;

Render dcm::render;

void Render::setup(){

}

void Render::update(){
	window.render_window.clear();

	// const size_t CELL_SIZE = 11;

	// sf::VertexArray block(sf::TriangleStrip, 4);

	// for(size_t x = 0; x < map.width; x++){
	// 	for(size_t y = 0; y < map.height; y++){
	// 		block[0].position = sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE);
	// 		block[1].position = sf::Vector2f(x * CELL_SIZE, (y + 1) * CELL_SIZE);
	// 		block[2].position = sf::Vector2f((x + 1) * CELL_SIZE, y * CELL_SIZE);
	// 		block[3].position = sf::Vector2f((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);

	// 		switch(map.cells[x][y].type){
	// 		case Map::Cell::Type::WIRE:
	// 			if(true) for(unsigned short i = 0; i < 4; i++) block[i].color = sf::Color(251, 242, 54);
	// 			else for(unsigned short i = 0; i < 4; i++) block[i].color = sf::Color(75, 72, 17);

	// 			window.render_window.draw(block);
	// 		}
	// 	}
	// }

	window.render_window.display();
}