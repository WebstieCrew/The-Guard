document.addEventListener("DOMContentLoaded", () => {
    const configPath = "project.c";

    const fetchConfig = async () => {
        try {
            const response = await fetch(configPath);
            if (!response.ok) {
                throw new Error(`Failed to load ${configPath}`);
            }
            const data = await response.text();
            parseAndRenderConfig(data);
        } catch (error) {
            console.error("Error loading configuration:", error);
        }
    };

    const parseAndRenderConfig = (rawConfig) => {
        const config = {
            name: "The Guard",
            version: "1.0.0",
            developer: "WebstieCrew",
            license: "MIT",
            realtime_protection: true,
            scan_threads: 4,
            heuristic_level: "high",
            max_file_size_mb: 100,
            on_threat_detected: "quarantine",
            quarantine_dir: "./vault"
        };

        const lines = rawConfig.split("\n");
        lines.forEach((line) => {
            const cleanLine = line.trim();
            if (cleanLine.includes("name =")) {
                config.name = extractValue(cleanLine);
            } else if (cleanLine.includes("version =")) {
                config.version = extractValue(cleanLine);
            } else if (cleanLine.includes("developer =")) {
                config.developer = extractValue(cleanLine);
            } else if (cleanLine.includes("realtime_protection =")) {
                config.realtime_protection = cleanLine.includes("true");
            } else if (cleanLine.includes("scan_threads =")) {
                config.scan_threads = parseInt(extractValue(cleanLine), 10);
            } else if (cleanLine.includes("heuristic_level =")) {
                config.heuristic_level = extractValue(cleanLine);
            } else if (cleanLine.includes("on_threat_detected =")) {
                config.on_threat_detected = extractValue(cleanLine);
            } else if (cleanLine.includes("quarantine_dir =")) {
                config.quarantine_dir = extractValue(cleanLine);
            }
        });

        updateUI(config);
    };

    const extractValue = (line) => {
        const match = line.match(/=\s*["']?([^"';]+)["']?/);
        return match ? match[1].trim() : "";
    };

    const updateUI = (config) => {
        const headerTitle = document.querySelector("header h1");
        if (headerTitle) {
            headerTitle.textContent = `${config.name} v${config.version}`;
        }

        const cards = document.querySelectorAll(".card");
        if (cards.length >= 3) {
            cards[0].querySelector("p").textContent = 
                `Monitors system activity continuously using ${config.scan_threads} scan threads with ${config.heuristic_level} heuristic level.`;
            cards[1].querySelector("p").textContent = 
                `Replaces XML with project.c structure. Maintained by ${config.developer} under ${config.license} license.`;
            cards[2].querySelector("p").textContent = 
                `Automated action set to "${config.on_threat_detected}". Threat files are moved directly to "${config.quarantine_dir}".`;
        }
    };

    fetchConfig();
});
