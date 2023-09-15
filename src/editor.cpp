#include "editor.hpp"
#include "render.hpp"

void Editor::update(Window& window, Map& map){
	if(window.render_window.hasFocus()){
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window.render_window);
		cursor_cell.position = (sf::Vector2u)window.render_window.mapPixelToCoords(mouse_position);
		cursor_cell.position /= Render::get_instance().CELL_SIZE;
	}

	cursor_cell.valid = cursor_cell.position.x < map.width && cursor_cell.position.y < map.height;
}

Editor::Editor(){
	
}