#include "logging/log.h"

class Device {
    private:
        char* name;
        bool enabled;
        std::function<void()> loop_function;
        std::function<void()> setup_function;   
    public:
        invokeLoopFunction() {
            loop_function();
        }
        invokeSetupFunction() {
            log("devman.c", 0, "Invoking setup function for " + name);
            setup_function();
        }
}