#pragma once

#ifndef _Q_CALCULATE_
#define _Q_CALCULATE_

#include "system.hpp"

void gain_input(std::string cali_file, Double_t*& gain, Double_t*& intercept);
Double_t Q_calculate(Int_t FE_ID, Int_t CH_ID, Double_t gain[channel_num], Double_t intercept[channel_num], Double_t amplitude);

#endif // !_Q_CALCULATE_

