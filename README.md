# Script Scheduling Engine
## **Roadmap: Script Scheduler Engine**

### **Phase 1: Project Setup (Planned)**
- [x] Create the project repository and set up the folder structure.
- [x] Define the project's core goals and requirements in the `README.md`.
- [x] Add a basic `Makefile` or `CMakeLists.txt` for building the project.

---

### **Phase 2: Core Script Execution (Planned)**
- [x] Implement a basic script runner module:
  - [x] Allow execution of Python and Bash scripts.
  - [x] Capture and log the output (stdout and stderr).
  - [x] Monitor script exit statuses.
- [ ] Test script execution with short and long-running scripts.

---

### **Phase 3: Scheduler and Monitoring (Planned)**
- [x] Create a basic job queue to manage script scheduling.
- [x] Enable concurrent execution of multiple scripts.
- [x] Add a timeout mechanism to terminate unresponsive scripts.
- [x] Implement process monitoring to detect and handle failures.

---

### **Phase 4: Logging and Resilience (Planned)**
- [x] Implement detailed logging of script activities (start, end, errors).
- [x] Add retry mechanisms for failed scripts.
- [x] Introduce a watchdog process to ensure engine reliability.

---

### **Phase 5: Advanced Features (Future Goals)**
- [ ] Support dependency management for scripts.
- [x] Add a configuration system (e.g., JSON or YAML files).
- [ ] Implement a CLI for scheduling and monitoring scripts.
- [ ] Explore options for a lightweight web interface for real-time monitoring.

---

### **Long-Term Vision**
- [ ] Optimize performance for large-scale use cases.
- [ ] Support integration with other tools or APIs.
- [ ] Add support for containerized script execution (e.g., Docker).
