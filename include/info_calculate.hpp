#pragma once

#ifndef _INFO_CALCULATE_
#define _INFO_CALCULATE_

#include "system.hpp"

void ch_to_strip(std::vector<Int_t> czt_lookup, Int_t FE_ID, Int_t CH_ID, Int_t& strip_xy, Int_t& strip_id);
void info_calculate(std::string test_data_dictionary, std::string Q_calculate_option, std::string gain_cali_dictionary);

#endif
