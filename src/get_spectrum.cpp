// created: 2025/02/10 by Zhao Maoyuan
#include "my_lib.hpp"
#include "get_spectrum.hpp"
#include "plot.hpp"

void get_Q_spectrum(std::string test_data_dictionary)
{
    // Set data path
    std::string event_data_dictionary = test_data_dictionary + "event_data/";
    std::string spec_data_dictionary = test_data_dictionary + "spec_data/";

    std::cout << "Start getting spectrum with data files at path:" << event_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(test_data_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << test_data_dictionary << " is not set!" << std::endl;
        exit(1);
    }

    // create the folders if not exist
    if (access(spec_data_dictionary.c_str(), 0) == -1)
    {
        #if defined(_WIN32)
            mkdir(spec_data_dictionary.c_str());
        #else
            mkdir(spec_data_dictionary.c_str(), 777);
        #endif
        
        std::cout << "The path " << spec_data_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
    }

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
    std::string spec_filename = spec_data_dictionary + "spectrum.root";

    // define tree
    spectrum_data spectrum_data_object;
    TFile* fp2 = new TFile(spec_filename.c_str(), "recreate");
    TTree* Q_tree = new TTree("Q_tree", "Q of every event");
    Q_tree->Branch("Q", &spectrum_data_object.Q, "Q/D");
    Q_tree->Branch("event_id", &spectrum_data_object.event_id, "event_id/I");

    for (Int_t file_index = 0; file_index < file_num; file_index++)
    {
        event_filename[file_index] = event_data_dictionary + file_names[file_index];
        std::cout << "Processing:" << event_filename[file_index] << std::endl;

        // Open the event file
        TFile* fp = new TFile(event_filename[file_index].c_str());
        std::cout << event_filename[file_index] << " was opened successfully" << std::endl;

        // Get the tree and set the branch
        event_data event_data_object;
        TTree* event_tree = (TTree*)fp->Get("event_tree");
        event_tree->SetBranchAddress("strip_num", &event_data_object.strip_num);
        event_tree->SetBranchAddress("FE_ID", event_data_object.FE_ID);
        event_tree->SetBranchAddress("CH_ID", event_data_object.CH_ID);
        event_tree->SetBranchAddress("strip_xy", event_data_object.strip_xy);
        event_tree->SetBranchAddress("strip_id", event_data_object.strip_id);
        event_tree->SetBranchAddress("event_id", &event_data_object.event_id);
        event_tree->SetBranchAddress("strip_amplitude", event_data_object.strip_amplitude);
		event_tree->SetBranchAddress("strip_Q", event_data_object.strip_Q);
        event_tree->SetBranchAddress("strip_timing", event_data_object.strip_timing);

        // Get entries
        Int_t event_entries = event_tree->GetEntries();
        if (event_entries <= 0)
        {
            std::cout << "error: Event entries can not be 0!" << std::endl;
            return;
        }
        std::cout << "Entries of the file :" << event_entries << std::endl;

        for (Int_t event_id = 0; event_id < event_entries; event_id++)
        {
            // Get an event
            event_tree->GetEntry(event_id);
            spectrum_data_object.event_id = event_data_object.event_id;
			spectrum_data_object.Q = 0;

            // correct gain of Q of all channels
			for (Int_t i = 0; i < event_data_object.strip_num; i++) 
            {
				spectrum_data_object.Q += event_data_object.strip_Q[i];
			}
            Q_tree->Fill();
        }

        // Close the file
        fp->Close();
        std::cout << "Have closed the file:" << event_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;
    }

    delete[] event_filename;

    // Open spec file
    fp2->cd();
    Q_tree->Write();
    fp2->Close();
    delete fp2;
    std::cout << spec_filename << " has been written!\n" << std::endl;

    std::cout << "Spectrum calculation has been completed successfully!" << std::endl;
}

void get_calibration_spectrum(std::string test_data_dictionary)
{
    // Set data path
    std::string event_data_dictionary = test_data_dictionary + "event_data/";
    std::string spec_data_dictionary = test_data_dictionary + "spec_data/";

    std::cout << "Start getting spectrum with data files at path:" << event_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(test_data_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << test_data_dictionary << " is not set!" << std::endl;
        exit(1);
    }

    // create the folders if not exist
    if (access(spec_data_dictionary.c_str(), 0) == -1)
    {
        #if defined(_WIN32)
            mkdir(spec_data_dictionary.c_str());
        #else
            mkdir(spec_data_dictionary.c_str(), 777);
        #endif

        std::cout << "The path " << spec_data_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
    }

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
    
    for (Int_t file_index = 0; file_index < file_num; file_index++)
    {
        event_filename[file_index] = event_data_dictionary + file_names[file_index];
        std::cout << "Processing:" << event_filename[file_index] << std::endl;

        Int_t chn_ID;
        Int_t chn_ID_start = event_filename[file_index].find("chn_") + 4;
        if (event_filename[file_index][chn_ID_start + 1] == '_')
        {
            std::string str_chn_ID = event_filename[file_index].substr(chn_ID_start, 1);
            chn_ID = std::stoi(str_chn_ID);
        }
        else
        {
            std::string str_chn_ID = event_filename[file_index].substr(chn_ID_start, 2);
            chn_ID = std::stoi(str_chn_ID);
        }

        std::vector <Double_t> amplitude_chip0;
		std::vector <Double_t> amplitude_chip1;
        

        // Open the event file
        TFile* fp = new TFile(event_filename[file_index].c_str());
        std::cout << event_filename[file_index] << " was opened successfully" << std::endl;

        // Get the tree and set the branch
        event_data event_data_object;
        TTree* event_tree = (TTree*)fp->Get("event_tree");
		event_tree->SetBranchAddress("strip_num", &event_data_object.strip_num);
		event_tree->SetBranchAddress("CH_ID", event_data_object.CH_ID);
        event_tree->SetBranchAddress("strip_Q", event_data_object.strip_Q);

        // Get entries
        Int_t event_entries = event_tree->GetEntries();
        if (event_entries <= 0)
        {
            std::cout << "error: Event entries can not be 0!" << std::endl;
            return;
        }
        std::cout << "Entries of the file :" << event_entries << std::endl;

        for (Int_t event_id = 0; event_id < event_entries; event_id++)
        {
            // Get an event
            event_tree->GetEntry(event_id);

            for (Int_t i = 0; i < event_data_object.strip_num; i++)
            {
                if (event_data_object.CH_ID[i] == chn_ID)
                    amplitude_chip0.push_back(event_data_object.strip_Q[i]);
                else if (event_data_object.CH_ID[i] == chn_ID + 32)
                    amplitude_chip1.push_back(event_data_object.strip_Q[i]);
                else
                    continue;
            }
        }

        // Draw histogram
        draw_histogram(amplitude_chip0.data(), amplitude_chip0.size(), spec_data_dictionary + file_names[file_index].substr(0, event_filename[file_index].size() - 5) + "_ch_" + std::to_string(chn_ID) + ".png");
		draw_histogram(amplitude_chip1.data(), amplitude_chip1.size(), spec_data_dictionary + file_names[file_index].substr(0, event_filename[file_index].size() - 5) + "_ch_" + std::to_string(chn_ID + 32) + ".png");

        // Close the file
        fp->Close();
        std::cout << "Have closed the file:" << event_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;
        
    }

    delete[] event_filename;

	std::cout << "Code spectrum calculation has been completed successfully!" << std::endl;

}