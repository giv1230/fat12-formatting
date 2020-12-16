#!/bin/bash -x
chmod +x ./school_format
./school_format floppy.img
gcc -Wall my_format.c 
./a.out floppy.img 
mkdir mntpoint
sudo mount -o loop,uid=$(id -u),gid=$(id -g) floppy.img mntpoint/
mount | grep mntpoint
mount | grep mntpoint | grep fat
mkdir mntpoint/a
echo "Hellow world" > mntpoint/a/foo
sudo umount mntpoint/
sudo mount -o loop,uid=$(id -u),gid=$(id -g) floppy.img mntpoint/
cat mntpoint/a/foo 
sudo umount mntpoint/
