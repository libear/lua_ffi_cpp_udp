local ffi = require("ffi")
local luaudp = ffi.load("luaudp")
ffi.cdef[[
int udp_send(char* inbuff, int inbufflen, char* outbuff, const char* ip, int port, int waits)
]]
local str = "libearxiong88888889990"
local port = 9800;
local waits = 1000;

inbuff = ffi.new("char[?]", #str)
ffi.copy(inbuff, str)
outbuff = ffi.new("char[8000]")
local res = luaudp.udp_send(inbuff, #str, outbuff, "10.191.16.215", port, waits)
if res > 0 then
    local str_back = ffi.string(outbuff, res)
    print(#str_back)
    print(str_back)
else
    print(res)
end
