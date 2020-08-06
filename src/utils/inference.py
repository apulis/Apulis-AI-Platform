import requests
import base64
import numpy as np
from PIL import Image
import os,io
import json
from io import BytesIO
import visualization_utils

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
        for ID, name in enumerate(data,1):
            print(name)
            names[ID] = {"name":name.strip('\n'),"id":ID}
    return names

def object_detaction_infer2(inference_url,imageFile,signature_name):
    image = Image.open(BytesIO(imageFile))
    (im_width, im_height) = image.size
    image_data = np.array(image.getdata()).reshape((im_height, im_width, 3)).astype(np.uint8)
    image_data_yolo_list = image_data[np.newaxis, :].tolist()
    headers = {"Content-type": "application/json"}
    inference_url = "http://127.0.0.1/endpoints/"+inference_url.split("/endpoints/")[1]
    r = requests.post(inference_url,headers=headers,
                      data=json.dumps({"signature_name": signature_name,"instances":image_data_yolo_list}))
    r = r.json()
    output_dict = r['predictions'][0]
    output_dict['num_detections'] = int(output_dict['num_detections'])
    output_dict['detection_classes'] = np.array([int(class_id) for class_id in output_dict['detection_classes']])
    output_dict['detection_boxes'] = np.array(output_dict['detection_boxes'])
    output_dict['detection_scores'] = np.array(output_dict['detection_scores'])
    category_index = read_class_names2("coco.names")
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
    return imgByteArr