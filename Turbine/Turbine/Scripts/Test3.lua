
-- OnTest test
function OnTest() 

    thingie = Turbine.Thing1.new()
    thongle = Turbine.Thing2.new()

    -- c = thingie.a + thingie.b
    -- c = c + thingie:combineab()

    -- p = thongle.a + thongle.b + thongle.c + thongle.d

    -- p = thingie:gimmeX() + thongle:gimmeX()

    -- p = thongle:gimmeY() + thingie:gimmeY()

    p = thingie:gimmeX() + thingie:gimmeX(5)

    return p
end
