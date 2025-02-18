import os
import time
import threading
from datetime import datetime
from queue import PriorityQueue
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from task import Task
from utils import log_error, load_tasks_from_json

CONFIG_FILE = "config.json"
TASK_QUEUE = PriorityQueue()
QUEUE_COND = threading.Condition()


class ConfigFileHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path.endswith(CONFIG_FILE):
            print("Config file modified, reloading tasks...")
            initialize_scheduler()


def initialize_scheduler():
    global TASK_QUEUE
    with QUEUE_COND:
        TASK_QUEUE = PriorityQueue()
        tasks = load_tasks_from_json(CONFIG_FILE)
        for task in tasks:
            TASK_QUEUE.put((task.scheduled_time, task))
        QUEUE_COND.notify_all()
    print("Scheduler Initialized.")


def scheduler_thread_func():
    while True:
        with QUEUE_COND:
            while TASK_QUEUE.empty():
                QUEUE_COND.wait()
            scheduled_time, task = TASK_QUEUE.get()
        now = datetime.now()
        if scheduled_time > now:
            time.sleep((scheduled_time - now).total_seconds())
        task.run()


def start_scheduler_thread():
    scheduler_thread = threading.Thread(target=scheduler_thread_func)
    scheduler_thread.daemon = True
    scheduler_thread.start()


def start_config_watcher():
    if not os.path.exists(CONFIG_FILE):
        log_error(f"Config file {CONFIG_FILE} does not exist.")
        return None

    event_handler = ConfigFileHandler()
    observer = Observer()
    observer.schedule(
        event_handler, path=os.path.dirname(CONFIG_FILE) or ".", recursive=False
    )
    observer.start()
    return observer
