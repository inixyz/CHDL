#include "render.hpp"

void Render::set_map_outline_position(Map& map){
	map_outline[0].position = map_outline[4].position = sf::Vector2f(0, 0);
	map_outline[1].position = sf::Vector2f(map.width, 0) * (float)CELL_SIZE;
	map_outline[2].position = sf::Vector2f(map.width, map.height) * (float)CELL_SIZE;
	map_outline[3].position = sf::Vector2f(0, map.height) * (float)CELL_SIZE;
}

void Render::update(Window& window, Camera& camera, Map& map, bool show_grid){
	set_view_bounds(camera, map);

	window.render_window.clear(COLOR_BACKGROUND);
	draw_world(window, camera, show_grid);
	window.render_window.display();
}

Render::Render(){
	map_outline = sf::VertexArray(sf::LineStrip, 5);
	for(unsigned int i = 0; i < map_outline.getVertexCount(); i++) map_outline[i].color = COLOR_MAP_OUTLINE;

	grid = sf::VertexArray(sf::Lines, 2);
	grid[0].color = grid[1].color = COLOR_GRID;
}

void Render::set_view_bounds(Camera& camera, Map& map){
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

void Render::draw_world(Window& window, Camera& camera, bool show_grid){
	window.render_window.setView(camera.view);

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