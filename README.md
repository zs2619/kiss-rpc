# KissRPC
KissRPC 是个简单、可扩展 、跨语言的异步RPC框架。可以应用于网络游戏开发等领域。

## 特性
* 传输层 支持tcp、websocket http(待实现) 
* 协议层 支持自定义格式 、josn protobuf(待实现)
* 支持语言
  * c++ (libevent网络层,单线程reactor模型，所有回调在主线程执行,便于实现复杂逻辑)
  * golang(回调在同一协程中处理)
  * cshape(待实现)
  * typescript(待实现)

## [IDL 格式](https://github.com/zs2619/kiss-rpc/tree/master/docs/IDL.md) 

## 依赖
```
flex and bison
```
## 目录结构
目录名称 |功能
------------------|----------------
[tools](https://github.com/zs2619/kiss-rpc/tree/master/tools/GnuWin32) |flex bison Windows库
[examples](https://github.com/zs2619/kiss-rpc/tree/master/example)     |rpc 编译器测试
[parser](https://github.com/zs2619/kiss-rpc/tree/master/parser)        |rpc编译器代码
[generator](https://github.com/zs2619/kiss-rpc/tree/master/parser)     |rpc编译器后端生成各种语言的代码实现
[lib](https://github.com/zs2619/kiss-rpc/tree/master/lib)       |rpc各种语言的框架代码实现和例子

## 构建
### Linux 
```
cmake .
make 
```

## 使用
* rpc编译IDL文件，生成前后端代码
```
Usage: rpc [options] file
Options:
  -o   dir    output file directory
  -i   dir    input  file directory
  -gen lang   cpp as3 cs go Generate code
  -json       serialize json
```
* 参考lib下对应语言例子,然后自己看着办。
