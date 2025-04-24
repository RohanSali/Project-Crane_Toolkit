import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
import cv2
from camera_stream.stream_handler import get_camera_streams
from utils.logger import log

def test_camera_feeds():
    streams = get_camera_streams()

    if not streams:
        log("[ERROR] No cameras available!")
        return

    log(f"[INFO] {len(streams)} camera(s) detected. Starting stream preview...")

    # Create a window for each camera
    window_names = [f"Camera {i}" for i in range(len(streams))]

    while True:
        for idx, cap in enumerate(streams):
            ret, frame = cap.read()
            if ret:
                cv2.imshow(window_names[idx], frame)  # Show video in a separate window
            else:
                log(f"[ERROR] Failed to capture frame from Camera {idx}")

        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):  # Press 'q' to quit
            break

    # Release the streams and close windows
    for cap in streams:
        cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    test_camera_feeds()
