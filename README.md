# Phony Device

This project creates a new device in /dev to make performance testing more accurate.

## Background

The history of this comes from some work my company (Innova Engineering) undertook to build a very high performance storage system for recording network traffic.  During development, we noticed that when using our own applications for measuring read and write performance we obtained much better results than when using standard Linux utilities such as 'dd'.

With some additional system tuning, we increased dd's read performance to match that of our own applications, but could not do the same for its write speed.  We were using very simple command lines:

```
# dd if=/dev/zero of=/data/testfile bs=1G oflag=direct count=1000
# dd if=/data/testfile of=/dev/null bs=1G iflag=direct
```

The key difference between these two commands is the device being used.  When dd is writing to /dev/null, its data is immediately discarded by the device without ever being inspected: the write to /dev/null executes in virtually no time, regardless of the size of the block of data being written.  But when dd is reading from /dev/zero, the device copies zeroes into the buffer which takes time proportionate to the volume of data requested.

## Performance

To confirm the theory that our use of /dev/zero was slowing things down, we developed a new input device: /dev/phony.  In the same way that /dev/null accepts all writes without doing anything, /dev/phony accepts all read requests without actually copying anything - it simply tells the calling application that its buffer is now full.  This makes it very fast indeed:

```
# dd if=/dev/phony of=/dev/null bs=1M count=1000000
1000000+0 records in
1000000+0 records out
1048576000000 bytes (1.0 TB) copied, 0.293653 s, 3.6 TB/s
```

Now that's good.  But if our theory is correct, increasing the block size should lead to a corresponding increase in throughput:

```
# dd if=/dev/phony of=/dev/null bs=1G count=1000000
1000000+0 records in
1000000+0 records out
1073741824000000 bytes (1.1 PB) copied, 0.296209 s, 3.6 PB/s
```

Now that's *really* good.  Performance has scaled linearly because neither /dev/phony nor /dev/null care about block size.  So let's push it to the maximum block size that dd can handle:

```
# dd if=/dev/phony of=/dev/null bs=2047M count=1000000
1000000+0 records in
1000000+0 records out
2146435072000000 bytes (2.1 PB) copied, 0.291501 s, 7.4 PB/s
```

And that's awesome!  dd is capable of forwarding over 7 petabytes of data per second on a standard Linux server (Ivy Bridge 3.3GHz, CentOS 7u0).  Never again will I doubt dd's capabilities, and nor should you.

## Conclusions

By using /dev/phony for input instead of /dev/zero, we have been able to more than double dd's performance when writing to our storage system, and bring it in line with our own dedicated applications.  It has enabled us to test the performance of the storage without any other overheads interfering in the measurements.

## Over To You

We are offering /dev/phony to the community so that anyone can use it to test storage or application performance.  It is licensed under GPL.

## Credits

This code is based on an article published by Valerie Henson at http://www.linuxdevcenter.com/pub/a/linux/2007/07/05/devhelloworld-a-simple-introduction-to-device-drivers-under-linux.html?page=1, and we would like to thank her for her permission to use it here.

## Security Note

/dev/phony makes no changes to the contents of the buffer passed to it when an application requests data: when the read completes, the buffer contains the same data that was in it originally.  So if some of the memory that the buffer occupies had previously been used for another purpose by the application, and not cleared out, it could still contain sensitive information.  'Reflecting' this via /dev/phony back into the application as input could potentially cause that information to leak.

This is only a risk for applications that contain sensitive data in the first place.  In typical performance testing scenarios, such as illustrated here with dd, there is no sensitive data involved and hence no risk of leakage.
