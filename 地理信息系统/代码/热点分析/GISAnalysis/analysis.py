import json
import requests
import numpy as np
from sklearn.cluster import MeanShift, estimate_bandwidth


# 分析热力图
def analysis_hot_power(points):
    points = np.array(points)
    max_lon = points[:, 0].max()
    min_lon = points[:, 0].min()
    max_lat = points[:, 1].max()
    min_lat = points[:, 1].min()
    width = max_lon - min_lon
    height = max_lat - min_lat
    calc_num = 100
    counts = np.zeros((calc_num, calc_num), dtype=np.int32)
    for point in points:
        width_offset = int(calc_num * (point[0] - min_lon) / width)
        height_offset = int(calc_num * (point[1] - min_lat) / height)
        if width_offset == calc_num:
            width_offset = calc_num-1
        if height_offset == calc_num:
            height_offset = calc_num-1
        counts[width_offset, height_offset] += 1
    hot_list = list()
    width_atom = width / calc_num
    height_atom = height / calc_num
    for i in range(calc_num):
        for j in range(calc_num):
            hot_list.append({
                'lng': float(min_lon + (i+0.5) * width_atom),
                'lat': float(min_lat + (j+0.5) * height_atom),
                'count': int(counts[i, j])
            })
    return hot_list


# 均值漂移聚类
def mean_shift(points):
    points = np.array(points)
    bandwidth = estimate_bandwidth(points, quantile=0.2, n_samples=500)
    ms = MeanShift(bandwidth=bandwidth, bin_seeding=True)
    ms.fit(points)
    centers = ms.cluster_centers_
    labels = ms.labels_
    laundry = dict()
    for label in labels:
        if label not in laundry:
            laundry[label] = 1
        else:
            laundry[label] += 1
    label_list = list()
    for label in laundry:
        label_list.append([label, laundry[label], centers[label]])
    mark_list = list()
    for label in label_list:
        if label[1] > 80:
            mark_list.append(True)
        else:
            mark_list.append(False)
    last_centers = list()
    for index in range(len(label_list)):
        if mark_list[index]:
            last_centers.append(label_list[index][2])
    nc = list()
    for center in last_centers:
        nc.append([float(center[0]), float(center[1])])
    return nc


# 逆地理编码
def get_address(points):
    mark = list()
    for point in points:
        params = {
            'callback': 'renderReverse',
            'location': str(point[1])+','+str(point[0]),
            'output': 'json',
            'pois': 1,
            'ak': 'pvhIiG4SN1PtV0ENPh2GMLvefAHvtHf3'
        }
        res = requests.get('http://api.map.baidu.com/geocoder/v2/', params=params)
        info = json.loads(res.text[res.text.find('(') + 1:len(res.text) - 1])['result']
        names = list()
        for name in info['pois']:
            names.append(name['name'])
        mark.append([info['addressComponent']['street'], names])
    return mark
