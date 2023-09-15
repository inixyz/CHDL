#include "render.hpp"
#include "editor.hpp"

void Render::set_map_outline_position(Map& map){
	map_outline[0].position = map_outline[4].position = sf::Vector2f(0, 0);
	map_outline[1].position = sf::Vector2f(map.width, 0) * (float)CELL_SIZE;
	map_outline[2].position = sf::Vector2f(map.width, map.height) * (float)CELL_SIZE;
	map_outline[3].position = sf::Vector2f(0, map.height) * (float)CELL_SIZE;
}

void Render::update(Window& window, Camera& camera, Map& map, bool show_grid){
	set_view_bounds(camera, map);

	window.render_window.clear(COLOR_BACKGROUND);
	draw_world(window, camera, map, show_grid);
	draw_cursor_cell(window);
	window.render_window.display();
}

Render::Render(){
	map_outline = sf::VertexArray(sf::LineStrip, 5);
	for(unsigned int i = 0; i < map_outline.getVertexCount(); i++) map_outline[i].color = COLOR_MAP_OUTLINE;

	grid = sf::VertexArray(sf::Lines, 2);
	grid[0].color = grid[1].color = COLOR_GRID;

	tileset.loadFromFile("assets/tileset.png");
	cell.setTexture(&tileset);
	cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));

	cursor_cell.setTexture(&tileset);
	cursor_cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	cursor_cell.setFillColor(sf::Color(255, 255, 255));
}

void Render::set_view_bounds(Camera& camera, Map& map){
	static float last_zoom_factor;
	static sf::Vector2f last_camera_center;

	if(camera.view.getCenter() == last_camera_center && camera.zoom_factor == last_zoom_factor) return;
	else{
		last_camera_center = camera.view.getCenter();
		last_zoom_factor = camera.zoom_factor;
	}

	sf::Vector2f camera_start_pos, camera_end_pos;
	camera_start_pos.x = camera.view.getCenter().x - camera.view.getSize().x / 2 - CELL_SIZE;
	camera_start_pos.y = camera.view.getCenter().y - camera.view.getSize().y / 2 - CELL_SIZE;
	camera_end_pos.x = camera.view.getCenter().x + camera.view.getSize().x / 2 + CELL_SIZE;
	camera_end_pos.y = camera.view.getCenter().y + camera.view.getSize().y / 2 + CELL_SIZE;

	view_start_pos.x = std::max(0, std::min((int)(camera_start_pos.x / CELL_SIZE), (int)map.width));
	view_start_pos.y = std::max(0, std::min((int)(camera_start_pos.y / CELL_SIZE), (int)map.height));
	view_end_pos.x = std::max(0, std::min((int)(camera_end_pos.x / CELL_SIZE), (int)map.width));
	view_end_pos.y = std::max(0, std::min((int)(camera_end_pos.y / CELL_SIZE), (int)map.height));
}

sf::Vector2u Render::get_texture_pos(Map::Cell::Type type, sf::Vector2i direction){
	sf::Vector2u texture_pos;

	switch(type){
	case Map::Cell::Type::WIRE: texture_pos = sf::Vector2u(1, 0); break;
	case Map::Cell::Type::JUNCTION: texture_pos = sf::Vector2u(3, 0); break;
	case Map::Cell::Type::AND_GATE: texture_pos = sf::Vector2u(4, 0); break;
	case Map::Cell::Type::NAND_GATE: texture_pos = sf::Vector2u(8, 0); break;
	case Map::Cell::Type::OR_GATE: texture_pos = sf::Vector2u(12, 0); break;
	case Map::Cell::Type::NOR_GATE: texture_pos = sf::Vector2u(0, 1); break;
	case Map::Cell::Type::XOR_GATE: texture_pos = sf::Vector2u(4, 1); break;
	case Map::Cell::Type::XNOR_GATE: texture_pos = sf::Vector2u(8, 1); break;
	case Map::Cell::Type::NOT_GATE: texture_pos = sf::Vector2u(12, 1); break;
	case Map::Cell::Type::BUFFER_GATE: texture_pos = sf::Vector2u(0, 2); break;
	}

	if(type != Map::Cell::Type::WIRE && type != Map::Cell::Type::JUNCTION){
		if(direction == sf::Vector2i(0, -1)) texture_pos.x += 1;
		else if(direction == sf::Vector2i(-1, 0)) texture_pos.x += 2;
		else if(direction == sf::Vector2i(0, 1)) texture_pos.x += 3;
	}

	return texture_pos;
}

void Render::update_cell_texture(Map::Cell map_cell){
	static Map::Cell::Type last_type;
	static sf::Vector2i last_direction;

	if(map_cell.type == last_type && map_cell.direction == last_direction) return;
	else{
		last_type = map_cell.type;
		last_direction = map_cell.direction;
	}

	sf::Vector2u texture_pos = get_texture_pos(map_cell.type, map_cell.direction);
	if(map_cell.type == Map::Cell::Type::WIRE && map_cell.wire_group->state) texture_pos.x += 1;

	cell.setTextureRect(sf::IntRect(texture_pos.x * CELL_SIZE, texture_pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE));
}

void Render::draw_world(Window& window, Camera& camera, Map& map, bool show_grid){
	window.render_window.setView(camera.view);

	for(unsigned int x = view_start_pos.x; x < view_end_pos.x; x++){
		for(unsigned int y = view_start_pos.y; y < view_end_pos.y; y++){
			if(map.cells[x][y].type == Map::Cell::Type::EMPTY) continue;

			cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
			update_cell_texture(map.cells[x][y]);
			window.render_window.draw(cell);
		}
	}

	if(show_grid){
		for(unsigned int x = view_start_pos.x + 1; x < view_end_pos.x; x++){
			grid[0].position = sf::Vector2f(x, view_start_pos.y) * (float)CELL_SIZE;
			grid[1].position = sf::Vector2f(x, view_end_pos.y) * (float)CELL_SIZE;

			window.render_window.draw(grid);
		}

		for(unsigned int y = view_start_pos.y + 1; y < view_end_pos.y; y++){
			grid[0].position = sf::Vector2f(view_start_pos.x, y) * (float)CELL_SIZE;
			grid[1].position = sf::Vector2f(view_end_pos.x, y) * (float)CELL_SIZE;

			window.render_window.draw(grid);
		}
	}
	
	window.render_window.draw(map_outline);
}

void Render::draw_cursor_cell(Window& window){
	if(Editor::get_instance().cursor_cell.type == Map::Cell::Type::EMPTY) return;

	Editor::get_instance().cursor_cell.position = Editor::get_instance().cursor_cell.position / CELL_SIZE;
 	cursor_cell.setPosition((sf::Vector2f)(Editor::get_instance().cursor_cell.position * CELL_SIZE));
	sf::Vector2u texture_pos = get_texture_pos(Editor::get_instance().cursor_cell.type, Editor::get_instance().cursor_cell.direction);
	cursor_cell.setTextureRect(sf::IntRect(texture_pos.x * CELL_SIZE, texture_pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE));

	window.render_window.draw(cursor_cell);
}