#pragma once

namespace Dua {

	class Scene;
	class Entity;

	class EditorLayer;

	class ViewportPanel
	{
	public:
		ViewportPanel(EditorLayer* editorLayer);

		void OnDraw();

	private:

	private:
		friend class Scene;
		EditorLayer* m_EditorLayer;

	};

}