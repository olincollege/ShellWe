#!/bin/bash

sudo apt update -y
sudo apt install -y gcc gdb make cmake clang-format \
clang-tidy libcriterion-dev jq
cd /home/ubuntu || exit
git clone https://github.com/olincollege/ShellWe.git
cd ShellWe || exit
mkdir build && cd build || exit
cmake ..
make
cd src/server || exit
ip_addr=$(ip -f inet -o -j addr show eth0 | jq -r '.[].addr_info[].local')
nohup ./ChatServer "$ip_addr" > server.log 2>&1 &
