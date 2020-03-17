#!/bin/bash

./rpc  -gen go  -i schema -o go/shuai  HelloMsg.rpc
./rpc  -gen cpp -i schema -o cpp/example HelloMsg.rpc
./rpc  -gen go  -i schema -o go/rpc  Packet.rpc
./rpc  -gen cpp -i schema -o cpp/rpc Packet.rpc