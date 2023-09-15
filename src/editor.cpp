#include "editor.hpp"

#include <iostream>

void Editor::update(Window& window){
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window.render_window);
	cursor_cell.position = (sf::Vector2u)window.render_window.mapPixelToCoords(mouse_position);
}

Editor::Editor(){
	cursor_cell.type == Map::Cell::Type::WIRE;
}