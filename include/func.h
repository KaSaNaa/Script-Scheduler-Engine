#ifndef FUNC_H
#define FUNC_H

// Declare the functions from func.c
void execute_script(const char *script);
void log_error(const char *message);
void execute_script_with_retries(const char *script);
void *execute_script_thread(void *arg);
void run_scheduler_concurrent(void);
void handle_shutdown(int signum);
void setup_signal_handlers();
void start_watchdog();
void load_tasks_from_json(const char *filename);

#endif // FUNC_H