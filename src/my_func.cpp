#include "my_func.hpp"

// Get file names in a folder.
std::vector<std::string> get_file_names(const std::string& directory) {
    std::vector<std::string> file_names;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
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

// function of fit
Double_t fitf(Double_t* x, Double_t* par)
{
    return par[0] * x[0] + par[1];
}

// linear fit of amplitude
void linearFit(std::vector<Double_t> xdata, std::vector<Double_t> ydata, Int_t N, Double_t(&par)[2])
{
    Double_t* x_data = new Double_t[N];
    Double_t* y_data = new Double_t[N];

    for (Int_t i = 0; i < N; i++)
    {
        x_data[i] = xdata[i];
        y_data[i] = ydata[i];
    }

    // fit
    // TF1* func = new TF1("<name>", <function>, MinX,MaxX, nPar);
    TF1* func = new TF1("func", fitf, 1, 10, 2);
    func->SetParameters(0, 0);
    func->SetParNames("Gain", "Intercept");
    TGraph* gr1 = new TGraph(N, x_data, y_data);
    gr1->Fit("func");
    func->GetParameters(par);
    delete gr1;
}

void draw_histogram(Double_t* data, Int_t size, std::string fig_name) {

    TCanvas* c1 = new TCanvas("canvas", "Histogram", 800, 600);
    TH1D* h1 = new TH1D("hist", "Data Distribution", 1000, 0, 1000);

    for (Int_t i = 0; i < size; ++i) {
        h1->Fill(data[i]);
    }

    c1->cd();
    h1->Draw();

    c1->Update();
    c1->SaveAs(fig_name.c_str());

}