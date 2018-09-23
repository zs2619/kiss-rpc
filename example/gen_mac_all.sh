#!/bin/bash

./rpc_mac  -gen go  -i . -o gen_go  shuai.rpc
./rpc_mac  -gen cpp -i . -o gen_cpp shuai.rpc
./rpc_mac  -gen cs  -i . -o gen_cs  shuai.rpc
./rpc_mac  -gen go  -i . -o gen_go  zhang.rpc
./rpc_mac  -gen cpp -i . -o gen_cpp zhang.rpc
./rpc_mac  -gen cs  -i . -o gen_cs  zhang.rpc