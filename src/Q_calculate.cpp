// created: 2025/02/10 by Zhao Maoyuan
#include "my_lib.hpp"
#include "Q_calculate.hpp"

void gain_input(std::string cali_file, Double_t* &gain, Double_t* &intercept)
{
	Double_t* gain_temp = new Double_t[channel_num * 4]();

	// load "gain.txt"
	std::ifstream cali_file_in(cali_file);
	if (!cali_file_in.is_open()) 
	{
		std::cout << "Error: There is no gain.txt files at path: " << cali_file << std::endl;
		exit(1);
	}

	for (Int_t i = 0; i < channel_num * 4; i++) 
	{
		if (!cali_file_in.eof()) 
		{
			cali_file_in >> gain_temp[i];
		}
		else
		{
			break;
		}		
	}
	cali_file_in.close();

	Int_t FE_ID;
	Int_t CH_ID;

	for (Int_t i = 0; i < channel_num; i++) {
		if (gain_temp[i * 4 + 2] != 0) {
			FE_ID = (Int_t)gain_temp[i * 4];
			CH_ID = (Int_t)gain_temp[i * 4 + 1];
			gain[FE_ID * 64 + CH_ID] = gain_temp[i * 4 + 2];
			intercept[FE_ID * 64 + CH_ID] = gain_temp[i * 4 + 3];
		}
		else
		{
			break;
		}	
	}
}

Double_t Q_calculate(Int_t FE_ID, Int_t CH_ID, Double_t gain[channel_num], Double_t intercept[channel_num], Double_t amplitude)
{	
	return (amplitude - intercept[FE_ID * 64 + CH_ID]) / gain[FE_ID * 64 + CH_ID];
}