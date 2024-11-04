#include "Dua.h"

class Sandbox : public Dua::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}
};

Dua::Application* CreateApplication() {
	return new Sandbox();
}