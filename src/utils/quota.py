import math
import collections
import logging

logger = logging.getLogger(__name__)


# cluster_total, cluster_available and cluster_unschedulable are of same type,
# they are map with key to be gpu type string, and value to be count.
# vc_info and vc_usage are of same type, they are map with key to be vc name, and
# value to be a map of gpu type to count.

# Let Qi to be quota admin set to each VC, and R to be unschedulable GPU in cluster,
# Ui to be used GPU in cluster and A to be available GPU in cluster, so to compute
# what real quota and available GPUs is for each VC is computed using following
# formula:
# Qi' = Qi - R * (Qi / sum(Qi))
# Qi'' = max(Qi' - Ui, 0)
# Ai = A * (Qi'' / sum(Qi''))
# To display:
# * total gpu: Qi
# * used gpu: Ui
# * available gpu: Ai
# * unschedulable gpu: Qi - Ui - Ai
def calculate_vc_gpu_counts(cluster_total, cluster_available, cluster_unschedulable, vc_info, vc_usage):
    logger.debug("cluster_total %s, cluster_available %s, cluster_unschedulable %s",
            cluster_total, cluster_available, cluster_unschedulable)
    logger.debug("vc_info %s, vc_usage %s", vc_info, vc_usage)
    vc_total = collections.defaultdict(lambda : {})
    vc_used = collections.defaultdict(lambda : {})
    vc_available = collections.defaultdict(lambda : {})
    vc_unschedulable = collections.defaultdict(lambda : {})

    vc_quota_sum = 0
    for vc_name, gpu_info in vc_info.items():
        for gpu_type, total in gpu_info.items():
            vc_total[vc_name][gpu_type] = total
            vc_quota_sum += total

    # key is vc_name, value is a map with key to be gpu_type and value to be real
    # quota
    ratio = collections.defaultdict(lambda : {})

    for vc_name, gpu_info in vc_info.items():
        for gpu_type, quota in gpu_info.items():
            if vc_quota_sum == 0:
                unschedulable = 0
            else:
                unschedulable = float(cluster_unschedulable.get(gpu_type, 0)) * quota / vc_quota_sum
            vc_quota = quota - int(math.ceil(unschedulable))

            used = vc_usage.get(vc_name, {}).get(gpu_type, 0)

            ratio[vc_name][gpu_type] = max(vc_quota - used, 0)

    ratio_sum = collections.defaultdict(lambda : 0 )
    for vc_name, gpu_info in ratio.items():
        for gpu_type, cur_ratio in gpu_info.items():
            ratio_sum[gpu_type] += cur_ratio

    # 除去已经使用了的和不可调度的GPU数量，返回剩下的数量
    logger.debug("ratio %s, ratio_sum %s", ratio, ratio_sum)

    # 计算出还没有正在running的job的vc以及有running job但该gpu-type没有正在running的job的vc的vc_used，vc_available，vc_unschedulable
    for vc_name, gpu_info in ratio.items():
        for gpu_type, cur_ratio in gpu_info.items():
            if vc_usage.get(vc_name, {}).get(gpu_type, 0) == 0:
                # no job running in this vc.
                if ratio_sum[gpu_type] == 0:
                    available = 0
                else:
                    # calculate per vc available num according to the cluster total num on percent
                    available = int(math.floor(min(float(cluster_available.get(gpu_type, 0)),ratio_sum[gpu_type]) * cur_ratio / ratio_sum[gpu_type]))
                quota = vc_info[vc_name][gpu_type]

                vc_used[vc_name][gpu_type] = 0
                vc_available[vc_name][gpu_type] = available
                vc_unschedulable[vc_name][gpu_type] = max(0, quota - available)
    # 计算有正在running job的vc的gpu-type的vc_used，vc_available，vc_unschedulable
    for vc_name, vc_usage_info in vc_usage.items():
        for gpu_type, vc_usage in vc_usage_info.items():
            if vc_name not in vc_info:
                logger.warning("ignore used gpu in %s, but vc quota do not have this vc, possible due to job template error", vc_name)
                continue

            if gpu_type not in vc_info[vc_name]:
                logger.warning("ignore used gpu %s in %s, but vc quota do not have this gpu_type", gpu_type, vc_name)
                continue

            cur_ratio = ratio[vc_name][gpu_type]
            quota = vc_info[vc_name][gpu_type]
            if ratio_sum[gpu_type] == 0:
                available = 0
            else:
                available = int(math.floor(min(float(cluster_available.get(gpu_type, 0)),ratio_sum[gpu_type]) * cur_ratio / ratio_sum[gpu_type]))
            vc_used[vc_name][gpu_type] = vc_usage
            vc_available[vc_name][gpu_type] = available
            vc_unschedulable[vc_name][gpu_type] = max(0, quota - vc_usage - available)

    logger.debug("vc_total %s, vc_used %s, vc_available %s, vc_unschedulable %s",
            vc_total, vc_used, vc_available, vc_unschedulable)
    return vc_total, vc_used, vc_available, vc_unschedulable

if __name__ == '__main__':
    vc_usage = collections.defaultdict(lambda :
            collections.defaultdict(lambda : 0))
    vc_usage["platform"] = {u'Huawei_A910': 3,'nvidia': 2}
    vc_usage["hanxianjie"] = {u'Huawei_A910': 2,'nvidia': 4}
    vc_usage["hanxianjie2"] = {u'Huawei_A910': 1,'nvidia': 2}
    re = calculate_vc_gpu_counts({u'Huawei_A910': 15, u'nvidia': 20},{u'Huawei_A910': 15, u'nvidia': 20},{u'Huawei_A910': 0, u'nvidia': 0},
                                 {'platform': {u'Huawei_A910': 0, u'nvidia': 0},
                                  'hanxianjie': {u'Huawei_A910': 0, u'nvidia': 0},
                                  'hanxianjie2': {u'Huawei_A910': 0, u'nvidia': 0}},vc_usage)
    for i in re:
        print(i)
