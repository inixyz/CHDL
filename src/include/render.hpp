#pragma once

#include <SFML/Graphics.hpp>
#include "camera.hpp"
#include "map.hpp"

namespace digital_circuit_maker{
class Render{
public:
	const unsigned int CELL_SIZE = 11;

	bool show_grid;

	void set_map_outline_position(const sf::Vector2u map_size);
	void render(sf::RenderWindow& render_window, const Camera& camera, 
		const Map& map);
	static Render& get_instance(){
		static Render instance;
		return instance;
	}
	Render(Render const&) = delete;
	void operator=(Render const&) = delete;

private:
	const sf::Color COLOR_BACKGROUND = sf::Color(16, 20, 31);
	const sf::Color COLOR_MAP_OUTLINE = sf::Color(166, 170, 181);
	const sf::Color COLOR_GRID = sf::Color(66, 70, 81);

	sf::Vector2u start_pos, end_pos;
	sf::VertexArray map_outline, grid;
	sf::Texture tileset;
	sf::RectangleShape cell, cursor_cell;

	Render();
	void set_bounds(const Camera& camera, const sf::Vector2u map_size);
	sf::Vector2u get_texture_pos(const Map::Cell::Type type, 
		const sf::Vector2i direction) const;
	void set_cell_texture(const Map::Cell& map_cell);
	void draw_cursor_cell(sf::RenderWindow& render_window);
	void draw_world(sf::RenderWindow& render_window, const Map& map);
};
}