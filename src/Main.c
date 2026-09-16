#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char name[64];
    char version[16];
    char developer[64];
    char license[32];
    bool realtime_protection;
    int scan_threads;
    char heuristic_level[16];
    int max_file_size_mb;
    char database_url[128];
    bool auto_update;
    int update_interval_hours;
    char on_threat_detected[32];
    char quarantine_dir[128];
} GuardConfig;

void parse_project_c(const char *filename, GuardConfig *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "name =")) {
            sscanf(line, " name = \"%[^\"]\"", config->name);
        } else if (strstr(line, "version =")) {
            sscanf(line, " version = \"%[^\"]\"", config->version);
        } else if (strstr(line, "developer =")) {
            sscanf(line, " developer = \"%[^\"]\"", config->developer);
        } else if (strstr(line, "license =")) {
            sscanf(line, " license = \"%[^\"]\"", config->license);
        } else if (strstr(line, "realtime_protection =")) {
            char val[16];
            sscanf(line, " realtime_protection = %[^;]", val);
            config->realtime_protection = (strstr(val, "true") != NULL);
        } else if (strstr(line, "scan_threads =")) {
            sscanf(line, " scan_threads = %d;", &config->scan_threads);
        } else if (strstr(line, "heuristic_level =")) {
            sscanf(line, " heuristic_level = \"%[^\"]\"", config->heuristic_level);
        } else if (strstr(line, "max_file_size_mb =")) {
            sscanf(line, " max_file_size_mb = %d;", &config->max_file_size_mb);
        } else if (strstr(line, "database_url =")) {
            sscanf(line, " database_url = \"%[^\"]\"", config->database_url);
        } else if (strstr(line, "auto_update =")) {
            char val[16];
            sscanf(line, " auto_update = %[^;]", val);
            config->auto_update = (strstr(val, "true") != NULL);
        } else if (strstr(line, "update_interval_hours =")) {
            sscanf(line, " update_interval_hours = %d;", &config->update_interval_hours);
        } else if (strstr(line, "on_threat_detected =")) {
            sscanf(line, " on_threat_detected = \"%[^\"]\"", config->on_threat_detected);
        } else if (strstr(line, "quarantine_dir =")) {
            sscanf(line, " quarantine_dir = \"%[^\"]\"", config->quarantine_dir);
        }
    }

    fclose(file);
}

void print_status(const GuardConfig *config) {
    printf("[%s v%s]\n", config->name, config->version);
    printf("Developer: %s\n", config->developer);
    printf("License: %s\n", config->license);
    printf("Realtime Protection: %s\n", config->realtime_protection ? "ENABLED" : "DISABLED");
    printf("Threads: %d\n", config->scan_threads);
    printf("Heuristics: %s\n", config->heuristic_level);
    printf("Max File Size: %d MB\n", config->max_file_size_mb);
    printf("Signature Server: %s\n", config->database_url);
    printf("Auto Update: %s (Every %dh)\n", config->auto_update ? "ON" : "OFF", config->update_interval_hours);
    printf("Threat Policy: %s\n", config->on_threat_detected);
    printf("Quarantine Vault: %s\n", config->quarantine_dir);
}

int main(int argc, char *argv[]) {
    const char *config_file = "project.c";
    if (argc > 1) {
        config_file = argv[1];
    }

    GuardConfig config = {0};
    parse_project_c(config_file, &config);

    print_status(&config);

    return 0;
}
