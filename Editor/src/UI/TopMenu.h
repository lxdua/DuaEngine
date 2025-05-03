#pragma once

namespace Dua {

	class Scene;
	class Entity;

	class EditorLayer;

	class TopMenu
	{
	public:
		TopMenu(EditorLayer* editorLayer);

		void OnDraw();

	private:
		void NewScene();
		void SaveScene();
		void OpenScene();
		void ExportScene();

		void DrawLeftMenu();
		//void DrawLeftMenu();

	private:
		friend class Scene;
		EditorLayer* m_EditorLayer;
		
	};

}