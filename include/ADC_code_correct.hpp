#pragma once

#ifndef _ADC_CODE_CORRECT_
#define _ADC_CODE_CORRECT_

#include "system.hpp"
#include "my_lib.hpp"

void corr_cali_input(std::string cali_file, Double_t(*&ADC_data_corr)[wave_length]);
void ADC_code_correct(Double_t(&ADC_data)[wave_length], Int_t Stop, Double_t ADC_data_corr[wave_length]);

#endif //

