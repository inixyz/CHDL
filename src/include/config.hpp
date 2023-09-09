#pragma once

#include <string>

namespace dcm{
	class Config{
	public:
		size_t window_width = 640, window_height = 480;
		unsigned int frame_rate_limit = 30;
		bool vsync = true;

	public:
		void load(std::string file_name);
	};

	extern Config config;
}