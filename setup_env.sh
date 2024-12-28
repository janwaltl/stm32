#!/usr/bin/env bash
#

# Install LLVM 14
sudo apt-get install llvm-14 llvm-14-tools lld-14 -y
# Prerequisites
sudo apt-get install build-essential cmake libusb-1.0 libusb-1.0-0-dev -y

# Install OpenOCD
git clone https://github.com/openocd-org/openocd
pushd openocd
./bootstrap
./configure
make -j4 
sudo make install
popd
rm -rf openocd


# Install STLink tools
git clone https://github.com/stlink-org/stlink
pushd stlink
make clean release -j4
sudo make install
sudo ldconfig

# Install UDEV rules for STLink
sudo cp -a config/udev/rules.d/* /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger

popd
rm -rf stlink

# Install compiledb for generating compile_commands.json
pip install compiledb

# Install SVD extension to GDB
git clone https://github.com/bnahill/PyCortexMDebug.git

pushd PyCortexMDebug
sudo python setup.py install
popd
rm -rf PyCortexMDebug

# Download SVD file for my board
wget https://raw.githubusercontent.com/modm-io/cmsis-svd-stm32/refs/heads/main/stm32f4/STM32F411.svd
