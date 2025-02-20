#pragma once
#include "main.hpp"

void corr_cali_input(std::string cali_file, Double_t(*&ADC_data_corr)[wave_length]);
void data_correct(Double_t(&ADC_data)[wave_length], Int_t Stop, Double_t ADC_data_corr[wave_length]);