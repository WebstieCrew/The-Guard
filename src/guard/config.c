#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

static void strip_quotes_and_semicolon(char *val) {
    size_t len = strlen(val);
    if (len == 0) return;

    if (val[len - 1] == ';') {
        val[--len] = '\0';
    }

    if (len >= 2 && val[0] == '"' && val[len - 1] == '"') {
        val[len - 1] = '\0';
        memmove(val, val + 1, len - 1);
    }
}

static bool parse_boolean(const char *val) {
    return (strcasecmp(val, "true") == 0 || strcmp(val, "1") == 0 || strcasecmp(val, "yes") == 0);
}

void config_set_defaults(GuardConfig *config) {
    if (!config) return;
    
    memset(config, 0, sizeof(GuardConfig));
    strncpy(config->name, "The-Guard", sizeof(config->name) - 1);
    strncpy(config->version, "1.0.0", sizeof(config->version) - 1);
    strncpy(config->developer, "WebstieCrew", sizeof(config->developer) - 1);
    strncpy(config->license, "MIT", sizeof(config->license) - 1);
    
    config->realtime_protection = true;
    config->scan_threads = 4;
    strncpy(config->heuristic_level, "MEDIUM", sizeof(config->heuristic_level) - 1);
    config->max_file_size_mb = 100;
    strncpy(config->database_url, "https://signatures.theguard.internal", sizeof(config->database_url) - 1);
    config->auto_update = true;
    config->update_interval_hours = 12;
    strncpy(config->on_threat_detected, "QUARANTINE", sizeof(config->on_threat_detected) - 1);
    strncpy(config->quarantine_dir, "/var/lib/theguard/quarantine", sizeof(config->quarantine_dir) - 1);
    config->log_level = LOG_LEVEL_INFO;
}

static void assign_config_key(GuardConfig *config, const char *key, char *value) {
    strip_quotes_and_semicolon(value);

    if (strcmp(key, "name") == 0) {
        snprintf(config->name, sizeof(config->name), "%s", value);
    } else if (strcmp(key, "version") == 0) {
        snprintf(config->version, sizeof(config->version), "%s", value);
    } else if (strcmp(key, "developer") == 0) {
        snprintf(config->developer, sizeof(config->developer), "%s", value);
    } else if (strcmp(key, "license") == 0) {
        snprintf(config->license, sizeof(config->license), "%s", value);
    } else if (strcmp(key, "realtime_protection") == 0) {
        config->realtime_protection = parse_boolean(value);
    } else if (strcmp(key, "scan_threads") == 0) {
        config->scan_threads = atoi(value);
    } else if (strcmp(key, "heuristic_level") == 0) {
        snprintf(config->heuristic_level, sizeof(config->heuristic_level), "%s", value);
    } else if (strcmp(key, "max_file_size_mb") == 0) {
        config->max_file_size_mb = atoi(value);
    } else if (strcmp(key, "database_url") == 0) {
        snprintf(config->database_url, sizeof(config->database_url), "%s", value);
    } else if (strcmp(key, "auto_update") == 0) {
        config->auto_update = parse_boolean(value);
    } else if (strcmp(key, "update_interval_hours") == 0) {
        config->update_interval_hours = atoi(value);
    } else if (strcmp(key, "on_threat_detected") == 0) {
        snprintf(config->on_threat_detected, sizeof(config->on_threat_detected), "%s", value);
    } else if (strcmp(key, "quarantine_dir") == 0) {
        snprintf(config->quarantine_dir, sizeof(config->quarantine_dir), "%s", value);
    }
}

bool config_load_from_file(const char *filepath, GuardConfig *config) {
    if (!filepath || !config) return false;

    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Config Error");
        return false;
    }

    char line[512];
    size_t line_num = 0;

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        char *clean_line = trim_whitespace(line);

        if (clean_line[0] == '\0' || clean_line[0] == '#' || (clean_line[0] == '/' && clean_line[1] == '/')) {
            continue;
        }

        char *delimiter = strchr(clean_line, '=');
        if (!delimiter) continue;

        *delimiter = '\0';
        char *key = trim_whitespace(clean_line);
        char *value = trim_whitespace(delimiter + 1);

        assign_config_key(config, key, value);
    }

    fclose(file);
    return true;
}

void config_print_status(const GuardConfig *config) {
    if (!config) return;

    printf("\n============================================\n");
    printf("   %s ENGINE CONTROL PANEL (v%s)\n", config->name, config->version);
    printf("============================================\n");
    printf(" Developer         : %s\n", config->developer);
    printf(" License           : %s\n", config->license);
    printf(" Realtime Guard    : %s\n", config->realtime_protection ? "ENABLED" : "DISABLED");
    printf(" Scan Threads      : %d cores\n", config->scan_threads);
    printf(" Heuristic Engine  : %s\n", config->heuristic_level);
    printf(" Max Stream Buffer : %d MB\n", config->max_file_size_mb);
    printf(" Updates Endpoint  : %s\n", config->database_url);
    printf(" Auto Update Sync  : %s (Interval: %dh)\n", config->auto_update ? "ACTIVE" : "INACTIVE", config->update_interval_hours);
    printf(" Threat Action     : %s\n", config->on_threat_detected);
    printf(" Isolation Vault   : %s\n", config->quarantine_dir);
    printf("============================================\n\n");
}
