
-- OnMessage test
function OnMessage(message) 

    local ee = ""


    -- all key value pairs for this thing
    if (message) then
        
        for k,v in pairs(message) do

            ee = ee .. k .. ":" .. v .. " | "

        end
    
    else
        ee = "no exists"

    end

    -- try to access this stupid variable
    b = message.hello

    if (b == nil) then
        ee = ee .. " Fukin nil"

    else

        ee = ee .. b

    end

    return ee

    -- a = {}     -- create a table and store its reference in `a'
    -- k = "x"
    -- a[k] = 10        -- new entry, with key="x" and value=10
    -- a[20] = "great"  -- new entry, with key=20 and value="great"
    -- -- print(a["x"])    --> 10
    -- k = 20
    -- -- print(a[k])      --> "great"
    -- a["x"] = a["x"] + 1     -- increments entry "x"
    -- -- print(a["x"])    --> 11

    -- return a["x"]

    -- data = {}
    -- data["p"] = "q"
    -- return data.p

    -- local newTable = copy(message);

    -- return newTable[3]

end

-- copy table
function copy(obj, seen)

    if type(obj) ~= 'table' then return obj end
    if seen and seen[obj] then return seen[obj] end
    local s = seen or {}
    local res = setmetatable({}, getmetatable(obj))
    s[obj] = res
    for k, v in pairs(obj) do res[copy(k, s)] = copy(v, s) end
    return res
end


