#pragma once

#ifndef _GENERAL_FUNC_
#define _GENERAL_FUNC_

#include "system.hpp"

std::vector<std::string> get_file_names(const std::string& directory);
Double_t calculate_RMS(std::vector<Double_t> waveform);
Double_t get_average(std::vector<Double_t> waveform);
void linearFit(std::vector<Double_t> x_data, std::vector<Double_t> y_data, Int_t fitNum, Double_t& slope, Double_t& intercept);
void GaussianFit(std::vector<Double_t> data, Double_t& mean, Double_t& sigma, Double_t& amplitude);

#endif