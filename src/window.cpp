#include "window.hpp"

namespace digital_circuit_maker{
void Window::create(const sf::Vector2u size, const bool fullscreen, 
	const unsigned int fps_limit, const bool vsync){

	sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

	render_window.create(sf::VideoMode(size.x, size.y), 
		"Digital Circuit Maker v0.0", style);
	render_window.setFramerateLimit(fps_limit);
	render_window.setVerticalSyncEnabled(vsync);
}

void Window::handle_events(){
	while(render_window.pollEvent(event))
		if(event.type == sf::Event::Closed) render_window.close();
}

void Window::refresh_delta_time(){
	delta_time = clock.restart().asSeconds();
}

sf::RenderWindow& Window::get_render_window(){
	return render_window;
}

float Window::get_delta_time() const{
	return delta_time;
}
}