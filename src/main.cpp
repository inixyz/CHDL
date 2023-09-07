#include "window.hpp"
#include "render.hpp"

using namespace dcm;

void global_setup(){
	window.setup();
	render.setup();
}

void global_update(){
	window.update();
	render.update();
}

int main(){
   global_setup();
   while(window.render_window.isOpen()) global_update();
}