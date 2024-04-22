#!/bin/bash

sudo apt update -y
sudo apt install -y gcc gdb make cmake clang-format \
clang-tidy libcriterion-dev jq
git clone --single-branch --branch aws https://github.com/olincollege/ShellWe.git
cd ShellWe || exit
mkdir build && cd build || exit
cmake ..
make
cd src/client || exit
ip_addr="3.130.117.42"
./ChatClient "$ip_addr"
