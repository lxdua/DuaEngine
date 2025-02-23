#include "duapch.h"
#include "Dua.h"


class ExampleLayer : public Dua::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		std::cout << "ExampleLayer: " << "Update" << std::endl;
	}

	void OnEvent(Dua::Event& event) override
	{
		std::cout << "ExampleLayer: " << event.ToString() << std::endl;
	}

};

class Sandbox : public Dua::Application {
public:

	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Dua::Application* Dua::CreateApplication() {
	return new Sandbox();
}