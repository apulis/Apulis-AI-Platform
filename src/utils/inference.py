import requests
import base64
import numpy as np
from PIL import Image
import os,io
import json
from io import BytesIO
import visualization_utils
import re
import dns.resolver
import logging

def object_classifier_infer(inference_url,image,signature_name):
    res = requests.post(inference_url, json={
        "signature_name": signature_name,
        "instances": [
            {"image_bytes": {"b64": base64.b64encode(image).decode("utf-8")}
             }
        ]})
    ret = res.json().get("predictions")
    return ret

def object_detaction_infer(inference_url,imageFile,signature_name):
    import serve_utils
    import cv2
    img_size = 608
    original_image = cv2.imdecode(np.frombuffer(imageFile, np.uint8), -1)
    image_data = serve_utils.image_preporcess(np.copy(original_image), [img_size, img_size])
    image_data_yolo_list = image_data[np.newaxis, :].tolist()
    headers = {"Content-type": "application/json"}
    inference_url = "http://127.0.0.1/endpoints/"+inference_url.split("/endpoints/")[1]
    r = requests.post(inference_url,headers=headers,
                      data=json.dumps({"signature_name": signature_name,"instances":image_data_yolo_list}))
    r = r.json()
    output = np.array(r['predictions'])
    output = np.reshape(output, (-1, 85))
    original_image_size = original_image.shape[:2]
    bboxes = serve_utils.postprocess_boxes(output, original_image_size, img_size, 0.3)
    bboxes = serve_utils.nms(bboxes, 0.45, method='nms')
    image = serve_utils.draw_bbox(original_image, bboxes)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    image = Image.fromarray(image)
    imgByteArr = io.BytesIO()
    image.save(imgByteArr,format='JPEG')
    imgByteArr = imgByteArr.getvalue()
    return imgByteArr

def read_class_names2(class_file_name):
    '''loads class name from a file'''
    names = {}
    with open(class_file_name, 'r') as data:
        li = json.loads(data.read())
        for one in li:
            names[one["id"]] = {"name":one["display_name"],"id":one['id']}
    return names

def query_service_domain(domain):
    my_resolver = dns.resolver.Resolver()
    my_resolver.nameservers = ['10.96.0.10']
    answer = my_resolver.query(domain)
    for i in answer:
        return i.to_text()



def object_detaction_infer2(inference_url,imageFile,signature_name,jobParams):
    image = Image.open(BytesIO(imageFile)).convert("RGB")
    (im_width, im_height) = image.size
    image_data = np.array(image.getdata()).reshape((im_height, im_width, 3)).astype(np.uint8)
    image_data_yolo_list = image_data[np.newaxis, :].tolist()
    headers = {"Content-type": "application/json","Host":"{}-predictor-default.kfserving-pod.example.com".format(inference_url.split("/endpoints/v3/v1/models/")[1].split(":")[0])}
    service_ip = query_service_domain('istio-ingressgateway.istio-system.svc.cluster.local')
    inference_url = "http://{}/v1/models/".format(service_ip)+inference_url.split("/endpoints/v3/v1/models/")[1]
    r = requests.post(inference_url,headers=headers,
                      data=json.dumps({"signature_name": "serving_default","instances":image_data_yolo_list}),
                      )
    if r.status_code!=200:
        logging.error(r.content)
    r = r.json()
    output_dict = r['predictions'][0]
    class_name_path = re.sub("^/home", "/dlwsdata/work", os.path.join(jobParams["model_base_path"], "class_names.json"))
    if not os.path.exists(class_name_path):
        class_name_path = "/DLWorkspace/src/utils/coco.names"
    category_index = read_class_names2(class_name_path)
    if "logits" in output_dict:
        length = len(output_dict['logits'])
        return {"data":[[v["name"],output_dict["logits"][k-1] if k<=length else [v["name"],None] ]for k,v in category_index.items()],"type":"classify"}
    else:
        output_dict['num_detections'] = int(output_dict['num_detections'])
        output_dict['detection_classes'] = np.array([int(class_id) for class_id in output_dict['detection_classes']])
        output_dict['detection_boxes'] = np.array(output_dict['detection_boxes'])
        output_dict['detection_scores'] = np.array(output_dict['detection_scores'])
        visualization_utils.visualize_boxes_and_labels_on_image_array(
            image_data,
            output_dict['detection_boxes'],
            output_dict['detection_classes'],
            output_dict['detection_scores'],
            category_index,
            instance_masks=output_dict.get('detection_masks'),
            use_normalized_coordinates=True,
            line_thickness=1,
        )
        Image.fromarray(image_data).show()
        image = Image.fromarray(image_data)
        imgByteArr = io.BytesIO()
        image.save(imgByteArr,format='JPEG')
        imgByteArr = imgByteArr.getvalue()
        imgByteArr = base64.b64encode(imgByteArr)
        return {"data":imgByteArr,"type":"detection"}