#pragma once
#include "main.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"

namespace GayFrogs {
    class GFHooks {
    private:
        inline static std::vector<void (*)(Logger &logger)> installFuncs;
    public:
        static void AddInstallFunc(void (*installFunc)(Logger &logger)) {
            installFuncs.push_back(installFunc);
        }

        static void InstallAllHooks(Logger &logger) {
            for (auto installFunc : installFuncs) {
                installFunc(logger);
            }
        }
    };
}

#define InstallHooks(func) \
struct __Register##func { \
    __Register##func() { \
        GayFrogs::GFHooks::AddInstallFunc(func); \
    } \
}; \
static __Register##func __RegisterInstance##func;