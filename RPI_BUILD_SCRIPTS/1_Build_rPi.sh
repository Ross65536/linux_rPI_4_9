cd .. 
KERNEL=kernel
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j 8
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs -j 8 2>
cat RPI_BUILD_SCRIPTS/BUILD_ERRORS

