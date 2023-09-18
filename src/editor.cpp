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
	cursor_cell.valid_position = x_ok && y_ok;
}

void Editor::process_rotation(){
	static bool last_key_state = false;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !last_key_state){
		sf::Vector2i new_dir, dir = cursor_cell.direction;

		if(dir == sf::Vector2i(0, -1)) new_dir = sf::Vector2i(-1, 0);
		else if(dir == sf::Vector2i(-1, 0)) new_dir = sf::Vector2i(0, 1);
		else if(dir == sf::Vector2i(0, 1)) new_dir = sf::Vector2i(1, 0);
		else new_dir = sf::Vector2i(0, -1);

		cursor_cell.direction = new_dir;
	}

	last_key_state = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
}

const Editor::CursorCell& Editor::get_cursor_cell() const{
	return cursor_cell;
}

Editor::Editor(){

}
}