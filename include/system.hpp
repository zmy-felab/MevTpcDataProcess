#pragma once

#if defined(_WIN32)
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

// C++ standard library
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>

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