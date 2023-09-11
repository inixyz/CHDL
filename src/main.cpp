#include <fstream>
#include "json.hpp"
#include <SFML/Graphics.hpp>
#include <list>
#include <algorithm>

class Config{
public:
	size_t window_width = 800, window_height = 800;
	unsigned int frame_rate_limit = 60;
	bool vsync = true;
	float camera_move_speed = 250, camera_zoom_speed = 1;

public:
	Config(const std::string file_name){
		load(file_name);
	}

	void load(const std::string file_name){
		std::ifstream file(file_name);
		nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		window_width = json["window_width"];
		window_height = json["window_height"];
		frame_rate_limit = json["frame_rate_limit"];
		vsync = json["vsync"];
		camera_move_speed = json["camera_move_speed"];
		camera_zoom_speed = json["camera_zoom_speed"];
	}
};

class Window{
public:
	sf::RenderWindow render_window;
	float delta_time;

private:
	sf::Event event;
	sf::Clock delta_time_clock;

public:
	Window(const size_t width, const size_t height, const unsigned int frame_rate_limit, const bool vsync){
		create(width, height, frame_rate_limit, vsync);
	}

	void create(const size_t width, const size_t height, const unsigned int frame_rate_limit, const bool vsync){
		render_window.create(sf::VideoMode(width, height), "DCM v0.0");
		render_window.setFramerateLimit(frame_rate_limit);
		render_window.setVerticalSyncEnabled(vsync);
	}

	void update(){
		while(render_window.pollEvent(event))
			if(event.type == sf::Event::Closed)
				render_window.close();

		delta_time = delta_time_clock.restart().asSeconds();
	}
};

class Map{
private:
	struct WireGroup{
		unsigned int state = 0;
		std::vector<sf::Vector2u> connected_inputs;
	};

public:
	struct Cell{
		enum Type{
			EMPTY, WIRE, JUNCTION, AND_GATE, NAND_GATE, OR_GATE, NOR_GATE, 
			XOR_GATE, XNOR_GATE, NOT_GATE, YES_GATE
		}type = EMPTY;

		WireGroup* wire_group = NULL;

		sf::Vector2i direction = sf::Vector2i(0, 0);
		bool last_out = false;
		void (*behaviour)() = NULL;
	};

public:
	size_t width, height;
	std::list<WireGroup> wire_groups;
	std::vector<std::vector<Cell>> cells;

public:
	void reset(const size_t width, const size_t height){
		this->width = width;
		this->height = height;

		cells = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height));
	}
};

float vector2f_magnitude(const sf::Vector2f vec){
	return vec.x * vec.x + vec.y * vec.y;
}

sf::Vector2f vector2f_normalize(sf::Vector2f vec){
	float magnitude = vector2f_magnitude(vec);
	if(magnitude > 0) vec /= magnitude;
	return vec;
}

class Camera{
public:
	sf::View view;

private:
	float move_speed, zoom_speed;
	float zoom_factor = 1;

public:
	Camera(const size_t width, const size_t height, const float move_speed, const float zoom_speed){
		this->move_speed = move_speed;
		this->zoom_speed = zoom_speed;
		view = sf::View(sf::FloatRect(0, 0, width, height));
	}

	void update(const float delta_time){
		sf::Vector2f direction(0, 0);

		direction.x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		direction.y = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W);

		view.move(vector2f_normalize(direction) * move_speed * zoom_factor * delta_time);

		short int zoom_direction = sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen) - sf::Keyboard::isKeyPressed(sf::Keyboard::Equal);
		float zoom_amount = 1 + zoom_direction * zoom_speed * delta_time;
		zoom_factor *= zoom_amount;

		view.zoom(zoom_amount);
	}
};

class Render{
public:
	const size_t CELL_SIZE = 11;

private:
	const sf::Color COLOR_BACKGROUND = sf::Color(16, 20, 31);
	const sf::Color COLOR_GRID = sf::Color(26, 30, 41);
	const sf::Color COLOR_MAP_OUTLINE = sf::Color(66, 70, 81);

	sf::VertexArray map_outline;
	sf::Vector2u world_view_start_pos, world_view_end_pos;

public:
	static Render& get_instance(){
		static Render instance;
		return instance;
	}

	Render(Render const&) = delete;
	void operator=(Render const&) = delete;

	void set_map_outline_position(const size_t width, const size_t height){
		map_outline[0].position = map_outline[4].position = sf::Vector2f(0, 0);
		map_outline[1].position = sf::Vector2f(width, 0);
		map_outline[2].position = sf::Vector2f(width, height);
		map_outline[3].position = sf::Vector2f(0, height);
	}

	void render(Window& window, Camera& camera, const size_t map_width, const size_t map_height){
		window.render_window.clear(COLOR_BACKGROUND);
		set_world_view_bounds(camera, map_width, map_height);
		draw_world(window, camera);
		window.render_window.display();
	}

private:
	Render(){
		map_outline = sf::VertexArray(sf::LineStrip, 5);
		for(size_t i = 0; i < map_outline.getVertexCount(); i++) map_outline[i].color = COLOR_MAP_OUTLINE;
	}

	void set_world_view_bounds(Camera& camera, const size_t map_width, const size_t map_height){
		sf::Vector2f camera_start_pos, camera_end_pos;
		camera_start_pos.x = camera.view.getCenter().x - camera.view.getSize().x / 2;
		camera_start_pos.y = camera.view.getCenter().y - camera.view.getSize().y / 2;
		camera_end_pos.x = camera.view.getCenter().x + camera.view.getSize().x / 2;
		camera_end_pos.y = camera.view.getCenter().y + camera.view.getSize().y / 2;

		world_view_start_pos.x = std::max(0, std::min((int)(camera_start_pos.x / CELL_SIZE), (int)map_width));
		world_view_start_pos.y = std::max(0, std::min((int)(camera_start_pos.y / CELL_SIZE), (int)map_height));
		world_view_end_pos.x = std::max(0, std::min((int)(camera_end_pos.x / CELL_SIZE), (int)map_width));
		world_view_end_pos.y = std::max(0, std::min((int)(camera_end_pos.y / CELL_SIZE), (int)map_height));
	}

	void draw_world(Window& window, Camera& camera){
		window.render_window.setView(camera.view);
		window.render_window.draw(map_outline);
	}
};

int main(){
	Config config("config.json");
	Window window(config.window_width, config.window_height, config.frame_rate_limit, config.vsync);

	Map map;
	map.reset(30, 30);
	Render::get_instance().set_map_outline_position(map.width * Render::get_instance().CELL_SIZE, map.height * Render::get_instance().CELL_SIZE);

	Camera camera(config.window_width, config.window_height, config.camera_move_speed, config.camera_zoom_speed);

	while(window.render_window.isOpen()){
		window.update();

		camera.update(window.delta_time);

		Render::get_instance().render(window, camera, map.width, map.height);
	}
}