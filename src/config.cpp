#include "config.hpp"
#include <fstream>
#include "json.hpp"

void Config::load(std::string file_name){
	std::ifstream file(file_name);
	nlohmann::json json = nlohmann::json::parse(file);
	file.close();

	window_width = json["window_width"];
	window_height = json["window_height"];
	fullscreen = json["fullscreen"];
	frame_rate_limit = json["frame_rate_limit"];
	vsync = json["vsync"];
	camera_move_speed = json["camera_move_speed"];
	camera_zoom_speed = json["camera_zoom_speed"];
	show_grid = json["show_grid"];
}