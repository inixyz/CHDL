#pragma once

#include "map.hpp"
#include <SFML/Graphics.hpp>

namespace digital_circuit_maker{
class Editor{
public:
	struct CursorCell{
		bool valid_position = false;
		Map::Cell::Type type;
		sf::Vector2u position;
		sf::Vector2i direction;
	};

	void process_movement(const sf::RenderWindow& render_window, 
		const sf::Vector2u map_size);
	void process_rotation();
	const CursorCell& get_cursor_cell() const;
	static Editor& get_instance(){
		static Editor instance;
		return instance;
	}
	Editor(Editor const&) = delete;
	void operator=(Editor const&) = delete;

private:
	CursorCell cursor_cell;

	Editor();
};
}