/**
 * @file nic_uio_interface.h
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __NIC_UIO_INTERFACE_H__
#define __NIC_UIO_INTERFACE_H__

#include <stdint.h>

// Export Interfaces
int uio_open_device(const char *uio_device_name);
int uio_close_device(const char *uio_device_name);

#endif // __NIC_UIO_INTERFACE_H__
