#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

class GuardSetup {
public:
    static bool initialize_environment() {
        std::cout << "[SETUP] Initializing The Guard environment...\n";

        const std::string directories[] = {
            "bin",
            "build",
            "logs",
            "vault"
        };

        for (const auto& dir : directories) {
            if (!fs::exists(dir)) {
                if (fs::create_directory(dir)) {
                    std::cout << "[SETUP] Created directory: " << dir << "\n";
                } else {
                    std::cerr << "[ERROR] Failed to create directory: " << dir << "\n";
                    return false;
                }
            }
        }

        return verify_default_config();
    }

    static bool verify_default_config() {
        if (!fs::exists("project.c")) {
            std::cout << "[SETUP] Creating default project.c configuration...\n";
            std::ofstream config_file("project.c");
            if (!config_file.is_open()) {
                std::cerr << "[ERROR] Unable to generate project.c file.\n";
                return false;
            }

            config_file << "name = \"The Guard\"\n"
                        << "version = \"1.0.0\"\n"
                        << "developer = \"WebstieCrew\"\n"
                        << "license = \"MIT\"\n\n"
                        << "realtime_protection = true\n"
                        << "scan_threads = 4\n"
                        << "heuristic_level = \"HIGH\"\n"
                        << "max_file_size_mb = 100\n\n"
                        << "database_url = \"https://signatures.theguard.internal\"\n"
                        << "auto_update = true\n"
                        << "update_interval_hours = 6\n\n"
                        << "on_threat_detected = \"QUARANTINE\"\n"
                        << "quarantine_dir = \"./vault\"\n";

            config_file.close();
            std::cout << "[SETUP] project.c created successfully.\n";
        }
        return true;
    }

    static void build_engine() {
        std::cout << "[SETUP] Compiling engine executable...\n";
        
        #ifdef _WIN32
            int result = std::system("gcc -O3 src/main.c src/guard/config.c -o bin/system-guard.exe");
        #else
            int result = std::system("gcc -O3 src/main.c src/guard/config.c -o bin/system-guard");
        #endif

        if (result == 0) {
            std::cout << "[SETUP] Engine compiled successfully in ./bin/\n";
        } else {
            std::cerr << "[ERROR] Build failed. Verify GCC installation.\n";
        }
    }
};

int main() {
    if (GuardSetup::initialize_environment()) {
        GuardSetup::build_engine();
        std::cout << "[SETUP] Setup sequence finished successfully.\n";
        return EXIT_SUCCESS;
    }
    
    return EXIT_FAILURE;
}
