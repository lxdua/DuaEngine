#include "SceneHierarchyPanel.h"

#include "Dua/Scene/Scene.h"
#include "Dua/Scene/Entity.h"
#include "Dua/Scene/Components.h"

#include "Dua/Utils/PlatformUtils.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ImGui/imgui_internal.h"

#include "EditorLayer.h"

namespace Dua {

	const float defaultColumnWidth = 80.0f;

	struct ComponentMeta
	{
		std::string name;
		std::string description;
		std::function<void(Entity&)> adder;
	};

	std::vector<ComponentMeta> componentMeta = {
		{"Sprite", "Render 2D sprite.", [](Entity& e) { e.AddComponent<SpriteComponent>(); }},
	};

	bool DrawBoolControl(const std::string& label, bool& value, bool resetValue = false, float columnWidth = defaultColumnWidth)
	{
		bool valueChanged = false;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2, 3 });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// 重置按钮
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }); // 绿色
			if (ImGui::Button("B", buttonSize)) {
				value = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
		}

		// 复选框
		{
			bool temp = value;
			if (ImGui::Checkbox("##BoolCheck", &temp))
			{
				value = temp;
				valueChanged = true;
			}
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return valueChanged;
	}

	bool DrawFloatControl(const std::string& label, float& value, float resetValue = 0.0f, float columnWidth = defaultColumnWidth)
	{
		bool valueChanged = false;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2, 3 });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// 重置按钮
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }); // 红色
			if (ImGui::Button("F", buttonSize))
			{
				value = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
		}

		// 拖动条
		{
			if (ImGui::DragFloat("##FloatDrag", &value, 0.1f))
			{
				valueChanged = true;
			}
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return valueChanged;
	}

	bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = defaultColumnWidth)
	{
		bool valueChanged = false;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2, 3 });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// X
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
			if (ImGui::Button("X", buttonSize)) {
				values.x = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::DragFloat("##X", &values.x, 0.1f)) {
				valueChanged = true;
			}
			ImGui::PopItemWidth();
		}

		// Y
		{
			ImGui::SameLine(0, 5);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.6f, 0.1f, 1.0f });
			if (ImGui::Button("Y", buttonSize)) {
				values.y = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::DragFloat("##Y", &values.y, 0.1f)) {
				valueChanged = true;
			}
			ImGui::PopItemWidth();
		}

		// Z
		{
			ImGui::SameLine(0, 5);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.2f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize)) {
				values.z = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::DragFloat("##Z", &values.z, 0.1f)) {
				valueChanged = true;
			}
			ImGui::PopItemWidth();
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return valueChanged;
	}

	bool DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue = 1.0f, float columnWidth = defaultColumnWidth)
	{
		bool valueChanged = false;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2, 3 });

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// X
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
			if (ImGui::Button("X", buttonSize)) {
				values.x = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::DragFloat("##X", &values.x, 0.1f)) {
				valueChanged = true;
			}
			ImGui::PopItemWidth();
		}

		// Y
		{
			ImGui::SameLine(0, 5);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.6f, 0.1f, 1.0f });
			if (ImGui::Button("Y", buttonSize)) {
				values.y = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::DragFloat("##Y", &values.y, 0.1f)) {
				valueChanged = true;
			}
			ImGui::PopItemWidth();
		}

		// Z
		{
			ImGui::SameLine(0, 5);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.2f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize)) {
				values.z = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::DragFloat("##Z", &values.z, 0.1f)) {
				valueChanged = true;
			}
			ImGui::PopItemWidth();
		}

		// W
		{
			ImGui::SameLine(0, 5);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.5f, 0.1f, 0.8f, 1.0f });
			if (ImGui::Button("W", buttonSize)) {
				values.w = resetValue;
				valueChanged = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::DragFloat("##W", &values.w, 0.1f)) {
				valueChanged = true;
			}
			ImGui::PopItemWidth();
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return valueChanged;
	}

	SceneHierarchyPanel::SceneHierarchyPanel(EditorLayer* editorLayer) : m_EditorLayer(editorLayer) {}

	void SceneHierarchyPanel::OnDraw()
	{
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar);

		if (ImGui::Button("Add"))
		{
			Ref<Entity> newEntity = m_EditorLayer->m_Scene->CreateEntity("New Entity");
			m_EditorLayer->m_SelectedEntity = newEntity;
		}

		if (m_EditorLayer->m_SelectedEntity)
		{
			ImGui::SameLine();
			if (ImGui::Button("Delete"))
			{
				m_EditorLayer->m_Scene->DestroyEntity(m_EditorLayer->m_SelectedEntity);
				m_EditorLayer->m_SelectedEntity.reset();
			}
		}

		ImGui::Separator();

		auto view = m_EditorLayer->m_Scene->m_Registry.view<TagComponent>();
		for (auto entityID : view)
		{
			DrawEntityNode(m_EditorLayer->m_Scene->m_EntityMap[entityID]);
		}
		ImGui::End();

		ImGui::Begin("Component", nullptr, ImGuiWindowFlags_NoScrollbar);
		if (m_EditorLayer->m_SelectedEntity)
		{
			if (ImGui::Button("Add"))
			{
				m_ShowAddComponentPopup = true;
				ImGui::OpenPopup("Add Component");
			}

			if (ImGui::BeginPopupModal("Add Component", &m_ShowAddComponentPopup,
				ImGuiWindowFlags_NoScrollbar))
			{
				ImVec2 m_ComponentWindowMinSize = ImVec2(300, 400);

				// 设置窗口尺寸约束
				ImGui::SetNextWindowSizeConstraints(m_ComponentWindowMinSize, ImVec2(FLT_MAX, FLT_MAX));

				// 搜索框
				static char searchText[64] = "";
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::InputTextWithHint("##Search", "Search component...", searchText, IM_ARRAYSIZE(searchText));

				ImGui::Separator();

				// 双列布局
				ImGui::Columns(2, "ComponentColumns", true);
				// 在设置列后立即定义宽度比例
				ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.4f); // 左侧40%
				ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() * 0.6f); // 右侧60%

				ImGui::BeginChild("LeftPanel", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
				
				ImGui::TextUnformatted("Select Component");
				ImGui::Separator();

				// 基础组件分类
				if (ImGui::TreeNodeEx("Common components", ImGuiTreeNodeFlags_DefaultOpen))
				{
					for (auto& comp : componentMeta)
					{
						// 搜索过滤
						if (strlen(searchText) > 0 &&
							comp.name.find(searchText) == std::string::npos)
							continue;

						// 按钮样式
						const bool isHovered = (m_HoveredComponent == comp.name);
						ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
						ImGui::PushStyleColor(ImGuiCol_Button, isHovered ?
							ImGui::GetColorU32(ImGuiCol_ButtonHovered) :
							ImGui::GetColorU32(ImGuiCol_FrameBg));

						// 组件按钮
						if (ImGui::Button(comp.name.c_str(), ImVec2(-FLT_MIN, 30)))
						{
							comp.adder(*m_EditorLayer->m_SelectedEntity);
							m_ShowAddComponentPopup = false;
							ImGui::CloseCurrentPopup();
						}

						// 悬停检测
						if (ImGui::IsItemHovered())
						{
							m_HoveredComponent = comp.name;
						}

						ImGui::PopStyleColor();
						ImGui::PopStyleVar();
					}
					ImGui::TreePop();
					

					// 其他分类（示例）
					if (ImGui::TreeNode("Advanced components"))
					{
						// ...类似上面的循环
						ImGui::TreePop();
					}
				}
				ImGui::EndChild();

				ImGui::NextColumn();

				// 右侧描述面板
				ImGui::BeginChild("RightPanel", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
				{
					ImGui::TextUnformatted("Description");
					ImGui::Separator();

					// 查找当前悬停组件的描述
					static std::string currentDesc = "Hover component to see details";
					if (!m_HoveredComponent.empty())
					{
						auto it = std::find_if(componentMeta.begin(), componentMeta.end(),
							[&](const auto& c) { return c.name == m_HoveredComponent; });

						if (it != componentMeta.end())
						{
							currentDesc = it->description;
						}
					}

					// 显示描述内容
					ImGui::TextWrapped("%s", currentDesc.c_str());
				}
				ImGui::EndChild();

				ImGui::Columns(1); // 结束分栏

				ImGui::EndPopup();
			}

			ImGui::Separator();
			DrawComponent(m_EditorLayer->m_SelectedEntity);
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
			m_EditorLayer->m_SelectedEntity = CreateRef<Entity>(entity);
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
			bool open = ImGui::CollapsingHeader("Tag", ImGuiTreeNodeFlags_DefaultOpen);
			if (open)
			{
				auto& tag = entity->GetComponent<TagComponent>().Tag;
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
				if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
				ImGui::PopItemWidth();
			}
		}

		if (entity->HasComponent<TransformComponent>())
		{
			bool open = ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen);
			if (open)
			{
				auto& transform = entity->GetComponent<TransformComponent>();

				bool positionChanged = DrawVec3Control("Position", transform.Position);

				bool scaleChanged = DrawVec3Control("Scale", transform.Scale, 1.0f);

				float rotationDegrees = glm::degrees(transform.Rotation);
				bool rotationChanged = DrawFloatControl("Rotation", rotationDegrees, 0.0f);
				if (rotationChanged)
				{
					transform.SetRotation(glm::radians(rotationDegrees));
				}

				if (positionChanged || scaleChanged || rotationChanged)
				{
					transform.Dirty = true;
				}
			}
		}

		if (entity->HasComponent<SpriteComponent>())
		{
			bool open = ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen);
			if (open)
			{
				auto& sprite = entity->GetComponent<SpriteComponent>();

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);

				// 纹理预览和选择
				ImGui::Text("Texture");
				ImGui::SameLine();

				// 显示纹理缩略图
				if (sprite.Texture)
				{
					ImGui::Image(
						(ImTextureID)sprite.Texture->GetRendererID(),
						ImVec2(64, 64),
						ImVec2(0, 1),
						ImVec2(1, 0)
					);
				}
				else
				{
					ImGui::Button("No Texture", ImVec2(64, 64));
				}

				// 纹理路径显示和选择按钮
				static char texturePath[256] = "";
				if (sprite.Texture)
					strcpy_s(texturePath, sprite.Texture->GetPath().c_str());

				ImGui::InputText("##TexPath", texturePath, IM_ARRAYSIZE(texturePath), ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				if (ImGui::Button("Select..."))
				{
					// 这里添加文件选择对话框逻辑
					auto path = FileDialogs::OpenFile("Image File\0*.png;*.jpg;*.jpeg\0");
					if (!path.empty())
					{
						sprite.Texture = Texture2D::Create(path);
					}
				}
				
				DrawBoolControl("Show", sprite.Show);

				// 颜色调制
				ImGui::ColorEdit4("Modulate", glm::value_ptr(sprite.Modulate),
					ImGuiColorEditFlags_HDR |
					ImGuiColorEditFlags_AlphaBar
				);

				ImGui::PopItemWidth();

				ImGui::Separator();
				DrawComponentRemoveButton("Sprite", [&]() { entity->RemoveComponent<SpriteComponent>(); });
			}
		}
	}

	void SceneHierarchyPanel::DrawComponentRemoveButton(const std::string& name, const std::function<void()>& removeCallback)
	{
		std::string text = "Remove " + name;
		ImGui::PushID(text.c_str());
		if (ImGui::Button(text.c_str()))
		{
			removeCallback();
		}
		ImGui::PopID();
	}

}