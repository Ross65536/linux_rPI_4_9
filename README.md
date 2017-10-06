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

./kernel/sched/rt.c:2325:#if defined(CONFIG_CISTER_RT_SCHEDULERS)
./kernel/sched/core.c:3997:    #ifdef CONFIG_CISTER_RT_SCHEDULERS
./kernel/sched/core.c:7614:#ifdef CONFIG_CISTER_RT_SCHEDULERS
./kernel/sched/sched.h:20:#ifdef CONFIG_CISTER_RT_SCHEDULERS
./kernel/sched/sched.h:136:#ifdef CONFIG_CISTER_RT_SCHEDULERS
./kernel/sched/sched.h:148:#ifdef CONFIG_CISTER_RT_SCHEDULERS
./kernel/sched/sched.h:651:#ifdef CONFIG_CISTER_RT_SCHEDULERS
./include/config/auto.conf:312:CONFIG_CISTER_RT_SCHEDULERS=y
./include/uapi/linux/sched.h:43:#if defined(CONFIG_CISTER_RT_SCHEDULERS)
./include/generated/autoconf.h:314:#define CONFIG_CISTER_RT_SCHEDULERS 1
./include/linux/sched.h:65:#ifdef CONFIG_CISTER_RT_SCHEDULERS
./include/linux/sched.h:1527:#ifdef CONFIG_CISTER_RT_SCHEDULERS
./include/uapi/linux/sched.h:44

### Syscalls
decimal:
7	sys_cister_tracing(int boolean1or0)
31	sys_cister_set_task_id(int custom_id)
32	sys_cister_set_task_rd(unsigned int relative_deadline) // only goes up to 4 seconds

#define __NR_cister_tracing    (__NR_SYSCALL_BASE+ 7)
#define __NR_cister_set_task_id     (__NR_SYSCALL_BASE+ 31)
#define __NR_cister_set_task_rd     (__NR_SYSCALL_BASE+ 32)

### Scheduler Numbers

#define SCHED_RTS	7

## NOTES


