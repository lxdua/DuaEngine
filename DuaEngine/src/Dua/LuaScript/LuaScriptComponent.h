#pragma once

#include <lua.hpp>
#define SOL_LUAJIT 1
#define SOL_USING_SOURCE 1
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 0
#include "sol/sol.hpp"

#include "Dua/Core/Timestep.h"
#include "LuaMathBindings.h"

namespace Dua {

    class Entity;
    class LuaScriptSystem;

    class LuaScriptComponent
    {
    public:
        LuaScriptComponent(Entity* owner);
        ~LuaScriptComponent();

        LuaScriptComponent(const LuaScriptComponent&) = delete;
        LuaScriptComponent& operator=(const LuaScriptComponent&) = delete;
        LuaScriptComponent(LuaScriptComponent&& other) noexcept;
        LuaScriptComponent& operator=(LuaScriptComponent&& other) noexcept;

        bool LoadScript(const std::string& path);

        void Init();
        void Update(Timestep ts);
        void Destroy();

    private:
        Entity* m_Owner;                // 所属实体
        std::string m_ScriptPath;       // 脚本路径
        sol::state m_Lua;               // Lua 状态
        sol::environment m_Env;         // 脚本环境（隔离全局表）

        sol::function m_InitFunc;
        sol::function m_UpdateFunc;
        sol::function m_DestroyFunc;

        void ClearLuaReferences();
        void BindEngineAPI();

        friend class LuaScriptSystem;
    };

}