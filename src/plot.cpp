// created: 2025/02/19 by Zhao Maoyuan

#include "my_lib.hpp"
#include "plot.hpp"

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

    TCanvas* c1 = new TCanvas("c1", "Histogram", 800, 600);
    TH1D* h1 = new TH1D("h1", "Data Distribution", 1000, 0, 1000);

    for (Int_t i = 0; i < size; ++i) {
        h1->Fill(data[i]);
    }

    c1->cd();
    h1->Draw();

    c1->Update();
    c1->SaveAs(fig_name.c_str());

    delete c1;
    delete h1;

}

// Import the data of the baseline test for DC correction and output the DC correction file
void plot(std::string plot_data_dictionary, std::string root_file_name, Int_t N)
{	
    // raw_data_read_1v0(plot_data_dictionary);

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
        c1->SetGrid();

        for (Int_t j = 0; j < wave_length; j++)
        {
            x_data[j] = j;
            y_data[j] = ADC_data[i][j];
        }

        TGraph* g = new TGraph(wave_length, x_data, y_data);
        c1->cd();
        g->Draw();

        std::string plot_name = plot_data_dictionary + root_file_name.substr(0, root_file_name.size() - 5) + "_" + std::to_string(i) + ".bmp";
        c1->Update();
        c1->Print();
        c1->SaveAs(plot_name.c_str());
    }

	// close event file
	fp->Close();
	std::cout << "Have closed the file:" << file_name << std::endl << "\n" << std::endl;

	delete fp;
}