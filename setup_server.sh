#!/bin/bash

sudo apt update -y
sudo apt install -y gcc make cmake clang-format \
clang-tidy libcriterion-dev git
cd /home/ubuntu || exit
git clone https://github.com/olincollege/ShellWe.git
cd ShellWe || exit
mkdir build && cd build || exit
cmake ..
make
cd src/server || exit
nohup ./ChatServer > server.log 2>&1 &
