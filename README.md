lua_ffi
=======
1、将libmytest.so拷贝到/usr/lib目录下。
2、FFI库，是LuaJIT中最重要的一个扩展库。它允许从纯Lua代码调用外部C函数，使用C数据结构。有了它，就不用再像Lua标准math库一样，编写Lua扩展库。把开发者从开发Lua扩展C库（语言/功能绑定库）的繁重工作中释放出来。


location = /lua/uuid_to_uin/so {
            content_by_lua '
                local cjson = require "cjson"
                local uuid_list = ngx.var.arg_uuid_list
                if uuid_list == nil then
                    ngx.status = 90101
                    ngx.say(cjson.encode({code=ngx.status,msg="参数错误"}))
                    return
                end
                local util = require "util"
                local pb_arr = {uuid_list={}}
                uuid_list = util.split(uuid_list,",")
                for k,v in ipairs(uuid_list) do
                    table.insert(pb_arr.uuid_list,v)
                end

                local Proto = require "proto"
                local proto = Proto:new()
                local m = require "uuid_svr.UuidAccountSvr"
                local pb = m.get_instance()
                local body = pb.encode("uuidaccountsvr_protos.UuidBatchQqReq",pb_arr)
                proto:set_command(0x3326)
                proto:set_sConnAddr(1051)
                proto:write_byte(0x02)
                proto:write_string(body)
                local req = proto:pack()
                --ngx.say(proto:str2hex(req))
                --ngx.say(req)

                local ffi = require("ffi")
                local luaudp = ffi.load("luaudp")
                ffi.cdef[[
                int udp_send(char* inbuff, int inbufflen, char* outbuff, const char* ip, int port, int waits)
                ]]

                local inbuff = ffi.new("char[?]", #req)
                ffi.copy(inbuff, req)
                local outbuff = ffi.new("char[8000]")
                local res = luaudp.udp_send(inbuff, #req, outbuff, "XXX.XXX.XXX.XXX", XXXX, 1000)
                local bytes = ffi.string(outbuff, res)
                if res <= 0 then
                    ngx.status = -93503
                    ngx.say(cjson.encode({code=ngx.status,msg="failed to recv data"}))
                    return
                end
                
                proto:unpack(bytes)
                local body = string.sub(proto:get_body(),2)
                local resp = pb.decode("uuidaccountsvr_protos.UuidBatchQqRsp",body)
                --ngx.say(cjson.encode(resp))
                if resp.result ~= 0 then
                    ngx.status = 93501
                    ngx.say(cjson.encode({code=ngx.status,msg="result error:"..resp.error_msg}))
                    return
                end
                local data={}
                for k,v in ipairs(resp.qq_uuid_list) do
                    --ngx.say("result=",v.result,",uin=",v.uin,",uuid=",v.uuid)
                    if v.result == 0 then
                        data[v.uuid] = v.uin
                    end
                end
                ngx.status = 0
                ngx.say(cjson.encode({code=ngx.status,msg="succ",data=data}))
            ';
        }
