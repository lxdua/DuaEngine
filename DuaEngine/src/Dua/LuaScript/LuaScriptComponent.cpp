#include "LuaScriptComponent.h"
#include "LuaMathBindings.h"
#include "Dua/Scene/Entity.h"
#include "Dua/Scene/Components.h"

namespace Dua {

    LuaScriptComponent::LuaScriptComponent(Entity* owner)
        : m_Owner(owner)
    {
        m_Lua.open_libraries(sol::lib::base, sol::lib::math);
        BindEngineAPI();
        m_Env = sol::environment(m_Lua, sol::create, m_Lua.globals());
    }

    LuaScriptComponent::~LuaScriptComponent()
    {
        ClearLuaReferences();
    }

    LuaScriptComponent::LuaScriptComponent(LuaScriptComponent&& other) noexcept :
        m_Owner(other.m_Owner),
        m_ScriptPath(std::move(other.m_ScriptPath)),
        m_Lua(std::move(other.m_Lua)),
        m_Env(std::move(other.m_Env)),
        m_InitFunc(std::move(other.m_InitFunc)),
        m_UpdateFunc(std::move(other.m_UpdateFunc)),
        m_DestroyFunc(std::move(other.m_DestroyFunc))
    {
        other.m_Owner = nullptr;
    }

    LuaScriptComponent& LuaScriptComponent::operator=(LuaScriptComponent&& other) noexcept
    {
        if (this != &other)
        {
            this->~LuaScriptComponent();
            new (this) LuaScriptComponent(std::move(other));
        }
        return *this;
    }

    bool LuaScriptComponent::LoadScript(const std::string& path)
    {
        m_ScriptPath = path;
        ClearLuaReferences();
        try
        {
            m_Lua.script_file(path, m_Env);

            sol::optional<sol::function> init = m_Env["Init"];
            sol::optional<sol::function> update = m_Env["Update"];
            sol::optional<sol::function> destroy = m_Env["Destroy"];

            if (init) m_InitFunc = *init;
            if (update) m_UpdateFunc = *update;
            if (destroy) m_DestroyFunc = *destroy;

            return true;
        }
        catch (const sol::error& e) 
        {
            std::cout << "[Lua] Load script failed : " << e.what() << std::endl;
            return false;
        }
    }

    void LuaScriptComponent::Init()
    {
        if (m_InitFunc.valid())
        {
            m_InitFunc(m_Owner);
        }
    }

    void LuaScriptComponent::Update(Timestep ts)
    {
        if (m_UpdateFunc.valid())
        {
            try
            {
                float dt = ts.GetSecond();
                std::cout << "[C++] 传入的 Timestep 秒数: " << dt << '\n';
                m_UpdateFunc(m_Owner, dt);
            }
            catch (const sol::error& e)
            {
                std::cerr << "[Lua] 错误: " << e.what() << '\n';
            }
        }
    }

    void LuaScriptComponent::Destroy()
    {
        if (m_DestroyFunc.valid())
        {
            m_DestroyFunc(m_Owner);
        }
    }

    void LuaScriptComponent::ClearLuaReferences()
    {
        m_InitFunc.abandon();
        m_UpdateFunc.abandon();
        m_DestroyFunc.abandon();
    }

    void LuaScriptComponent::BindEngineAPI()
    {
        LuaMathBindings::Bind(m_Lua);

        m_Lua.new_usertype<Entity>("Entity",
            "get_transform", [](Entity& e) -> TransformComponent&
            {
                return e.GetComponent<TransformComponent>();
            },
            "has_transform", [](Entity& e)
            {
                return e.HasComponent<TransformComponent>();
            }
        );

        m_Lua.new_usertype<TransformComponent>("TransformComponent",
            sol::no_constructor,
            "position", sol::property(
                [](TransformComponent& tc) { return tc.Position; },
                [](TransformComponent& tc, const glm::vec3& pos) { tc.SetPosition(pos); }
            ),
            "rotation", sol::property(
                [](TransformComponent& tc) { return tc.Rotation; },
                [](TransformComponent& tc, float degree) { tc.SetRotation(degree); }
            ),
            "scale", sol::property(
                [](TransformComponent& tc) { return tc.Scale; },
                [](TransformComponent& tc, const glm::vec3& scale) { tc.SetScale(scale); }
            )
        );

        std::cout << "Successfully bind lua!" << std::endl;
    }

}