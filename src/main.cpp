#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "window.hpp"
#include "camera.hpp"
#include "map.hpp"
#include "render.hpp"
#include "editor.hpp"
using namespace digital_circuit_maker;

int main(){
	std::ifstream cfg_file("config.json");
	json cfg = json::parse(cfg_file);
	cfg_file.close();

	Window window;
	window.create(sf::Vector2u(cfg["window_size_x"], cfg["window_size_y"]), 
		cfg["fullscreen"], cfg["fps_limit"], cfg["vsync"]);

	Camera camera;
	camera.create(sf::Vector2u(cfg["window_size_x"], cfg["window_size_y"]),
		cfg["camera_move_speed"], cfg["camera_zoom_speed"]);

	Map map;
	map.reset(sf::Vector2u(30, 30));

	Render::get_instance().show_grid = cfg["show_grid"];
	Render::get_instance().set_map_outline_position(map.get_size());

	while(window.get_render_window().isOpen()){
		window.refresh_delta_time();
		window.handle_events();

		if(window.get_render_window().hasFocus()){
			camera.process_movement(window.get_delta_time());
			camera.process_zoom(window.get_delta_time());

			Editor::get_instance().process_movement(window.get_render_window(), 
				map.get_size());
			Editor::get_instance().process_rotation();
		}

		Render::get_instance().render(window.get_render_window(), camera, map);
	}
}