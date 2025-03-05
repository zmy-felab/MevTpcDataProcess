// created: 2025/02/09 by Zhao Maoyuan

#include "my_lib.hpp"
#include "plot.hpp"
#include "gain_cali.hpp"

// Get gain of each channel
void gain_cali(std::string gain_cali_dictionary, std::vector<Double_t> Q)
{
    // Set data path
    std::string event_data_dictionary = gain_cali_dictionary + "event_data/";
    std::string linear_plot_dictionary = gain_cali_dictionary + "linear_plot/";
    std::cout << "Start amplitude calibration with data files at path:" << event_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(gain_cali_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << gain_cali_dictionary << " is not set!" << std::endl;
        exit(1);
    }

    // get names of event files
    std::vector<std::string> file_names = get_file_names(event_data_dictionary);
    Int_t file_num = file_names.size();
    if (file_num == 0)
    {
        std::cout << "There is no event files in the path: " << event_data_dictionary << std::endl;
        exit(1);
    }

    // create the folders if not exist
    if (access(linear_plot_dictionary.c_str(), 0) == -1)
    {
        #if defined(_WIN32)
            mkdir(linear_plot_dictionary.c_str());
        #else
		    mkdir(linear_plot_dictionary.c_str(), 0777);
        #endif

        std::cout << "The path " << linear_plot_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
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

            if (event_id >= 1293)
            {
                continue;
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
    
    
    // output gain.txt
    std::ofstream dataFile;
    dataFile.open(gain_cali_dictionary + "gain.txt"); // 打开文件，如果不存在则创建

    for (Int_t i = 0; i < channel_num; i++)
    {   
        if (amplitude_average[i].size() != 0) 
        {
            if(amplitude_average[i].size() > Q.size())
			{
                // erase the amplitude less than 30
                for (std::vector<Double_t>::iterator it = amplitude_average[i].begin(); it != amplitude_average[i].end();) {
                    if (*it < 30)
                    {
                        it = amplitude_average[i].erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
			}
            if (amplitude_average[i].size() == Q.size()) 
            {
                // linear fit and get parameter
                Double_t par[2];
                linearFitDraw(Q, amplitude_average[i], amplitude_average[i].size(), par[0], par[1], linear_plot_dictionary + "FE_" + std::to_string((i / 64)) + "chn_" + std::to_string((i % 64)) + "_gain_cali.png");
                dataFile << (Int_t)(i / 64) << " " << (i % 64) << " " << par[0] << " " << par[1] << std::endl; // 写入数据
                
            }            
        }
    }
    dataFile.close(); // 关闭文件

    std::cout << "amplitude calibration has been completed successfully!" << std::endl;
}
    