#pragma once

#ifndef _GENERAL_FUNC_
#define _GENERAL_FUNC_

#include "system.hpp"

std::vector<std::string> get_file_names(const std::string& directory);
Double_t calculate_RMS(std::vector<Double_t> waveform);
Double_t get_average(std::vector<Double_t> waveform);

#endif