
#include <linux/module.h>
#include <linux/kernel.h>

int __init SecureCModuleInit(void)
{
    return 0;
}

void __exit SecureCModuleExit(void)
{
   return;
}

module_init(SecureCModuleInit);
module_exit(SecureCModuleExit);

MODULE_AUTHOR("Huawei Tech. Co., Ltd.");
MODULE_DESCRIPTION("secure c library for ko");
MODULE_ALIAS("host-kernel-seclib");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");

