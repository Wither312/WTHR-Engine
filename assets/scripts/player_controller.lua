function player_controller_onInit(entity)
    print("Player controller initialized for entity: " .. entity)
    local transform = getTransform(entity)
    if transform then
        transform.position.x = 5.0
    end
end

-- Store direction as a global or entity-specific variable
local direction = 1  -- 1 = right, -1 = left
-- Define direction and speed outside the function
local direction = 1       -- 1 = right, -1 = left
local speed = 0.10           -- movement speed (units per second)

function player_controller_onUpdate(entity, dt)
    local transform = getTransform(entity)

    if transform then
        -- Move smoothly
        transform.position.x = transform.position.x + direction * speed * dt

        -- Check boundaries and flip direction
        if transform.position.x > 10 then
            transform.position.x = 10
            direction = -1  -- go left
        elseif transform.position.x < -10 then
            transform.position.x = -10
            direction = 1   -- go right
        end

        print("Updating, x = " .. transform.position.x)
    end
end



function player_controller_onDestroy(entity)
    local transform = getTransform(entity)
    if transform then
        -- Optional cleanup, but don't use dt here
        print("Player controller destroyed for entity: " .. entity)
        transform.position.x = 10
								print("Did player move??")
    else
        print("Player controller destroyed: transform not found for entity: " .. entity)
    end
end
