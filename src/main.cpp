// created: 2025/01/31 by Zhao Maoyuan

// standard library
#include "system.hpp"

// user define .h file
#include "raw_data_read_1v0.hpp"
#include "ADC_code_correct_cali.hpp"
#include "noise_analysis.hpp"
#include "info_calculate.hpp"
#include "gain_cali.hpp"
#include "get_spectrum.hpp"
#include "plot.hpp"

// std::string work_dictionary = "D:/lab/MeV_TPC_data_process_1V0/data/";
std::string work_dictionary = "G:/MeV/test/data/";

std::string correct_cali_dictionary = work_dictionary + "correct_cali_data/";
std::string noise_data_dictionary = work_dictionary + "noise_data_range_0_gain_0/";
std::string gain_cali_dictionary = work_dictionary + "gain_cali_data_range_0_gain_0/";
std::string test_data_dictionary = work_dictionary + "test_data/";
std::string plot_data_dictionary = work_dictionary + "plot_data/";

int main()
{	
	// ADC code calibration
	// raw_data_read_1v0(correct_cali_dictionary, "ADC_code_calibration", ""); // option = "ADC_code_calibration" or "ADC_code_correction"
    // ADCcode_correct_cali(correct_cali_dictionary);

	// noise analysis
	// raw_data_read_1v0(noise_data_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
    // noise_analysis(noise_data_dictionary);

	// gain calibration
	// raw_data_read_1v0(work_dictionary + "gain_cali_data_range_1_gain_16/", "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(work_dictionary + "gain_cali_data_range_1_gain_16/", "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	// raw_data_read_1v0(work_dictionary + "gain_cali_data_range_1_gain_31/", "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(work_dictionary + "gain_cali_data_range_1_gain_31/", "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	// raw_data_read_1v0(work_dictionary + "gain_cali_data_range_3_gain_0/", "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(work_dictionary + "gain_cali_data_range_3_gain_0/", "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	// raw_data_read_1v0(work_dictionary + "gain_cali_data_range_3_gain_16/", "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(work_dictionary + "gain_cali_data_range_3_gain_16/", "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	// raw_data_read_1v0(work_dictionary + "gain_cali_data_range_3_gain_31/", "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(work_dictionary + "gain_cali_data_range_3_gain_31/", "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	// std::vector<Double_t> Q = {20,30,40,50,60,70,80,90,100,110,120,130,140};
	// gain_cali(gain_cali_dictionary, Q);

	// get calibration spectrum
	// raw_data_read_1v0(work_dictionary + "gain_cali_data_range_1_gain_16/", "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(gain_cali_dictionary, "Q_calculate", gain_cali_dictionary); // option = "gain_cali" or "Q_calculate"
	// get_calibration_spectrum(gain_cali_dictionary);
	
	// get test spectrum
	// raw_data_read_1v0(work_dictionary + "gain_cali_data_range_1_gain_16/", "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(work_dictionary + "gain_cali_data_range_1_gain_16/", "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	// get_Q_spectrum(test_data_dictionary);
	
	// plot
	// plot(plot_data_dictionary, "TPC_FEE_range_3_shape_7_gain_28_thre_32_chn_0_ampl_500mV_gain_cali_FE1_20250221164854_0.root", 1);

	std::cout << "Finish!" << std::endl;
    return 0;

}
