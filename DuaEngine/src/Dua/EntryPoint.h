#pragma once

extern Dua::Application* Dua::CreateApplication();

int main(int argc, char** argv) {
	std::cout << "DuaEngine!\n";
	auto app = Dua::CreateApplication();
	app->Run();
	delete app;
	return 0;
}