#include "SceneHierarchyPanel.h"

#include "ImGui/imgui.h"

#include "Dua/Scene/Scene.h"
#include "Dua/Scene/Entity.h"
#include "Dua/Scene/Components.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Dua {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) : m_Context(context)
	{
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar);
		auto view = m_Context->m_Registry.view<TagComponent>();
		for (auto entityID : view) 
		{
			DrawEntityNode(Entity::EntityMap[entityID]);
		}
		ImGui::End();

		ImGui::Begin("Component", nullptr, ImGuiWindowFlags_NoScrollbar);
		if (m_SelectedEntity)
		{
			DrawComponent(m_SelectedEntity);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Ref<Entity> entity)
	{
		auto& tag = entity->GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_OpenOnArrow |      // 点击箭头展开
			ImGuiTreeNodeFlags_SpanAvailWidth |   // 占满宽度
			ImGuiTreeNodeFlags_OpenOnDoubleClick; // 双击展开

		

		// 绘制树节点
		bool isOpen = ImGui::TreeNodeEx((void*)(entity->GetID()), flags, "%s", tag.c_str());
		
		// 处理节点点击（选择实体）
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			m_SelectedEntity = CreateRef<Entity>(entity);
		}

		if (isOpen)
		{
			ImGui::TreePop();
		}

		// 拖放功能
		//HandleDragAndDrop(entity);

		// 右键菜单
		if (ImGui::BeginPopupContextItem())
		{
			//DrawEntityContextMenu(entity);
			ImGui::EndPopup();
		}

	}

	void SceneHierarchyPanel::DrawComponent(Ref<Entity> entity)
	{
		if (entity->HasComponent<TagComponent>())
		{
			auto& tag = entity->GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity->HasComponent<TransformComponent>())
		{
			bool componentOpen = ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen);
			if (componentOpen)
			{
				auto& transform = entity->GetComponent<TransformComponent>();

				// Position
				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
				bool positionChanged = ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);

				// Rotation (假设是绕Y轴旋转)
				float rotationDegrees = glm::degrees(transform.Rotation);
				if (ImGui::DragFloat("Rotation", &rotationDegrees, 1.0f, -180.0f, 180.0f))
				{
					transform.SetRotation(glm::radians(rotationDegrees));
				}

				// Scale
				bool scaleChanged = ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f, 0.0f);
				ImGui::PopItemWidth();

				// 自动更新Dirty标志
				if (positionChanged || scaleChanged)
				{
					transform.Dirty = true;
				}

				// 显示变换矩阵（只读）
				if (ImGui::TreeNode("Transform Matrix"))
				{
					for (int row = 0; row < 4; ++row)
					{
						ImGui::Text("%.2f\t%.2f\t%.2f\t%.2f",
							transform.Transform[row][0],
							transform.Transform[row][1],
							transform.Transform[row][2],
							transform.Transform[row][3]);
					}
					ImGui::TreePop();
				}
			}
		}
	}

}