#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/list.h>

//PID of the queried process
int pid = -1;

//Recieve as argument
module_param(pid, int, 0000);

int mdinit(void)
{
  //Check if PID valid
  if(pid<1){
    printk(KERN_ALERT "No valid pid argument given, aborting.\n");
    return 0;
  }

  struct task_struct *tTask;
  struct list_head *sList;

  //Find task_struct from PID
  tTask = pid_task(find_vpid(pid), PIDTYPE_PID);
  printk("-------------\n");
  printk(" Parent name: %s (%d)\n", tTask->parent->comm, tTask->parent->pid);
  printk("-------------\n");
  printk("Process name: %s (%d)\n", tTask->comm, tTask->pid);
  printk("-------------\n");

  //Traverse all children
  list_for_each(sList, &tTask->children) {
      struct task_struct *task;

      task = list_entry(sList, struct task_struct, sibling);
      printk("  Child name: %s (%d)\n", task->comm, task->pid);
  }

  printk("-------------\n");
  return 0;

}

void mdexit(void) {
  printk(KERN_INFO "Removing Module \n");
}

module_init(mdinit);
module_exit(mdexit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("[Process Module]");
MODULE_AUTHOR("kyildirim");