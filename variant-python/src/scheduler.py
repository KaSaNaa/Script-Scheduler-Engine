import json
import os
import time
import threading
from datetime import datetime
from .task import Task
from .utils import log_error, load_tasks_from_json

CONFIG_FILE = "config.json"
TASK_QUEUE = []
QUEUE_COND = threading.Condition()

def initialize_scheduler():
    global TASK_QUEUE
    TASK_QUEUE = load_tasks_from_json(CONFIG_FILE)
    print("Scheduler Initialized.")

def scheduler_thread_func():
    while True:
        with QUEUE_COND:
            while not TASK_QUEUE:
                QUEUE_COND.wait()
            task = TASK_QUEUE.pop(0)
        task_thread = threading.Thread(target=task.run)
        task_thread.start()

def start_scheduler_thread():
    scheduler_thread = threading.Thread(target=scheduler_thread_func)
    scheduler_thread.daemon = True
    scheduler_thread.start()

def watch_config_file():
    last_modified = os.path.getmtime(CONFIG_FILE)
    while True:
        time.sleep(1)
        current_modified = os.path.getmtime(CONFIG_FILE)
        if current_modified != last_modified:
            print("Config file modified, reloading tasks...")
            initialize_scheduler()
            last_modified = current_modified

def start_config_watcher():
    watcher_thread = threading.Thread(target=watch_config_file)
    watcher_thread.daemon = True
    watcher_thread.start()