
package = {}

-- OnTest test
function package.OnTest() 

    thingie = Thing1.new()
    thingoo = Thing1.new(1, 2)

    thongle = Thing2.new()

    q = glm.vec3.new(
        thongle.a,
        thongle.b,
        thongle.c
    )


    q.y = q.y + thingie.slammer.y
    p = q.x + q.y + q.z

    -- creates an emtpy entity
    entity = Turbine.Entity.new("Floobi", Turbine.EntityType.EMPTY)

    -- creates an EntityType enum
    -- shongs = Turbine.EntityType.EMPTY

    if (entity:GetEntityType() == Turbine.EntityType.EMPTY) then

        return entity -- Turbine.EntityType.LIGHT
    
    else

        return nil;

    end
end

return package

