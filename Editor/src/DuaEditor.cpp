#include "duapch.h"
#include "Dua.h"
#include "Dua/EntryPoint.h"

#include "EditorLayer.h"

namespace Dua {

	class DuaEditor : public Application
	{
	public:

		DuaEditor()
		{
			PushLayer(new EditorLayer());
		}

		~DuaEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new DuaEditor();
	}

}

