lua_ffi
=======
1、将libmytest.so拷贝到/usr/lib目录下。
2、FFI库，是LuaJIT中最重要的一个扩展库。它允许从纯Lua代码调用外部C函数，使用C数据结构。有了它，就不用再像Lua标准math库一样，编写Lua扩展库。把开发者从开发Lua扩展C库（语言/功能绑定库）的繁重工作中释放出来。
