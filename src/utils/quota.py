#!/usr/bin/env python3

import copy
import math
import collections
import logging

from cluster_resource import ClusterResource

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
def calculate_vc_gpu_counts(cluster_total, cluster_available,
                            cluster_unschedulable, vc_info, vc_usage):
    logger.debug(
        "cluster_total %s, cluster_available %s, cluster_unschedulable %s",
        cluster_total, cluster_available, cluster_unschedulable)
    logger.debug("vc_info %s, vc_usage %s", vc_info, vc_usage)
    vc_total = collections.defaultdict(lambda: {})
    vc_used = collections.defaultdict(lambda: {})
    vc_available = collections.defaultdict(lambda: {})
    vc_unschedulable = collections.defaultdict(lambda: {})

    vc_quota_sum = 0
    for vc_name, gpu_info in list(vc_info.items()):
        for gpu_type, total in list(gpu_info.items()):
            vc_total[vc_name][gpu_type] = total
            vc_quota_sum += total

    # key is vc_name, value is a map with key to be gpu_type and value to be real
    # quota
    ratio = collections.defaultdict(lambda: {})

    for vc_name, gpu_info in list(vc_info.items()):
        for gpu_type, quota in list(gpu_info.items()):
            if vc_quota_sum == 0:
                unschedulable = 0
            else:
                unschedulable = float(cluster_unschedulable.get(
                    gpu_type, 0)) * quota / vc_quota_sum
            vc_quota = quota - int(math.ceil(unschedulable))

            used = vc_usage.get(vc_name, {}).get(gpu_type, 0)

            ratio[vc_name][gpu_type] = max(vc_quota - used, 0)

    ratio_sum = collections.defaultdict(lambda : 0 )
    for vc_name, gpu_info in ratio.items():
        for gpu_type, cur_ratio in gpu_info.items():
            ratio_sum[gpu_type] += cur_ratio

    # subtract used and unshcduled GPU num,return the left
    logger.debug("ratio %s, ratio_sum %s", ratio, ratio_sum)

    # calculate vc_used,vc_available and vc_unschedulable of vc not having running job and having a gputype which has not running job
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
    # calculate vc_used,vc_available and vc_unschedulable of vc having a gputype which has not running job
    for vc_name, vc_usage_info in vc_usage.items():
        for gpu_type, vc_usage in vc_usage_info.items():
            if vc_name not in vc_info:
                logger.warning(
                    "ignore used gpu in %s, but vc quota do not have this vc, possible due to job template error",
                    vc_name)
                continue

            if gpu_type not in vc_info[vc_name]:
                logger.warning(
                    "ignore used gpu %s in %s, but vc quota do not have this gpu_type",
                    gpu_type, vc_name)
                continue

            cur_ratio = ratio[vc_name][gpu_type]
            quota = vc_info[vc_name][gpu_type]
            if ratio_sum[gpu_type] == 0:
                available = 0
            else:
                available = int(math.floor(min(float(cluster_available.get(gpu_type, 0)),ratio_sum[gpu_type]) * cur_ratio / ratio_sum[gpu_type]))
            vc_used[vc_name][gpu_type] = vc_usage
            vc_available[vc_name][gpu_type] = available
            vc_unschedulable[vc_name][gpu_type] = max(
                0, quota - vc_usage - available)

    logger.debug(
        "vc_total %s, vc_used %s, vc_available %s, vc_unschedulable %s",
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

def __get_valid_vc_usage(vc_info, vc_usage):
    valid_vc_usage = collections.defaultdict(lambda: ClusterResource())

    for vc_name, usage in vc_usage.items():
        if vc_name not in vc_info:
            logger.warning(
                "ignore used resource in %s. vc quota do not have this vc, "
                "possible due to job template error", vc_name)
        else:
            valid_vc_usage[vc_name] = usage

    return valid_vc_usage


def calculate_vc_resources(cluster_capacity, cluster_avail, cluster_reserved,
                           vc_info, vc_usage):
    """Calculates vc resources based on cluster resources and vc info.

    Qi' = Qi - R * (Qi / sum(Qi))
    Qi'' = max(Qi' - Ui, 0)
    Ai = A * (Qi'' / sum(Qi''))

    Where
    - R: cluster reserved
    - A: cluster avail
    - Qi: vc quota
    - Ui: vc used

    Args:
        cluster_capacity: Total resource capacity in the cluster
        cluster_avail: Currently available resource in the cluster
        cluster_reserved: Currently reserved resource in the cluster
        vc_info: VC quota information
        vc_usage: Currently used resource by VC in the cluster

    Returns:
        Qi: vc_total
        Ui: vc_used
        Ai: vc_avail
        max(Qi - Ui - Ai, 0): vc_unschedulable
    """
    logger.debug("cluster_capacity %s, cluster_avail %s, cluster_reserved %s",
                 cluster_capacity, cluster_avail, cluster_reserved)
    logger.debug("vc_info %s, vc_usage %s", vc_info, vc_usage)

    vc_usage = __get_valid_vc_usage(vc_info, vc_usage)

    vc_total = collections.defaultdict(lambda: ClusterResource())
    vc_used = collections.defaultdict(lambda: ClusterResource())
    vc_avail = collections.defaultdict(lambda: ClusterResource())
    vc_unschedulable = collections.defaultdict(lambda: ClusterResource())

    # vc total == assigned quota
    for vc_name, quota in vc_info.items():
        vc_total[vc_name] = copy.deepcopy(quota)

    quota_sum = ClusterResource()
    for vc_name, quota in vc_info.items():
        quota_sum += quota

    # ratios for calculating vc avail
    #   Qi' = Qi - R * (Qi / sum(Qi))
    #   Qi'' = max(Qi' - Ui, 0)
    ratios = collections.defaultdict(lambda: ClusterResource())
    for vc_name, quota in vc_info.items():
        reserved = (cluster_reserved * quota / quota_sum).ceil # over-reserve
        used = vc_usage.get(vc_name, ClusterResource())
        ratio = quota - reserved
        ratios[vc_name] = ratio - used

    ratio_sum = ClusterResource()
    for vc_name, ratio in ratios.items():
        ratio_sum += ratio

    logger.debug("ratios %s, ratio_sum %s", ratios, ratio_sum)

    # calculate avail and unschedulable
    # Ai = A * (Qi'' / sum(Qi''))
    # max(Qi - Ui - Ai, 0)
    for vc_name, ratio in ratios.items():
        used = copy.deepcopy(vc_usage.get(vc_name, ClusterResource()))
        avail = (cluster_avail * ratio / ratio_sum).floor # under-avail
        quota = vc_total.get(vc_name, ClusterResource())

        vc_used[vc_name] = used
        vc_avail[vc_name] = avail
        vc_unschedulable[vc_name] = quota - used - avail

    logger.debug("vc_total %s, vc_used %s, vc_avail %s, vc_unschedulable %s",
                 vc_total, vc_used, vc_avail, vc_unschedulable)
    return vc_total, vc_used, vc_avail, vc_unschedulable
