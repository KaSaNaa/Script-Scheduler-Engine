#include <func.h>
#include <scheduler.h>
#include <stddef.h>
#include <unistd.h>

int main(void) {
  initialize_scheduler();
  start_scheduler_thread();

  load_tasks_from_json("config.json");

  setup_signal_handlers();
  start_watchdog();
  start_config_watcher();

  while (1) {
    pause();
  }
}