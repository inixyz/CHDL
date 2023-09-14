#include "camera.hpp"
#include "vector2_math.hpp"

void Camera::create(unsigned int width, unsigned int height, float move_speed, float zoom_speed){
	this->move_speed = move_speed;
	this->zoom_speed = zoom_speed;
	view = sf::View(sf::FloatRect(0, 0, width, height));
}

void Camera::update(Window& window){
	if(window.render_window.hasFocus()){
		sf::Vector2f direction;
		direction.x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		direction.y = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W);

		view.move(vector2f_normalize(direction) * move_speed * zoom_factor * window.delta_time);

		short int zoom_direction = sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen) - sf::Keyboard::isKeyPressed(sf::Keyboard::Equal);
		float zoom_amount = 1 + zoom_direction * zoom_speed * window.delta_time;
		zoom_factor *= zoom_amount;

		view.zoom(zoom_amount);
	}
}
