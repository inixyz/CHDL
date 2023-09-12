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
	bool show_grid = true;

public:
	Config(const std::string file_name){
		std::ifstream file(file_name);
		nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		window_width = json["window_width"];
		window_height = json["window_height"];
		frame_rate_limit = json["frame_rate_limit"];
		vsync = json["vsync"];
		camera_move_speed = json["camera_move_speed"];
		camera_zoom_speed = json["camera_zoom_speed"];
		show_grid = json["show_grid"];
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
			XOR_GATE, XNOR_GATE, NOT_GATE, BUFFER_GATE
		}type = EMPTY;

		WireGroup* wire_group;

		sf::Vector2i direction = sf::Vector2i(0, 0);
		bool last_out = false;
		void (*behaviour)() = NULL;
	};

public:
	size_t width, height;
	std::list<WireGroup> wire_groups;
	std::vector<std::vector<Cell>> cells;

public:
	Map(const size_t width, const size_t height){
		this->width = width;
		this->height = height;

		cells = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height));
	}
};

sf::Vector2f vector2f_normalize(sf::Vector2f vec){
	float magnitude = vec.x * vec.x + vec.y * vec.y;
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

	void update(Window& window){
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
};

class Render{
private:
	const size_t CELL_SIZE = 11;

	const sf::Color COLOR_BACKGROUND = sf::Color(16, 20, 31);
	const sf::Color COLOR_MAP_OUTLINE = sf::Color(200, 200, 200);
	const sf::Color COLOR_GRID = sf::Color(66, 70, 81);

	sf::Vector2u world_view_start_pos, world_view_end_pos;
	sf::VertexArray map_outline, grid;

public:
	static Render& get_instance(){
		static Render instance;
		return instance;
	}

	Render(Render const&) = delete;
	void operator=(Render const&) = delete;

	void set_map_outline_position(const size_t map_width, const size_t map_height){
		map_outline[0].position = map_outline[4].position = sf::Vector2f(0, 0);
		map_outline[1].position = sf::Vector2f(map_width, 0) * (float)CELL_SIZE;
		map_outline[2].position = sf::Vector2f(map_width, map_height) * (float)CELL_SIZE;
		map_outline[3].position = sf::Vector2f(0, map_height) * (float)CELL_SIZE;
	}

	void update(Window& window, Camera& camera, Map& map, const bool show_grid){
		sf::Vector2f camera_start_pos, camera_end_pos;
		camera_start_pos.x = camera.view.getCenter().x - camera.view.getSize().x / 2 - CELL_SIZE;
		camera_start_pos.y = camera.view.getCenter().y - camera.view.getSize().y / 2 - CELL_SIZE;
		camera_end_pos.x = camera.view.getCenter().x + camera.view.getSize().x / 2 + CELL_SIZE;
		camera_end_pos.y = camera.view.getCenter().y + camera.view.getSize().y / 2 + CELL_SIZE;

		world_view_start_pos.x = std::max(0, std::min((int)(camera_start_pos.x / CELL_SIZE), (int)map.width));
		world_view_start_pos.y = std::max(0, std::min((int)(camera_start_pos.y / CELL_SIZE), (int)map.height));
		world_view_end_pos.x = std::max(0, std::min((int)(camera_end_pos.x / CELL_SIZE), (int)map.width));
		world_view_end_pos.y = std::max(0, std::min((int)(camera_end_pos.y / CELL_SIZE), (int)map.height));

		window.render_window.clear(COLOR_BACKGROUND);
		draw_world(window, camera, show_grid);
		window.render_window.display();
	}

private:
	Render(){
		map_outline = sf::VertexArray(sf::LineStrip, 5);
		for(size_t i = 0; i < map_outline.getVertexCount(); i++) map_outline[i].color = COLOR_MAP_OUTLINE;

		grid = sf::VertexArray(sf::Lines, 2);
		grid[0].color = grid[1].color = COLOR_GRID;
	}

	void draw_world(Window& window, Camera& camera, const bool show_grid){
		window.render_window.setView(camera.view);

		if(show_grid){
			for(unsigned int x = world_view_start_pos.x + 1; x < world_view_end_pos.x; x++){
				grid[0].position = sf::Vector2f(x, world_view_start_pos.y) * (float)CELL_SIZE;
				grid[1].position = sf::Vector2f(x, world_view_end_pos.y) * (float)CELL_SIZE;

				window.render_window.draw(grid);
			}

			for(unsigned int y = world_view_start_pos.y + 1; y < world_view_end_pos.y; y++){
				grid[0].position = sf::Vector2f(world_view_start_pos.x, y) * (float)CELL_SIZE;
				grid[1].position = sf::Vector2f(world_view_end_pos.x, y) * (float)CELL_SIZE;

				window.render_window.draw(grid);
			}
		}
		
		window.render_window.draw(map_outline);
	}
};

int main(){
	Config config("config.json");
	Window window(config.window_width, config.window_height, config.frame_rate_limit, config.vsync);

	Map map(30, 30);
	Render::get_instance().set_map_outline_position(map.width, map.height);

	Camera camera(config.window_width, config.window_height, config.camera_move_speed, config.camera_zoom_speed);

	while(window.render_window.isOpen()){
		window.update();
		camera.update(window);
		Render::get_instance().update(window, camera, map, config.show_grid);
	}
}