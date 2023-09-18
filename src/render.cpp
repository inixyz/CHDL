#include "render.hpp"
#include "imgui/imgui.h"
#include "editor.hpp"

namespace digital_circuit_maker{
void Render::set_map_outline_position(const sf::Vector2u map_size){
	map_outline[0].position = map_outline[4].position = sf::Vector2f(0, 0);
	map_outline[1].position = sf::Vector2f(map_size.x, 0);
	map_outline[2].position = sf::Vector2f(map_size.x, map_size.y);
	map_outline[3].position = sf::Vector2f(0, map_size.y);

	for(unsigned int i = 1; i < map_outline.getVertexCount(); i++)
		map_outline[i].position *= (float)CELL_SIZE;
}

void Render::render(sf::RenderWindow& render_window, const Camera& camera, 
		const Map& map){

	set_bounds(camera, map.get_size());

	render_window.clear(COLOR_BACKGROUND);

	render_window.setView(camera.get_view());
	draw_world(render_window, map);

	ImGui::SFML::Render(render_window);

	render_window.display();
}

Render::Render(){
	map_outline = sf::VertexArray(sf::LineStrip, 5);
	for(unsigned int i = 0; i < map_outline.getVertexCount(); i++) 
		map_outline[i].color = COLOR_MAP_OUTLINE;

	grid = sf::VertexArray(sf::Lines, 2);
	grid[0].color = grid[1].color = COLOR_GRID;

	tileset.loadFromFile("assets/tileset.png");
	cell.setTexture(&tileset);
	cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));

	cursor_cell.setTexture(&tileset);
	cursor_cell.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	cursor_cell.setFillColor(sf::Color(150, 150, 150, 200));
}

void Render::set_bounds(const Camera& camera, const sf::Vector2u map_size){
	const sf::View& view = camera.get_view();

	static float last_zoom_factor;
	static sf::Vector2f last_view_center;

	bool view_center_same = view.getCenter() == last_view_center;
	bool zoom_factor_same = camera.get_zoom_factor() == last_zoom_factor;
	if(view_center_same && zoom_factor_same) return;
	else{
		last_view_center = view.getCenter();
		last_zoom_factor = camera.get_zoom_factor();
	}

	sf::Vector2f view_start_pos, view_end_pos;
	view_start_pos.x = view.getCenter().x - view.getSize().x / 2 - CELL_SIZE;
	view_start_pos.y = view.getCenter().y - view.getSize().y / 2 - CELL_SIZE;
	view_end_pos.x = view.getCenter().x + view.getSize().x / 2 + CELL_SIZE;
	view_end_pos.y = view.getCenter().y + view.getSize().y / 2 + CELL_SIZE;
	view_start_pos /= (float)CELL_SIZE;
	view_end_pos /= (float)CELL_SIZE;

	auto bound = [](const int min, const int val, const int max) -> unsigned int{
		return std::max(min, std::min(val, max));
	};

	start_pos.x = bound(0, view_start_pos.x, map_size.x);
	start_pos.y = bound(0, view_start_pos.y, map_size.y);
	end_pos.x = bound(0, view_end_pos.x, map_size.x);
	end_pos.y = bound(0, view_end_pos.y, map_size.y);	
}

sf::Vector2u Render::get_texture_pos(const Map::Cell::Type type, 
	const sf::Vector2i direction) const{

	sf::Vector2u texture_pos;

	switch(type){
	case Map::Cell::Type::WIRE:        texture_pos = sf::Vector2u( 1, 0); break;
	case Map::Cell::Type::JUNCTION:    texture_pos = sf::Vector2u( 3, 0); break;
	case Map::Cell::Type::AND_GATE:    texture_pos = sf::Vector2u( 4, 0); break;
	case Map::Cell::Type::NAND_GATE:   texture_pos = sf::Vector2u( 8, 0); break;
	case Map::Cell::Type::OR_GATE:     texture_pos = sf::Vector2u(12, 0); break;
	case Map::Cell::Type::NOR_GATE:    texture_pos = sf::Vector2u( 0, 1); break;
	case Map::Cell::Type::XOR_GATE:    texture_pos = sf::Vector2u( 4, 1); break;
	case Map::Cell::Type::XNOR_GATE:   texture_pos = sf::Vector2u( 8, 1); break;
	case Map::Cell::Type::NOT_GATE:    texture_pos = sf::Vector2u(12, 1); break;
	case Map::Cell::Type::BUFFER_GATE: texture_pos = sf::Vector2u( 0, 2); break;
	}

	if(type != Map::Cell::Type::WIRE && type != Map::Cell::Type::JUNCTION){
		if(direction == sf::Vector2i(0, -1)) texture_pos.x += 1;
		else if(direction == sf::Vector2i(-1, 0)) texture_pos.x += 2;
		else if(direction == sf::Vector2i(0, 1)) texture_pos.x += 3;
	}

	return texture_pos;
}

void Render::set_cell_texture(const Map::Cell& map_cell){
	static Map::Cell::Type last_type;
	static sf::Vector2i last_direction;

	bool type_same = map_cell.type == last_type;
	bool direction_same = map_cell.direction == last_direction;
	if(type_same && direction_same) return;
	else{
		last_type = map_cell.type;
		last_direction = map_cell.direction;
	}

	sf::Vector2u texture_pos = get_texture_pos(map_cell.type, map_cell.direction);

	if(map_cell.type == Map::Cell::Type::WIRE)
		if(map_cell.wire_group->state) texture_pos.x += 1;

	sf::IntRect texture_rect(texture_pos.x * CELL_SIZE, 
		texture_pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
	cell.setTextureRect(texture_rect);
}

void Render::draw_cursor_cell(sf::RenderWindow& render_window){
	const Editor::CursorCell& editor_cursor = 
		Editor::get_instance().get_cursor_cell();

	if(editor_cursor.type == Map::Cell::Type::EMPTY) return;
	if(!editor_cursor.valid_position) return;

 	cursor_cell.setPosition((sf::Vector2f)(editor_cursor.position * CELL_SIZE));

	sf::Vector2u texture_pos = get_texture_pos(editor_cursor.type, 
		editor_cursor.direction);

	sf::IntRect texture_rect(texture_pos.x * CELL_SIZE, 
		texture_pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
	cursor_cell.setTextureRect(texture_rect);

	render_window.draw(cursor_cell);
}

void Render::draw_world(sf::RenderWindow& render_window, const Map& map){
	for(unsigned int x = start_pos.x; x < end_pos.x; x++){
		for(unsigned int y = start_pos.y; y < end_pos.y; y++){
			if(map.cells[x][y].type == Map::Cell::Type::EMPTY) continue;

			cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
			set_cell_texture(map.cells[x][y]);
			render_window.draw(cell);
		}
	}

	draw_cursor_cell(render_window);

	if(show_grid){
		for(unsigned int x = start_pos.x + 1; x < end_pos.x; x++){
			grid[0].position = sf::Vector2f(x, start_pos.y) * (float)CELL_SIZE;
			grid[1].position = sf::Vector2f(x, end_pos.y) * (float)CELL_SIZE;

			render_window.draw(grid);
		}

		for(unsigned int y = start_pos.y + 1; y < end_pos.y; y++){
			grid[0].position = sf::Vector2f(start_pos.x, y) * (float)CELL_SIZE;
			grid[1].position = sf::Vector2f(end_pos.x, y) * (float)CELL_SIZE;

			render_window.draw(grid);
		}
	}
	
	render_window.draw(map_outline);
}
}