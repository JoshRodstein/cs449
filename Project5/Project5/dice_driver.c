/*
 * dice_driver - /dev version
 *
 * Joshua Rodstein <jor94@pitt.edu>
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>

unsigned char get_random_byte(int);

/*
 * dice_driver_read is the function called when a process calls read() on
 * /dev/dice_driver.  
 */

static ssize_t dice_driver_read(struct file * file, char * buf,
                          size_t count, loff_t *ppos)
{
  /* Allocate kernel memory on the heap with kalloc.  
   * Flag: GFP_NOENTRY; DO NOT re-attempt allocation upon failure.*/
  //unsigned char * roll_buf = kmalloc(count, GFP_KERNEL);
  char * roll_buf = kmalloc(count, GFP_KERNEL);
  int i;
  /*
   * We only support reading the whole string at once.
   */
  if (count == 0){
    return -EINVAL;
  }
  
  for(i = 0; i < count; i++){
    roll_buf[i] = get_random_byte(6);
  }
  
  
  /*
   * Besides copying the string to the user provided buffer,
   * this function also checks that the user has permission to
   * write to the buffer, that it is mapped, etc.
   */
  if (copy_to_user(buf, roll_buf, count)){
    return -EINVAL;
  }
  /*
   * Tell the user how much data we wrote.
   */
  *ppos = count;
  kfree(roll_buf);
  return count;

}

unsigned char get_random_byte(int max) {
  unsigned char c;
  get_random_bytes(&c, 1);
  return c%max;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations dice_driver_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_driver_read,
};

static struct miscdevice dice_driver = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice_driver
	 */
	"dice_driver",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&dice_driver_fops
};

static int __init 
dice_driver_init(void)
{
	int ret;

	/*
	 * Create the "dice_driver" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_driver);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"dice_driver\" misc device\n");

	return ret;
}

module_init(dice_driver_init);

static void __exit 
dice_driver_exit(void)
{
	misc_deregister(&dice_driver);
}

module_exit(dice_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joshua Rodstein <jor94@pitt.edu>");
MODULE_DESCRIPTION("/dev/dice_driver module");
MODULE_VERSION("dev");
