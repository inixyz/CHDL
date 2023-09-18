#include "window.hpp"
#include "imgui/imgui.h"
#include <iostream>

namespace digital_circuit_maker{
void Window::create(const sf::Vector2u size, const bool fullscreen, 
	const unsigned int fps_limit, const bool vsync){

	sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

	render_window.create(sf::VideoMode(size.x, size.y), 
		"Digital Circuit Maker v0.0", style);
	render_window.setFramerateLimit(fps_limit);
	render_window.setVerticalSyncEnabled(vsync);

	if(!ImGui::SFML::Init(render_window))
		std::cout << "ImGui::SFML::Init error" << std::endl;

	ImGui::StyleColorsClassic();
}

void Window::handle_events(){
	sf::Event event;
	while(render_window.pollEvent(event)){
		ImGui::SFML::ProcessEvent(render_window, event);
		if(event.type == sf::Event::Closed) render_window.close();
	}
}

void Window::refresh_delta_time(){
	static sf::Clock delta_time_clock;
	delta_time = delta_time_clock.restart().asSeconds();

	static sf::Clock imgui_clock;
	ImGui::SFML::Update(render_window, imgui_clock.restart());
}

sf::RenderWindow& Window::get_render_window(){
	return render_window;
}

float Window::get_delta_time() const{
	return delta_time;
}

Window::~Window(){
	ImGui::SFML::Shutdown();
}
}

