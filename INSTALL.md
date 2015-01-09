# Installing /dev/phony

## Build Instructions

/dev/phony is very simple to build:

```
# make
```

## Installation

Copy the rules files so udev can find it:

```
# cp phony.rules /etc/udev/rules.d/
```

## Load the kernel module:

```
# insmod ./phony_dev.ko
```

## Check the device has been created:

```
# ls -l /dev/phony 
cr--r--r--. 1 root root 10, 57 Jan  9 18:47 /dev/phony
```

## Try it!

```
dd if=/dev/phony of=/dev/null bs=1G count=1000000
```

## Notes

The device can be removed with:

```
# rmmod phony_dev
```

The device will not persist between reboots - it must be reloaded if the system is restarted.
