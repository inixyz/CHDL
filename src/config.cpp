#include "config.hpp"
#include <fstream>
#include "json.hpp"

using namespace dcm;

Config dcm::config;

void Config::load(std::string file_name){
	std::ifstream file(file_name);
	nlohmann::json json = nlohmann::json::parse(file);
	file.close();

	window_width = json["window_width"];
	window_height = json["window_height"];
	frame_rate_limit = json["frame_rate_limit"];
	vsync = json["vsync"];
}