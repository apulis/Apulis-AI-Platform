import requests
import base64
import numpy as np
import cv2
import serve_utils
from PIL import Image
import os

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
    img_size = 608
    with open("tmpfile","w") as f:
        f.write(imageFile)
    with open("tmpfile","rb") as f:
        a = f.read()
    original_image = cv2.imdecode(np.frombuffer(a, np.uint8), -1)
    image_data = serve_utils.image_preporcess(np.copy(original_image), [img_size, img_size])
    image_data_yolo_list = image_data[np.newaxis, :].tolist()
    headers = {"Content-type": "application/json"}
    r = requests.post(inference_url,headers=headers,json={
        "signature_name": signature_name,
        "instances":image_data_yolo_list
    }).json()
    output = np.array(r['predictions'])
    output = np.reshape(output, (-1, 85))
    original_image_size = original_image.shape[:2]
    bboxes = serve_utils.postprocess_boxes(output, original_image_size, img_size, 0.3)
    bboxes = serve_utils.nms(bboxes, 0.45, method='nms')
    image = serve_utils.draw_bbox(original_image, bboxes)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    image = Image.fromarray(image)
    return image