local server = require "resty.websocket.server"

local function receiveData(wb)
    local data, typ, err = wb:recv_frame()
    if not data then
        ngx.log(ngx.ERR, "failed to receive a frame: ", err)
        return ngx.exit(444)
    end
    if typ == "text" or typ == "binary" then
        -- log data & type
        -- return data
        ngx.log(ngx.INFO, "received Data: ", data, " (", typ, ") ");
        return data
    elseif typ == "close" then
        -- send a close frame back:
        local bytes, err = wb:send_close(1000, "enough, enough!")
        if not bytes then
            ngx.log(ngx.ERR, "failed to send the close frame: ", err)
            return nil
        end
        local code = err
        ngx.log(ngx.INFO, "closing with status code ", code, " and message ", data)
        return nil
    else
        -- log error
        ngx.log(ngx.ERR, "failed to receive Data, ", "the type is: ", typ)
        return nil
    end                                                                         
end

local wb, err = server:new{
    timeout = 5000,  -- in milliseconds
    max_payload_len = 65535,
}
if not wb then
    ngx.log(ngx.ERR, "failed to new websocket: ", err)
    return ngx.exit(444)
end

local data = receiveData(wb)

wb:set_timeout(1000)  -- change the network timeout to 1 second
bytes, err = wb:send_text(data)
if not bytes then
    ngx.log(ngx.ERR, "failed to send a text frame: ", err)
    return ngx.exit(444)
end
wb:set_timeout(5000)
--[[
bytes, err = wb:send_binary("blah blah blah...")
if not bytes then
    ngx.log(ngx.ERR, "failed to send a binary frame: ", err)
    return ngx.exit(444)
end
]]--
local bytes, err = wb:send_close(1000, "enough, enough!")
if not bytes then
    ngx.log(ngx.ERR, "failed to send the close frame: ", err)
    return
end