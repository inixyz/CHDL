#include "render.hpp"
#include "window.hpp"
#include "map.hpp"

#define COLOR_BACKGROUND sf::Color(16, 20, 31)
#define COLOR_GRID sf::Color(26, 30, 41)

using namespace dcm;

Render dcm::render;

void Render::update_outline_position(){
	outline[0].position = sf::Vector2f(0, 0);
	outline[1].position = sf::Vector2f(map.width * CELL_SIZE, 0);
	outline[2].position = sf::Vector2f(map.width * CELL_SIZE, map.height * CELL_SIZE);
	outline[3].position = sf::Vector2f(0, map.height * CELL_SIZE);
	outline[4].position = sf::Vector2f(0, 0);
}

void Render::setup(){
	outline = sf::VertexArray(sf::LineStrip, 5);
	for(size_t i = 0; i < outline.getVertexCount(); i++) outline[i].color = COLOR_GRID;
	update_outline_position();
}

void Render::update(){
	window.render_window.clear(COLOR_BACKGROUND);

	window.render_window.draw(outline);

	window.render_window.display();
}