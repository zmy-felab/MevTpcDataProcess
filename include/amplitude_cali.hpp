#pragma once
#include "main.hpp"

Double_t fitf(Double_t* x, Double_t* par);
void linearFit(std::vector<Double_t> xdata, std::vector<Double_t> ydata, Int_t N);
void amplitude_cali(std::string test_data_dictionary);