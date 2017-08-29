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

local function getCodeServerInfo(robot_id, ip_client, code_id)
    local res, err = red:hmget(ip_client..code_id, "used_times_server", "total_times", "due_time")
    local key_redis = ip_client .. code_id
    if not res then
        ngx.log(ngx.ERR, "failed to get "..ip_client..code_id .. " : ", err)
        return ngx.null, ngx.null, ngx.null, ngx.null
    end
    if res[1]==ngx.null then
        ngx.log(ngx.ERR, ip_client..code_id.. " is null: ", err)
        res, err = red:hmget(robot_id..code_id, "used_times_server", "total_times", "due_time")
        if not res then
            ngx.log(ngx.ERR, "failed to get "..robot_id..code_id .. " : ", err)
            return ngx.null, ngx.null, ngx.null, ngx.null
        end
        if res[1]==ngx.null then
            ngx.log(ngx.ERR, robot_id..code_id.. " is null: ", err)
            return ngx.null, ngx.null, ngx.null, ngx.null
        end
        ngx.log(ngx.INFO, "get from " .. robot_id .. code_id)
        key_redis = robot_id .. code_id
    end

    return tonumber(res[1]), tonumber(res[2]), tonumber(res[3]), key_redis
end

local function updateCodeServerInfo(key_redis, used_times_server)
    local res, err = red:hmset(key_redis, "used_times_server", tostring(used_times_server+1))
    ngx.log(ngx.INFO, "updateCodeServerInfo: ", res, tostring(used_times_server+1))
    if not res then
    ngx.say("failed to updateCodeServerInfo ", ip_client..code_id, ": ", err)
    return
    end

    --ngx.log(ngx.INFO, "updateCodeServerInfo OK")
    return true
end

local function validate(key_redis, used_times_server, timestamp, total_times, due_time)
    if (used_times_server  < total_times ) and (timestamp < due_time) then
        --ngx.log(ngx.INFO, "used_times_server: ", used_times_server, type(used_times_server))
        updateCodeServerInfo(key_redis, used_times_server)
        ngx.log(ngx.INFO, key_redis, " OK")
        return true
    else
        ngx.log(ngx.INFO, "used_times: " , (used_times_server < total_times ) )
        ngx.log(ngx.INFO, "time: " , (timestamp < due_time) )
        return false
    end
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

    local ip_client = getIpClient()
    local code_id = obj_receive.code_id
    local timestamp = obj_receive.timestamp
    local robot_id = obj_receive.robot_id
    -- connect redis-server
    local ok, err = red:connect("127.0.0.1", 6379)
    if not ok then
    ngx.log(ngx.ERR, "failed to connect: ", err)
    return
    end
    ngx.log(ngx.INFO, "connect redis-server ok!")

    local used_times_server, total_times, due_time, key_redis = getCodeServerInfo(robot_id, ip_client, code_id)
    local status = false
    if not (used_times_server == ngx.null) then
        status = validate(key_redis, used_times_server, timestamp, total_times, due_time)
    end
    local obj_send = {
        robot_id = robot_id,
        code_id = code_id,
        timestamp = timestamp,
        ip_client = ip_client,
        used_times_server = used_times_server,
        total_times = total_times,
        due_time = due_time,
        status = status
    }
    local data_send = cjson.encode(obj_send)
    bytes, err = wb:send_text(data_send)
    if not bytes then
        ngx.log(ngx.ERR, "failed to send a text frame: ", err)
        return ngx.exit(444)
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
