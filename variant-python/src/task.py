import subprocess
from datetime import datetime

class Task:
    def __init__(self, script_path, scheduled_time):
        self.script_path = script_path
        self.scheduled_time = datetime.strptime(scheduled_time, "%Y-%m-%dT%H:%M:%S")

    def run(self):
        print(f"Starting task: {self.script_path} at {datetime.now()}")
        try:
            result = subprocess.run(self.script_path, shell=True, check=True, capture_output=True, text=True)
            print(f"Task completed: {self.script_path} at {datetime.now()}")
            print(f"Output: {result.stdout}")
        except subprocess.CalledProcessError as e:
            print(f"Task failed: {self.script_path} at {datetime.now()}")
            print(f"Error: {e.stderr}")