#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/decompress/mm.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <asm/uaccess.h>

#define TRY "TRY"

MODULE_LICENSE("zzq_LICENSE");

static char *msg;
char *msg_Ptr;

module_param(msg, charp, 0644);

struct proc_dir_entry *pde;

int hello_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "open\n");
	msg_Ptr = msg;
	return 0;
}

ssize_t hello_read(struct file *filp, char __user *buff, size_t len, loff_t *data) {
	printk(KERN_INFO "READ\n");
	int bytes_read = 0;
	
	if (! *msg_Ptr) return 0;
	
	while (len && *msg_Ptr) {
		printk(KERN_INFO "read in while: %c\n", *msg_Ptr);
		copy_to_user(buff++, msg_Ptr++, 1);
		len--;
		bytes_read++;
	}

	return bytes_read;
}

ssize_t hello_write(struct file *filp, const char __user *buff, size_t len, loff_t *data) {
	printk(KERN_INFO "WRITE\n");
	
	if (copy_from_user(msg, buff, len))
		return -1;
	
	//int l = strlen(msg);
	//copy_from_user(msg+l-1, buff, len);
	return len;
}

static int __init hello_world_init(void) {
	printk(KERN_INFO "zzq's hello world init\n");
	//msg = malloc(sizeof(char)*100);
	//sprintf(msg, "init\n");
	
	struct file_operations *proc_fops = malloc(sizeof(struct file_operations));
	proc_fops->open = hello_open;
	proc_fops->read = hello_read;
	proc_fops->write = hello_write;
	//proc_fops->write = hello_write;
	pde = proc_create("hello", 00777, NULL, proc_fops);	
	
	return 0;
}


static void __exit hello_world_exit(void) {
	proc_remove(pde);
	free(pde);
	free(msg);
 	printk(KERN_INFO "zzq's hello world exit\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_AUTHOR("ZZQ");
