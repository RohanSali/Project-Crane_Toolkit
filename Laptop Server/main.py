import asyncio
from websocket_server.server import start_server
from utils.logger import log

if __name__ == "__main__":
    log("Initializing system...")
    asyncio.get_event_loop().run_until_complete(start_server())
    log("Server running.")
    asyncio.get_event_loop().run_forever()
