// created: 2025/02/08 by Zhao Maoyuan

#include "noise_analysis.hpp"

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

// Noise analysis
void noise_analysis(std::string noise_data_dictionary)
{
    // Set data path
    std::string raw_data_dictionary = noise_data_dictionary + "raw_data/";
    std::string root_data_dictionary = noise_data_dictionary + "root_data/";

    std::cout << "Start noise analysis with noise data files at path:" << raw_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(noise_data_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << noise_data_dictionary << " is not set!" << std::endl;
        exit(1);
    }

    // read raw data to root data
    raw_data_read_1v0(noise_data_dictionary);

    // create the folders if not exist
    if (access(root_data_dictionary.c_str(), 0) == -1)
    {
        mkdir(root_data_dictionary.c_str(), 0777);
        std::cout << "The path " << root_data_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
    }

    // load correct_cali file
    std::string cali_file = noise_data_dictionary + "../correct_cali_data/correct_cali.txt";
    Double_t (*ADC_data_corr)[wave_length] = new Double_t[channel_num][wave_length]();
    corr_cali_input(cali_file, ADC_data_corr);

    // get names of root files
    std::vector<std::string> file_names = get_file_names(root_data_dictionary);
    Int_t file_num = file_names.size();
    if (file_num == 0)
    {
        std::cout << "There is no .root files in the path: " << root_data_dictionary << std::endl;
        exit(1);
    }

    // define the Intermediate variable
    std::string* raw_root_filename = new std::string[file_num];
    std::vector<std::vector<Double_t>> noise_data(channel_num);

    for (Int_t file_index = 0; file_index < file_num; file_index++)
    {
        raw_root_filename[file_index] = root_data_dictionary + file_names[file_index];
        std::cout << "Processing:" << raw_root_filename[file_index] << std::endl;

        // Open the root file
        TFile* fp = new TFile(raw_root_filename[file_index].c_str());
        std::cout << raw_root_filename[file_index] << " was opened successfully" << std::endl;

        pulse_data pulse;

        // Get the tree and set the branch
        TTree* raw_tree = (TTree*)fp->Get("raw_tree");
        raw_tree->SetBranchAddress("FE_ID", &pulse.FE_ID);
        raw_tree->SetBranchAddress("CH_ID", &pulse.CH_ID);
        raw_tree->SetBranchAddress("Stop", &pulse.Stop);
        raw_tree->SetBranchAddress("Event_Num", &pulse.Event_Num);
        raw_tree->SetBranchAddress("ADC_Data", pulse.ADC_Data);;

        // Get entries
        Int_t pulse_entries = raw_tree->GetEntries();
        if (pulse_entries <= 0)
        {
            std::cout << "error: Pulse_entries can not be 0!" << std::endl;
            return;
        }
        std::cout << "Entries of the file :" << pulse_entries << std::endl;

        for (Int_t pulse_id = 0; pulse_id < pulse_entries; pulse_id++)
        {
            // Get a pulse
            raw_tree->GetEntry(pulse_id);
            if (pulse_id % 10000 == 0)
            {
                std::cout << "Processed: " << pulse_id * 100 / pulse_entries << "%" << std::endl;
            }
          
            // ADC code correct
            Double_t ADC_data_double[wave_length];
            for (Int_t i = 0; i < wave_length; i++)
            {
                ADC_data_double[i] = (Double_t)pulse.ADC_Data[i];
            }
            data_correct(ADC_data_double, pulse.Stop, ADC_data_corr[pulse.FE_ID * 64 + pulse.CH_ID]);

            // noise data splicing
            for (Int_t i = 0; i < wave_length; i++)
            {
                noise_data[pulse.FE_ID * 64 + pulse.CH_ID].push_back(ADC_data_double[i]);
            }
        }

        // close root file
        fp->Close();
        std::cout << "Have closed the file:" << raw_root_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;
       
    }

    // define tree
    Double_t noise;
    TTree* noise_tree = new TTree("noise_tree", "noise of every channel");
    noise_tree->Branch("noise", &noise, "noise_RMS/D");

    // calculate noise
    Double_t noise_RMS[channel_num];
    for (Int_t i = 0; i < channel_num; i++) {
        noise_RMS[i] = calculate_RMS(noise_data[i]);
        noise = noise_RMS[i]/4.6;
        if (!isnan(noise)) {
            noise_tree->Fill();
        }
    }

    // write to .root
    TFile* fp2 = new TFile((noise_data_dictionary + "noise.root").c_str(), "recreate");
    fp2->cd();
    noise_tree->Write();
    std::cout << noise_data_dictionary << "noise.root has been written!\n" << std::endl;
    fp2->Close();
    delete fp2;

    // write to .txt
    std::ofstream dataFile;
    dataFile.open(noise_data_dictionary + "noise.txt");
    for (Int_t i = 0; i < channel_num; i++)
    {
        dataFile << std::fixed << std::setprecision(3) << noise_RMS[i] << std::endl << "\n"; 
    }
    dataFile.close();
    std::cout << noise_data_dictionary << "noise.txt has been written!\n" << std::endl;

    std::cout << "noise analysis has been completed successfully!" << std::endl;

    delete[] *ADC_data_corr;
    delete[] raw_root_filename;

}