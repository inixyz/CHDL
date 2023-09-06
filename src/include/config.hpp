#pragma once

#include <string>

namespace dcm{
	class Config{
	public:
		unsigned int window_width = 640;
		unsigned int window_height = 480;
		unsigned int frame_rate_limit = 30;
		bool vsync = true;

	public:
		void load(std::string file_name);
	};

	extern Config config;
}