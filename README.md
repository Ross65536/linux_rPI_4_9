linux source for raspberry pi taken from https://github.com/raspberrypi/linux/tree/rpi-4.13.y and modified to add realtime schedulers

## Environment Setup
taken [from](https://www.raspberrypi.org/documentation/linux/kernel/building.md)

### Burn Raspbian image on SDCAR of the raspberry PI

download from website and follow their tutorial to burn a image

### INSTALL TOOLCHAIN

Use the following command to install the toolchain:

git clone https://github.com/raspberrypi/tools

You can then copy the  /tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64 directory to a common location (like ~/bin), and add  /tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin to your $PATH in the .bashrc in your home directory (Add this line 

export PATH=$PATH:PATH_TO_TOOLS_FOLDER/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin

to the end of your ~/.bashrc)

### Build Kernel

clone the repo and then

in linux/BUILD_SCRIPTS
run 

./1_Build_rPi.sh

to build (might take a while)

and then use 

lsblk #to see find the sdcard partitions

and then export your path to be visible for ./3_Enable_SSH.sh

export RPI_BOOT_PARTITION=#dev/sd?? #boot partition

export RPI_ROOT_PARTITION=#dev/sd?? #root partition


and then use 

./2_Mount.sh

./3_Enable_SSH.sh

you should also export

export ARM_GCC_PATH_PREFIX=PATH_TO_TOOLCHAIN/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-

for:

./4_Install_rPi.sh

to install the kernel

## Modifications

### Syscalls

397	sys_cister_tracing(int boolean1or0)
398	sys_cister_set_task_id(int custom_id)
399	sys_cister_set_task_rd(unsigned int relative_deadline) // only goes up to 4 seconds

### Scheduler Numbers

#define SCHED_EDF	7

## NOTES


