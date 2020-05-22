
-- OnTest test
function OnTest() 

    thingie = Thing1.new()
    thongle = Thing2.new()

    -- c = thingie.a + thingie.b
    -- c = c + thingie:combineab()

    -- p = thongle.a + thongle.b + thongle.c + thongle.d

    -- p = thingie:gimmeX() + thongle:gimmeX()

    p = thongle:gimmeY()

    return p
end
