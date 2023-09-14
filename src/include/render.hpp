#pragma once

#include <SFML/Graphics.hpp>
#include "window.hpp"
#include "camera.hpp"
#include "map.hpp"

class Render{
private:
	const unsigned int CELL_SIZE = 11;

	const sf::Color COLOR_BACKGROUND = sf::Color(16, 20, 31);
	const sf::Color COLOR_MAP_OUTLINE = sf::Color(166, 170, 181);
	const sf::Color COLOR_GRID = sf::Color(66, 70, 81);

	sf::Vector2u view_start_pos, view_end_pos;
	sf::VertexArray map_outline, grid;

public:
	static Render& get_instance(){
		static Render instance;
		return instance;
	}
	Render(Render const&) = delete;
	void operator=(Render const&) = delete;

	void set_map_outline_position(Map& map);
	void update(Window& window, Camera& camera, Map& map, bool show_grid);

private:
	Render();
	void set_view_bounds(Camera& camera, Map& map);
	void draw_world(Window& window, Camera& camera, bool show_grid);
};