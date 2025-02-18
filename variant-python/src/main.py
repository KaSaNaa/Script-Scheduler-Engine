import time
from scheduler import initialize_scheduler, start_scheduler_thread, start_config_watcher

def main():
    initialize_scheduler()
    start_scheduler_thread()
    observer = start_config_watcher()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == "__main__":
    main()