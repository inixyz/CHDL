#include "render.hpp"
#include "window.hpp"

using namespace dcm;

Render dcm::render;

void Render::setup(){

}

void Render::update(){
	window.render_window.clear();
	window.render_window.display();
}