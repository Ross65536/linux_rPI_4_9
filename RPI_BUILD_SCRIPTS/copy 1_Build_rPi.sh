cd .. 
KERNEL=kernel
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcmrpi_defconfig -j 8
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs -j 8 2> RPI_BUILD_SCRIPTS/BUILD_ERRORS
cat RPI_BUILD_SCRIPTS/BUILD_ERRORS

