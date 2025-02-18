import os
import json
from datetime import datetime
from task import Task

def log_error(message):
    if not os.path.exists("logs"):
        os.makedirs("logs")
    with open("logs/scheduler.log", "a") as log_file:
        log_file.write(f"[{datetime.now()}] ERROR: {message}\n")

def load_tasks_from_json(filename):
    if not os.path.exists(filename):
        with open(filename, "w") as file:
            json.dump({"tasks": []}, file)
    with open(filename, "r") as file:
        data = json.load(file)
    tasks = []
    for task_data in data.get("tasks", []):
        task = Task(task_data["script"], task_data["time"])
        tasks.append(task)
    return tasks