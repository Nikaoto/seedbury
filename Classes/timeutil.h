#pragma once 

namespace timeutil {
    static unsigned long getEpochSeconds() {
        return std::chrono::duration_cast<std::chrono::seconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    }
    
    static unsigned long long getEpochMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    }
}
