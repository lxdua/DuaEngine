#pragma once

#include "Dua/Layer.h"

#include "Dua/Events/ApplicationEvent.h"
#include "Dua/Events/KeyEvent.h"
#include "Dua/Events/MouseEvent.h"

namespace Dua {

	class DUA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		
		void Begin();
		void End();

	private:
		float m_Time = 0.0f;

	};

}
