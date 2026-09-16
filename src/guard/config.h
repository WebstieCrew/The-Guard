#ifndef GUARD_CONFIG_H
#define GUARD_CONFIG_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

typedef struct {
    char name[64];
    char version[16];
    char developer[64];
    char license[32];
    bool realtime_protection;
    int scan_threads;
    char heuristic_level[16];
    int max_file_size_mb;
    char database_url[256];
    bool auto_update;
    int update_interval_hours;
    char on_threat_detected[32];
    char quarantine_dir[256];
    LogLevel log_level;
} GuardConfig;

void config_set_defaults(GuardConfig *config);
bool config_load_from_file(const char *filepath, GuardConfig *config);
void config_print_status(const GuardConfig *config);

#endif
