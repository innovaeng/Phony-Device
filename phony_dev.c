/*
 * Based on "Hello, world!" minimal kernel module - /dev version - by Valerie Henson <val@nmt.edu>
 *
 * http://www.linuxdevcenter.com/pub/a/linux/2007/07/05/devhelloworld-a-simple-introduction-to-device-drivers-under-linux.html
 *
 * Phony device kernel module - /dev/phony
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

/*
 * phony_read is the function called when a process calls read() on
 * /dev/phony.  It pretends to completely fill the buffer supplied.
 */

static ssize_t phony_read(struct file * file, char * buf, size_t count, loff_t *ppos)
{
	/*
	 * Do nothing, but indicate buffer completely filled.
	 */
	*ppos += count;

	return count;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations phony_fops = {
	.owner		= THIS_MODULE,
	.read		= phony_read,
};

static struct miscdevice phony_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/phony.
	 */
	"phony",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&phony_fops
};

static int __init
phony_init(void)
{
	int ret;

	/*
	 * Create the "phony" device in the /sys/class/misc directory.
	 * udev will automatically create the /dev/phony device using
	 * the default rules.
	 */
	ret = misc_register(&phony_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \'phony\' misc device\n");

	return ret;
}

module_init(phony_init);

static void __exit
phony_exit(void)
{
	misc_deregister(&phony_dev);
}

module_exit(phony_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Innova Engineering <innova@innovaengineering.co.uk>");
MODULE_DESCRIPTION("\'phony\' device module");
MODULE_VERSION("1.0");
