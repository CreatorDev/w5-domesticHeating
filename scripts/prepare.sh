#!/bin/sh

storage="/mnt/usb"
#prepare external drive
if ! mount | grep "$storage" > /dev/null; then
  echo "Mounting $storage "
  mount -t ext4 /dev/sda1 $storage
  echo "Done"
fi

#prepare FS
projectDir=ci40-project5
if [ ! -e "$storage/$projectDir" ]; then
  echo "$storage/$projectDir does not exists, creating"
  mkdir "$storage/$projectDir"
fi

echo "Copying project5-*.bsc to /etc/lwm2m"
cp ~/ci40-project5/project5-*.bsc /etc/lwm2m/
