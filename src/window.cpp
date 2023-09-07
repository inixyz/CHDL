#include "window.hpp"
#include "config.hpp"

using namespace dcm;

Window dcm::window;

void Window::setup(){
	render_window.create(sf::VideoMode(config.window_width, config.window_height), "DCM v0.0");
	render_window.setFramerateLimit(config.frame_rate_limit);
	render_window.setVerticalSyncEnabled(config.vsync);
}

void Window::update(){
	while(render_window.pollEvent(event))
		if(event.type == sf::Event::Closed)
			render_window.close();
}