// created: 2025/01/31 by Zhao Maoyuan
#include "main.hpp"

// std::string work_dictionary = "D:/lab/MeV_TPC_data_process_1V0/data/";
std::string work_dictionary = "G:/MeV/test/data/";

std::string correct_cali_dictionary = work_dictionary + "correct_cali_data/";
std::string noise_data_dictionary = work_dictionary + "noise_data_range_0_gain_0/";
std::string amplitude_cali_dictionary = work_dictionary + "gain_cali_data/";
std::string test_data_dictionary = work_dictionary + "gain_cali_data/";
std::string plot_data_dictionary = work_dictionary + "gain_cali_data/";

int main()
{
    // ADCcode_correct_cali(correct_cali_dictionary);

    // noise_analysis(noise_data_dictionary);

	// amplitude_cali(amplitude_cali_dictionary);

	// get_spectrum(test_data_dictionary);

	// plot(plot_data_dictionary,"TPC_FEE_range_0_shape_7_gain_0_thre_32_chn_0_ampl_700mV_gain_cali_FE1_20250220144412_0.root", 1);

    return 0;

}
