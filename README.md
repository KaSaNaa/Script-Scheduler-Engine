# Script Scheduling Engine
## **Roadmap: Script Scheduler Engine**

### **Phase 1: Project Setup (Planned)**
- [x] Create the project repository and set up the folder structure.
- [x] Define the project's core goals and requirements in the `README.md`.
- [x] Add a basic `Makefile` or `CMakeLists.txt` for building the project.

---

### **Phase 2: Core Script Execution (Planned)**
- [ ] Implement a basic script runner module:
  - [ ] Allow execution of Python and Bash scripts.
  - [ ] Capture and log the output (stdout and stderr).
  - [ ] Monitor script exit statuses.
- [ ] Test script execution with short and long-running scripts.

---

### **Phase 3: Scheduler and Monitoring (Planned)**
- [ ] Create a basic job queue to manage script scheduling.
- [ ] Enable concurrent execution of multiple scripts.
- [ ] Add a timeout mechanism to terminate unresponsive scripts.
- [ ] Implement process monitoring to detect and handle failures.

---

### **Phase 4: Logging and Resilience (Planned)**
- [ ] Implement detailed logging of script activities (start, end, errors).
- [ ] Add retry mechanisms for failed scripts.
- [ ] Introduce a watchdog process to ensure engine reliability.

---

### **Phase 5: Advanced Features (Future Goals)**
- [ ] Support dependency management for scripts.
- [ ] Add a configuration system (e.g., JSON or YAML files).
- [ ] Implement a CLI for scheduling and monitoring scripts.
- [ ] Explore options for a lightweight web interface for real-time monitoring.

---

### **Long-Term Vision**
- [ ] Optimize performance for large-scale use cases.
- [ ] Support integration with other tools or APIs.
- [ ] Add support for containerized script execution (e.g., Docker).
