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
    # use for judge case:  1/3   1/3   1/3 ,decide the unschedule 1 belong to which vc:
    ratio_unschedulable = collections.defaultdict(lambda : {})

    for vc_name, gpu_info in vc_info.items():
        for gpu_type, quota in gpu_info.items():
            if vc_quota_sum == 0:
                unschedulable = 0
            else:
                cluster_schedulable_num = cluster_total.get(gpu_type,0)-cluster_unschedulable.get(gpu_type, 0)
                cluster_unschedulable_num = max(vc_quota_sum - cluster_schedulable_num,0)
                unschedulable = float(cluster_unschedulable_num) * quota / vc_quota_sum
            ratio_unschedulable[vc_name][gpu_type] = unschedulable

    def caculate_unschedulable(ratio_unschedulable):
        unschedulables = []
        for vc_name, gpu_info in ratio_unschedulable.items():
            for gpu_type, unschedulable in gpu_info.items():
                unschedulables.append([unschedulable,vc_name,gpu_type])
                ratio_unschedulable[vc_name][gpu_type]=math.floor(unschedulable)
        unschedulables = sorted(unschedulables,key=lambda x:x[1])
        floats = collections.defaultdict(lambda : [])
        for one in unschedulables:
            floats[one[2]].append([math.modf(one[0])[0],one[1]])

        if all([all([i[0] for i in floats_vc_list])==0 for _,floats_vc_list in floats.items()]):
            return

        for gpu_type,floats_vc_list in floats.items():
            floats_list = [i[0] for i in floats_vc_list]
            max_index = floats_list.index(max(floats_list))
            max_vc_name = floats_vc_list[max_index][1]
            ratio_unschedulable[max_vc_name][gpu_type]+=1

    caculate_unschedulable(ratio_unschedulable)

    for vc_name, gpu_info in vc_info.items():
        for gpu_type, quota in gpu_info.items():
            vc_quota = quota - int(ratio_unschedulable[vc_name][gpu_type])
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

def caculate_n_th_max(li,n):
    assert len(li)>=n
    tmp = sorted(li,reverse=True)
    return tmp[n-1]

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
