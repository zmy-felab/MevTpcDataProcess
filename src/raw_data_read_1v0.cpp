// created: 2023/08/01 by qyh
// modified: 2025/01/31 by Zhao Maoyuan

#include "raw_data_read_1v0.hpp"

// Get file names in a folder.
std::vector<std::string> get_file_names(const std::string& directory) {
    std::vector<std::string> file_names;
    for (const auto & entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            file_names.push_back(entry.path().filename().string());
        }
    }
    return file_names;
}

// Read .dat files to .root files
void raw_data_read_1v0(std::string test_data_dictionary)
{
    // Set data path
	std::string raw_data_dictionary = test_data_dictionary + "raw_data/";
	std::string root_data_dictionary = test_data_dictionary + "root_data/";

    // pop warnings if path is not set
    if (access(test_data_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << test_data_dictionary << " is not set!" << std::endl << "\n" << std::endl;
        exit(1);
    }

    if (access(raw_data_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << raw_data_dictionary << " is not set!" << std::endl << "\n" << std::endl;
        exit(1);
    }

    std::cout << "Start reading .dat files at path:" << raw_data_dictionary << std::endl << "\n" << std::endl;

    // create the folders if not exist
    if (access(root_data_dictionary.c_str(), 0) == -1)
    {
        mkdir(root_data_dictionary.c_str(),0777);
        std::cout << "The path " << root_data_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
    }

    // get names of dat files
    std::vector<std::string> file_names = get_file_names(raw_data_dictionary);
    Int_t file_num = file_names.size();
    if (file_num == 0)
    {
        std::cout << "There is no .dat files in the path: " << raw_data_dictionary << std::endl << "\n" << std::endl;
        exit(1);
    }

    // define the Intermediate variable
    std::string* root_filename = new std::string[file_num];
    std::string raw_filename;

    const Int_t packet_header1 = 165;
    const Int_t packet_header2 = 90;
    const Int_t packet_size = 294;
    const Int_t asic_header1 = 1;
    const Int_t asic_header2 = 205;

    for (Int_t file_index = 0; file_index < file_num; file_index++)
    {
        raw_filename = raw_data_dictionary + file_names[file_index];
        root_filename[file_index] = root_data_dictionary + file_names[file_index].substr(0,file_names[file_index].size() - 4) + ".root";
        std::cout << "Processing:" << file_names[file_index] << std::endl;

        // Open the dat file
        std::ifstream fin;
        fin.open(raw_filename.c_str(), std::ios::in | std::ios::binary);
        if (!fin.is_open())
        {
            std::cout << "Cannot open file " << raw_filename.c_str() << std::endl;
            return;
        }
        else
        {
            std::cout << "Have opened the file " << raw_filename.c_str() << std::endl;
        }
      
        Char_t buf_8bits = 0;
        Char_t buf[2] = {0};
        Char_t buf_size[2] = { 0 };
        Char_t buf_ID[2] = { 0 };
        Char_t buf_asic_header[2] = { 0 };
        Char_t buf_course[4] = { 0 };
        Char_t buf_Q[2] = { 0 };
        Char_t buf_T[2] = { 0 };
        Char_t buf_c1[4] = { 0 };
        Char_t buf_c2[4] = { 0 };
        Char_t buf_stop[2] = { 0 };
        Char_t buf_adc[2] = {0};
        Char_t buf_time[6] = { 0 };
        Char_t buf_eventID[4] = { 0 };
        UChar_t buf_temp;
        ULong64_t time_stamp = 0;
        ULong64_t time_stamp_temp = 0;
        Int_t event_num = 0;

        pulse_data pulse;

        // define the tree
        TTree *raw_tree = new TTree("raw_tree", "raw data");
        raw_tree->Branch("Packet_Size", &pulse.Packet_Size, "Packet_Size/I");
        raw_tree->Branch("course", &pulse.course, "course/I");
        raw_tree->Branch("Q", &pulse.Q, "Q/I");
        raw_tree->Branch("T", &pulse.T, "T/I");
        raw_tree->Branch("c1", &pulse.c1, "c1/I");
        raw_tree->Branch("c2", &pulse.c2, "c2/I");
        raw_tree->Branch("Num", &pulse.Num, "Num/I");
        raw_tree->Branch("Stop", &pulse.Stop, "Stop/I");
        raw_tree->Branch("FE_ID", &pulse.FE_ID, "FE_ID/I");
        raw_tree->Branch("CH_ID", &pulse.CH_ID, "CH_ID/I");
        raw_tree->Branch("Time_Stamp", &pulse.Time_Stamp, "Time_Stamp/L");
        raw_tree->Branch("Event_Num", &pulse.Event_Num, "Event_Num/L");
        raw_tree->Branch("ADC_Data", pulse.ADC_Data, "ADC_Data[128]/I");

        while(1)
        {
            fin.read(&buf_8bits, sizeof(buf_8bits));
            if ((int)(unsigned char)buf_8bits == packet_header1)
            {
                fin.read(&buf_8bits, sizeof(buf_8bits));
                if ((int)(unsigned char)buf_8bits == packet_header2)
                {
                    fin.seekg(-2, std::ios::cur);
                    fin.read(buf, sizeof(buf));
                }
            }
            while ((int)(unsigned char)buf[0] == packet_header1 && (int)(unsigned char)buf[1] == packet_header2)
            {
                fin.read(buf_size, sizeof(buf_size));
                pulse.Packet_Size = (((int)(unsigned char)buf_size[0]) << 8) + ((int)(unsigned char)buf_size[1]);

                fin.read(buf_ID, sizeof(buf_ID));
                pulse.FE_ID = (int)(unsigned char)buf_ID[0];
                pulse.CH_ID = (int)(unsigned char)buf_ID[1];

                fin.seekg(2, std::ios::cur);

                fin.read(buf_course, sizeof(buf_course));
                pulse.course = ((int)(unsigned char)buf_course[0] << 24) + ((int)(unsigned char)buf_course[1] << 16) + ((int)(unsigned char)buf_course[2] << 8) + ((int)(unsigned char)buf_course[3]);
                
                fin.read(buf_Q, sizeof(buf_Q));
                pulse.Q = (int)(unsigned char)buf_Q[0] << 8 + (int)(unsigned char)buf_Q[1];

                fin.read(buf_T, sizeof(buf_T));
                pulse.T = (int)(unsigned char)buf_T[0] << 8 + (int)(unsigned char)buf_T[1];

                fin.read(buf_c1, sizeof(buf_c1));
                pulse.c1 = ((int)(unsigned char)buf_c1[0] << 24) + ((int)(unsigned char)buf_c1[1] << 16) + ((int)(unsigned char)buf_c1[2] << 8) + ((int)(unsigned char)buf_c1[3]);

                fin.read(buf_c2, sizeof(buf_c2));
                pulse.c2 = ((int)(unsigned char)buf_c2[0] << 24) + ((int)(unsigned char)buf_c2[1] << 16) + ((int)(unsigned char)buf_c2[2] << 8) + ((int)(unsigned char)buf_c2[3]);

                fin.read(buf_stop, sizeof(buf_stop));
                pulse.Num = (int)(unsigned char)buf_stop[0] << 8 + (((int)(unsigned char)buf_stop[1]) & 128) >> 7;
                pulse.Stop = ((int)(unsigned char)buf_stop[1]) & 127;

                for (int j = 0; j < 128; j++)
                {
                    fin.read(buf_adc, sizeof(buf_adc));
                    pulse.ADC_Data[j] = ((int)(unsigned char)buf_adc[0] << 8) + ((int)(unsigned char)buf_adc[1]);
                }

                fin.read(buf_time, sizeof(buf_time));
                pulse.Time_Stamp = (((unsigned long long int)(unsigned char)buf_time[0]) << 40) + (((unsigned long long int)(unsigned char)buf_time[1]) << 32) + (((unsigned long long int)(unsigned char)buf_time[2]) << 24) + (((unsigned long long int)(unsigned char)buf_time[3]) << 16) + (((unsigned long long int)(unsigned char)buf_time[4]) << 8) + ((unsigned long long int)(unsigned char)buf_time[5]);

                fin.read(buf_eventID, sizeof(buf_eventID));
                pulse.Event_Num = (((unsigned long int)(unsigned char)buf_eventID[0]) << 24) + (((unsigned long int)(unsigned char)buf_eventID[1]) << 16) + (((unsigned long int)(unsigned char)buf_eventID[2]) << 8) + (((unsigned long int)(unsigned char)buf_eventID[3]));

                fin.seekg(2, std::ios::cur);

                raw_tree->Fill();

                fin.read(buf, sizeof(buf));

                if (fin.peek() == EOF)
                    break;
            }
            if (fin.peek() == EOF)
                break;
        }

        fin.close();
        std::cout << "have closed file " << raw_filename << std::endl;

        // Open the root file
        TFile* fp = new TFile(root_filename[file_index].c_str(), "recreate");
        std::cout << root_filename[file_index] << " was opened successfully!" << std::endl;
        fp->cd();
        raw_tree->Write();
        fp->Close();
        std::cout << "have saved file " << root_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;
    }
    delete[] root_filename;
}   