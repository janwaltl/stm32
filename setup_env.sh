#!/usr/bin/env bash
#

# Install LLVM 13
sudo apt-get install llvm-13 llvm-13-tools lld-13 -y
# Prerequisites
sudo apt-get install build-essential cmake libusb-1.0 libusb-1.0-0-dev libgtk-3-dev pandoc -y

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
