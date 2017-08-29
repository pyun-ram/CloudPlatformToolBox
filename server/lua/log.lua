local server = require "resty.websocket.server"
local cjson = require "cjson.safe"
local h = ngx.req.get_headers()

local redis = require "resty.redis"
local red = redis:new()

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
            return ngx.exit(444)
        end
        local code = err
        ngx.log(ngx.INFO, "closing with status code ", code, " and message ", data)
        return ngx.exit(444)
    else
        -- log error
        ngx.log(ngx.ERR, "failed to receive Data, ", "the type is: ", typ)
        return nil
    end                                                                         
end

local function getIpClient()
    ngx.log(ngx.INFO, "getIpClient: remoteIpAddr: ", ngx.var.remote_addr)
    return tostring(ngx.var.remote_addr)
end    

local function log(robot_id, code_id, message)
    local res, err = red:lpush("L"..robot_id..code_id, message)
    if not res then
        ngx.log(ngx.ERR, "failed to push "..robot_id..code_id .. " : ", err)
        return false, err
    end
    return true, "OK"
end

local wb, err = server:new{
    timeout = 24*60*60*1000,  -- in milliseconds
    max_payload_len = 65535,
}
if not wb then
    ngx.log(ngx.ERR, "failed to new websocket: ", err)
    return ngx.exit(444)
end

while true 
do
    local data_receive = receiveData(wb)
    local obj_receive = cjson.decode(data_receive)

    local robot_id = obj_receive.robot_id
    local code_id = obj_receive.code_id
    local ip_client = getIpClient()
    local message = obj_receive.message
    local mode = obj_receive.mode
    -- connect redis-server
    local ok, err = red:connect("127.0.0.1", 6379)
    if not ok then
    ngx.log(ngx.ERR, "failed to connect: ", err)
    return
    end
    ngx.log(ngx.INFO, "connect redis-server ok!")
    -- log message
    local res,err = log(robot_id, code_id, message)

    if mode == "SendandReceive" then
        ngx.log(ngx.INFO, "mode : SendandReceive")
        local obj_send = {
            robot_id = obj_receive.robot_id,
            code_id = obj_receive.code_id,
            ip_client = getIpClient(),
            res = res ,
            comment = err
        }
        local data_send = cjson.encode(obj_send)
        bytes, err = wb:send_text(data_send)
        if not bytes then
            ngx.log(ngx.ERR, "failed to send a text frame: ", err)
            return ngx.exit(444)
        end
    else
        ngx.log(ngx.INFO, "mode : SendOnly")
        -- do nothing
    end
    -- close redis-server
    local red_ok, red_err = red:close()
    if not red_ok then
        ngx.log(ngx.ERR, "failed to close redis connection: ", red_err)
        return ngx.exit(444)
    end
end

local bytes, err = wb:send_close(1000, "enough, enough!")
if not bytes then
    ngx.log(ngx.ERR, "failed to send the close frame: ", err)
    return ngx.exit(444)
end
