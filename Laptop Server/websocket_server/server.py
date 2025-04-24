import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

import asyncio
import websockets
import cv2
import json
import base64
from camera_stream.stream_handler import get_camera_streams
from utils.logger import log  # If you have a custom logger

# Get up to 4 available camera streams
streams = get_camera_streams()

async def handle_client(websocket, path):
    try:
        if path != "/camera-streams":
            log(f"[WARN] Rejected connection with invalid path: {path}")
            await websocket.close()
            return

        if streams:
            while True:
                frames_data = []
                for cap in streams:
                    ret, frame = cap.read()
                    if ret:
                        _, buffer = cv2.imencode('.jpg', frame)
                        frame_encoded = base64.b64encode(buffer).decode('utf-8')
                        frames_data.append(frame_encoded)
                    else:
                        frames_data.append(None)

                message = json.dumps({"frames": frames_data})
                await websocket.send(message)
                await asyncio.sleep(0.05)  # ~20 FPS
        else:
            await websocket.send(json.dumps({"error": "No cameras available."}))
    except Exception as e:
        log(f"[ERROR] Exception in handle_client: {e}")
    finally:
        await websocket.close()

async def start_server():
    return await websockets.serve(handle_client, "0.0.0.0",8888, ping_interval=None)

# Entry point
if __name__ == "__main__":
    log("Initializing system...")

    async def main():
        try:
            server = await websockets.serve(handle_client, "0.0.0.0", 8888, ping_interval=None)
            log("WebSocket server started at ws://0.0.0.0:8888")
            await server.wait_closed()
        except asyncio.CancelledError:
            log("[WARN] Server was cancelled")
        except Exception as e:
            log(f"[ERROR] Unexpected error: {e}")

    asyncio.run(main())
