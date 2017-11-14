sudo mkdir /mnt/ext4 &
sudo mkdir /mnt/fat32 &
sudo mount /dev/$RPI_BOOT_PARTITION /mnt/fat32/ && sudo mount /dev/$RPI_ROOT_PARTITION /mnt/ext4/

