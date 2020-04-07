#pragma once

enum config_type { debug_token = 0, debug_ast = 1 };

bool is_config_set(config_type cfg);
void set_config(config_type cfg);
