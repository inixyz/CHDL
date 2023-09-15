#pragma once

#include <string>

class Config{
public:
	unsigned int window_width = 800, window_height = 800;
	bool fullscreen = false;
	unsigned int frame_rate_limit = 60;
	bool vsync = true;
	float camera_move_speed = 250, camera_zoom_speed = 1;
	bool show_grid = true;

public:
	void load(std::string file_name);
};