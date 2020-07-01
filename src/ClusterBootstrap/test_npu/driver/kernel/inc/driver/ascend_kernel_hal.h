/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei 
 * Create: 2019-10-15
 */
#ifndef __ASCEND_KERNEL_HAL_H__
#define __ASCEND_KERNEL_HAL_H__

int is_cloud_pcie_card(unsigned int dev_id);
/**
* @ingroup driver-stub
* @brief  interface for reporting the in-position information of the optical module 
* @param [in]  unsigned int qsfp_index Optical port index   
* @param [out] unsigned int *val presence information  
* @return   0 for success, others for fail
*/
int hclge_get_qsfp_present(unsigned int qsfp_index, unsigned int *val);
/**
* @ingroup driver-stub
* @brief  read cdr upgrade register value.
* @param [out]  unsigned int * for register value
* @return   0 for success, others for fail
*/

int drv_cpld_qsfp_present_query(unsigned int qsfp_index, unsigned int *val);

int drv_cpld_cdr_update_check(unsigned int *val);
/**
* @ingroup driver-stub
* @brief  obtain the MAC address of the user configuration area.   
* @param [in]  unsigned int dev_id Device ID 
* @param [in]  unsigned char *buf Buffer for storing information 
* @param [in]  unsigned int buf_size Size of the buffer for storing information     
* @param [out] unsigned int *info_size Data length of the returned MAC information   
* @return   0 for success, others for fail
*/
int devdrv_config_get_mac_info(unsigned int dev_id,
                               unsigned char *buf,
                               unsigned int buf_size,
                               unsigned int *info_size);
/**
* @ingroup driver-stub
* @brief   interface for obtaining the information about the user configuration area   
* @param [in]  unsigned int dev_id Device ID 
* @param [in]  const char *name: configuration item name  
* @param [in]  unsigned char *buf Buffer for storing information       
* @param [out] unsigned int *buf_size Obtain the information length   
* @return   0 for success, others for fail
*/

int devdrv_get_user_config(unsigned int dev_id, const char *name, unsigned char *buf, unsigned int *buf_size);
/**
* @ingroup driver-stub
* @brief   This interface is used to set the information about the user configuration area. 
*          Currently, this interface can be invoked only by the DMP process. In other cases, the permission fails to be returned   
* @param [in]  unsigned int dev_id Device ID 
* @param [in]  const char *name: configuration item name    
* @param [in]  unsigned char *buf Buffer for storing information        
* @param [out] unsigned int *buf_size Obtain the information length
*              Due to the storage space limit, when the configuration area information is set, 
*              The length of the setting information needs to be limited. 
*              The current length range is as follows: For cloud-related forms, 
*              the maximum value of buf_size is 0x8000, that is, 32 KB. 
*              For mini-related forms, the maximum value of buf_size is 0x800, that is, 2 KB. 
*              If the length is greater than the value of this parameter, a message is displayed, 
*              indicating that the setting fails.
* @return   0 for success, others for fail
*/
int devdrv_set_user_config(unsigned int dev_id, const char *name, unsigned char *buf, unsigned int buf_size);
/**
* @ingroup driver-stub
* @brief   This interface is used to clear the configuration items in the user configuration area. 
*          Currently, this interface can be invoked only by the DMP process. 
*          In other cases, a permission failure is returned.   
* @param [in]  unsigned int dev_id Device ID  
* @param [in]  const char *name: configuration item name   
* @param [in]  unsigned char *buf Buffer for storing information        
* @return   0 Success, others for fail
*/
int devdrv_clear_user_config(unsigned int devid, const char *name);

/**
* @ingroup driver-stub
* @brief   This interface is used to convert user virtual address to physical address.
*
* @param [in]  int pid: process id
* @param [in]  void *buf: user virtual address that need to be converted
* @param [in]  unsigned long long *phy_addr: a pointer point to the converted physical address
* @return   0 Success, others for fail
*/
int hal_kernel_uva_to_pa(int pid, void *uva, unsigned long long *phy_addr);

/**
* @ingroup driver-stub
* @brief   This interface is used to convert physical address to kernel virtual address.
*
* @param [in]  unsigned long long phy_addr: physical address that need to be converted
* @param [in]  void **kva: a pointer point to the converted kernel virtual address
* @return   0 Success, others for fail
*/
int hal_kernel_pa_to_kva(unsigned long long phy_addr, void **kva);

#endif
