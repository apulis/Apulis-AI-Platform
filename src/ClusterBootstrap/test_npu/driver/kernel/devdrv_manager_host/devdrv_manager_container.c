/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/nsproxy.h>
#include <linux/sched.h>
#include <linux/rbtree.h>
#include <linux/uaccess.h>
#include <linux/cred.h>
#include <linux/namei.h>

#include "devdrv_manager.h"
#include "devdrv_manager_common.h"
#include "devdrv_manager_container.h"
#include "devdrv_driver_adapt.h"

struct devdrv_container_table {
    struct list_head item_list_head;
    spinlock_t spinlock;
};

struct devdrv_container_pid_list {
    struct list_head pid_list_node;
    pid_t tgid;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
    u64 start_time;
    u64 real_start_time;
#else
    struct timespec start_time;
    struct timespec real_start_time;
#endif

    struct rb_node rb_node;
};

struct devdrv_container_item {
    struct list_head item_list_node;
    struct list_head pid_list_head;

    u32 id;
    u32 dev_num;
    u32 pid_num;
    u32 dev_id[DEVDRV_MAX_DAVINCI_NUM];
    u64 dev_list_map[DEVDRV_CONTIANER_NUM_OF_LONG];

    struct mnt_namespace *mnt_ns;
    struct pid_namespace *pid_ns;
    spinlock_t spinlock;

    struct rb_root rb_root;
};

enum docker_status_type {
    DOCKER_STATUS_IDLE = 0,
    DOCKER_STATUS_USED
};

#define D_PATH_BUF_SIZE 256

struct mnt_namespace *hiai_admin_mnt_ns[MAX_SUP_MNT_NS_NUM] = {NULL};
STATIC struct mnt_namespace *host_mnt_ns = NULL;
STATIC struct pid_namespace *host_pid_ns = NULL;
STATIC pid_t hiai_admin_pid[MAX_SUP_MNT_NS_NUM] = {0};
STATIC int docker_status[MAX_DOCKER_NUM] = {DOCKER_STATUS_IDLE};
STATIC DEFINE_MUTEX(assign_mutex);

STATIC inline struct devdrv_container_pid_list *devdrv_manager_container_get_first_pidlist(
    struct devdrv_container_item *item);
STATIC int devdrv_manager_container_assign_notify(void);

struct mnt_namespace *devdrv_manager_get_host_mnt_ns(void)
{
    return host_mnt_ns;
}

STATIC int devdrv_manager_container_is_admin(struct mnt_namespace *mnt_ns)
{
    int i;

    mutex_lock(&assign_mutex);
    for (i = 0; i < MAX_SUP_MNT_NS_NUM; i++) {
        if (mnt_ns == hiai_admin_mnt_ns[i]) {
            mutex_unlock(&assign_mutex);
            return IS;
        }
    }
    mutex_unlock(&assign_mutex);
    return 0;
}

/**
 * Check the process is running in  the physical machine or in container
 */
int devdrv_manager_container_is_host_system(struct mnt_namespace *mnt_ns)
{
    if (mnt_ns == host_mnt_ns || host_mnt_ns == NULL) {
        return IS;
    }

    return 0;
}

void devdrv_manager_container_print_list(struct devdrv_container_table *dev_container_table)
{
    struct devdrv_container_item *item = NULL;
    struct list_head *pos = NULL, *n = NULL;

    struct devdrv_container_pid_list *pidlist = NULL;
    struct list_head *pos_pid = NULL, *n_pid = NULL;

    devdrv_drv_info("=============device container table==============\n");
    devdrv_drv_info("host_mnt_ns 0x%pK,host_pid_ns 0x%pK\n", (void *)(uintptr_t)host_mnt_ns,
                    (void *)(uintptr_t)host_pid_ns);
    if (!list_empty_careful(&dev_container_table->item_list_head)) {
        list_for_each_safe(pos, n, &dev_container_table->item_list_head)
        {
            item = list_entry(pos, struct devdrv_container_item, item_list_node);
            devdrv_drv_info("item id(%u) dev_num(%u) dev_map(0x%pK) mnt_ns(0x%pK) pid_ns(0x%pK)\n",
                            item->id, item->dev_num,
                            (void *)(uintptr_t)item->dev_list_map[0], (void *)(uintptr_t)item->mnt_ns,
                            (void *)(uintptr_t)item->pid_ns);
            if (!list_empty_careful(&item->pid_list_head)) {
                list_for_each_safe(pos_pid, n_pid, &item->pid_list_head)
                {
                    pidlist = list_entry(pos_pid, struct devdrv_container_pid_list, pid_list_node);
                    devdrv_drv_info("-->pid tgid:(%u) \n", (u32)pidlist->tgid);
                }
            }
        }
    }
}

int devdrv_manager_container_get_devids_list(struct devdrv_manager_hccl_devinfo *hccl_devinfo)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_container_item *item = NULL;
    struct list_head *pos = NULL, *n = NULL;
    u32 array_num = 0;
    u32 i;

    if (dev_manager_info == NULL) {
        devdrv_drv_err("dev_manager_info = %pK\n", dev_manager_info);
        return -EINVAL;
    }

    dev_container_table = dev_manager_info->container_table;
    if (dev_container_table == NULL) {
        devdrv_drv_err("container table is NULL\n");
        return -ENOMEM;
    }

    spin_lock(&dev_container_table->spinlock);
    if (!list_empty_careful(&dev_container_table->item_list_head)) {
        list_for_each_safe(pos, n, &dev_container_table->item_list_head)
        {
            if (array_num != 0) {
                item = list_entry(pos, struct devdrv_container_item, item_list_node);
                hccl_devinfo->num_dev += item->dev_num;
                for (i = 0; i < item->dev_num; i++) {
                    hccl_devinfo->devids[i] = item->dev_id[i];
                }
            }
            array_num++;
        }
    }
    spin_unlock(&dev_container_table->spinlock);

    return 0;
}

STATIC int devdrv_manager_container_check_current(void)
{
    /* current->nsproxy is NULL when the release function is called */
    if (current == NULL || current->nsproxy == NULL || current->nsproxy->mnt_ns == NULL) {
        devdrv_drv_err("(current == NULL) is %d, (current->nsproxy == NULL) is %d\n",
            (current == NULL), ((current == NULL) ? (-EINVAL) : (current->nsproxy == NULL)));
        return -EINVAL;
    }

    return 0;
}

int devdrv_manager_container_get_docker_id(u32 *docker_id)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct mnt_namespace *current_mnt_ns = NULL;
    struct devdrv_container_item *item = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;

    if (docker_id == NULL) {
        devdrv_drv_err("docker_id is NULL\n");
        return -EINVAL;
    }

    *docker_id = MAX_DOCKER_NUM;

    if (dev_manager_info == NULL) {
        devdrv_drv_err("dev_manager_info is NULL\n");
        return -EINVAL;
    }

    dev_container_table = dev_manager_info->container_table;
    if (dev_container_table == NULL) {
        devdrv_drv_err("container table is NULL\n");
        return -EINVAL;
    }

    if (devdrv_manager_container_is_in_container()) {
        current_mnt_ns = current->nsproxy->mnt_ns;
    } else {
        /*
         * Operation not permitted: the env is not docker.
         * there is no need to print to prevent too many prints, just need to return a clear error code.
         */
        return -EPERM;
    }

    spin_lock(&dev_container_table->spinlock);
    if (!list_empty_careful(&dev_container_table->item_list_head)) {
        list_for_each_safe(pos, n, &dev_container_table->item_list_head) {
            item = list_entry(pos, struct devdrv_container_item, item_list_node);
            if (item->mnt_ns == current_mnt_ns) {
                *docker_id = item->id;
                break;
            }
        }
    }
    spin_unlock(&dev_container_table->spinlock);

    if (*docker_id >= MAX_DOCKER_NUM) {
        devdrv_drv_err("can't get docker_id or the docker_id is invalid: docker_id = %u\n", *docker_id);
        return -EINVAL;
    }

    return 0;
}
EXPORT_SYMBOL(devdrv_manager_container_get_docker_id);

STATIC struct devdrv_container_item *devdrv_manager_container_item_init(
    struct devdrv_container_table *dev_container_table,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_item *item = NULL;
    u32 i;

    item = kzalloc(sizeof(struct devdrv_container_item), GFP_ATOMIC);
    if (item == NULL) {
        devdrv_drv_err("item kzalloc failed\n");
        return NULL;
    }

    for (i = 0; i < DEVDRV_CONTIANER_NUM_OF_LONG; i++) {
        item->dev_list_map[i] = 0;
    }
    item->dev_num = 0;
    item->pid_num = 0;
    item->mnt_ns = para->mnt_ns;
    item->pid_ns = para->pid_ns;

    item->rb_root = RB_ROOT;
    INIT_LIST_HEAD(&item->pid_list_head);
    spin_lock_init(&item->spinlock);

    list_add_tail(&item->item_list_node, &dev_container_table->item_list_head);

    return item;
}

STATIC int devdrv_manager_container_rb_insert_pidlist(struct devdrv_container_item *item,
    struct devdrv_container_pid_list *pidlist)
{
    struct rb_node **node = &item->rb_root.rb_node;
    struct rb_node *parent = NULL;

    while (*node != NULL) {
        struct devdrv_container_pid_list *pidlist_tmp = NULL;
        parent = *node;
        pidlist_tmp = rb_entry(parent, struct devdrv_container_pid_list, rb_node);
        if (pidlist->tgid < pidlist_tmp->tgid) {
            node = &(*node)->rb_left;
        } else if (pidlist->tgid > pidlist_tmp->tgid) {
            node = &(*node)->rb_right;
        } else {
            devdrv_drv_err("pid already exists\n");
            return -EINVAL;
        }
    }
    rb_link_node(&pidlist->rb_node, parent, node);
    rb_insert_color(&pidlist->rb_node, &item->rb_root);
    return 0;
}

STATIC struct devdrv_container_pid_list *devdrv_manager_container_rb_find_pidlist(struct devdrv_container_item *item,
    pid_t tgid)
{
    struct rb_node *node = item->rb_root.rb_node;

    while (node != NULL) {
        struct devdrv_container_pid_list *pidlist_tmp = NULL;
        pidlist_tmp = rb_entry(node, struct devdrv_container_pid_list, rb_node);
        if (tgid < pidlist_tmp->tgid)
            node = node->rb_left;
        else if (tgid > pidlist_tmp->tgid)
            node = node->rb_right;
        else
            return pidlist_tmp;
    }
    return NULL;
}

STATIC inline void devdrv_manager_container_rb_delete_pidlist(struct devdrv_container_item *item,
                                                              struct devdrv_container_pid_list *pidlist)
{
    struct devdrv_container_pid_list *pidlist_tmp = NULL;

    pidlist_tmp = devdrv_manager_container_rb_find_pidlist(item, pidlist->tgid);
    if (pidlist_tmp == NULL) {
        devdrv_drv_err("find pidlist failed\n");
        return;
    }
    rb_erase(&pidlist->rb_node, &item->rb_root);
    RB_CLEAR_NODE(&pidlist->rb_node);
}

STATIC struct devdrv_container_pid_list *devdrv_manager_container_pidlist_init(struct devdrv_container_item *item,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_pid_list *pidlist = NULL;

    if (item->pid_num >= DEVDRV_MAX_PID_NUM) {
        devdrv_drv_err("pidlist reaches the maximum(%u).\n", item->pid_num);
        return NULL;
    }
    pidlist = kzalloc(sizeof(struct devdrv_container_pid_list), GFP_ATOMIC);
    if (pidlist == NULL) {
        devdrv_drv_err("pidlist kzalloc failed");
        return NULL;
    }
    pidlist->tgid = para->tgid;
    if (devdrv_manager_container_rb_insert_pidlist(item, pidlist)) {
        kfree(pidlist);
        return NULL;
    }
    pidlist->start_time = para->start_time;
    pidlist->real_start_time = para->real_start_time;
    list_add_tail(&pidlist->pid_list_node, &item->pid_list_head);
    item->pid_num++;

    return pidlist;
}

STATIC inline void devdrv_manager_container_pidlist_free(struct devdrv_container_item *item,
                                                         struct devdrv_container_pid_list *pidlist)
{
    devdrv_manager_container_rb_delete_pidlist(item, pidlist);
    list_del(&pidlist->pid_list_node);
    item->pid_num--;
    kfree(pidlist);
    pidlist = NULL;
}

STATIC int devdrv_manager_container_init_docker_id(struct devdrv_container_item *item)
{
    u32 i;

    for (i = 0; i < MAX_DOCKER_NUM; i++) {
        if (docker_status[i] == DOCKER_STATUS_IDLE) {
            item->id = i;
            docker_status[i] = DOCKER_STATUS_USED;
            break;
        }
    }

    if (i >= MAX_DOCKER_NUM) {
        devdrv_drv_err("no idle docker, i = %d\n", i);
        return -EBUSY;
    }
    return 0;
}

STATIC void devdrv_manager_container_uninit_docker_id(struct devdrv_container_item *item)
{
    docker_status[item->id] = DOCKER_STATUS_IDLE;
    item->id = MAX_DOCKER_NUM;
}

STATIC void devdrv_manager_container_item_free(struct devdrv_container_item *item)
{
    struct devdrv_container_pid_list *pidlist = NULL;
    struct list_head *pos = NULL, *n = NULL;
    u32 i;

    devdrv_manager_container_uninit_docker_id(item);

    if (!list_empty_careful(&item->pid_list_head)) {
        list_for_each_safe(pos, n, &item->pid_list_head)
        {
            pidlist = list_entry(pos, struct devdrv_container_pid_list, pid_list_node);
            devdrv_manager_container_pidlist_free(item, pidlist);
        }
    }

    for (i = 0; i < DEVDRV_CONTIANER_NUM_OF_LONG; i++) {
        item->dev_list_map[i] = 0;
    }
    item->dev_num = 0;
    item->pid_num = 0;
    item->mnt_ns = NULL;
    list_del(&item->item_list_node);
    kfree(item);
    item = NULL;
}

/**
 * return 1: container start with --pid = host  option£¨don't care first process, only care pid_list£©
 * return 0: container start without  --pid = host opion
 */
STATIC inline int devdrv_manager_container_is_host_pid(struct devdrv_container_item *item)
{
    /*evade find pid ns hault problem, fit debian*/
	return IS;
}

STATIC struct task_struct *devdrv_manager_container_get_first_proc(struct devdrv_container_item *item)
{
    struct pid_namespace *pid_ns = NULL;
    struct task_struct *tsk = NULL;
    struct pid *p = NULL;

    pid_ns = item->pid_ns;
    if (pid_ns == NULL) {
        return NULL;
    }
    rcu_read_lock();
    p = find_pid_ns(1, pid_ns);
    if (p == NULL) {
        rcu_read_unlock();
        return NULL;
    }
    tsk = pid_task(p, PIDTYPE_PID);
    if (tsk == NULL) {
        rcu_read_unlock();
        return NULL;
    }
    rcu_read_unlock();
    return tsk;
}

STATIC int devdrv_manager_container_check_first_proc_timestamp(struct devdrv_container_item *item)
{
    struct devdrv_container_pid_list *pidlist = NULL;
    struct task_struct *tsk = NULL;

    /* find first process in container */
    tsk = devdrv_manager_container_get_first_proc(item);
    if (tsk == NULL) {
        devdrv_drv_info("docker has exited.\n");
        return IS;
    }

    pidlist = devdrv_manager_container_get_first_pidlist(item);
    if (pidlist == NULL) {
        /* won't come here */
        devdrv_drv_err(" the item pid list is null! \n");
        return IS;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
    if (pidlist->start_time == tsk->start_time) {
        return 0;
    }

#else
    if (timespec_equal(&pidlist->start_time, &tsk->start_time)) {
        return 0;
    }
#endif
    devdrv_drv_debug(" the timestamp is not equal\n");
    return IS;
}

STATIC struct devdrv_container_pid_list *devdrv_manager_container_insert_first_proc(struct devdrv_container_item *item)
{
    struct task_struct *tsk = NULL;
    struct devdrv_container_pid_list *pidlist = NULL;

    tsk = devdrv_manager_container_get_first_proc(item);
    if (tsk == NULL) {
        devdrv_drv_err(" first proc is not exist\n");
        return NULL;
    }
    pidlist = kzalloc(sizeof(struct devdrv_container_pid_list), GFP_ATOMIC);
    if (pidlist == NULL) {
        devdrv_drv_err("pidlist kzalloc failed\n");
        return NULL;
    }

    pidlist->tgid = tsk->tgid;
    if (devdrv_manager_container_rb_insert_pidlist(item, pidlist)) {
        kfree(pidlist);
        devdrv_drv_err("insert pidlist failed\n");
        return NULL;
    }
    pidlist->start_time = tsk->start_time;
    pidlist->real_start_time = tsk->real_start_time;
    list_add_tail(&pidlist->pid_list_node, &item->pid_list_head);
    return pidlist;
}

STATIC void devdrv_manager_container_check_and_free_item(struct devdrv_container_item *item)
{
    if (devdrv_manager_container_is_host_pid(item)) {
        /* when container start with host pid option */
        if (list_empty_careful(&item->pid_list_head)) {
            devdrv_drv_info("item will be free :list map (0x%pK)\n", (void *)(uintptr_t)item->dev_list_map[0]);
            devdrv_manager_container_item_free(item);
        }
        return;
    }
    if (devdrv_manager_container_check_first_proc_timestamp(item)) {
        devdrv_drv_debug("item will be free :dev map (0x%pK) mnt_ns(0x%pK)\n",
            (void *)(uintptr_t)item->dev_list_map[0],
            (void *)(uintptr_t)item->mnt_ns);
        devdrv_manager_container_item_free(item);
        return;
    }

    return;
}

STATIC void devdrv_manager_container_check_and_free_table(struct devdrv_container_table *dev_container_table)
{
    struct devdrv_container_item *item = NULL;
    struct list_head *pos = NULL, *n = NULL;

    if (!list_empty_careful(&dev_container_table->item_list_head)) {
        list_for_each_safe(pos, n, &dev_container_table->item_list_head)
        {
            item = list_entry(pos, struct devdrv_container_item, item_list_node);
            devdrv_manager_container_check_and_free_item(item);
        }
    }
}

STATIC struct devdrv_container_item *devdrv_manager_container_find_item(
    struct devdrv_container_table *dev_container_table,
    struct mnt_namespace *mnt_ns)
{
    struct devdrv_container_item *item = NULL;
    struct list_head *pos = NULL, *n = NULL;

    if (!list_empty_careful(&dev_container_table->item_list_head)) {
        list_for_each_safe(pos, n, &dev_container_table->item_list_head)
        {
            item = list_entry(pos, struct devdrv_container_item, item_list_node);
            if (item->mnt_ns == mnt_ns) {
                return item;
            }
        }
    }

    return NULL;
}

STATIC int devdrv_manager_container_table_add_new_devids(struct devdrv_container_item *item,
    struct devdrv_container_alloc_para *para)
{
    u32 index, offset;
    u32 i;

    for (i = 0; i < para->num; i++) {
        if (para->npu_id[i] >= DEVDRV_MAX_DAVINCI_NUM) {
            devdrv_drv_err("wrong device id, i = %u, dev_id = %u\n", i, para->npu_id[i]);
            return -EINVAL;
        }
        index = para->npu_id[i] / DEVDRV_MAX_DAVINCI_NUM; /* 64 is sizeof unsigned long long */
        offset = para->npu_id[i] % DEVDRV_MAX_DAVINCI_NUM;
        item->dev_id[i] = para->npu_id[i];
        item->dev_list_map[index] |= (0x01ULL << offset);
    }
    item->dev_num = para->num;

    return 0;
}

STATIC int devdrv_manager_container_item_compare_devlist(struct devdrv_container_item *item,
    struct devdrv_container_alloc_para *para)
{
    u32 index, offset;
    u32 i;

    for (i = 0; i < para->num; i++) {
        if (para->npu_id[i] >= DEVDRV_MAX_DAVINCI_NUM) {
            devdrv_drv_err("wrong device id, i = %u, dev_id = %u\n", i, para->npu_id[i]);
            return -EINVAL;
        }
        index = para->npu_id[i] / DEVDRV_MAX_DAVINCI_NUM; /* 64 is sizeof unsigned long long */
        offset = para->npu_id[i] % DEVDRV_MAX_DAVINCI_NUM;
        if ((item->dev_list_map[index] & (0x01ULL << offset)) == 0) {
            devdrv_drv_err("compare device id failed, "
                           "i = %u, dev_id = %u\n",
                           i, para->npu_id[i]);
            devdrv_drv_err("item->dev_list_map[index] = 0x%pK, item->mnt_ns = %pK\n",
                           (void *)(uintptr_t)item->dev_list_map[index], item->mnt_ns);
            return -EFAULT;
        }
    }

    return 0;
}

STATIC int devdrv_manager_container_devid_check(struct devdrv_container_table *dev_container_table,
    struct devdrv_container_alloc_para *para)
{
    struct devdrv_container_item *item = NULL;
    struct list_head *pos = NULL;
    struct list_head *n = NULL;
    u32 i, j;

    if (list_empty_careful(&dev_container_table->item_list_head))
        return 0;

    list_for_each_safe(pos, n, &dev_container_table->item_list_head) {
        item = list_entry(pos, struct devdrv_container_item, item_list_node);
        if (item->mnt_ns == host_mnt_ns) {
            continue;
        }
        for (i = 0; i < item->dev_num; i++) {
            for (j = 0; j < para->num; j++) {
                if (item->dev_id[i] == para->npu_id[j]) {
                    devdrv_drv_err("device %u is in used\n", para->npu_id[j]);
                    return IS;
                }
            }
        }
    }

    return 0;
}

STATIC int devdrv_manager_container_table_add_new_item(struct devdrv_container_table *dev_container_table,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_pid_list *pidlist = NULL;
    struct devdrv_container_item *item = NULL;
    int ret;

    item = devdrv_manager_container_item_init(dev_container_table, para);
    if (item == NULL) {
        devdrv_drv_err("init item failed\n");
        return -ENOMEM;
    }

    ret = devdrv_manager_container_init_docker_id(item);
    if (ret) {
        devdrv_manager_container_item_free(item);
        devdrv_drv_err("init docker id failed, ret = %d\n", ret);
        return ret;
    }

    if (!devdrv_manager_container_is_host_pid(item)) {
        pidlist = devdrv_manager_container_insert_first_proc(item);
        if (pidlist == NULL) {
            devdrv_manager_container_item_free(item);
            devdrv_drv_err("init first proc fail\n");
            return -ENOMEM;
        }
    } else {
        pidlist = devdrv_manager_container_pidlist_init(item, para);
        if (pidlist == NULL) {
            devdrv_manager_container_item_free(item);
            devdrv_drv_err("init pid list failed\n");
            return -ENOMEM;
        }
    }
    return 0;
}

STATIC int devdrv_manager_container_start_time_is_equal(struct devdrv_container_pid_list *pidlist,
                                                        struct devdrv_manager_container_para *para)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)
    return (pidlist->start_time == para->start_time);
#else
    return timespec_equal(&para->start_time, &pidlist->start_time);
#endif
}

STATIC struct devdrv_container_pid_list *devdrv_manager_container_find_pidlist(struct devdrv_container_item *item,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_pid_list *pidlist = NULL;

    pidlist = devdrv_manager_container_rb_find_pidlist(item, para->tgid);
    if (pidlist == NULL) {
        devdrv_drv_info("find pidlist failed\n");
        return NULL;
    }
    if (devdrv_manager_container_start_time_is_equal(pidlist, para)) {
        return pidlist;
    }

    devdrv_drv_err("start_time is not equal\n");
    return NULL;
}

STATIC inline struct devdrv_container_pid_list *devdrv_manager_container_get_first_pidlist(
    struct devdrv_container_item *item)
{
    struct devdrv_container_pid_list *pidlist = NULL;

    if (!list_empty_careful(&item->pid_list_head))
        pidlist = list_first_entry(&item->pid_list_head, struct devdrv_container_pid_list, pid_list_node);

    return pidlist;
}

STATIC int devdrv_manager_container_table_add_exist_item(struct devdrv_container_item *item,
    struct devdrv_manager_container_para *para)
{
    u32 index, offset;
    u32 i;

    for (i = 0; i < para->dev_num; i++) {
        index = para->dev_id[i] / DEVDRV_MAX_DAVINCI_NUM;
        offset = para->dev_id[i] % DEVDRV_MAX_DAVINCI_NUM;
        if ((item->dev_list_map[index] & (0x01ULL << offset)) != 0) {
            /* hot reset will step to this normal branch */
            devdrv_drv_warn("dev_id = %u exist already\n", para->dev_id[i]);
            return 0;
        }
        item->dev_list_map[index] |= (0x01ULL << offset);
    }
    item->dev_num += para->dev_num;

    return 0;
}

STATIC int devdrv_manager_container_table_check_item(struct devdrv_container_item *item,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_pid_list *pidlist = NULL;

    if (!devdrv_manager_container_is_host_pid(item)) {
        return 0;
    }
    pidlist = devdrv_manager_container_find_pidlist(item, para);
    if (pidlist == NULL) {
        pidlist = devdrv_manager_container_pidlist_init(item, para);
        if (pidlist == NULL) {
            devdrv_drv_err("pidlist init failed\n");
            return -ENOMEM;
        }
    }

    return 0;
}

STATIC int devdrv_manager_container_table_add_devlist_to_item(struct devdrv_container_table *dev_container_table,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_item *item = NULL;

    item = devdrv_manager_container_find_item(dev_container_table, para->mnt_ns);
    if (item == NULL) {
        devdrv_drv_err("find item failed, para->mnt_ns = %pK\n", para->mnt_ns);
        return -EINVAL;
    }

    return devdrv_manager_container_table_add_exist_item(item, para);
}

STATIC int devdrv_manager_container_table_overlap_item(struct devdrv_container_table *dev_container_table,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_item *item = NULL;
    int ret;

    devdrv_manager_container_check_and_free_table(dev_container_table);
    item = devdrv_manager_container_find_item(dev_container_table, para->mnt_ns);
    if (item == NULL) {
        ret = devdrv_manager_container_table_add_new_item(dev_container_table, para);
    } else {
        ret = devdrv_manager_container_table_check_item(item, para);
    }
    return ret;
}

int devdrv_manager_container_table_devlist_add_ns(struct devdrv_manager_info *manager_info, u32 *physical_devlist,
                                                  u32 physical_dev_num)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_manager_container_para para = {0};
    int ret;
    u32 i;

    if (manager_info == NULL || physical_dev_num > DEVDRV_MAX_DAVINCI_NUM || physical_devlist == NULL) {
        devdrv_drv_err("manager_info = %pK, physical_devlist = %pK, physical_dev_num = %d\n", manager_info,
                       physical_devlist, physical_dev_num);
        return -EINVAL;
    }
    for (i = 0; i < physical_dev_num; i++) {
        para.dev_id[i] = physical_devlist[i];
    }
    para.dev_num = physical_dev_num;
    para.mnt_ns = current->nsproxy->mnt_ns;
    para.tgid = task_tgid_vnr(current);
    para.start_time = current->start_time;
    para.real_start_time = current->real_start_time;

    dev_container_table = manager_info->container_table;
    if (dev_container_table == NULL) {
        devdrv_drv_err("container table is NULL\n");
        return -ENOMEM;
    }
    spin_lock(&dev_container_table->spinlock);
    ret = devdrv_manager_container_table_add_devlist_to_item(dev_container_table, &para);
    if (ret) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("add devlist to item failed, ret = %d\n", ret);
        return -ENODEV;
    }
    spin_unlock(&dev_container_table->spinlock);

    devdrv_manager_container_print_list(dev_container_table);
    return 0;
}

STATIC int devdrv_manager_container_table_del_exist_item(struct devdrv_container_item *item,
    struct devdrv_manager_container_para *para)
{
    u32 index, offset;
    u32 i;

    for (i = 0; i < para->dev_num; i++) {
        index = para->dev_id[i] / DEVDRV_MAX_DAVINCI_NUM;
        offset = para->dev_id[i] % DEVDRV_MAX_DAVINCI_NUM;
        if ((item->dev_list_map[index] & (0x01ULL << offset)) == 0) {
            devdrv_drv_err("exist failed, dev_id = %u\n", para->dev_id[i]);
            return -ENODEV;
        }
        item->dev_list_map[index] &= ~(0x01ULL << offset);
    }
    item->dev_num -= para->dev_num;

    return 0;
}

STATIC int devdrv_manager_container_table_del_devlist_to_item(struct devdrv_container_table *dev_container_table,
    struct devdrv_manager_container_para *para)
{
    struct devdrv_container_item *item = NULL;

    item = devdrv_manager_container_find_item(dev_container_table, para->mnt_ns);
    if (item == NULL) {
        devdrv_drv_err("find item failed, para->mnt_ns = %pK\n", para->mnt_ns);
        return -EINVAL;
    }

    return devdrv_manager_container_table_del_exist_item(item, para);
}

int devdrv_manager_container_table_devlist_del_ns(struct devdrv_manager_info *manager_info, u32 *physical_devlist,
                                                  u32 physical_dev_num)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_manager_container_para para = {0};
    int ret;
    u32 i;

    if (manager_info == NULL || physical_dev_num > DEVDRV_MAX_DAVINCI_NUM || physical_devlist == NULL) {
        devdrv_drv_err("manager_info = %pK, physical_devlist = %pK, physical_dev_num = %d\n", manager_info,
                       physical_devlist, physical_dev_num);
        return -EINVAL;
    }
    for (i = 0; i < physical_dev_num; i++) {
        para.dev_id[i] = physical_devlist[i];
    }
    para.dev_num = physical_dev_num;
    para.mnt_ns = current->nsproxy->mnt_ns;
    para.tgid = task_tgid_vnr(current);
    para.start_time = current->start_time;
    para.real_start_time = current->real_start_time;

    dev_container_table = manager_info->container_table;
    if (dev_container_table == NULL) {
        devdrv_drv_err("container table is NULL\n");
        return -ENOMEM;
    }
    spin_lock(&dev_container_table->spinlock);
    ret = devdrv_manager_container_table_del_devlist_to_item(dev_container_table, &para);
    if (ret) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("add devlist to item failed, ret = %d\n", ret);
        return -ENODEV;
    }
    spin_unlock(&dev_container_table->spinlock);

    return 0;
}

void devdrv_manager_container_table_process_release(struct devdrv_manager_context *dev_manager_context)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_container_pid_list *pidlist = NULL;
    struct devdrv_manager_container_para para = {0};
    struct devdrv_container_item *item = NULL;

    if (dev_manager_context == NULL || dev_manager_info == NULL || dev_manager_info->container_table == NULL) {
        devdrv_drv_err("invalid parameter, dev_manager_context = %pK, manager_info = %pK\n", dev_manager_context,
                       dev_manager_info);
        return;
    }
    if (devdrv_manager_container_is_host_system(dev_manager_context->mnt_ns))
    {
    	return;
    }

    dev_container_table = dev_manager_info->container_table;
    para.tgid = dev_manager_context->tgid;
    para.mnt_ns = dev_manager_context->mnt_ns;
    para.start_time = dev_manager_context->start_time;
    para.real_start_time = dev_manager_context->real_start_time;

    spin_lock(&dev_container_table->spinlock);
    item = devdrv_manager_container_find_item(dev_container_table, para.mnt_ns);
    if (item == NULL) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("find item failed\n");
        return;
    }

    if (!devdrv_manager_container_is_host_pid(item)) {
        spin_unlock(&dev_container_table->spinlock);
        return;
    }

    pidlist = devdrv_manager_container_find_pidlist(item, &para);
    if (pidlist == NULL) {
        goto find_pidlist_failed;
    }

    devdrv_manager_container_pidlist_free(item, pidlist);

    if (list_empty_careful(&item->pid_list_head)) {
        devdrv_manager_container_item_free(item);
    }

    spin_unlock(&dev_container_table->spinlock);

    return;
find_pidlist_failed:
    pidlist = devdrv_manager_container_get_first_pidlist(item);
    spin_unlock(&dev_container_table->spinlock);
    if (pidlist == NULL) {
        devdrv_drv_err("find first pidlist failed\n");
        return;
    }

    if (pidlist->tgid != 1)
        devdrv_drv_err("something wrong, pidlist->tgid = %d\n", pidlist->tgid);

    return;
}

int devdrv_manager_container_table_init(struct devdrv_manager_info *manager_info, struct task_struct *tsk)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_manager_container_para para = {0};
    struct devdrv_container_pid_list *pidlist = NULL;
    struct devdrv_container_item *item = NULL;

    dev_container_table = kzalloc(sizeof(struct devdrv_container_table), GFP_KERNEL);
    if (dev_container_table == NULL) {
        devdrv_drv_err("container_table kzalloc failed\n");
        return -ENOMEM;
    }
    INIT_LIST_HEAD(&dev_container_table->item_list_head);
    spin_lock_init(&dev_container_table->spinlock);

    para.tgid = 1;
    para.start_time = tsk->start_time;
    para.real_start_time = tsk->real_start_time;
    para.mnt_ns = tsk->nsproxy->mnt_ns;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
    para.pid_ns = tsk->nsproxy->pid_ns_for_children;
#else
    para.pid_ns = tsk->nsproxy->pid_ns;
#endif

    /* create host item */
    item = devdrv_manager_container_item_init(dev_container_table, &para);
    if (item == NULL) {
        devdrv_drv_err("host item failed\n");
        kfree(dev_container_table);
        dev_container_table = NULL;
        return -ENODEV;
    }

    /* create host pid list */
    pidlist = devdrv_manager_container_pidlist_init(item, &para);
    if (pidlist == NULL) {
        devdrv_manager_container_item_free(item);
        kfree(dev_container_table);
        dev_container_table = NULL;
        devdrv_drv_err("pidlist init failed");
        return -ENOMEM;
    }

    manager_info->container_table = dev_container_table;
    host_mnt_ns = para.mnt_ns;
    host_pid_ns = para.pid_ns;

    devdrv_drv_info("mnt ns table inited, host_mnt_ns = 0x%pK host_pid_ns = %pK\n", (void *)(uintptr_t)host_mnt_ns,
                    (void *)(uintptr_t)host_pid_ns);

    return 0;
}

void devdrv_manager_container_table_exit(struct devdrv_manager_info *manager_info)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct list_head *pos_item = NULL, *n_item = NULL;
    struct list_head *pos_pid = NULL, *n_pid = NULL;
    struct devdrv_container_pid_list *pidlist = NULL;
    struct devdrv_container_item *item = NULL;

    if (manager_info == NULL || manager_info->container_table == NULL) {
        devdrv_drv_err("invalid parameter, dev_manager_info = %pK\n", manager_info);
        return;
    }
    dev_container_table = manager_info->container_table;

    spin_lock(&dev_container_table->spinlock);
    if (!list_empty_careful(&dev_container_table->item_list_head)) {
        list_for_each_safe(pos_item, n_item, &dev_container_table->item_list_head)
        {
            item = list_entry(pos_item, struct devdrv_container_item, item_list_node);
            if (!list_empty_careful(&item->pid_list_head)) {
                list_for_each_safe(pos_pid, n_pid, &item->pid_list_head)
                {
                    pidlist = list_entry(pos_pid, struct devdrv_container_pid_list, pid_list_node);
                    devdrv_manager_container_pidlist_free(item, pidlist);
                }
            }
            devdrv_manager_container_item_free(item);
        }
    }
    spin_unlock(&dev_container_table->spinlock);

    kfree(manager_info->container_table);
    manager_info->container_table = NULL;
}

int devdrv_manager_container_get_bare_pid(struct devdrv_manager_context *dev_manager_context,
                                          struct devdrv_container_para *cmd)
{
    pid_t pid;
    int ret;

    if (cmd->para.out == NULL) {
        devdrv_drv_err("invalid parameter\n");
        return -EINVAL;
    }
    pid = current->pid;
    ret = copy_to_user(cmd->para.out, &pid, sizeof(pid_t));
    if (ret) {
        devdrv_drv_err("pid = %d, copy to user failed: %d.\n", pid, ret);
        return -ENOMEM;
    }

    return 0;
}

int devdrv_manager_container_get_bare_tgid(struct devdrv_manager_context *dev_manager_context,
                                           struct devdrv_container_para *cmd)
{
    pid_t tgid;
    int ret;

    if (cmd->para.out == NULL) {
        devdrv_drv_err("invalid parameter\n");
        return -EINVAL;
    }
    tgid = current->tgid;
    ret = copy_to_user(cmd->para.out, &tgid, sizeof(pid_t));
    if (ret) {
        devdrv_drv_err("tgid = %d, copy to user failed: %d.\n", tgid, ret);
        return -ENOMEM;
    }

    return 0;
}

STATIC int devdrv_manager_container_init_devlist_ns(u32 *num, u32 *list, u32 list_size)
{
    char dev_name_buf[D_PATH_BUF_SIZE] = {0};
    struct path path_buf = {
        .mnt = NULL,
        .dentry = NULL,
    };
    u32 dev_num;
    u32 dev;
    int ret;

    dev_num = devdrv_manager_container_get_devnum(devdrv_manager_get_host_mnt_ns());
    if (dev_num > list_size) {
        devdrv_drv_err("total device num(%u) exceed\n", dev_num);
        return -EINVAL;
    }
    *num = 0;

    for (dev = 0; dev < DEVDRV_MAX_DAVINCI_NUM; dev++) {
        ret = snprintf_s(dev_name_buf, D_PATH_BUF_SIZE, D_PATH_BUF_SIZE - 1, "/dev/davinci%u", dev);
        if (ret < 0) {
            devdrv_drv_err("snprintf_s error,ret=%d\n", ret);
            return ret;
        }
        ret = kern_path(dev_name_buf, LOOKUP_FOLLOW, &path_buf);
        if (!ret) {
            path_put(&path_buf);
            list[*num] = dev;
            devdrv_drv_info("num(%u), dev(%u)\n", *num, dev);
            *num = *num + 1;
        }
    }

    if (*num > dev_num) {
        devdrv_drv_err("find dev_num error, dev_num = %u, max_num=%u\n", *num, dev_num);
        return -EINVAL;
    }

    return 0;
}

int devdrv_manager_container_table_overlap(struct devdrv_manager_context *dev_manager_context, u32 *docker_id)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_manager_container_para para = {0};
    int ret;

    if (devdrv_manager_container_is_host_system(dev_manager_context->mnt_ns)) {
        devdrv_drv_info("The current environment is in a host system.\n");
        *docker_id = MAX_DOCKER_NUM;
        return 0;
    }

    dev_container_table = dev_manager_info->container_table;
    if (dev_container_table == NULL) {
        devdrv_drv_err("container table is NULL\n");
        return -ENOMEM;
    }

    para.tgid = dev_manager_context->tgid;
    para.mnt_ns = dev_manager_context->mnt_ns;
    para.pid_ns = dev_manager_context->pid_ns;
    para.start_time = dev_manager_context->start_time;
    para.real_start_time = dev_manager_context->real_start_time;

    spin_lock(&dev_container_table->spinlock);
    ret = devdrv_manager_container_table_overlap_item(dev_container_table, &para);
    if (ret) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("add devlist to item failed, ret = %d\n", ret);
        return -ENODEV;
    }
    spin_unlock(&dev_container_table->spinlock);

    ret = devdrv_manager_container_get_docker_id(docker_id);
    if (ret) {
        devdrv_manager_container_table_process_release(dev_manager_context);
        devdrv_drv_err("container get docker_id failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    return 0;
}

STATIC u32
devdrv_manager_container_get_physical_id_by_logical_id(struct devdrv_container_table *dev_container_table,
    struct devdrv_manager_container_para *para, u32 logical_dev_id)
{
    u32 physical_dev_id = DEVDRV_MAX_DAVINCI_NUM;
    struct mnt_namespace *mnt_ns = para->mnt_ns;
    struct devdrv_container_item *item = NULL;
    u32 index, offset;
    u32 dev_index = 0;

    spin_lock(&dev_container_table->spinlock);
    item = devdrv_manager_container_find_item(dev_container_table, mnt_ns);
    if (item == NULL) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("find item failed, mnt_ns = %pK\n", mnt_ns);
        return DEVDRV_MAX_DAVINCI_NUM;
    }
    for (index = 0; index < DEVDRV_CONTIANER_NUM_OF_LONG; index++) {
        for (offset = 0; offset < DEVDRV_MAX_DAVINCI_NUM; offset++) {
            if ((item->dev_list_map[index] & (0x1ULL << offset)) != 0) {
                if (dev_index == logical_dev_id) {
                    physical_dev_id = index * DEVDRV_MAX_DAVINCI_NUM + offset;
                    spin_unlock(&dev_container_table->spinlock);
                    return physical_dev_id;
                }
                dev_index++;
            }
        }
    }
    spin_unlock(&dev_container_table->spinlock);

    return physical_dev_id;
}

STATIC int devdrv_manager_container_get_devlist_host(u32 *devlist, u32 devlist_len, u32 *devnum)
{
    struct devdrv_info *dev_info = NULL;
    u32 dev_num_tmp;
    u32 j = 0;
    u32 i;

    dev_num_tmp = devlist_len > DEVDRV_MAX_DAVINCI_NUM ? DEVDRV_MAX_DAVINCI_NUM : devlist_len;
    if (dev_num_tmp == 0) {
        devdrv_drv_err("wrong devlist length, devlist_len = %u\n", devlist_len);
        return -ENODEV;
    }

    for (i = 0; i < dev_num_tmp; i++) {
        dev_info = devdrv_manager_get_devdrv_info(i);
        if (dev_info == NULL) {
            continue;
        }
        devlist[j++] = dev_info->dev_id;
    }
    *devnum = j;
    return 0;
}

int devdrv_manager_container_get_devlist(u32 *devlist, u32 devlist_len, u32 *devnum, struct mnt_namespace *mnt_ns)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_container_item *item = NULL;
    u32 devnum_tmp;
    u32 i;

    if (devlist == NULL || devlist_len == 0 || devlist_len > DEVDRV_MAX_DAVINCI_NUM || devnum == NULL ||
        dev_manager_info == NULL || dev_manager_info->container_table == NULL) {
        devdrv_drv_err("invalid parameter, dev_manager_info = %pK, "
                       "devlist = %pK, devlist_len = %u, devnum = %pK, mnt_ns = 0x%pK\n",
                       dev_manager_info, devlist, devlist_len, devnum, (void *)(uintptr_t)mnt_ns);
        return -EINVAL;
    }

    if (devdrv_manager_container_is_admin(mnt_ns) == 1) {
        mnt_ns = host_mnt_ns;
    }

    if (host_mnt_ns == NULL) {
        return devdrv_manager_container_get_devlist_host(devlist, devlist_len, devnum);
    }

    dev_container_table = dev_manager_info->container_table;

    spin_lock(&dev_container_table->spinlock);
    item = devdrv_manager_container_find_item(dev_container_table, mnt_ns);
    if (item == NULL) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("item find failed, mnt_ns = 0x%pK\n", (void *)(uintptr_t)mnt_ns);
        return -ENODEV;
    }
    if (devlist_len < item->dev_num) {
        devdrv_drv_warn("weird devlist length, devlist_len = %u, item->dev_num = %u\n", devlist_len, item->dev_num);
    }
    devnum_tmp = devlist_len > item->dev_num ? item->dev_num : devlist_len;
    spin_unlock(&dev_container_table->spinlock);

    for (i = 0; i < devnum_tmp; i++) {
        devlist[i] = i;
    }

    *devnum = devnum_tmp;
    return 0;
}

STATIC int devdrv_manager_container_item_add_devlist(struct devdrv_container_alloc_para *para,
    struct devdrv_container_item *item, struct devdrv_container_table *dev_container_table)
{
    int ret;

    if (item->dev_num == 0) {
        if (devdrv_manager_container_devid_check(dev_container_table, para)) {
            devdrv_drv_err("device is in used\n");
            return ENOMEM;
        }
        if (devdrv_manager_container_table_add_new_devids(item, para)) {
            devdrv_drv_err("add device ids failed\n");
            return -EINVAL;
        }
    } else {
        ret = devdrv_manager_container_item_compare_devlist(item, para);
        if (ret) {
            devdrv_drv_err("compare device ids failed, ret = %d\n", ret);
            return -EINVAL;
        }
    }
    return 0;
}

STATIC int devdrv_manager_container_get_devlist_ns(struct devdrv_container_alloc_para *para,
    struct mnt_namespace *mnt_ns, u32 admin)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_container_item *item = NULL;
    int ret;

    dev_container_table = dev_manager_info->container_table;
    if (dev_container_table == NULL) {
        devdrv_drv_err("container table is NULL.\n");
        return -ENOMEM;
    }

    ret = devdrv_manager_container_init_devlist_ns(&para->num, para->npu_id, DEVDRV_MAX_DAVINCI_NUM);
    if (ret) {
        devdrv_drv_err("init devlist failed, ret(%d), dev_num(%u).\n", ret, para->num);
        return -ENODEV;
    }

    if (devdrv_manager_container_is_host_system(mnt_ns))
        return 0;

    /* Privileged container admin is 1 */
    if (admin == 1) {
        ret = devdrv_manager_container_assign_notify();
        return ret;
    }

    spin_lock(&dev_container_table->spinlock);
    item = devdrv_manager_container_find_item(dev_container_table, mnt_ns);
    if (item == NULL) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("item find failed, mnt_ns (%pK).\n", mnt_ns);
        return -ENODEV;
    }

    ret = devdrv_manager_container_item_add_devlist(para, item, dev_container_table);
    if (ret) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("item check_devlist failed, ret(%d).\n", ret);
        return -EINVAL;
    }
    devdrv_manager_container_print_list(dev_container_table);

    spin_unlock(&dev_container_table->spinlock);

    return 0;
}

int devdrv_manager_container_get_davinci_devlist(struct devdrv_manager_context *dev_manager_context,
    struct devdrv_container_para *cmd)
{
    struct devdrv_container_alloc_para para = {0};
    int ret;

    if (cmd == NULL || cmd->para.out == NULL) {
        devdrv_drv_err("invaild cmd(%pK).\n", cmd);
        return -EINVAL;
    }

    ret = devdrv_manager_container_get_devlist_ns(&para, dev_manager_context->mnt_ns, cmd->admin);
    if (ret) {
        devdrv_drv_err("get davinci devlist failed, ret(%d).\n", ret);
        return -EINVAL;
    }

    ret = copy_to_user_safe(cmd->para.out, &para, sizeof(struct devdrv_container_alloc_para));
    if (ret) {
        devdrv_drv_err("copy_to_user failed, ret = %d\n", ret);
        return -ENOMEM;
    }

    return 0;
}

u32 devdrv_manager_container_get_devnum_bare(void)
{
    u32 dev_num;
    unsigned long flags;

    spin_lock_irqsave(&dev_manager_info->spinlock, flags);
    dev_num = dev_manager_info->num_dev;
    spin_unlock_irqrestore(&dev_manager_info->spinlock, flags);

    return dev_num;
}

u32 devdrv_manager_container_get_devnum(struct mnt_namespace *mnt_ns)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_container_item *item = NULL;
    u32 dev_num;

    if (mnt_ns == NULL || dev_manager_info == NULL || dev_manager_info->container_table == NULL) {
        devdrv_drv_err("invalid parameter, mnt_ns = %pK, dev_manager_info = %pK\n", mnt_ns, dev_manager_info);
        return DEVDRV_MAX_DAVINCI_NUM + 1;
    }

    if (devdrv_manager_container_is_admin(mnt_ns) == 1) {
        mnt_ns = host_mnt_ns;
    }

    /* arm centos environment, host_mnt_ns is NULL */
    if (host_mnt_ns == NULL) {
        return devdrv_manager_container_get_devnum_bare();
    }

    dev_container_table = dev_manager_info->container_table;
    spin_lock(&dev_container_table->spinlock);
    item = devdrv_manager_container_find_item(dev_container_table, mnt_ns);
    if (item == NULL) {
        spin_unlock(&dev_container_table->spinlock);
        devdrv_drv_err("item find failed, if you can search <get devnum succ>on the previous print. "
            "it's ok. mnt_ns = 0x%pK.\n", (void *)(uintptr_t)mnt_ns);
        return DEVDRV_MAX_DAVINCI_NUM + 1;
    }
    dev_num = item->dev_num;
    spin_unlock(&dev_container_table->spinlock);

    if (dev_num == 0 || dev_num > DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_warn("weird device number, dev_num = %u\n", dev_num);
    }
    return dev_num;
}

STATIC int devdrv_manager_container_task_struct_check(struct task_struct *tsk)
{
    if (tsk == NULL) {
        devdrv_drv_err("tsk = 0x%pK\n", (void *)(uintptr_t)tsk);
        return -EINVAL;
    }
    if (tsk->nsproxy == NULL) {
        devdrv_drv_err("tsk->nsproxy = 0x%pK\n", (void *)(uintptr_t)tsk->nsproxy);
        return -ENODEV;
    }
    if (tsk->nsproxy->mnt_ns == NULL) {
        devdrv_drv_err("tsk->nsproxy->mnt_ns = 0x%pK\n", (void *)(uintptr_t)tsk->nsproxy->mnt_ns);
        return -EFAULT;
    }
    return 0;
}

int devdrv_manager_container_logical_id_to_physical_id(u32 logical_dev_id, u32 *physical_dev_id)
{
    struct devdrv_container_table *dev_container_table = NULL;
    struct devdrv_manager_container_para para = {0};
    u32 num_dev;
    int ret;

    num_dev = devdrv_manager_container_get_devnum(current->nsproxy->mnt_ns);

    if (physical_dev_id == NULL || logical_dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("invalid device num, physical_dev_id = 0x%pK, logical_dev_id = %u,num_dev=%u\n",
                       (void *)(uintptr_t)physical_dev_id, logical_dev_id, num_dev);
        return -EINVAL;
    }

    if (logical_dev_id >= num_dev) {
        return -EAGAIN;
    }

    ret = devdrv_manager_container_task_struct_check(current);
    if (ret) {
        devdrv_drv_err("invalid current pointer, ret = %d\n", ret);
        return -ENODEV;
    }

    if (dev_manager_info == NULL || dev_manager_info->container_table == NULL) {
        devdrv_drv_err("dev manager info check failed, dev_manager_info = %pK\n", dev_manager_info);
        return -EFAULT;
    }
    dev_container_table = dev_manager_info->container_table;
    para.mnt_ns = current->nsproxy->mnt_ns;
    if (devdrv_manager_container_is_admin(para.mnt_ns) == 1) {
        para.mnt_ns = host_mnt_ns;
    }
    if (host_mnt_ns == NULL) {
        *physical_dev_id = logical_dev_id;
        return 0;
    }
    *physical_dev_id = devdrv_manager_container_get_physical_id_by_logical_id(dev_container_table, &para,
                                                                              logical_dev_id);
    if (*physical_dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        devdrv_drv_err("get physical dev id failed, logical_id = %u, physic_id = %u\n", logical_dev_id,
                       *physical_dev_id);
        return -ENODEV;
    }
    return 0;
}
EXPORT_SYMBOL(devdrv_manager_container_logical_id_to_physical_id);

STATIC int devdrv_container_assign_admin_start(void)
{
    int i;

    /*
     * already lock assign_mutex in devdrv_manager_container_assign_notify
     * do not lock assign_mutex here.
     */
    mutex_lock(&assign_mutex);
    for (i = 0; i < MAX_SUP_MNT_NS_NUM; i++) {
        if ((hiai_admin_mnt_ns[i] == NULL) && (hiai_admin_pid[i] == 0)) {
            break;
        }
        if ((hiai_admin_mnt_ns[i] == current->nsproxy->mnt_ns) && (hiai_admin_pid[i] != current->tgid)) {
            devdrv_drv_err("already started admin process(%d), do not running again, "
                           "current mnt: %pK, pid(in hot env): %d.\n",
                           i, current->nsproxy->mnt_ns, current->tgid);
            mutex_unlock(&assign_mutex);
            return -EINVAL;
        }
    }

    if (i == MAX_SUP_MNT_NS_NUM) {
        devdrv_drv_err("already started (%d) admin process, can not running more, "
                       "current mnt: %pK, pid(in hot env): %d.\n",
                       MAX_SUP_MNT_NS_NUM, current->nsproxy->mnt_ns, current->tgid);
        mutex_unlock(&assign_mutex);
        return -EINVAL;
    }

    hiai_admin_mnt_ns[i] = current->nsproxy->mnt_ns;
    hiai_admin_pid[i] = current->tgid;

    devdrv_drv_info("docker admin process start, pid(in host env): %d, mnt: %pK.\n", hiai_admin_pid[i],
                    hiai_admin_mnt_ns[i]);

    mutex_unlock(&assign_mutex);
    return 0;
}

void devdrv_container_assign_admin_end(void)
{
    int i;
    mutex_lock(&assign_mutex);
    for (i = 0; i < MAX_SUP_MNT_NS_NUM; i++) {
        if (hiai_admin_pid[i] == current->tgid) {
            devdrv_drv_info("hiai plugin exits, mnt ns: %pK, pid(in host env): %d.\n", hiai_admin_mnt_ns[i],
                            hiai_admin_pid[i]);
            hiai_admin_mnt_ns[i] = NULL;
            hiai_admin_pid[i] = 0;
            break;
        }
    }
    mutex_unlock(&assign_mutex);
}

int devdrv_manager_container_is_in_container(void)
{
    struct mnt_namespace *current_mnt = NULL;
    struct mnt_namespace *host_mnt = NULL;
    int is_in;

    if (devdrv_manager_container_check_current()) {
        return 0;
    }

    current_mnt = current->nsproxy->mnt_ns;
    host_mnt = devdrv_manager_get_host_mnt_ns();
    if (host_mnt == NULL) {
        is_in = 0;
    } else if ((!devdrv_manager_container_is_admin(current_mnt)) && (current_mnt != host_mnt)) {
        is_in = 1;
    } else {
        is_in = 0;
    }

    return is_in;
}
EXPORT_SYMBOL(devdrv_manager_container_is_in_container);

STATIC int devdrv_manager_container_assign_notify(void)
{
    int ret;
    kuid_t euid;
    u32 root;

    euid = current->cred->euid;
    root = (u32)(euid.val);
    if (root != 0) {
        devdrv_drv_err("check user authority fail.\n");
        return -EPERM;
    }

    /* judge docker authority */
    if (!capable(CAP_SYS_ADMIN)) {
        devdrv_drv_err("check docker authority fail.\n");
        return -EPERM;
    }

    ret = devdrv_container_assign_admin_start();
    if (ret) {
        devdrv_drv_err("devdrv_container_assign_hiai_plugin_start "
                       "return error: %d.\n",
                       ret);
        return ret;
    }

    return 0;
}

STATIC int (*CONST devdrv_manager_container_process_handler[DEVDRV_CONTAINER_MAX_CMD])(
    struct devdrv_manager_context *dev_manager_context, struct devdrv_container_para *cmd) = {
    [DEVDRV_CONTAINER_GET_DAVINCI_DEVLIST] = devdrv_manager_container_get_davinci_devlist,
    [DEVDRV_CONTAINER_GET_BARE_PID] = devdrv_manager_container_get_bare_pid,
    [DEVDRV_CONTAINER_GET_BARE_TGID] = devdrv_manager_container_get_bare_tgid,
};

int devdrv_manager_container_process(struct file *filep, unsigned long arg)
{
    struct devdrv_manager_context *dev_manager_context = NULL;
    struct devdrv_container_para container_cmd;
    int ret;

    if (!arg || (filep == NULL) || (filep->private_data == NULL)) {
        devdrv_drv_err("arg = %lu, filep = %pK\n", arg, filep);
        return -EINVAL;
    }
    dev_manager_context = filep->private_data;

    ret = copy_from_user_safe(&container_cmd, (void *)((uintptr_t)arg), sizeof(struct devdrv_container_para));
    if (ret) {
        devdrv_drv_err("copy_from_user return error: %d.\n", ret);
        return ret;
    }

    if (container_cmd.para.cmd >= DEVDRV_CONTAINER_MAX_CMD) {
        devdrv_drv_err("invalid input container process cmd: %d.\n", container_cmd.para.cmd);
        return -EINVAL;
    }

    if (devdrv_manager_container_process_handler[container_cmd.para.cmd] == NULL) {
        devdrv_drv_err("not supported cmd: %d.\n", container_cmd.para.cmd);
        return -EINVAL;
    }

    ret = devdrv_manager_container_process_handler[container_cmd.para.cmd](dev_manager_context, &container_cmd);

    return ret;
}
