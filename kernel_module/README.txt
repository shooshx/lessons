Oracle VirtualBox

sudo apt-get install build-essential linux-headers-`uname -r`
  doesn't seem to be needed in Ubuntu
sudo apt-get install libelf-dev

https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234
http://devarea.com/linux-kernel-development-kernel-module-parameters/#.W4xSe-gzaUk


create mymod.c file
  __init, __exit not really needed for module
create Makefile
  where is the .o coming from?
  example of simple make file, how it works
  obj-m built as a module (opposed to obj-y builtin module)

sudo insmod mymod.ko
sudo dmesg
sudo rmmod mymod
modinfo mymod.ko

talk about /proc
/proc/modules

add module_param
check in another tab /sys/modules/mymod/parameters
check module_param implementation
module_param_cb - ops
param_ops_int

A line is defined as characters and \n

whoami
what is root

id  -show user id

