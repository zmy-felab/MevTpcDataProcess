// created: 2025/02/19 by Zhao Maoyuan

#include "plot.hpp"

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