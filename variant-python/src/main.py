from src.scheduler import initialize_scheduler, start_scheduler_thread, start_config_watcher
import time

def main():
    initialize_scheduler()
    start_scheduler_thread()
    start_config_watcher()

    while True:
        time.sleep(1)

if __name__ == "__main__":
    main()