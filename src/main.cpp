#include "window.hpp"

using namespace dcm;

void global_setup(){
	window.setup();
}

void global_update(){
	window.update();
}

int main(){
   global_setup();
   while(window.render_window.isOpen()) global_update();
}