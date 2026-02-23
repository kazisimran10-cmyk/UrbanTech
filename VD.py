print(">>> SMART TRAFFIC SYSTEM – FAST REAL TIME MODE <<<")

from ultralytics import YOLO
import cv2
import requests
import numpy as np
import time
import threading

ESP32_IP = "192.168.137.216"

CAMERAS = {
    "cam-1": "http://192.168.137.50/capture",
    "cam-2": "http://192.168.137.51/capture",
    "cam-3": "http://192.168.137.52/capture",
    "cam-4": "http://192.168.137.53/capture"
}

model = YOLO("yolov8n.pt")
CONF = 0.25
IMG_SIZE = 320

VEHICLE_CLASSES = {"car","bus","truck","motorcycle","bicycle"}

GREEN_TIME = 8
YELLOW_TIME = 3
DETECT_INTERVAL = 5   # 🔥 detection every 5 sec only

vehicle_counts = {cam:0 for cam in CAMERAS}

# ================= FAST LED SEND =================
def send_led(cam,color):
    url = f"http://{ESP32_IP}/{cam}/{color}"
    threading.Thread(
        target=lambda: requests.get(url,timeout=1),
        daemon=True
    ).start()

# ================= BACKGROUND DETECTION =================
def detect_traffic():
    global vehicle_counts

    while True:
        print("\nDETECTING IN BACKGROUND...")

        for cam,url in CAMERAS.items():
            try:
                r = requests.get(url,timeout=2)
                frame = cv2.imdecode(
                    np.frombuffer(r.content,np.uint8),
                    cv2.IMREAD_COLOR
                )
            except:
                continue

            results = model(frame,imgsz=IMG_SIZE,conf=CONF,verbose=False)

            count = 0
            for res in results:
                if res.boxes is None: continue
                for b in res.boxes:
                    cls = model.names[int(b.cls[0])]
                    if cls in VEHICLE_CLASSES:
                        count+=1

            vehicle_counts[cam] = count

        print("DENSITY:",vehicle_counts)
        time.sleep(DETECT_INTERVAL)

# 🔥 START BACKGROUND DETECTION
threading.Thread(target=detect_traffic,daemon=True).start()

# ================= MAIN SIGNAL LOOP =================
while True:

    max_count = max(vehicle_counts.values())

    # 🚨 NO VEHICLE
    if max_count == 0:
        for cam in CAMERAS:
            send_led(cam,"red")
        time.sleep(2)
        continue

    # -------- BUSIEST LANE --------
    green_lane = max(vehicle_counts,key=vehicle_counts.get)
    print("GREEN:",green_lane)

    for cam in CAMERAS:
        send_led(cam,"red")
    time.sleep(0.5)

    send_led(green_lane,"green")
    time.sleep(GREEN_TIME)

    send_led(green_lane,"yellow")
    time.sleep(YELLOW_TIME)

    send_led(green_lane,"red")
