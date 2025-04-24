import json

def format_frames(frames_dict):
    return json.dumps({
        "type": "camera_frames",
        "data": frames_dict
    })