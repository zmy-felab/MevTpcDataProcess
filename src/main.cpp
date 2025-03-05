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
std::string noise_data_dictionary = work_dictionary + "20250304_noise_MMs/noise_data_range_0_gain_0_1404/";
std::string gain_cali_dictionary = work_dictionary + "gain_cali_data_range_0_gain_0_ii_FE2/";
std::string experiment_data_dictionary = work_dictionary + "experiment_data/";
std::string plot_data_dictionary = work_dictionary + "plot_data/";
std::string test_data_dictionary = work_dictionary + "test_data/";

int main()
{	
	// ADC code calibration
	// raw_data_read_1v0(correct_cali_dictionary, "ADC_code_calibration", ""); // option = "ADC_code_calibration" or "ADC_code_correction"
    // ADC_code_correct_cali(correct_cali_dictionary);

	// noise analysis
	// raw_data_read_1v0(noise_data_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
    // noise_analysis(noise_data_dictionary, gain_cali_dictionary);

	// gain calibration
	//raw_data_read_1v0(gain_cali_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	//info_calculate(gain_cali_dictionary, "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	//std::vector<Double_t> Q = {20,30,40,50,60,70,80,90,100,110,120,130,140};
	//gain_cali(gain_cali_dictionary, Q);
	
	// get calibration spectrum
	// raw_data_read_1v0(gain_cali_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(gain_cali_dictionary, "Q_calculate", gain_cali_dictionary); // option = "gain_cali" or "Q_calculate"
	// get_calibration_spectrum(gain_cali_dictionary);
	
	// get experimental data spectrum
	// raw_data_read_1v0(experiment_data_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(experiment_data_dictionary, "Q_calculate", gain_cali_dictionary); // option = "gain_cali" or "Q_calculate"
	// get_Q_spectrum(test_data_dictionary);
	// SpectrumPlot(test_data_dictionary + "spec_data/", "spectrum.root");
	
	// plot
	// raw_data_read_1v0(plot_data_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// wavePlot(noise_data_dictionary, "TPC_FEE_range_0_shape_7_gain_0_thre32_noise_noise_FE1234_20250304113954_0.root", 20);

	std::cout << "Finish!" << std::endl;
    return 0;

}
