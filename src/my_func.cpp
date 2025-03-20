#include "my_func.hpp"

// Get file names in a folder.
std::vector<std::string> get_file_names(const std::string& directory) {
    std::vector<std::string> file_names;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".root") {
            file_names.push_back(entry.path().filename().string());
        }
    }
    return file_names;
}

// Get average value of waveform
Double_t get_average(std::vector<Double_t> waveform)
{
    Double_t sum = 0;
    Int_t N = waveform.size();
    for (Int_t i = 0; i < N; i++) {
        sum += waveform[i];
    }
    return sum / N;
}

// Get RMS of waveform
Double_t calculate_RMS(std::vector<Double_t> waveform)
{
    Double_t sum = 0;
    Double_t average = 0;
    Int_t N = waveform.size();
    average = get_average(waveform);
    for (Int_t i = 0; i < N; i++) {
        sum += pow(waveform[i] - average, 2);
    }
    return sqrt(sum / N);
}

void linearFit(std::vector<Double_t> x_data, std::vector<Double_t> y_data, Int_t fitNum, Double_t& slope, Double_t& intercept) {

    // Calculate the minimum number of points
    Int_t Nmin = TMath::Min(x_data.size(), y_data.size());

    Double_t* xdata = new Double_t[Nmin];
    Double_t* ydata = new Double_t[Nmin];

    for (Int_t i = 0; i < Nmin; i++)
    {
        xdata[i] = x_data[i];
        ydata[i] = y_data[i];
    }

    // Create a graph for the data points
    TGraph* graph = new TGraph(fitNum, xdata, ydata);
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(kBlue);
    graph->SetTitle("Linear Fit;X;Y");

    // Perform the linear fit
    TF1* fitFunc = new TF1("fitFunc", "[0]*x + [1]", xdata[0], xdata[fitNum-1]);
    graph->Fit(fitFunc, "Q");

    // Retrieve the fit parameters
    slope = fitFunc->GetParameter(0);
    intercept = fitFunc->GetParameter(1);

    // Clean up
    delete graph;
    delete fitFunc;
}

void GaussianFit(std::vector<Double_t> data, Double_t& mean, Double_t& sigma, Double_t& amplitude) {
    
    TH1D* histogram = new TH1D("histogram", "Data Distribution", 1000, 0, 1000);

    for (Int_t i = 0; i < data.size(); ++i) {
        histogram->Fill(data[i]);
    }

    // Find the bin with the maximum content
    int maxBin = histogram->GetMaximumBin();
    double maxBinCenter = histogram->GetBinCenter(maxBin);
    double maxBinContent = histogram->GetBinContent(maxBin);

    // Define the range for the fit around the maximum bin
    double fitRangeLow = maxBinCenter - 2 * histogram->GetRMS();
    double fitRangeHigh = maxBinCenter + 2 * histogram->GetRMS();

    // Create a Gaussian function for fitting
    TF1* gaussFit = new TF1("gaussFit", "gaus", fitRangeLow, fitRangeHigh);
    gaussFit->SetParameters(maxBinContent, maxBinCenter, histogram->GetRMS());

    // Perform the fit
    histogram->Fit(gaussFit, "R");

    // Retrieve the fit parameters
    amplitude = gaussFit->GetParameter(0);
    mean = gaussFit->GetParameter(1);
    sigma = gaussFit->GetParameter(2);

    // Clean up
    delete gaussFit;
}

