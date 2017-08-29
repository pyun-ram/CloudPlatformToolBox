local redis = require "resty.redis"
local red = redis:new()

red:set_timeout(1000) -- 1 sec

-- or connect to a unix domain socket file listened
-- by a redis server:
--     local ok, err = red:connect("unix:/path/to/redis.sock")

local ok, err = red:connect("127.0.0.1", 6379)
if not ok then
ngx.say("failed to connect: ", err)
return
end

local res, err = red:hmset("12.34.56.78", {used_times_server = 5, total_times = 10})
if not ok then
ngx.say("failed to set dog: ", err)
return
end

ngx.say("set result: ", ok)

local res, err = red:hmget("12.34.56.78", "used_times_server", "total_times", "nofield")
if not res then
ngx.say("failed to get dog: ", err)
return
end

if res == ngx.null then
ngx.say("12.34.56.78 not found.")
return
end

ngx.say("12.34.56.78: ", res[])

--[[
red:init_pipeline()
red:set("cat", "Marry")
red:set("horse", "Bob")
red:get("cat")
red:get("horse")
local results, err = red:commit_pipeline()
if not results then
ngx.say("failed to commit the pipelined requests: ", err)
return
end

for i, res in ipairs(results) do
if type(res) == "table" then
if res[1] == false then
    ngx.say("failed to run command ", i, ": ", res[2])
else
    -- process the table value
end
else
-- process the scalar value
end
end
]]--
-- put it into the connection pool of size 100,
-- with 10 seconds max idle time
--local ok, err = red:set_keepalive(10000, 100)
--if not ok then
--ngx.say("failed to set keepalive: ", err)
--return
--end

-- or just close the connection right away:
-- local ok, err = red:close()
-- if not ok then
--     ngx.say("failed to close: ", err)
--     return
-- end