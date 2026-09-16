app_info {
    name = "Guard Antivirus";
    version = "1.0.0";
    developer = "WebstieCrew";
    license = "MIT";
}

engine_settings {
    realtime_protection = true;
    scan_threads = 4;
    heuristic_level = "high";
    max_file_size_mb = 100;
}

signatures {
    database_url = "https://signatures.guard.internal/v1/definitions.bin";
    auto_update = true;
    update_interval_hours = 6;
}

scanner_rules {
    file_extensions = [".exe", ".dll", ".bat", ".vbs", ".ps1", ".elf"];
    
    exclude_paths {
        path = "/sys/kernel/debug";
        path = "C:\\Windows\\SoftwareDistribution";
    }
}

action_policies {
    on_threat_detected = "quarantine";
    quarantine_dir = "./vault";
}
