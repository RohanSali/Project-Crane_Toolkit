import cv2
from utils.logger import log

MAX_CAMERAS = 4  # Limit to 4 cameras for performance

def detect_connected_cameras(max_cameras=MAX_CAMERAS):
    available_cameras = []
    log("Scanning for available cameras...")

    for index in range(max_cameras):
        cap = cv2.VideoCapture(index)
        if cap.isOpened() and cap.read()[0]:  # Check both camera availability and the first frame
            log(f"Camera found at index {index}")
            available_cameras.append(index)
        else:
            log(f"No camera at index {index}")
        cap.release()  # Always release after the check

    if not available_cameras:
        log("[WARN] No cameras detected.")
    return available_cameras


def get_camera_streams():
    camera_indexes = detect_connected_cameras()
    streams = []

    for index in camera_indexes:
        cap = cv2.VideoCapture(index,cv2.CAP_DSHOW)
        if cap.isOpened():
            log(f"Opening stream for camera {index}")
            streams.append(cap)
        else:
            log(f"[ERROR] Could not open camera {index}")
    return streams
