#include <stddef.h>
#include "dsmi_common_interface.h"
#include <stdlib.h>
#include <unistd.h>

int main()
{
int ret = 0;
int device_id = 0; 
int port_type = 1;
int port_id = 0;
ip_addr_t  ip_address = {0};
ip_addr_t ip_mask_address={0};

char ipadd[64]= {0};
int num = 8;
for (int i = 0; i < num; i++ ) {

 ret = dsmi_enable_container_service();
 if(ret != 0) { 
 //todo:记录日志 
   printf("failed. ret: %d\n", ret);
   }
   else {
   printf("enable container succ. \n");
   printf("test\n");
   }

  while(true) {
	system("npu-smi info");
        sleep(2);
  }
}

return 0;
}
