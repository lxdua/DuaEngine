function Init(entity)

end

function Update(entity, dt)
    print("[Lua] dt 值: ", dt)
    if entity:has_transform() then
        print("[Lua] 实体有 TransformComponent")
        local transform = entity:get_transform()
        print("[Lua] 原位置: ", transform.position.x)
        local new_pos = transform.position
        new_pos.x = new_pos.x + 1 * dt
        transform.position = new_pos
        print("[Lua] 新位置: ", transform.position.x)
    else
        print("[Lua] 实体无 TransformComponent")
    end
    print("[Lua] Update结束")
end

function Destroy(entity)
    print(entity:get_tag().tag .. " 被销毁")
end