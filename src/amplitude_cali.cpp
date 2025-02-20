// created: 2025/02/09 by Zhao Maoyuan

#include "amplitude_cali.hpp"

// function of fit
Double_t fitf(Double_t* x, Double_t* par)
{
    return par[0] * x[0] + par[1];
}

// linear fit of amplitude
void linearFit(std::vector<Double_t> xdata, std::vector<Double_t> ydata, Int_t N, Double_t (&par)[2])
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

// Get gain of each channel
void amplitude_cali(std::string amplitude_cali_dictionary)
{
    // Set data path
    std::string event_data_dictionary = amplitude_cali_dictionary + "event_data/";

    std::cout << "Start amplitude calibration with data files at path:" << event_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(amplitude_cali_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << amplitude_cali_dictionary << " is not set!" << std::endl;
        exit(1);
    }

    // calculate event info of amplitude calibration data
    info_calculate(amplitude_cali_dictionary);

    // get names of event files
    std::vector<std::string> file_names = get_file_names(event_data_dictionary);
    Int_t file_num = file_names.size();
    if (file_num == 0)
    {
        std::cout << "There is no event files in the path: " << event_data_dictionary << std::endl;
        exit(1);
    }

    // define the Intermediate variable
    std::string* event_filename = new std::string[file_num];
    std::vector<std::vector<Double_t>> amplitude_average(channel_num);

    for (Int_t file_index = 0; file_index < file_num; file_index++)
    {
        std::vector<std::vector<Double_t>> amplitude(channel_num);
        event_filename[file_index] = event_data_dictionary + file_names[file_index];

        std::cout << "Processing:" << event_filename[file_index] << std::endl;

        // Open the root file
        TFile* fp = new TFile(event_filename[file_index].c_str());
        std::cout << event_filename[file_index] << " was opened successfully" << std::endl;

        event_data event_data_object;

        // Get the tree and set the branch
        TTree* event_tree = (TTree*)fp->Get("event_tree");
        event_tree->SetBranchAddress("strip_num", &event_data_object.strip_num);
        event_tree->SetBranchAddress("FE_ID", event_data_object.FE_ID);
        event_tree->SetBranchAddress("CH_ID", event_data_object.CH_ID);
        event_tree->SetBranchAddress("strip_amplitude", event_data_object.strip_amplitude);

        // Get entries
        Int_t event_entries = event_tree->GetEntries();
        if (event_entries <= 0)
        {
            std::cout << "error:event_entries can not be 0!" << std::endl;
            return;
        }
        std::cout << "Entries of the file :" << event_entries << std::endl;

        for (Int_t event_id = 0; event_id < event_entries; event_id++)
        {
            // Get an event
            event_tree->GetEntry(event_id);

            // Push every amplitude of one channel with the input signal of fixed amplitude in amplitude[]
            for (Int_t i = 0; i < event_data_object.strip_num; i++)
            {
                amplitude[event_data_object.FE_ID[i] * 64 + event_data_object.CH_ID[i]].push_back(event_data_object.strip_amplitude[i]);
            }
        }

        // close event file
        fp->Close();
        std::cout << "Have closed the file:" << event_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;

        // Get average value of every amplitude of one channel with the input signal of fixed amplitude in amplitude[]
        for (Int_t i = 0; i < channel_num; i++) 
        {
            if (amplitude[i].size() != 0) 
            {
                amplitude_average[i].push_back(get_average(amplitude[i]));
            }
        }
    }
    delete[] event_filename;
    
    // Q input
    std::vector<Double_t> Q = {20,30,40,50,60,70,80,90,100,110,120,130,140};
    for (Int_t i = 0; i < channel_num; i++)
    {   
        if (amplitude_average[i].size() != 0) {

            // linear fit and get parameter
			Double_t par[2];
            linearFit(Q, amplitude_average[i], amplitude_average[i].size(), par);

            // output gain.txt
            std::ofstream dataFile;
            dataFile.open(amplitude_cali_dictionary + "gain.txt", std::ios::app); // ���ļ�������������򴴽�
            dataFile << (Int_t)(i / 64) << " " << (i % 64) << " " << par[0] << " " << par[1] << std::endl; // д������
            dataFile.close(); // �ر��ļ�
        }
    }

    std::cout << "amplitude calibration has been completed successfully!" << std::endl;
}
    