#include "config.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "map.hpp"
#include "editor.hpp"
#include "render.hpp"

int main(){
	Config config;
	config.load("config.json");

	Window window;
	window.create(config.window_width, config.window_height, config.fullscreen, config.frame_rate_limit, config.vsync);

	Camera camera;
	camera.create(config.window_width, config.window_height, config.camera_move_speed, config.camera_zoom_speed);

	Map map;
	map.reset(30, 30);

	Render::get_instance().set_map_outline_position(map);

	while(window.render_window.isOpen()){
		window.update();
		camera.update(window);
		Editor::get_instance().update(window);
		Render::get_instance().update(window, camera, map, config.show_grid);
	}
}