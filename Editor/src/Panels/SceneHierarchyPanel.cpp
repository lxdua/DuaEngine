#include "SceneHierarchyPanel.h"

#include "ImGui/imgui.h"

#include "Dua/Scene/Entity.h"
#include "Dua/Scene/Components.h"

#include <iostream>

namespace Dua {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) : m_Context(context)
	{
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		auto view = m_Context->m_Registry.view<const TagComponent>();
		for (auto entityID : view) 
		{
			DrawEntityNode(Entity(entityID, m_Context.get()));
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_OpenOnArrow |      // 点击箭头展开
			ImGuiTreeNodeFlags_SpanAvailWidth |   // 占满宽度
			ImGuiTreeNodeFlags_OpenOnDoubleClick; // 双击展开

		// 绘制树节点
		bool isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

		// 处理节点点击（选择实体）
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			m_SelectedEntityID = entity;
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



}