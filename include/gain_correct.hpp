#pragma once
#include "main.hpp"

void gain_corr_input(std::string cali_file, Double_t*& gain, Double_t*& intercept);
Double_t gain_correct(Int_t FE_ID, Int_t CH_ID, Double_t gain[channel_num], Double_t intercept[channel_num], Double_t amplitude);
