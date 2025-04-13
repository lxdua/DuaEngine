#pragma once


#define DECLARE_SCRIPT(SCRIPT_TYPE) \
extern "C" { \
    void Script_Init_##SCRIPT_TYPE(void* instance, Entity e); \
    void Script_Destroy_##SCRIPT_TYPE(void* instance, Entity e); \
}

#define DEFINE_SCRIPT(SCRIPT_TYPE) \
void Script_Init_##SCRIPT_TYPE(void* instance, Entity e) { \
    static_cast<SCRIPT_TYPE*>(instance)->OnCreate(e); \
} \
void Script_Destroy_##SCRIPT_TYPE(void* instance, Entity e) { \
    static_cast<SCRIPT_TYPE*>(instance)->OnDestroy(e); \
} \
template<> \
struct ScriptBinder<SCRIPT_TYPE> { \
    static void Bind(NativeScriptComponent& script) { \
        script.Bind<SCRIPT_TYPE>(); \
        script.OnInit = Script_Init_##SCRIPT_TYPE; \
        script.OnDestroy = Script_Destroy_##SCRIPT_TYPE; \
    } \
};