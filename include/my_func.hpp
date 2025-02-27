#pragma once

#ifndef _GENERAL_FUNC_
#define _GENERAL_FUNC_

#include "system.hpp"

std::vector<std::string> get_file_names(const std::string& directory);
Double_t calculate_RMS(std::vector<Double_t> waveform);
Double_t get_average(std::vector<Double_t> waveform);
Double_t fitf(Double_t* x, Double_t* par);
void linearFit(std::vector<Double_t> xdata, std::vector<Double_t> ydata, Int_t N, Double_t(&par)[2]);
void draw_histogram(Double_t* data, Int_t size, std::string fig_name);
#endif