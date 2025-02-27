#pragma once

#ifndef _PLOT_
#define _PLOT_

#include "system.hpp"

Double_t fitf(Double_t* x, Double_t* par);
void linearFit(std::vector<Double_t> xdata, std::vector<Double_t> ydata, Int_t N, Double_t(&par)[2]);
void draw_histogram(Double_t* data, Int_t size, std::string fig_name);
void plot(std::string root_file_dic, std::string root_file_name, Int_t N);

#endif
