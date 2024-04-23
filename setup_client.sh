#!/bin/bash

sudo apt update -y
sudo apt install -y gcc make cmake clang-format \
clang-tidy libcriterion-dev
git clone --single-branch --branch main https://github.com/olincollege/ShellWe.git
cd ShellWe || exit
mkdir build && cd build || exit
cmake ..
make
cd src/client || exit
ip_addr="$1"
./ChatClient "$ip_addr"
