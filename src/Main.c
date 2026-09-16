#include "guard/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <getopt.h>

static volatile sig_atomic_t g_running = 1;

static void signal_handler(int signum) {
    (void)signum;
    g_running = 0;
}

static void register_signal_handlers(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
}

static void display_help(const char *executable) {
    printf("Usage: %s [OPTIONS]\n", executable);
    printf("Options:\n");
    printf("  -c, --config <path>    Specify configuration file path\n");
    printf("  -t, --test             Validate configuration file syntax and exit\n");
    printf("  -v, --verbose          Enable debug logging level\n");
    printf("  -h, --help             Display usage details\n");
    printf("  -V, --version          Print engine version\n");
}

int main(int argc, char *argv[]) {
    const char *config_path = "project.c";
    bool dry_run = false;
    bool force_verbose = false;

    static struct option long_options[] = {
        {"config",   required_argument, 0, 'c'},
        {"test",     no_argument,       0, 't'},
        {"verbose",  no_argument,       0, 'v'},
        {"help",     no_argument,       0, 'h'},
        {"version",  no_argument,       0, 'V'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "c:tvhV", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c':
                config_path = optarg;
                break;
            case 't':
                dry_run = true;
                break;
            case 'v':
                force_verbose = true;
                break;
            case 'h':
                display_help(argv[0]);
                return EXIT_SUCCESS;
            case 'V':
                printf("The-Guard Security Engine v1.0.0\n");
                return EXIT_SUCCESS;
            default:
                display_help(argv[0]);
                return EXIT_FAILURE;
        }
    }

    register_signal_handlers();

    GuardConfig config;
    config_set_defaults(&config);

    if (!config_load_from_file(config_path, &config)) {
        fprintf(stderr, "[ERROR] Failed to parse configuration file: %s\n", config_path);
        return EXIT_FAILURE;
    }

    if (force_verbose) {
        config.log_level = LOG_LEVEL_DEBUG;
    }

    if (dry_run) {
        printf("[OK] Configuration file syntax is valid.\n");
        return EXIT_SUCCESS;
    }

    config_print_status(&config);

    printf("[INFO] Initializing engine core runtime...\n");

    while (g_running) {
    }

    printf("[INFO] Performing graceful engine shutdown sequence...\n");
    return EXIT_SUCCESS;
}
