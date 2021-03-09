/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Author: huawei
 * Create: 2019-10-15
 */
#include "devdrv_manager_msg.h"

#define DEVDRV_MANAGER_MSG_GET_TASKID_NUM 15

int devdrv_manager_h2d_sync_get_devinfo(struct devdrv_info *dev_info)
{
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_device_info *device_info = NULL;
    u32 dev_id = dev_info->dev_id;
    void *no_trans_chan = NULL;
    u32 out_len;
    int ret;

    dev_manager_msg_info.header.dev_id = dev_id;
    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_DEVINFO;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;

    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get no trans chan failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }

    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);
    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc stream d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    device_info = (struct devdrv_device_info *)dev_manager_msg_info.payload;

    /* just get ip address from device at present */
    dev_info->ctrl_cpu_ip = device_info->ctrl_cpu_ip;

    devdrv_drv_debug("ctrl_cpu_ip = 0x%x. dev_id(%u)\n", dev_info->ctrl_cpu_ip, dev_id);

    return 0;
}

int devdrv_manager_msg_h2d_alloc_sync_stream(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    void *no_trans_chan = NULL;
    u32 stream_num;
    u32 out_len;
    u32 dev_id;
    int ret;
    u32 tsid;

    if ((cce_ctrl == NULL) || (cce_ctrl->dev_info == NULL) ||
        (cce_ctrl->tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("param is invalid.");
        return -ENODEV;
    }
    tsid = cce_ctrl->tsid;
    dev_info = cce_ctrl->dev_info;
    dev_id = dev_info->dev_id;

    id_data = devdrv_get_id_data(dev_info->pdata, tsid);

    devdrv_drv_debug("start to alloc stream from device side "
                     "dev_id(%u), stream_num = %u\n",
                     dev_id, cce_ctrl->stream_num);

    if (id_data->cur_stream_num >= DEVDRV_MAX_STREAM_ID) {
        devdrv_drv_info("cur_stream_num(%u), don't try to get stream from opposite side, dev_id(%u)\n",
                        id_data->cur_stream_num, dev_id);
        return -ENODEV;
    }

    dev_manager_msg_info.header.dev_id = dev_id;
    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_STREAM;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;
    dev_manager_msg_info.header.tsid = cce_ctrl->tsid;

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info.payload;
    dev_msg_resource_id->id_type = MSG_ID_TYPE_STREAM;
    dev_msg_resource_id->sync_type = MSG_CMD_TYPE_SYNC;
    dev_msg_resource_id->req_id_num = DEVDRV_MANAGER_MSG_GET_ID_NUM;
    dev_msg_resource_id->ret_id_num = 0;

    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get device(%u) no trans chan failed", dev_id);
        return -ENODEV;
    }

    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);

    devdrv_drv_debug("ret = %d, dev_manager_msg_info.header.result = %d, "
                     "dev_manager_msg_info.header.valid = 0x%x, ret_id_num = %u\n",
                     ret, dev_manager_msg_info.header.result, dev_manager_msg_info.header.valid,
                     dev_msg_resource_id->ret_id_num);

    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc stream d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    stream_num = cce_ctrl->stream_num;

    dev_info->drv_ops->add_stream_msg_chan(dev_info, cce_ctrl->tsid, dev_msg_resource_id);
    id_data->cur_stream_num += dev_msg_resource_id->ret_id_num;
    devdrv_drv_debug("get %u stream from host, stream num => before: %u, "
                     "after : %u, id_data->cur_stream_num = %u\n",
                     dev_msg_resource_id->ret_id_num, stream_num,
                     cce_ctrl->stream_num,
                     id_data->cur_stream_num);

    return 0;
}

int devdrv_manager_msg_h2d_alloc_sync_event(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    void *no_trans_chan = NULL;
    u32 out_len;
    u32 dev_id;
    int ret;
    u32 tsid;

    if ((cce_ctrl == NULL) || (cce_ctrl->dev_info == NULL) ||
        (cce_ctrl->tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("param is invalid.");
        return -ENODEV;
    }
    tsid = cce_ctrl->tsid;
    dev_info = cce_ctrl->dev_info;
    dev_id = dev_info->dev_id;

    dev_manager_msg_info.header.dev_id = dev_info->dev_id;

    devdrv_drv_debug("start to alloc evet from device side dev_id(%u)\n", dev_id);
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    if (id_data->cur_event_num >= DEVDRV_MAX_EVENT_ID) {
        devdrv_drv_info("cur_event_num(%u), don't try to get event from opposite side, dev_id(%u)\n",
                        id_data->cur_event_num, dev_id);
        return -ENODEV;
    }

    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_EVENT;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;
    dev_manager_msg_info.header.tsid = cce_ctrl->tsid;

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info.payload;
    dev_msg_resource_id->id_type = MSG_ID_TYPE_EVENT;
    dev_msg_resource_id->sync_type = MSG_CMD_TYPE_SYNC;
    dev_msg_resource_id->req_id_num = DEVDRV_MANAGER_MSG_GET_ID_NUM;
    dev_msg_resource_id->ret_id_num = 0;

    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get device(%u) no trans chan failed", dev_id);
        return -ENODEV;
    }

    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);
    devdrv_drv_debug("ret = %d, result = %d, valid = 0x%x, ret_id_num = %u\n", ret, dev_manager_msg_info.header.result,
                     dev_manager_msg_info.header.valid, dev_msg_resource_id->ret_id_num);

    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc stream d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    dev_info->drv_ops->add_event_msg_chan(dev_info, cce_ctrl->tsid, dev_msg_resource_id);
    id_data->cur_event_num += dev_msg_resource_id->ret_id_num;

    devdrv_drv_debug("finish to alloc evet from device side "
                     "dev_id = %u, id_data->cur_event_num = %u\n",
                     dev_id, id_data->cur_event_num);

    return 0;
}

int devdrv_manager_msg_h2d_alloc_sync_sq(struct devdrv_info *dev_info)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_id_data *id_data = NULL;
    u32 dev_id = dev_info->dev_id;
    void *no_trans_chan = NULL;
    u32 out_len;
    int ret;
    u32 tsid = 0;

    devdrv_drv_debug("start to alloc sq from device side dev_id(%u)\n", dev_id);

    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    if (id_data->cur_sq_num >= DEVDRV_MAX_SQ_NUM) {
        devdrv_drv_info("cur_sq_num(%u), don't try to get sq from opposite side, dev_id(%u)\n", id_data->cur_sq_num,
                        dev_id);
        return -ENODEV;
    }

    dev_manager_msg_info.header.dev_id = dev_info->dev_id;
    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_SQ;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info.payload;
    dev_msg_resource_id->id_type = MSG_ID_TYPE_SQ;
    dev_msg_resource_id->sync_type = MSG_CMD_TYPE_SYNC;
    dev_msg_resource_id->req_id_num = DEVDRV_MANAGER_MSG_GET_ID_NUM;
    dev_msg_resource_id->ret_id_num = 0;

    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get device(%u) no trans chan failed", dev_id);
        return -ENODEV;
    }

    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);
    devdrv_drv_debug("ret = %d, result = %d, valid = 0x%x, ret_id_num = %u\n", ret, dev_manager_msg_info.header.result,
                     dev_manager_msg_info.header.valid, dev_msg_resource_id->ret_id_num);

    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc stream d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    dev_info->drv_ops->add_sq_msg_chan(dev_info, dev_msg_resource_id);
    id_data->cur_sq_num += dev_msg_resource_id->ret_id_num;

    devdrv_drv_debug("finish to alloc sq from device side "
                     "dev_id(%u), id_data->cur_sq_num = %u\n",
                     dev_id, id_data->cur_sq_num);
    return 0;
}

int devdrv_manager_msg_h2d_alloc_sync_cq(struct devdrv_info *dev_info)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_id_data *id_data = NULL;
    u32 dev_id = dev_info->pci_dev_id;
    void *no_trans_chan = NULL;
    u32 out_len;
    int ret;
    u32 tsid = 0;

    devdrv_drv_debug("start to alloc cq from device side dev_id(%u)\n", dev_id);

    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    if (id_data->cur_cq_num >= DEVDRV_MAX_CQ_NUM) {
        devdrv_drv_info("cur_cq_num(%u), don't try to get cq from opposite side, dev_id(%u)\n", id_data->cur_cq_num,
                        dev_id);
        return -ENODEV;
    }

    dev_manager_msg_info.header.dev_id = dev_id;
    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_CQ;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info.payload;
    dev_msg_resource_id->id_type = MSG_ID_TYPE_CQ;
    dev_msg_resource_id->sync_type = MSG_CMD_TYPE_SYNC;
    dev_msg_resource_id->req_id_num = DEVDRV_MANAGER_MSG_GET_ID_NUM;
    dev_msg_resource_id->ret_id_num = 0;

    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get device(%u) no trans chan failed", dev_id);
        return -ENODEV;
    }

    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);

    devdrv_drv_debug("ret = %d, result = %d, valid = 0x%x, ret_id_num = %u\n", ret, dev_manager_msg_info.header.result,
                     dev_manager_msg_info.header.valid, dev_msg_resource_id->ret_id_num);

    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc stream d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    dev_info->drv_ops->add_cq_msg_chan(dev_info, dev_msg_resource_id);
    id_data->cur_cq_num += dev_msg_resource_id->ret_id_num;

    devdrv_drv_debug("finish to alloc cq from device side "
                     "dev_id(%u), id_data->cur_cq_num = %u\n",
                     dev_id, id_data->cur_cq_num);
    return 0;
}

int devdrv_manager_msg_h2d_alloc_sync_task(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    void *no_trans_chan = NULL;
    u32 out_len;
    u32 dev_id;
    int ret;

    if ((cce_ctrl == NULL) || (cce_ctrl->dev_info == NULL) ||
        (cce_ctrl->tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("param is invalid.");
        return -ENODEV;
    }
    dev_info = cce_ctrl->dev_info;
    dev_id = dev_info->dev_id;

    id_data = devdrv_get_id_data(dev_info->pdata, cce_ctrl->tsid);
    dev_manager_msg_info.header.dev_id = dev_info->dev_id;

    devdrv_drv_info("start to alloc task id from device side dev_id(%u).\n", dev_id);
    if (id_data->cur_task_num >= DEVDRV_MAX_TASK_ID) {
        devdrv_drv_info("cur_task_num(%u), don't try to get task-id from opposite side, dev_id(%u)\n",
                        id_data->cur_task_num, dev_id);
        return -ENODEV;
    }
    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_TASK;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;
    dev_manager_msg_info.header.tsid = cce_ctrl->tsid;
    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info.payload;
    dev_msg_resource_id->id_type = MSG_ID_TYPE_TASK;
    dev_msg_resource_id->sync_type = MSG_CMD_TYPE_SYNC;
    dev_msg_resource_id->req_id_num = DEVDRV_MANAGER_MSG_GET_TASKID_NUM;
    dev_msg_resource_id->ret_id_num = 0;
    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get device(%u) no trans chan failed", dev_id);
        return -ENODEV;
    }
    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);

    devdrv_drv_debug("ret = %d, result = %d, valid = 0x%x, ret_id_num = %d\n", ret, dev_manager_msg_info.header.result,
                     dev_manager_msg_info.header.valid, dev_msg_resource_id->ret_id_num);
    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc task d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    dev_info->drv_ops->add_task_msg_chan(dev_info, cce_ctrl->tsid, dev_msg_resource_id);
    id_data->cur_task_num += dev_msg_resource_id->ret_id_num;
    devdrv_drv_info("finish to alloc task from device side "
                    "dev_id(%u), dev_info->cur_task_num = %u\n",
                    dev_id, id_data->cur_task_num);
    return 0;
}
int devdrv_manager_msg_h2d_alloc_sync_model(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    void *no_trans_chan = NULL;
    u32 out_len;
    u32 dev_id;
    int ret;

    if ((cce_ctrl == NULL) || (cce_ctrl->dev_info == NULL) ||
        (cce_ctrl->tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("param is invalid.");
        return -ENODEV;
    }
    dev_info = cce_ctrl->dev_info;
    dev_id = dev_info->dev_id;

    dev_manager_msg_info.header.dev_id = dev_info->dev_id;
    devdrv_drv_debug("start to alloc model from device side dev_id(%u)\n", dev_id);

    id_data = devdrv_get_id_data(dev_info->pdata, cce_ctrl->tsid);
    if (id_data->cur_model_num >= DEVDRV_MAX_MODEL_ID) {
        devdrv_drv_info("cur_model_num(%u), don't try to get model from opposite side, dev_id(%u)\n",
                        id_data->cur_model_num, dev_id);
        return -ENODEV;
    }

    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_MODEL;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;
    dev_manager_msg_info.header.tsid = cce_ctrl->tsid;

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info.payload;
    dev_msg_resource_id->id_type = MSG_ID_TYPE_MODEL;
    dev_msg_resource_id->sync_type = MSG_CMD_TYPE_SYNC;
    dev_msg_resource_id->req_id_num = DEVDRV_MANAGER_MSG_GET_ID_NUM;
    dev_msg_resource_id->ret_id_num = 0;

    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get device(%u) no trans chan failed", dev_id);
        return -ENODEV;
    }

    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);
    devdrv_drv_debug("ret = %d, result = %d, valid = 0x%x, ret_id_num = %u\n", ret, dev_manager_msg_info.header.result,
                     dev_manager_msg_info.header.valid, dev_msg_resource_id->ret_id_num);

    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc model d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    dev_info->drv_ops->add_model_msg_chan(dev_info, cce_ctrl->tsid, dev_msg_resource_id);
    id_data->cur_model_num += dev_msg_resource_id->ret_id_num;

    devdrv_drv_debug("finish to alloc model from device side "
                     "dev_id = %d, dev_info->cur_model_num = %u\n",
                     dev_id, id_data->cur_model_num);

    return 0;
}

int devdrv_manager_msg_h2d_alloc_sync_notify(struct devdrv_cce_ctrl *cce_ctrl)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info dev_manager_msg_info = {{0}};
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    void *no_trans_chan = NULL;
    u32 out_len;
    u32 dev_id;
    int ret;

    if ((cce_ctrl == NULL) || (cce_ctrl->dev_info == NULL) ||
        (cce_ctrl->tsid >= DEVDRV_MAX_TS_NUM)) {
        devdrv_drv_err("param is invalid.");
        return -ENODEV;
    }
    dev_info = cce_ctrl->dev_info;
    dev_id = dev_info->dev_id;

    dev_manager_msg_info.header.dev_id = dev_info->dev_id;
    id_data = devdrv_get_id_data(dev_info->pdata, cce_ctrl->tsid);

    devdrv_drv_debug("start to alloc model from device side dev_id(%u)\n", dev_id);
    if (id_data->cur_notify_num >= DEVDRV_MAX_NOTIFY_ID) {
        devdrv_drv_info("cur_notify_num(%u), don't try to get notify from opposite side, dev_id(%u)\n",
                        id_data->cur_notify_num, dev_id);
        return -ENODEV;
    }

    dev_manager_msg_info.header.msg_id = DEVDRV_MANAGER_CHAN_H2D_SYNC_GET_NOTIFY;
    dev_manager_msg_info.header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    dev_manager_msg_info.header.result = DEVDRV_MANAGER_MSG_INVALID_RESULT;
    dev_manager_msg_info.header.tsid = cce_ctrl->tsid;

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info.payload;
    dev_msg_resource_id->id_type = MSG_ID_TYPE_NOTIFY;
    dev_msg_resource_id->sync_type = MSG_CMD_TYPE_SYNC;
    dev_msg_resource_id->req_id_num = DEVDRV_MANAGER_MSG_GET_ID_NUM;
    dev_msg_resource_id->ret_id_num = 0;

    no_trans_chan = devdrv_manager_get_no_trans_chan(dev_id);
    if (no_trans_chan == NULL) {
        devdrv_drv_err("get device(%u) no trans chan failed", dev_id);
        return -ENODEV;
    }

    ret = devdrv_sync_msg_send(no_trans_chan, &dev_manager_msg_info, sizeof(dev_manager_msg_info),
                               sizeof(dev_manager_msg_info), &out_len);
    devdrv_drv_debug("ret = %d, result = %d, valid = 0x%x, ret_id_num = %u\n", ret, dev_manager_msg_info.header.result,
                     dev_manager_msg_info.header.valid, dev_msg_resource_id->ret_id_num);

    if (ret || (dev_manager_msg_info.header.result != 0) ||
        (dev_manager_msg_info.header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC)) {
        devdrv_drv_err("device(%u) msg chan alloc notify d2h failed\n", dev_id);
        devdrv_drv_err("ret = %d, result = %d, valid = 0x%x\n", ret, dev_manager_msg_info.header.result,
                       dev_manager_msg_info.header.valid);
        return -EFAULT;
    }

    dev_info->drv_ops->add_notify_msg_chan(dev_info, cce_ctrl->tsid, dev_msg_resource_id);
    id_data->cur_notify_num += dev_msg_resource_id->ret_id_num;

    devdrv_drv_debug("finish to alloc notify from device side "
                     "dev_id(%u), dev_info->cur_notify_num = %u\n",
                     dev_id, id_data->cur_notify_num);

    return 0;
}

int devdrv_manager_d2h_sync_get_stream(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 stream_num;
    u32 tsid;
    int dev_id;
    int ret;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC) {
        devdrv_drv_err("invalid message from host %d.\n", dev_manager_msg_info->header.valid);
        return -EINVAL;
    }

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info->payload;
    if ((dev_msg_resource_id->id_type != MSG_ID_TYPE_STREAM) || (dev_msg_resource_id->req_id_num == 0) ||
        (dev_msg_resource_id->ret_id_num != 0)) {
        devdrv_drv_err("invalid request from host. dev_id(%u)\n", dev_id);
        devdrv_drv_err("id_type = %d, req_id_num = %u, ret_id_num = %u.\n", dev_msg_resource_id->id_type,
                       dev_msg_resource_id->req_id_num, dev_msg_resource_id->ret_id_num);
        return -EINVAL;
    }

    tsid = dev_manager_msg_info->header.tsid;
    if (tsid >= DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("tsid is invalid, tsid = %d \n", tsid);
        return -ENODEV;
    }

    dev_id = dev_manager_msg_info->header.dev_id;
    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    stream_num = dev_info->cce_ctrl[tsid]->stream_num;
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);

    ret = dev_info->drv_ops->alloc_stream_msg_chan(dev_info, tsid, dev_msg_resource_id);
    dev_manager_msg_info->header.result = ret;
    dev_manager_msg_info->header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    *ack_len = sizeof(*dev_manager_msg_info);

    if (id_data->cur_stream_num >= dev_msg_resource_id->ret_id_num) {
        id_data->cur_stream_num -= dev_msg_resource_id->ret_id_num;
    } else {
        devdrv_drv_err("invalid ret_id_num=%u, cur_stream_num=%u. dev_id(%u)\n", dev_msg_resource_id->ret_id_num,
                       id_data->cur_stream_num, dev_id);
        return -EINVAL;
    }

    devdrv_drv_debug("get %u stream from host, stream num => before: %u, after : %u "
                     "dev_info->cur_stream_num = %u. dev_id(%u)\n",
                     dev_msg_resource_id->ret_id_num, stream_num, dev_info->cce_ctrl[tsid]->stream_num,
                     id_data->cur_stream_num, dev_id);

    return 0;
}

int devdrv_manager_d2h_sync_get_event(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 event_num;
    u32 tsid;
    int dev_id;
    int ret;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;
    tsid = dev_manager_msg_info->header.tsid;
    if (tsid >= DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("tsid is invalid, tsid = %d \n", tsid);
        return -ENODEV;
    }

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC) {
        devdrv_drv_err("invalid message from host valid = 0x%x. dev_id(%u)\n", dev_manager_msg_info->header.valid,
                       dev_id);
        return -EINVAL;
    }

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info->payload;
    if ((dev_msg_resource_id->id_type != MSG_ID_TYPE_EVENT) || (dev_msg_resource_id->req_id_num == 0) ||
        (dev_msg_resource_id->ret_id_num != 0)) {
        devdrv_drv_err("invalid request from host. dev_id(%u)\n", dev_id);
        devdrv_drv_err("id_type = %d, req_id_num = %u, ret_id_num = %u.\n", dev_msg_resource_id->id_type,
                       dev_msg_resource_id->req_id_num, dev_msg_resource_id->ret_id_num);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    event_num = dev_info->cce_ctrl[tsid]->event_num;

    ret = dev_info->drv_ops->alloc_event_msg_chan(dev_info, tsid, dev_msg_resource_id);
    dev_manager_msg_info->header.result = ret;
    dev_manager_msg_info->header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    *ack_len = sizeof(*dev_manager_msg_info);

    if (id_data->cur_event_num >= dev_msg_resource_id->ret_id_num) {
        id_data->cur_event_num -= dev_msg_resource_id->ret_id_num;
    } else {
        devdrv_drv_err("invalid ret_id_num=%u, cur_event_num=%u. dev_id(%u)\n", dev_msg_resource_id->ret_id_num,
                       id_data->cur_event_num, dev_id);
        return -EINVAL;
    }

    devdrv_drv_debug("get %u event from host, "
                     "event num => before: %u, after : %u "
                     "dev_info->cur_event_num = %u. dev_id(%u)\n",
                     dev_msg_resource_id->ret_id_num, event_num, dev_info->cce_ctrl[tsid]->event_num,
                     id_data->cur_event_num, dev_id);

    return 0;
}

int devdrv_manager_d2h_sync_get_sq(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;
    int ret;
    u32 tsid = 0;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC) {
        devdrv_drv_err("invalid message from host valid = 0x%x. dev_id(%u)\n", dev_manager_msg_info->header.valid,
                       dev_id);
        return -EINVAL;
    }

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info->payload;
    if ((dev_msg_resource_id->id_type != MSG_ID_TYPE_SQ) || (dev_msg_resource_id->req_id_num == 0) ||
        (dev_msg_resource_id->ret_id_num != 0)) {
        devdrv_drv_err("invalid request from host. dev_id(%u)\n", dev_id);
        devdrv_drv_err("id_type = %d, req_id_num = %u, ret_id_num = %u.\n", dev_msg_resource_id->id_type,
                       dev_msg_resource_id->req_id_num, dev_msg_resource_id->ret_id_num);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);

    ret = dev_info->drv_ops->alloc_sq_msg_chan(dev_info, dev_msg_resource_id);
    dev_manager_msg_info->header.result = ret;
    dev_manager_msg_info->header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    *ack_len = sizeof(*dev_manager_msg_info);

    if (id_data->cur_sq_num >= dev_msg_resource_id->ret_id_num) {
        id_data->cur_sq_num -= dev_msg_resource_id->ret_id_num;
    } else {
        devdrv_drv_err("invalid ret_id_num=%u, cur_sq_num=%u. dev_id(%u)\n", dev_msg_resource_id->ret_id_num,
                       id_data->cur_sq_num, dev_id);
        return -EINVAL;
    }

    devdrv_drv_debug("finish to alloc event for host side, "
                     "dev_info->cur_sq_num = %u. dev_id(%u)\n",
                     id_data->cur_sq_num, dev_id);

    return 0;
}

int devdrv_manager_d2h_sync_get_cq(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 dev_id;
    int ret;
    u32 tsid = 0;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC) {
        devdrv_drv_err("invalid message from host valid = 0x%x. dev_id(%u)\n", dev_manager_msg_info->header.valid,
                       dev_id);
        return -EINVAL;
    }

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info->payload;
    if ((dev_msg_resource_id->id_type != MSG_ID_TYPE_CQ) || (dev_msg_resource_id->req_id_num == 0) ||
        (dev_msg_resource_id->ret_id_num != 0)) {
        devdrv_drv_err("invalid request from host. dev_id(%u)\n", dev_id);
        devdrv_drv_err("id_type = %d, req_id_num = %u, ret_id_num = %u.\n", dev_msg_resource_id->id_type,
                       dev_msg_resource_id->req_id_num, dev_msg_resource_id->ret_id_num);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);

    ret = dev_info->drv_ops->alloc_cq_msg_chan(dev_info, dev_msg_resource_id);
    dev_manager_msg_info->header.result = ret;
    dev_manager_msg_info->header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    *ack_len = sizeof(*dev_manager_msg_info);

    if (id_data->cur_cq_num >= dev_msg_resource_id->ret_id_num) {
        id_data->cur_cq_num -= dev_msg_resource_id->ret_id_num;
    } else {
        devdrv_drv_err("invalid ret_id_num=%u, cur_cq_num=%u. dev_id(%u)\n", dev_msg_resource_id->ret_id_num,
                       id_data->cur_cq_num, dev_id);
        return -EINVAL;
    }

    devdrv_drv_debug("finish to alloc event for host side, "
                     "dev_info->cur_cq_num = %u. dev_id(%u)\n",
                     id_data->cur_cq_num, dev_id);

    return 0;
}

int devdrv_manager_d2h_sync_get_task(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 task_id_num;
    u32 tsid;
    u32 dev_id;
    int ret;

    devdrv_drv_debug("start to alloc task id from host side\n");

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    tsid = dev_manager_msg_info->header.tsid;
    if (tsid >= DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("tsid is invalid, tsid(%u) \n", tsid);
        return -ENODEV;
    }

    dev_id = dev_manager_msg_info->header.dev_id;
    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC) {
        devdrv_drv_err("invalid message from host valid = 0x%x. dev_id(%u)\n", dev_manager_msg_info->header.valid,
                       dev_id);
        return -EINVAL;
    }

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info->payload;
    if ((dev_msg_resource_id->id_type != MSG_ID_TYPE_TASK) || (dev_msg_resource_id->req_id_num == 0) ||
        (dev_msg_resource_id->ret_id_num != 0)) {
        devdrv_drv_err("invalid request from host. dev_id(%u)\n", dev_id);
        devdrv_drv_err("id_type = %d, req_id_num = %u, ret_id_num = %u\n", dev_msg_resource_id->id_type,
                       dev_msg_resource_id->req_id_num, dev_msg_resource_id->ret_id_num);
        return -EINVAL;
    }
    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);

    task_id_num = dev_info->cce_ctrl[tsid]->task_id_num;

    ret = dev_info->drv_ops->alloc_task_msg_chan(dev_info, tsid, dev_msg_resource_id);
    dev_manager_msg_info->header.result = ret;
    dev_manager_msg_info->header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    *ack_len = sizeof(*dev_manager_msg_info);

    if (id_data->cur_task_num >= dev_msg_resource_id->ret_id_num) {
        id_data->cur_task_num -= dev_msg_resource_id->ret_id_num;
    } else {
        devdrv_drv_err("invalid ret_id_num=%u, cur_task_id=%u. dev_id(%u)\n", dev_msg_resource_id->ret_id_num,
                       id_data->cur_task_num, dev_id);
        return -EINVAL;
    }
    devdrv_drv_debug("get %u task from host, model id num => before: %u, after : %u "
                     "dev_info->cur_model_num = %u. dev_id(%u)\n",
                     dev_msg_resource_id->ret_id_num, task_id_num, dev_info->cce_ctrl[tsid]->task_id_num,
                     id_data->cur_task_num, dev_id);
    return 0;
}

int devdrv_manager_d2h_sync_get_model(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 model_id_num;
    u32 tsid;
    int dev_id;
    int ret;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;
    tsid = dev_manager_msg_info->header.tsid;
    if (tsid >= DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("tsid is invalid, tsid = %d \n", tsid);
        return -ENODEV;
    }

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC) {
        devdrv_drv_err("invalid message from host valid = 0x%x. dev_id(%u)\n", dev_manager_msg_info->header.valid,
                       dev_id);
        return -EINVAL;
    }

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info->payload;
    if ((dev_msg_resource_id->id_type != MSG_ID_TYPE_MODEL) || (dev_msg_resource_id->req_id_num == 0) ||
        (dev_msg_resource_id->ret_id_num != 0)) {
        devdrv_drv_err("invalid request from host. dev_id(%u)\n", dev_id);
        devdrv_drv_err("id_type = %d, req_id_num = %u, ret_id_num = %u.\n", dev_msg_resource_id->id_type,
                       dev_msg_resource_id->req_id_num, dev_msg_resource_id->ret_id_num);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    model_id_num = dev_info->cce_ctrl[tsid]->model_id_num;

    ret = dev_info->drv_ops->alloc_model_msg_chan(dev_info, tsid, dev_msg_resource_id);
    dev_manager_msg_info->header.result = ret;
    dev_manager_msg_info->header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    *ack_len = sizeof(*dev_manager_msg_info);

    if (id_data->cur_model_num >= dev_msg_resource_id->ret_id_num) {
        id_data->cur_model_num -= dev_msg_resource_id->ret_id_num;
    } else {
        devdrv_drv_err("invalid ret_id_num=%u, cur_model_num=%u. dev_id(%u)\n", dev_msg_resource_id->ret_id_num,
                       id_data->cur_model_num, dev_id);
        return -EINVAL;
    }

    devdrv_drv_debug("get %u model from host, "
                     "model id num => before: %u, after : %u "
                     "dev_info->cur_model_num = %u. dev_id(%u)\n",
                     dev_msg_resource_id->ret_id_num, model_id_num, dev_info->cce_ctrl[tsid]->model_id_num,
                     id_data->cur_model_num, dev_id);

    return 0;
}

int devdrv_manager_d2h_sync_get_notify(void *msg, u32 *ack_len)
{
    struct devdrv_manager_msg_resource_id *dev_msg_resource_id = NULL;
    struct devdrv_manager_msg_info *dev_manager_msg_info = NULL;
    struct devdrv_id_data *id_data = NULL;
    struct devdrv_info *dev_info = NULL;
    u32 notify_id_num;
    u32 tsid;
    int dev_id;
    int ret;

    dev_manager_msg_info = (struct devdrv_manager_msg_info *)msg;
    dev_id = dev_manager_msg_info->header.dev_id;
    tsid = dev_manager_msg_info->header.tsid;
    if (tsid >= DEVDRV_MAX_TS_NUM) {
        devdrv_drv_err("tsid is invalid, tsid = %d \n", tsid);
        return -ENODEV;
    }

    if (dev_manager_msg_info->header.valid != DEVDRV_MANAGER_MSG_D2H_MAGIC) {
        devdrv_drv_err("invalid message from host valid = 0x%x. dev_id(%u)\n", dev_manager_msg_info->header.valid,
                       dev_id);
        return -EINVAL;
    }

    dev_msg_resource_id = (struct devdrv_manager_msg_resource_id *)dev_manager_msg_info->payload;
    if ((dev_msg_resource_id->id_type != MSG_ID_TYPE_NOTIFY) || (dev_msg_resource_id->req_id_num == 0) ||
        (dev_msg_resource_id->ret_id_num != 0)) {
        devdrv_drv_err("invalid request from host. dev_id(%u)\n", dev_id);
        devdrv_drv_err("id_type = %d, req_id_num = %u, ret_id_num = %u.\n", dev_msg_resource_id->id_type,
                       dev_msg_resource_id->req_id_num, dev_msg_resource_id->ret_id_num);
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        devdrv_drv_err("get dev_info failed, dev_id(%u)\n", dev_id);
        return -ENODEV;
    }
    id_data = devdrv_get_id_data(dev_info->pdata, tsid);
    notify_id_num = dev_info->cce_ctrl[tsid]->notify_id_num;

    ret = dev_info->drv_ops->alloc_notify_msg_chan(dev_info, tsid, dev_msg_resource_id);
    dev_manager_msg_info->header.result = ret;
    dev_manager_msg_info->header.valid = DEVDRV_MANAGER_MSG_H2D_MAGIC;
    *ack_len = sizeof(*dev_manager_msg_info);

    if (id_data->cur_notify_num >= dev_msg_resource_id->ret_id_num) {
        id_data->cur_notify_num -= dev_msg_resource_id->ret_id_num;
    } else {
        devdrv_drv_err("invalid ret_id_num=%u, cur_notify_num=%u. dev_id(%u)\n", dev_msg_resource_id->ret_id_num,
                       id_data->cur_notify_num, dev_id);
        return -EINVAL;
    }

    devdrv_drv_debug("get %u notify from host, "
                     "notify_id_num => before: %u, after : %u "
                     "dev_info->cur_notify_num = %u. dev_id(%u)\n",
                     dev_msg_resource_id->ret_id_num, notify_id_num, dev_info->cce_ctrl[tsid]->notify_id_num,
                     id_data->cur_notify_num, dev_id);

    return 0;
}
