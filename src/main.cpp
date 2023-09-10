#include <fstream>
#include "json.hpp"
#include <SFML/Graphics.hpp>
#include <list>

class Config{
public:
	size_t window_width = 640, window_height = 480;
	unsigned int frame_rate_limit = 30;
	bool vsync = true;

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
	Window(const Config config){
		create(config);
	}

	void create(const Config config){
		render_window.create(sf::VideoMode(config.window_width, config.window_height), "DCM v0.0");
		render_window.setFramerateLimit(config.frame_rate_limit);
		render_window.setVerticalSyncEnabled(config.vsync);
	}

	void update(){
		while(render_window.pollEvent(event))
			if(event.type == sf::Event::Closed)
				render_window.close();

		delta_time = delta_time_clock.restart().asMilliseconds();
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

		union{
			WireGroup* wire_group = NULL;
			struct{
				sf::Vector2i direction = sf::Vector2i(0, 0);
				bool last_out = false;
				void (*behaviour)() = NULL;
			}gate;
		};
	};

public:
	size_t width, height;
	std::list<WireGroup> wire_groups;
	std::vector<std::vector<Cell>> cells;

public:
	void reset(const size_t width,  const size_t height){
		this->width = width;
		this->height = height;

		cells = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height));
	}
};

class Render{
private:
	const sf::Color COLOR_BACKGROUND = sf::Color(16, 20, 31);
	const sf::Color COLOR_GRID = sf::Color(26, 30, 41);
	const sf::Color COLOR_MAP_OUTLINE = sf::Color(66, 70, 81);

	const size_t CELL_SIZE = 11;
	sf::VertexArray map_outline;

public:
	static Render& get_instance(){
		static Render instance;
		return instance;
	}

	Render(Render const&) = delete;
	void operator=(Render const&) = delete;

	void set_map_outline_position(const Map& map){
		map_outline[0].position = map_outline[4].position = sf::Vector2f(0, 0);
		map_outline[1].position = sf::Vector2f(map.width * CELL_SIZE, 0);
		map_outline[2].position = sf::Vector2f(map.width * CELL_SIZE, map.height * CELL_SIZE);
		map_outline[3].position = sf::Vector2f(0, map.height * CELL_SIZE);
	}

	void render_world(Window& window){
		window.render_window.clear(COLOR_BACKGROUND);
		window.render_window.draw(map_outline);
		window.render_window.display();
	}

private:
	Render(){
		map_outline = sf::VertexArray(sf::LineStrip, 5);
		for(size_t i = 0; i < map_outline.getVertexCount(); i++) map_outline[i].color = COLOR_MAP_OUTLINE;
	}
};

int main(){
	Config config("config.json");
	Window window(config);

	Map map;
	map.reset(30, 30);
	Render::get_instance().set_map_outline_position(map);

	while(window.render_window.isOpen()){
		window.update();

		Render::get_instance().render_world(window);
	}
}