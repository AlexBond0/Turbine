
package = {}

-- OnMessage test
function package.OnMessage(message) 

    return message.hello .. " Again!"
end

return package
