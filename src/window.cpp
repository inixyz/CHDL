#include "window.hpp"

void Window::create(unsigned int width, unsigned int height, bool fullscreen, unsigned int frame_rate_limit, bool vsync){
	sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
	render_window.create(sf::VideoMode(width, height), "Digital Circuit Maker v0.0", style);

	render_window.setFramerateLimit(frame_rate_limit);
	render_window.setVerticalSyncEnabled(vsync);
}

void Window::update(){
	while(render_window.pollEvent(event))
		if(event.type == sf::Event::Closed)
			render_window.close();

	delta_time = clock.restart().asSeconds();
}
