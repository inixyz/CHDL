#include "menu.hpp"
#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"

namespace digital_circuit_maker::menu{
static sf::Texture empty_selection_texture;
static sf::Texture wire_texture;
static sf::Texture junction_texture;
static sf::Texture and_texture;
static sf::Texture nand_texture;
static sf::Texture or_texture;
static sf::Texture nor_texture;
static sf::Texture xor_texture;
static sf::Texture xnor_texture;
static sf::Texture not_texture;
static sf::Texture buffer_texture;

void init(){
	const unsigned int PART_SIZE = 33;

	empty_selection_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(0, 0, PART_SIZE, PART_SIZE));
	wire_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(1 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	junction_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(2 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	and_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(3 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	nand_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(4 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	or_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(5 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	nor_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(6 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	xor_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(7 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	xnor_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(8 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	not_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(9 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
	buffer_texture.loadFromFile("assets/menu_tileset.png", 
		sf::IntRect(10 * PART_SIZE, 0, PART_SIZE, PART_SIZE));
}

void editor_tools(){
	ImGui::Begin("Editor tools");

	ImGui::SeparatorText("Part picker");

	ImGui::ImageButton(empty_selection_texture);
	ImGui::SameLine();
	ImGui::ImageButton(wire_texture);
	ImGui::SameLine();
	ImGui::ImageButton(junction_texture);

	ImGui::ImageButton(buffer_texture);
	ImGui::SameLine();
	ImGui::ImageButton(not_texture);
	ImGui::SameLine();
	ImGui::ImageButton(or_texture);
	ImGui::SameLine();
	ImGui::ImageButton(and_texture);

	ImGui::ImageButton(xor_texture);
	ImGui::SameLine();
	ImGui::ImageButton(xnor_texture);
	ImGui::SameLine();
	ImGui::ImageButton(nor_texture);
	ImGui::SameLine();
	ImGui::ImageButton(nand_texture);

	ImGui::End();
}
}