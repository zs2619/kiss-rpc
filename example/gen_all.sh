#!/bin/bash

./rpc  -gen go  -i . -o gen_go  shuai.rpc
./rpc  -gen cpp -i . -o gen_cpp shuai.rpc
./rpc  -gen go  -i . -o gen_go  zhang.rpc
./rpc  -gen cpp -i . -o gen_cpp zhang.rpc