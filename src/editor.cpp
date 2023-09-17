#include "editor.hpp"
#include "render.hpp"

namespace digital_circuit_maker{
void Editor::process_movement(const sf::RenderWindow& render_window, 
	const sf::Vector2u map_size){

	sf::Vector2i mpos = sf::Mouse::getPosition(render_window);
	cursor_cell.position = (sf::Vector2u)render_window.mapPixelToCoords(mpos);
	cursor_cell.position /= Render::get_instance().CELL_SIZE;

	bool x_ok = cursor_cell.position.x < map_size.x;
	bool y_ok = cursor_cell.position.y < map_size.y;
	cursor_cell.valid = x_ok && y_ok;
}

const Editor::CursorCell& Editor::get_cursor_cell() const{
	return cursor_cell;
}

Editor::Editor(){

}
}