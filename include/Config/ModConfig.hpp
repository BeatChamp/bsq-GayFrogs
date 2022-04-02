#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(xDist, float, "X Distance", 2.5f);
    CONFIG_VALUE(yDist, float, "X Distance", 2.0f);
    CONFIG_VALUE(frogCount, int, "frogCount", 20);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(xDist);
        CONFIG_INIT_VALUE(yDist);
        CONFIG_INIT_VALUE(frogCount);
    )
)