cd ..
KERNEL=kernel
sudo make ARCH=arm CROSS_COMPILE=$ARM_GCC_PATH_PREFIX INSTALL_MOD_PATH=/mnt/ext4 modules_install
#/mnt/ext4 is the path to the root of the linux in the raspberry pi, you might need to mount this folder first
sudo cp arch/arm/boot/zImage /mnt/fat32/$KERNEL.img
sudo cp arch/arm/boot/dts/*.dtb /mnt/fat32/
sudo cp arch/arm/boot/dts/overlays/*.dtb* /mnt/fat32/overlays/
sudo cp arch/arm/boot/dts/overlays/README /mnt/fat32/overlays/

