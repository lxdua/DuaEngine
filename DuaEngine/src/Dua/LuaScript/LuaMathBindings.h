#pragma once
#include "sol/sol.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/compatibility.hpp"

namespace Dua {

    class LuaMathBindings
    {
    public:
        static void Bind(sol::state& lua)
        {
            sol::table glmTable = lua["glm"].get_or_create<sol::table>();

            BindVectorTypes(glmTable);
            BindMatrixTypes(glmTable);
            BindQuaternion(glmTable);

            BindMathFunctions(glmTable);
            BindMatrixFunctions(glmTable);
            BindQuaternionFunctions(glmTable);
        }

    private:
        static void BindVectorTypes(sol::table& glm)
        {
            glm.new_usertype<glm::vec2>("vec2",
                sol::constructors<
                    glm::vec2(),
                    glm::vec2(float),
                    glm::vec2(float, float)
                >(),
                "x", &glm::vec2::x,
                "y", &glm::vec2::y,
                sol::meta_function::addition, [](const glm::vec2& a, const glm::vec2& b) { return a + b; },
                sol::meta_function::subtraction, [](const glm::vec2& a, const glm::vec2& b) { return a - b; },
                sol::meta_function::multiplication, sol::overload(
                    [](const glm::vec2& a, float s) { return a * s; },
                    [](float s, const glm::vec2& a) { return s * a; }
                ),
                "normalize", [](const glm::vec2& v) { return glm::normalize(v); },
                "length", [](const glm::vec2& v) { return glm::length(v); },
                "dot", [](const glm::vec2& a, const glm::vec2& b) { return glm::dot(a, b); }
            );

            glm.new_usertype<glm::vec3>("vec3",
                sol::constructors<
                    glm::vec3(),
                    glm::vec3(float),
                    glm::vec3(float, float, float)
                >(),
                "x", &glm::vec3::x,
                "y", &glm::vec3::y,
                "z", &glm::vec3::z,
                sol::meta_function::addition, [](const glm::vec3& a, const glm::vec3& b) { return a + b; },
                sol::meta_function::subtraction, [](const glm::vec3& a, const glm::vec3& b) { return a - b; },
                sol::meta_function::multiplication, sol::overload(
                    [](const glm::vec3& a, float s) { return a * s; },
                    [](float s, const glm::vec3& a) { return s * a; }
                ),
                "cross", [](const glm::vec3& a, const glm::vec3& b) { return glm::cross(a, b); },
                "normalize", [](const glm::vec3& v) { return glm::normalize(v); },
                "length", [](const glm::vec3& v) { return glm::length(v); }
            );

            glm.new_usertype<glm::vec4>("vec4",
                sol::constructors<
                    glm::vec4(),
                    glm::vec4(float),
                    glm::vec4(float, float, float, float)
                >(),
                "x", &glm::vec4::x,
                "y", &glm::vec4::y,
                "z", &glm::vec4::z,
                "w", &glm::vec4::w,
                sol::meta_function::multiplication, [](const glm::vec4& v, const glm::mat4& m) { return m * v; }
            );
        }

        static void BindMatrixTypes(sol::table& glm)
        {
            glm.new_usertype<glm::mat4>("mat4",
                sol::constructors<
                    glm::mat4(),
                    glm::mat4(float)
                >(),
                sol::meta_function::multiplication, sol::overload(
                    [](const glm::mat4& a, const glm::mat4& b) { return a * b; },
                    [](const glm::mat4& m, const glm::vec4& v) { return m * v; }
                ),
                "transpose", [](const glm::mat4& m) { return glm::transpose(m); },
                "inverse", [](const glm::mat4& m) { return glm::inverse(m); }
            );
        }

        static void BindQuaternion(sol::table& glm)
        {
            glm.new_usertype<glm::quat>("quat",
                sol::constructors<
                    glm::quat(),
                    glm::quat(float, float, float, float),
                    glm::quat(glm::vec3)
                >(),
                "x", &glm::quat::x,
                "y", &glm::quat::y,
                "z", &glm::quat::z,
                "w", &glm::quat::w,
                "normalize", [](const glm::quat& q) { return glm::normalize(q); },
                "euler_angles", [](const glm::quat& q) { return glm::eulerAngles(q); }
                );
        }

        static void BindMathFunctions(sol::table& glm)
        {
            glm["radians"] = [](float degrees) { return glm::radians(degrees); };
            glm["degrees"] = [](float radians) { return glm::degrees(radians); };
            glm["lerp"] = sol::overload(
                [](float a, float b, float t) { return glm::lerp(a, b, t); },
                [](const glm::vec3& a, const glm::vec3& b, float t) { return glm::lerp(a, b, t); }
            );
        }

        static void BindMatrixFunctions(sol::table& glm)
        {
            glm["translate"] = [](const glm::mat4& m, const glm::vec3& v) { return glm::translate(m, v); };
            glm["rotate"] = sol::overload(
                [](const glm::mat4& m, float angle, const glm::vec3& axis) { return glm::rotate(m, angle, axis); },
                [](float angle, const glm::vec3& axis) { return glm::rotate(angle, axis); }
            );
            glm["scale"] = [](const glm::mat4& m, const glm::vec3& v)
                { return glm::scale(m, v); };
            glm["look_at"] = [](const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) 
                { return glm::lookAt(eye, center, up); };
        }

        static void BindQuaternionFunctions(sol::table& glm)
        {
            glm["angle_axis"] = [](float angle, const glm::vec3& axis) { return glm::angleAxis(angle, axis); };
            glm["quat_look_at"] = [](const glm::vec3& direction, const glm::vec3& up)
                { return glm::quatLookAt(direction, up); };
        }
    };

}