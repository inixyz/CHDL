#include "camera.hpp"

namespace digital_circuit_maker{
void Camera::create(const sf::Vector2u size, const float move_speed, 
	const float zoom_speed){

	this->move_speed = move_speed;
	this->zoom_speed = zoom_speed;

	view = sf::View(sf::FloatRect(0, 0, size.x, size.y));
}

void Camera::process_movement(const float delta_time){
	sf::Vector2f direction;

	direction.x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) 
		- sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	direction.y = sf::Keyboard::isKeyPressed(sf::Keyboard::S) 
		- sf::Keyboard::isKeyPressed(sf::Keyboard::W);

	auto normalize_vector2f = [](sf::Vector2f v){
		float magnitude = v.x * v.x + v.y * v.y;
		if(magnitude > 0) v /= magnitude;
		return v;
	};

	sf::Vector2f velocity = normalize_vector2f(direction) * move_speed;
	view.move(velocity * zoom_factor * delta_time);
}

void Camera::process_zoom(const float delta_time){
	int direction = sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen) 
		- sf::Keyboard::isKeyPressed(sf::Keyboard::Equal);
	
	float zoom_amount = 1 + direction * zoom_speed * delta_time;
	view.zoom(zoom_amount);

	zoom_factor *= zoom_amount;
}

const sf::View& Camera::get_view() const{
	return view;
}

float Camera::get_zoom_factor() const{
	return zoom_factor;
}
}
