#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/efi.h>

#include "efi_custom_rt.h"

MODULE_AUTHOR("sjtu-se");
MODULE_DESCRIPTION("EFI Custom Runtime Service");
MODULE_LICENSE("GPL");

static long efi_custom_rt_entry(unsigned long arg)
{
	struct efi_custom_rt_service __user *custom_user;
	struct efi_custom_rt_service custom;
	efi_status_t status;
	struct efi_custom_rt_service_in in;
	struct efi_custom_rt_service_out out;

	custom_user = (struct efi_custom_rt_service __user *)arg;
	if (copy_from_user(&custom, custom_user,
			   sizeof(struct efi_custom_rt_service))) {
		return -EFAULT;
	}
	if (copy_from_user(&in, custom.in,
			   sizeof(struct efi_custom_rt_service_in))) {
		return -EFAULT;
	}

	status = efi.custom_rt_service((void *)&in, (void *)&out);

	if (put_user(status, custom.status)) {
		return -EFAULT;
	}

	if (custom.out && put_user(out, custom.out)) {
		return -EFAULT;
	}

	return status == EFI_SUCCESS ? 0 : -EINVAL;
}

static long efi_custom_rt_ioctl(struct file *file, unsigned int cmd,
				unsigned long arg)
{
	if (cmd == EFI_CUSTOM_RT_SERVICE) {
		return efi_custom_rt_entry(arg);
	}

	return -ENOTTY;
}

static int efi_custom_rt_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int efi_custom_rt_close(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations efi_custom_rt_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = efi_custom_rt_ioctl,
	.open = efi_custom_rt_open,
	.release = efi_custom_rt_close,
	.llseek = no_llseek,
};

static struct miscdevice efi_custom_rt_dev = { MISC_DYNAMIC_MINOR,
					       "efi_custom_rt",
					       &efi_custom_rt_fops };

static int __init efi_custom_rt_init(void)
{
	int ret = misc_register(&efi_custom_rt_dev);
	if (ret) {
		pr_err("efi_custom_rt: misc_register error, ret=%d\n", ret);
	}

	return ret;
}

static void __exit efi_custom_rt_exit(void)
{
	misc_deregister(&efi_custom_rt_dev);
}

module_init(efi_custom_rt_init);
module_exit(efi_custom_rt_exit);
