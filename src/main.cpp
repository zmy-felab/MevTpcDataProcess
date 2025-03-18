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
std::string noise_data_dictionary = work_dictionary + "20250313_noise_MMs/noise_data_range_3_gain_16_MMs_tb_shelter_HV0/";
std::string gain_cali_dictionary = work_dictionary + "20250313_FE_2/gain_cali_range_3_gain_31/";
std::string experiment_data_dictionary = work_dictionary + "20250312_experiment_MMs/range_3_gain_0_thre_68_HV840560_fe55_FE13/";
std::string plot_data_dictionary = work_dictionary + "gain_cali_data_range_3_gain_16/";

int main()
{	
	// ADC code calibration
	//raw_data_read_1v0(correct_cali_dictionary, "ADC_code_calibration", ""); // option = "ADC_code_calibration" or "ADC_code_correction"
 //   ADC_code_correct_cali(correct_cali_dictionary);

	// gain calibration
	//raw_data_read_1v0(gain_cali_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	//info_calculate(gain_cali_dictionary, "gain_cali", ""); // option = "gain_cali" or "Q_calculate"
	std::vector<Double_t> Q = {50,75,100,125,150,175,200,225,250,275,300,325};
	gain_cali(gain_cali_dictionary, Q);

	// noise analysis
	// raw_data_read_1v0(noise_data_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
    // noise_analysis(noise_data_dictionary, gain_cali_dictionary);

	// get calibration spectrum
	// raw_data_read_1v0(gain_cali_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	//info_calculate(gain_cali_dictionary, "Q_calculate", gain_cali_dictionary); // option = "gain_cali" or "Q_calculate"
	//get_calibration_spectrum(gain_cali_dictionary);
	
	// get experimental data spectrum
	// raw_data_read_1v0(experiment_data_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// info_calculate(experiment_data_dictionary, "Q_calculate", gain_cali_dictionary); // option = "gain_cali" or "Q_calculate"
	// get_Q_spectrum(experiment_data_dictionary);
	// SpectrumPlot(experiment_data_dictionary + "spec_data/", "spectrum.root");
	
	// plot
	// raw_data_read_1v0(plot_data_dictionary, "ADC_code_correction", correct_cali_dictionary); // option = "ADC_code_calibration" or "ADC_code_correction"
	// wavePlot(plot_data_dictionary, "TPC_FEE_range_3_shape_7_gain_16_thre_32_chn_0_ampl_1000mV_gain_cali_FE1_20250313110310_0.root", 20);

	std::cout << "Finish!" << std::endl;
    return 0;

}
