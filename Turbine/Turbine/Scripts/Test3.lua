
-- OnTest test
function OnTest() 

    thingie = Turbine.Thing1.new()
    thongle = Turbine.Thing2.new()

    -- c = thingie.a + thingie.b
    -- c = c + thingie:combineab()

    -- p = thongle.a + thongle.b + thongle.c + thongle.d

    -- p = thingie:gimmeX() + thongle:gimmeX()

    -- p = thongle:gimmeY() + thingie:gimmeY()

    -- p = thingie:gimmeX() + thingie:gimmeX(5)


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

    return p;
end

