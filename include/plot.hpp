#pragma once

#ifndef _PLOT_
#define _PLOT_

#include "system.hpp"

void linearFitDraw(std::vector<Double_t> x_data, std::vector<Double_t> y_data, Int_t fitNum, Double_t& slope, Double_t& intercept, std::string fig_name);
void GaussianDraw(std::vector<Double_t> data, Double_t& mean, Double_t& sigma, Double_t& amplitude, std::string fig_name);
void wavePlot(std::string plot_data_dictionary, std::string root_file_name, Int_t N);
void SpectrumPlot(std::string plot_data_dictionary, std::string root_file_name, std::string fit_option, Double_t fitRangeLow, Double_t fitRangeHigh);
void NoiseDistributionPlot(std::string noise_data_dictionary, std::string root_file_name);
void StripNumDistributionPlot(std::string event_data_dictionary, std::string event_file_name);
void plotXYHits(std::string fileName);

#endif
