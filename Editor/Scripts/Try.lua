function Init(entity)

end

function Update(entity, dt)
    print("[Lua] dt ֵ: ", dt)
    if entity:has_transform() then
        print("[Lua] ʵ���� TransformComponent")
        local transform = entity:get_transform()
        print("[Lua] ԭλ��: ", transform.position.x)
        local new_pos = transform.position
        new_pos.x = new_pos.x + 1 * dt
        transform.position = new_pos
        print("[Lua] ��λ��: ", transform.position.x)
    else
        print("[Lua] ʵ���� TransformComponent")
    end
    print("[Lua] Update����")
end

function Destroy(entity)
    print(entity:get_tag().tag .. " ������")
end