import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

import asyncio
import websockets
import json
import cv2
import numpy as np
import base64

async def test_ws_connection():
    uri = "ws://localhost:8888"
    async with websockets.connect(uri) as websocket:
        print("Connected to WebSocket server.")
        
        while True:
            message = await websocket.recv()
            data = json.loads(message)

            if "error" in data:
                print("Server Error:", data["error"])
                break

            frames = data.get("frames", [])
            for idx, frame_data in enumerate(frames):
                if frame_data:
                    frame_bytes = base64.b64decode(frame_data)
                    np_arr = np.frombuffer(frame_bytes, dtype=np.uint8)
                    img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
                    if img is not None:
                        cv2.imshow(f"Camera {idx}", img)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    cv2.destroyAllWindows()

if __name__ == "__main__":
    asyncio.run(test_ws_connection())
