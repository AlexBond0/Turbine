
-- OnTest test
function OnTest() 

    thingie = Thing1.new()
    thingoo = Thing1.new(1, 2)

    thongle = Thing2.new()

    -- c = thingie.a + thingie.b
    -- c = c + thingie:combineab()

    -- p = thongle.a + thongle.b + thongle.c + thongle.d

    -- p = thingie:gimmeX() + thongle:gimmeX()

    -- p = thongle:gimmeY() + thingie:gimmeY()

    -- p = thingie:gimmeX() + thingie:gimmeX(5) 	-- Thing2:
 

	-- Thing2:

    q = glm.vec3.new(
        thongle.a,
        thongle.b,
        thongle.c
    )
    -- thingie:setupVec()

    -- wahoo = glm.vec3.new(1.0, 1.0, 1.0)

    -- n = thingie.slammer[0]

    -- return tostring(thingie.a)-- wahoo.y

    -- n =  thingie.a + 3;

    q.y = q.y + thingie.slammer.y

    p = q.x + q.y + q.z

    -- creates an emtpy entity
    entity = Turbine.Entity.new("Floobi", Turbine.EntityType.EMPTY)

    -- creates an EntityType enum
    -- shongs = Turbine.EntityType.EMPTY

    if (shongs == Turbine.EntityType.EMPTY) then

        return entity -- Turbine.EntityType.LIGHT
    
    else

        return nil;

    end
end

