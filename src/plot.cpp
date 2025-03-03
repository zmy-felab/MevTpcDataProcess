// created: 2025/02/19 by Zhao Maoyuan

#include "my_lib.hpp"
#include "plot.hpp"

void linearFitDraw(std::vector<Double_t> x_data, std::vector<Double_t> y_data, Int_t fitNum, Double_t& slope, Double_t& intercept, std::string fig_name) {

    // Calculate the minimum number of points
    Int_t Nmin = TMath::Min(x_data.size(),y_data.size());

	Double_t* xdata = new Double_t[Nmin];
	Double_t* ydata = new Double_t[Nmin];
    Double_t* residual = new Double_t[fitNum];

	for (Int_t i = 0; i < Nmin; i++)
	{
		xdata[i] = x_data[i];
		ydata[i] = y_data[i];
	}

    // Create a graph for the data points
    TGraph* graph = new TGraph(Nmin, xdata, ydata);
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(kBlue);
    graph->SetTitle("Linear Fit;X;Y");

    // Perform the linear fit
    TF1* fitFunc = new TF1("fitFunc", "[0]*x + [1]", xdata[0], xdata[fitNum - 1]);
    graph->Fit(fitFunc, "Q");

    // Retrieve the fit parameters
    slope = fitFunc->GetParameter(0);
    intercept = fitFunc->GetParameter(1);

    for (Int_t i = 0; i < fitNum; i++)
    {
        residual[i] = abs(ydata[i] - (slope * xdata[i] + intercept));
    }

    Double_t nonLinearFit = TMath::MaxElement(fitNum, residual) / ydata[fitNum - 1];

    // Create a canvas
    TCanvas* canvas = new TCanvas("canvas", "Linear Fit", 800, 600);
    graph->Draw("AP");
    graph->GetXaxis()->SetTitle("Charge/fC");
    graph->GetYaxis()->SetTitle("ADC Data/LSB");

    // Draw the fit line
    fitFunc->SetLineColor(kRed);
    fitFunc->Draw("same");

    // Add text to display the slope and intercept
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.03);
    latex.DrawLatex(0.15, 0.85, Form("Slope: %.3f", slope));
    latex.DrawLatex(0.15, 0.80, Form("Intercept: %.3f", intercept));
    latex.DrawLatex(0.15, 0.75, Form("Nonlinear Fit: %.3f", nonLinearFit));

    // Save the canvas as an image
    canvas->SaveAs(fig_name.c_str());

    // Clean up
    delete graph;
    delete fitFunc;
    delete canvas;
}

void GaussianDraw(std::vector<Double_t> data, Double_t& mean, Double_t& sigma, Double_t& amplitude, std::string fig_name) {
    
    TH1D* histogram = new TH1D("histogram", "", 1000, 0, 100);

    for (Int_t i = 0; i < data.size(); i++) {
        histogram->Fill(data[i]);
    }

    // Find the bin with the maximum content
    int maxBin = histogram->GetMaximumBin();
    double maxBinCenter = histogram->GetBinCenter(maxBin);
    double maxBinContent = histogram->GetBinContent(maxBin);

    // Define the range for the fit around the maximum bin
    double fitRangeLow = maxBinCenter - 3 * histogram->GetRMS();
    double fitRangeHigh = maxBinCenter + 3 * histogram->GetRMS();

    // Create a Gaussian function for fitting
    TF1* gaussFit = new TF1("gaussFit", "gaus", fitRangeLow, fitRangeHigh);
    gaussFit->SetParameters(maxBinContent, maxBinCenter, histogram->GetRMS());

    // Perform the fit
    histogram->Fit(gaussFit, "R");

    // Retrieve the fit parameters
    amplitude = gaussFit->GetParameter(0);
    mean = gaussFit->GetParameter(1);
    sigma = gaussFit->GetParameter(2);

    // Draw the histogram and the fit
    TCanvas* canvas = new TCanvas("canvas", "Gaussian Fit", 800, 600);
    // Set the axis labels and title
    histogram->GetXaxis()->SetTitle("Charge/fC");
    histogram->GetYaxis()->SetTitle("Counts");

    // Draw the histogram and the fit
    histogram->GetXaxis()->SetRangeUser(fitRangeLow, fitRangeHigh);
    histogram->Draw("");
    gStyle->SetOptStat(0);
    gaussFit->Draw("same");

    // Add text for Gaussian fit results
    TPaveText* text = new TPaveText(0.6, 0.7, 0.9, 0.9, "NDC");
    text->AddText(("Mean: " + std::to_string(mean).substr(0, (std::to_string(mean).find('.') + 4))).c_str());
    text->AddText(("Sigma: " + std::to_string(sigma).substr(0, (std::to_string(sigma).find('.') + 4))).c_str());
    text->AddText(("Amplitude: " + std::to_string(amplitude).substr(0, (std::to_string(amplitude).find('.') + 4))).c_str());
    text->Draw();

    // Save the canvas as an image
    canvas->SaveAs(fig_name.c_str());

    // Clean up
    delete canvas;
    delete gaussFit;
}

// Import the data of the baseline test for DC correction and output the DC correction file
void wavePlot(std::string plot_data_dictionary, std::string root_file_name, Int_t N){	
    // Open the root file
	std::string file_name = plot_data_dictionary + "root_data/" + root_file_name;
    TFile* fp = new TFile(file_name.c_str());
    std::cout << file_name << " was opened successfully" << std::endl;

    pulse_data pulse;

    // Get the tree and set the branch
    TTree* raw_tree = (TTree*)fp->Get("raw_tree");
    raw_tree->SetBranchAddress("FE_ID", &pulse.FE_ID);
    raw_tree->SetBranchAddress("CH_ID", &pulse.CH_ID);
    raw_tree->SetBranchAddress("ADC_Data", pulse.ADC_Data);

	Double_t ADC_data[10][wave_length];

    for (Int_t pulse_id = 0; pulse_id < N; pulse_id++)
    {
        // Get a pulse
        raw_tree->GetEntry(pulse_id);

		for (Int_t i = 0; i < wave_length; i++)
		{
			ADC_data[pulse_id][i] = pulse.ADC_Data[i];
		}
	}

    Double_t x_data[wave_length];
    Double_t y_data[wave_length];
    for (Int_t i = 0; i < N; i++)
    {   
        // Create a canvas
        TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);

        for (Int_t j = 0; j < wave_length; j++)
        {
            x_data[j] = j * 0.01;
            y_data[j] = ADC_data[i][j];
        }

        TGraph* g = new TGraph(wave_length, x_data, y_data);
        c1->cd();
        g->Draw();
        g->GetXaxis()->SetTitle("Time/us");
        g->GetYaxis()->SetTitle("ADC Data/LSB");

        std::string plot_name = plot_data_dictionary + root_file_name.substr(0, root_file_name.size() - 5) + "_wave" + std::to_string(i) + ".png";
        c1->Update();
        c1->Print();
        c1->SaveAs(plot_name.c_str());
    }

	// close event file
	fp->Close();
	std::cout << "Have closed the file:" << file_name << std::endl << "\n" << std::endl;

	delete fp;
}

void SpectrumPlot(std::string plot_data_dictionary, std::string root_file_name) {

    // Open the root file
	std::string file_name = plot_data_dictionary + root_file_name;
    TFile* fp = new TFile(file_name.c_str());
    std::cout << file_name << " was opened successfully" << std::endl;

    spectrum_data spectrum_data_object;

    // Get the tree and set the branch
    TTree* Q_tree = (TTree*)fp->Get("Q_tree");
    Q_tree->SetBranchAddress("Q", &spectrum_data_object.Q);
    Q_tree->SetBranchAddress("event_id", &spectrum_data_object.event_id);

    // Set up the histogram
    TH1D* hist = new TH1D("energy_spectrum", "Energy Spectrum;Energy (ADC);Counts", 10000, 0, 1000);

    // Fill the histogram
    Long64_t nEntries = Q_tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++) {
        Q_tree->GetEntry(i);
        hist->Fill(spectrum_data_object.Q);
    }

    // Draw the histogram
    TCanvas* canvas = new TCanvas("canvas", "Energy Spectrum", 800, 600);

    // Set the axis labels and title
    hist->GetXaxis()->SetTitle("Charge/fC");
    hist->GetYaxis()->SetTitle("Counts");
    gStyle->SetOptStat(0);

    // Draw the histogram and the fit
    int maxBin = hist->GetMaximumBin();
    double maxBinContent = hist->GetBinContent(maxBin);
    double threshold = 0.01 * maxBinContent;

    // Find the range for bins exceeding 1% of the maximum bin content
    int firstBin = maxBin;
    int lastBin = maxBin;
    for (int bin = 1; bin <= hist->GetNbinsX(); bin++) {
        if (hist->GetBinContent(bin) > threshold) {
            firstBin = bin;
            break;
        }
    }
    for (int bin = hist->GetNbinsX(); bin >= 1; bin--) {
        if (hist->GetBinContent(bin) > threshold) {
            lastBin = bin;
            break;
        }
    }
    
    // Set the range for the histogram
    hist->GetXaxis()->SetRange(firstBin * 0.9, lastBin * 1.1);
    hist->Draw();

    // Save the canvas as an image
    std::string plotName = std::string(file_name).substr(0, std::string(file_name).find_last_of('.')) + ".png";
    canvas->SaveAs(plotName.c_str());

    // Clean up
    fp->Close();
    delete fp;
    delete canvas;
}