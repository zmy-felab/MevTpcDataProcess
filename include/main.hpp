#pragma once

// system .h file
#include <sys/io.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cmath>

// cernroot .h file
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TObject.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TSpectrum.h"
#include "TMath.h"
#include "TRandom.h"
#include "TROOT.h"

#define wave_length 128
#define channel_num 1024

//user define .h file
#include "raw_data_read_1v0.hpp"
#include "info_calculate.hpp"
#include "ADCcode_correct_cali.hpp"
#include "data_correct.hpp"
#include "noise_analysis.hpp"
#include "amplitude_cali.hpp"
#include "gain_correct.hpp"
#include "get_spectrum.hpp"
#include "plot.hpp"

//class file
#include "my_class.hpp" 



