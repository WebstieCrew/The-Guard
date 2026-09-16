class GuardPowerEngine {
    constructor() {
        this.status = "OFFLINE";
        this.mode = "BALANCED";
        this.cpuLimit = 80;
        this.memoryUsage = 0;
        this.backgroundScan = true;
    }

    initialize(config = {}) {
        this.mode = config.heuristic_level === "high" ? "HIGH_PERFORMANCE" : "BALANCED";
        this.cpuLimit = config.scan_threads ? Math.min(config.scan_threads * 20, 100) : 80;
        this.status = "ONLINE";
    }

    setPowerMode(mode) {
        const validModes = ["SAVER", "BALANCED", "HIGH_PERFORMANCE"];
        if (validModes.includes(mode)) {
            this.mode = mode;
            this.adjustResourceAllocation();
        }
    }

    adjustResourceAllocation() {
        switch (this.mode) {
            case "SAVER":
                this.cpuLimit = 30;
                this.backgroundScan = false;
                break;
            case "BALANCED":
                this.cpuLimit = 60;
                this.backgroundScan = true;
                break;
            case "HIGH_PERFORMANCE":
                this.cpuLimit = 100;
                this.backgroundScan = true;
                break;
        }
    }

    getSystemMetrics() {
        return {
            status: this.status,
            mode: this.mode,
            cpuLimitPercentage: this.cpuLimit,
            backgroundScanActive: this.backgroundScan
        };
    }

    shutdown() {
        this.status = "OFFLINE";
        this.cpuLimit = 0;
        this.backgroundScan = false;
    }
}

if (typeof module !== "undefined" && module.exports) {
    module.exports = GuardPowerEngine;
}
