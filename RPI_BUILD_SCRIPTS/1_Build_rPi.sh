cd .. 
KERNEL=kernel
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j 8
cat RPI_BUILD_SCRIPTS/BUILD_ERRORS

