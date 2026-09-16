[app_info]
name = "The Guard Security Core"
version = "2.1.0-prod"
developer = "WebstieCrew"
license = "MIT"
environment = "production"
cluster_node_id = "node-us-east-01"

[engine_settings]
realtime_protection = true
scan_threads = 0
process_priority = "HIGH"
heuristic_level = "DEEP"
max_file_size_mb = 1024
max_recursion_depth = 32
memory_scan_enabled = true
behavioral_analysis = true
kernel_filter_driver = true
zero_day_sandbox = true
sandbox_timeout_ms = 3000

[signatures]
primary_url = "https://signatures.guard.internal/v2/definitions.bin"
secondary_url = "https://backup-sigs.guard.internal/v2/definitions.bin"
signature_format = "ENCRYPTED_ED25519"
auto_update = true
update_interval_hours = 1
verify_tls_certificates = true
delta_updates_only = true
public_key_fingerprint = "9f8a7b6c5d4e3f2a1b0c9d8e7f6a5b4c3d2e1f0a"

[scanner_rules]
target_extensions = [
    ".exe", ".dll", ".sys", ".drv", ".so", ".dylib",
    ".bat", ".vbs", ".ps1", ".sh", ".py", ".elf",
    ".msi", ".jar", ".cmd", ".scr", ".hta", ".docm",
    ".vhd", ".vmdk", ".iso"
]

archive_types = [".zip", ".tar.gz", ".7z", ".rar", ".cab", ".tar.xz"]
scan_inside_archives = true
max_archive_depth = 8

exclude_paths = [
    "/sys/kernel/debug",
    "/proc",
    "/dev",
    "/run",
    "C:\\Windows\\SoftwareDistribution\\Download",
    "C:\\Program Files\\Windows Defender"
]

exclude_hashes = [
    "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
    "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"
]

[action_policies]
on_threat_detected = "QUARANTINE"
quarantine_dir = "/var/lib/theguard/vault"
quarantine_max_size_gb = 20
auto_purge_days = 45
encryption_cipher = "AES-256-GCM"

[network_guard]
firewall_integration = true
block_suspicious_ip = true
intercept_dns_queries = true
http_payload_inspection = true
blacklisted_ports = [135, 137, 138, 139, 445, 3389]

[telemetry_and_logging]
log_level = "TRACE"
log_format = "JSON"
log_path = "/var/log/theguard/engine.log"
syslog_export = true
syslog_endpoint = "10.0.0.50:514"
alert_on_critical_threat = true
notification_webhook = "https://hooks.slack.com/services/THEGUARD/ALERT/HOOK"
