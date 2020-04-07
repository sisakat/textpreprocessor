#include "configuration.h"

static unsigned char cfg_bits = 0;

bool is_config_set(config_type cfg) { return cfg_bits & (1 << (int)cfg) == 1; }

void set_config(config_type cfg) { cfg_bits = cfg_bits | (1 << (int)cfg); }
