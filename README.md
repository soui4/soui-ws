# soui-ws
soui websocket moudle, 支持wss

## build
1、确保已经安装了soui4并可以通过环境变量SOUI4PATH找到soui4
2、需要获取子模块，包括openssl, libwebsocket
3、使用build_openssl_x86.bat或者build_openssl_x64.bat编译openssl1.1.1的lib,编译成功后会输出到openssl_output文件夹中。
4、使用cmake生成工程项目。
5、依次编译好websockets, ws, ServerTest, ClientTest。
### 服务器连接分组
服务器通过实现IConnGroup，可以将连接分组，类似游戏房间功能。进入相同房间的连接在同一个组，服务器中可以根据业务逻辑在小组中广播，也可以在全服务器广播。

### 测试
将ws\cert下两个文件复制到debug 或者release目录。
启动ServerTest.exe及ClientTest.exe
ClientTest支持命名行参数："roomid=100&userid=200", 通过输入不同的值进入不同分组。具体用法参见ServerTest.cpp的用法
