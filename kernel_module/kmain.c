#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

#include <linux/sched.h>
#include <linux/cred.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shy Shalom");
MODULE_DESCRIPTION("Something.");
MODULE_VERSION("0.01");

static int __init lkm_example_init(void) {
    printk(KERN_INFO "Hello, World!\n");
    return 0; // succeeded init
}

static void __exit lkm_example_exit(void) {
    printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);

static int mytrigger = 0;
// simple version
// module_param(mytrigger, int, 0664);

// how much buffer do we have? PAGE_SIZE - 4k
// returns how many bytes written
int param_get_trigger_1(char* val, const struct kernel_param *kp) {
	
	val[0] = 'X';
	return 1;
}
int param_get_trigger_2(char* val, const struct kernel_param *kp) {
	
	strcpy(val, "hello!\n");
	return strlen(val);
}

int param_get_trigger_3(char* val, const struct kernel_param *kp) {
	printk("UID_a=%d\n", current->cred->euid);
	val[0] = 'A';
	val[1] = '\n';
	//*(int*)&(((struct cred*)current->cred)->uid) = 0;
	//*(int*)&(((struct cred*)current->cred)->gid) = 0;
	//*(int*)&(((struct cred*)current->cred)->euid) = 0;
	//*(int*)&(((struct cred*)current->cred)->egid) = 0;
	int* uptr = (int*)&current->cred->euid;
	*uptr = 0;
	uptr = (int*)&current->cred->uid;
	*uptr = 0;
	uptr = (int*)&current->cred->gid;
	*uptr = 0;
	uptr = (int*)&current->cred->egid;
	*uptr = 0;


	printk("UID_b=%d\n", current->cred->euid);
	printk("UID_c=%p %d %d\n", uptr, *uptr, current->pid);

	return 2;
}


static int param_set_trigger(const char *buffer, const struct kernel_param *kp) {
	return 0;
}

static const struct kernel_param_ops myops = {
	.set = param_set_trigger,
	.get = param_get_trigger_3,
};
module_param_cb(mytrigger, &myops, &mytrigger, 0664);






