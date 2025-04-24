def log(msg: str):
    from datetime import datetime
    print(f"[LOG {datetime.now().strftime('%H:%M:%S')}] {msg}")
