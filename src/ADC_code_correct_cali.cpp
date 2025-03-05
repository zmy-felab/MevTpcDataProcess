// created: 2025/02/03 by Zhao Maoyuan

#include "my_lib.hpp"
#include "ADC_code_correct_cali.hpp"

// Import the data of the baseline test for DC correction and output the DC correction file
void ADC_code_correct_cali(std::string correct_cali_dictionary)
{   
    // Set data path
    std::string root_data_dictionary = correct_cali_dictionary + "root_data/";

    std::cout << "Start ADC code calibration with data files at path:" << root_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(correct_cali_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << correct_cali_dictionary << " is not set!" << std::endl;
        exit(1);
    }
    
    // create the folders if not exist
    if (access(root_data_dictionary.c_str(), 0) == -1)
    {
        #if defined(_WIN32)
            mkdir(root_data_dictionary.c_str());
        #else
            mkdir(root_data_dictionary.c_str(), 0777);
        #endif
        std::cout << "The path " << root_data_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
    }

    // get names of root files
	std::vector<std::string> file_names = get_file_names(root_data_dictionary);
	Int_t file_num = file_names.size();
    if (file_num == 0)
    {
        std::cout << "There is no .root files in the path: " << root_data_dictionary << std::endl;
        exit(1);
    }

    // define the Intermediate variable
    std::string* root_filename = new std::string[file_num];
    Int_t* pulse_entries_sum = new Int_t[channel_num]();
    Long_t (*ADC_data_sum)[wave_length] = new Long_t[channel_num][wave_length]();
    Int_t* ADC_data_sorted=new Int_t[wave_length]();

    for (Int_t file_index = 0; file_index < file_num; file_index++)
    {
        root_filename[file_index] = root_data_dictionary + file_names[file_index];
        std::cout << "Processing:" << file_names[file_index] << std::endl;

        // Open the root file
        TFile* fp = new TFile(root_filename[file_index].c_str());
        std::cout << root_filename[file_index] << " was opened successfully" << std::endl;

        pulse_data pulse;

        // Get the tree and set the branch
        TTree* raw_tree = (TTree*)fp->Get("raw_tree");
        raw_tree->SetBranchAddress("Packet_Size", &pulse.Packet_Size);
        raw_tree->SetBranchAddress("FE_ID", &pulse.FE_ID);
        raw_tree->SetBranchAddress("CH_ID", &pulse.CH_ID);
        raw_tree->SetBranchAddress("course", &pulse.course);
        raw_tree->SetBranchAddress("Q", &pulse.Q);
        raw_tree->SetBranchAddress("T", &pulse.T);
        raw_tree->SetBranchAddress("c1", &pulse.c1);
        raw_tree->SetBranchAddress("c2", &pulse.c2);
        raw_tree->SetBranchAddress("Num", &pulse.Num);
        raw_tree->SetBranchAddress("Stop", &pulse.Stop);
        raw_tree->SetBranchAddress("Time_Stamp", &pulse.Time_Stamp);
        raw_tree->SetBranchAddress("Event_Num", &pulse.Event_Num);
        raw_tree->SetBranchAddress("ADC_Data", pulse.ADC_Data);

        // Get entries 
        Int_t pulse_entries = raw_tree->GetEntries();
        if (pulse_entries <= 0)
        {
            std::cout << "error: Pulse_entries can not be 0!" << std::endl;
            return;
        }
        std::cout << "Entries of the root file :" << pulse_entries << std::endl;

        for (Int_t pulse_id = 0; pulse_id < pulse_entries; pulse_id++)
        {
            // Get a pulse
            raw_tree->GetEntry(pulse_id);
            if (pulse_id % 10000 == 0)
            {
                std::cout << "Processed: " << pulse_id * 100 / pulse_entries  << "%" << std::endl;
            }
            
            // Sort the SCA cell.
            // Here stop means the last sample point is samples by which ADC.
            for (Int_t i = 0; i < pulse.Stop; i++) 
            {
                ADC_data_sorted[i] = pulse.ADC_Data[pulse.Stop - 1 - i];
                ADC_data_sum[pulse.FE_ID * 64 + pulse.CH_ID][i] += ADC_data_sorted[i];
            }
            for (Int_t i = pulse.Stop; i < wave_length; i++) 
            {
                ADC_data_sorted[i] = pulse.ADC_Data[wave_length + pulse.Stop - 1 - i];
                ADC_data_sum[pulse.FE_ID * 64 + pulse.CH_ID][i] += ADC_data_sorted[i];
            }

            // The number of the waveforms for each channel.
            pulse_entries_sum[pulse.FE_ID * 64 + pulse.CH_ID]++;
        }

        // Close the root file
        fp->Close();
        std::cout << "Have closed the file:" << root_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;
    }

    // Get the correct value refer to the 1st ADC of the 1st valid channel.
    Double_t (*correct_cali)[wave_length] = new Double_t[channel_num][wave_length]();
    for (Int_t i = 0; i < channel_num; i++)
    {
        if (pulse_entries_sum[i] > 0) {
            for (Int_t j = 0; j < wave_length; j++)
            {
                correct_cali[i][j] = (Double_t)ADC_data_sum[i][j] / (Double_t)pulse_entries_sum[i];
            }
        }
        
    }

    // Output the correct_cali.txt
    std::ofstream dataFile;
    dataFile.open(correct_cali_dictionary + "correct_cali.txt");
    for (Int_t i = 0; i < channel_num; i++)
    {
        for (Int_t j = 0; j < wave_length; j++)
        {
            dataFile << std::fixed << std::setprecision(3) << correct_cali[i][j] << " ";
        }
        dataFile << "\n";
    }
    dataFile.close();
    std::cout << correct_cali_dictionary << "correct_cali.txt has been written!\n" << std::endl;
    std::cout << "ADC code correct calibration has been completed successfully!" << std::endl;

    delete[] root_filename;
    delete[] pulse_entries_sum;
    delete[] *ADC_data_sum;                              
    delete[] ADC_data_sorted;
    delete[] *correct_cali;
}