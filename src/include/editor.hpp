#pragma once

#include "map.hpp"
#include <SFML/System.hpp>
#include "window.hpp"

class Editor{
public:
	struct{
		bool valid;
		Map::Cell::Type type;
		sf::Vector2u position;
		sf::Vector2i direction;
	}cursor_cell;

public:
	static Editor& get_instance(){
		static Editor instance;
		return instance;
	}

	Editor(Editor const&) = delete;
	void operator=(Editor const&) = delete;

	void update(Window& window, Map& map);

private:
	Editor();
};