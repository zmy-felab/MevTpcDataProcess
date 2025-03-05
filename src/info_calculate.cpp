// created: 2025/02/01 by Zhao Maoyuan

#include "my_lib.hpp"
#include "Q_calculate.hpp"
#include "info_calculate.hpp"

constexpr auto baseline_average_length = 10;

// find strip_x、strip_y by FE_ID and CH_ID
void ch_to_strip(std::vector<Int_t> czt_lookup, Int_t FE_ID, Int_t CH_ID, Int_t& strip_xy, Int_t& strip_id)
{
    if (FE_ID <= 7) {
        strip_xy = 0;
    }
    else {
        strip_xy = 1;
    }

    Int_t flag = 0;
    for (Int_t i = 0; i < channel_num / 2; i++)
    {
        if (czt_lookup[strip_xy * channel_num / 2 + i] == CH_ID)
        {
            flag = 1;
            strip_id = i;
            break;
        }
    }
}

// calculate event information
void info_calculate(std::string test_data_dictionary, std::string Q_calculate_option, std::string gain_cali_dictionary)
{   
    // load gain.txt
    Double_t* gain = new Double_t[channel_num];
    Double_t* intercept = new Double_t[channel_num];
    std::string cali_file = gain_cali_dictionary + "gain.txt";

	if (Q_calculate_option == "Q_calculate")
	{
        gain_input(cali_file, gain, intercept);
	}
    else if (Q_calculate_option == "gain_cali")
    {

    }
	else
	{
		std::cout << "error: Q_calculate_option can not be " << Q_calculate_option << "!" << std::endl;
		exit(1);
	}

    // Set data path
	std::string root_data_dictionary = test_data_dictionary + "root_data/";
	std::string event_data_dictionary = test_data_dictionary + "event_data/";

    std::cout << "Start info calculated with data files at path:" << root_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(test_data_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << test_data_dictionary << " is not set!" << std::endl;
        exit(1);
    }

    // create the folders if not exist
    if (access(event_data_dictionary.c_str(), 0) == -1)
    {   
        #if defined(_WIN32)
            mkdir(event_data_dictionary.c_str());
        #else
            mkdir(root_data_dictionary.c_str(), 0777);
        #endif
        std::cout << "The path " << event_data_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
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
    std::string* event_filename = new std::string[file_num];
	std::vector<Int_t> czt_lookup;
	for (Int_t i = 0; i < 1024; i++) {
		if (i < 512) {
			czt_lookup.push_back(i);
		}
		else {
			czt_lookup.push_back(i - 512);
		}
	}

    for (Int_t file_index = 0; file_index < file_num; file_index++)
    {
        root_filename[file_index] = root_data_dictionary + file_names[file_index];
        event_filename[file_index] = event_data_dictionary + "event_" + file_names[file_index];

        std::cout << "Processing:" << root_filename[file_index] << std::endl;

        // Open the root file
        TFile *fp = new TFile(root_filename[file_index].c_str());
        std::cout << root_filename[file_index] << " was opened successfully" << std::endl;

        pulse_data pulse;

        // Get the tree and set the branch
        TTree *raw_tree = (TTree *)fp->Get("raw_tree");
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
        std::cout << "Entries of the file :" << pulse_entries << std::endl;

        // define the tree
        event_data event_data_object;
        TFile* fp2 = new TFile(event_filename[file_index].c_str(), "recreate");
        TTree *event_tree = new TTree("event_tree", "every event effective imformation");
        event_tree->Branch("strip_num", &event_data_object.strip_num, "event_data_object.strip_num/I");
        event_tree->Branch("FE_ID", event_data_object.FE_ID, "event_data_object.FE_ID[event_data_object.strip_num]/I");
        event_tree->Branch("CH_ID", event_data_object.CH_ID, "event_data_object.CH_ID[event_data_object.strip_num]/I");
        event_tree->Branch("strip_xy", event_data_object.strip_xy, "event_data_object.strip_xy[event_data_object.strip_num]/I");
        event_tree->Branch("strip_id", event_data_object.strip_id, "event_data_object.strip_id[event_data_object.strip_num]/I");
        event_tree->Branch("event_id", &event_data_object.event_id, "event_data_object.event_id/I");
        event_tree->Branch("strip_amplitude", event_data_object.strip_amplitude, "event_data_object.strip_amplitude[event_data_object.strip_num]/D");
		event_tree->Branch("strip_Q", event_data_object.strip_Q, "event_data_object.strip_Q[event_data_object.strip_num]/D");
        event_tree->Branch("strip_timing", event_data_object.strip_timing, "event_data_object.strip_timing[event_data_object.strip_num]/D");

        // define the Intermediate variable
        Double_t *ADC_data_sub_baseline = new Double_t[wave_length];
        Double_t strip_baseline = 0;
        Double_t strip_timing = 0;
        Double_t ADC_data_max = 0;
        Int_t ADC_data_max_index = 0;
        Int_t event_id_temp = 0;
        event_data_object.strip_num = 0;
        event_data_object.event_id = -1;

        for (Int_t pulse_id = 0; pulse_id < pulse_entries; pulse_id++)
        {
            // Get a pulse
            raw_tree->GetEntry(pulse_id);
            if (event_data_object.event_id != pulse.Event_Num)
            {
                if(event_data_object.event_id != -1)
                {
                    event_tree->Fill();
                }
                event_data_object.event_id = pulse.Event_Num;
                event_data_object.strip_num = 0;
            }

            // calculate event xy imformation
            if (pulse.FE_ID <= 16 && pulse.FE_ID >= 1 && pulse.CH_ID >= 0 && pulse.CH_ID <= 63)
            {
                event_data_object.FE_ID[event_data_object.strip_num] = pulse.FE_ID;
                event_data_object.CH_ID[event_data_object.strip_num] = pulse.CH_ID;
            }    
            else
			{
                continue;
			}

            ch_to_strip(czt_lookup, pulse.FE_ID, pulse.CH_ID, event_data_object.strip_xy[event_data_object.strip_num], event_data_object.strip_id[event_data_object.strip_num]);

            // calculate baseline
            strip_baseline = 0;
            for (Int_t ADC_index = 0; ADC_index < baseline_average_length; ADC_index++)
            {
                strip_baseline += pulse.ADC_Data[ADC_index];
            }
            strip_baseline = strip_baseline / baseline_average_length;

            // calculate amplitude
            ADC_data_max = 0;
            ADC_data_max_index = 0;
            for (Int_t ADC_index = 0; ADC_index < wave_length; ADC_index++)
            {
                ADC_data_sub_baseline[ADC_index] = strip_baseline - pulse.ADC_Data[ADC_index];
                if (ADC_data_max <= ADC_data_sub_baseline[ADC_index])
                {
                    ADC_data_max = ADC_data_sub_baseline[ADC_index];
                    ADC_data_max_index = ADC_index;
                }
            }
            event_data_object.strip_amplitude[event_data_object.strip_num] = ADC_data_max;

            if (Q_calculate_option == "Q_calculate")
            {
                event_data_object.strip_Q[event_data_object.strip_num] = Q_calculate(event_data_object.FE_ID[event_data_object.strip_num], event_data_object.CH_ID[event_data_object.strip_num], gain, intercept, event_data_object.strip_amplitude[event_data_object.strip_num]);
			}
			else if(Q_calculate_option == "gain_cali")
			{
				event_data_object.strip_Q[event_data_object.strip_num] = 0;
			}
            else
            {
				std::cout << "error: Q_calculate_option can not be " << Q_calculate_option << "!" << std::endl;
                exit(1);
            }
            
            // calculate timing
            strip_timing = 0;
            for (Int_t ADC_index = 0; ADC_index < ADC_data_max_index; ADC_index++)
            {
                if (ADC_data_sub_baseline[ADC_index] >= (ADC_data_max / 2) && (ADC_data_sub_baseline[ADC_index - 1] <= ADC_data_max / 2))
                {
                    strip_timing = (ADC_data_max / 2 - ADC_data_sub_baseline[ADC_index]) / (ADC_data_sub_baseline[ADC_index] - ADC_data_sub_baseline[ADC_index - 1]) + ADC_index;
                    break;
                }
            }
            event_data_object.strip_timing[event_data_object.strip_num] = strip_timing;
            event_data_object.strip_num ++;

			if (event_data_object.strip_num >= channel_num)
			{   
                event_data_object.strip_num = 0;
                event_data_object.event_id = -1;
			}

            if (pulse_id % 10000 == 0)
            {
                std::cout << "Processed: " << pulse_id * 100 / pulse_entries << "%" << std::endl;
            }
        }

		delete[] ADC_data_sub_baseline;
		
        // Close the file
        fp->Close();
        std::cout << "Have closed the file:" << root_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;

        // Open event file
        fp2->cd();
        event_tree->Write();
        fp2->Close();
		delete fp2;
        std::cout << event_filename[file_index] << " has been written!\n" << std::endl;
    }

    std::cout << "Info calculation has been completed successfully!" << std::endl;

	delete[] root_filename;
	delete[] event_filename;
    delete[] gain;
    delete[] intercept;
}