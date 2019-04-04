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

## 依赖
```
flex and bison
```

## 构建
### Linux 
```
cmake .
make 
```

## 使用
```
Usage: rpc [options] file
Options:
  -o   dir    output file directory
  -i   dir    input  file directory
  -gen lang   cpp as3 cs go Generate code
  -json       serialize json
```
