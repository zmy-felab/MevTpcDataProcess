// created: 2025/02/04 by Zhao Maoyuan

#include "my_lib.hpp"
#include "ADC_code_correct.hpp"

// Import the correct_cali.txt for DC correction
void corr_cali_input(std::string cali_file, Double_t (*&ADC_data_corr)[wave_length])
{
	std::ifstream cali_file_in(cali_file);
	if (!cali_file_in.is_open()) {
		std::cout << "Error: There is no cali.txt file at path: " << cali_file << std::endl;
		exit(1);
	}
	for (Int_t i = 0; i < channel_num; i++) {
		for (Int_t j = 0; j < wave_length; j++) {
			cali_file_in >> ADC_data_corr[i][j];
		}
	}
	cali_file_in.close();
}

// DC correction
void ADC_code_correct(Double_t (&ADC_data)[wave_length], Int_t Stop, Double_t ADC_data_corr[wave_length])
{	
    for (Int_t i = 0; i < Stop; i++) {
        ADC_data[i] -= ADC_data_corr[Stop - 1 - i];
    }

    for (Int_t i = Stop; i < wave_length; i++) {
        ADC_data[i] -= ADC_data_corr[wave_length - 1 - i + Stop];
    }
}