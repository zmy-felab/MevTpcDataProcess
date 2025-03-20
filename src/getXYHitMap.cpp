// created: 2025/03/20 by Zhao Maoyuan
#include "my_lib.hpp"
#include "getXYHitMap.hpp"

void getXYHitMap(std::string test_data_dictionary)
{
    // Set data path
    std::string event_data_dictionary = test_data_dictionary + "event_data/";
    std::string hitmap_dictionary = test_data_dictionary + "hitmap/";

    std::cout << "Start getting spectrum with data files at path:" << event_data_dictionary << std::endl << "\n" << std::endl;

    // pop warnings if path is not set
    if (access(test_data_dictionary.c_str(), 0) == -1)
    {
        std::cout << "Path: " << test_data_dictionary << " is not set!" << std::endl;
        exit(1);
    }

    // create the folders if not exist
    if (access(hitmap_dictionary.c_str(), 0) == -1)
    {
        #if defined(_WIN32)
            mkdir(hitmap_dictionary.c_str());
        #else
            mkdir(hitmap_dictionary.c_str(), 0777);
        #endif

        std::cout << "The path " << hitmap_dictionary << " does not exist. Creating..." << std::endl << "\n" << std::endl;
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
    std::string hitmap_filename = hitmap_dictionary + "hitmap.root";

    // define tree
    hitmap_data hitmap_data_object;
    TFile* fp2 = new TFile(hitmap_filename.c_str(), "recreate");
    TTree* hitmap_tree = new TTree("hitmap_tree", "hit position of every event");
    hitmap_tree->Branch("event_id", &hitmap_data_object.event_id, "event_id/I");
	hitmap_tree->Branch("x", &hitmap_data_object.x, "x/I");
	hitmap_tree->Branch("y", &hitmap_data_object.y, "y/I");

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
            hitmap_data_object.event_id = event_data_object.event_id;
            hitmap_data_object.x = 0;
			Double_t xQMax = 0;
			hitmap_data_object.y = 0;
			Double_t yQMax = 0;

            // correct gain of Q of all channels
            for (Int_t i = 0; i < event_data_object.strip_num; i++)
            {
                if (event_data_object.strip_xy[i] == 0 && event_data_object.strip_Q[i] > xQMax)
                {
                    hitmap_data_object.x = event_data_object.strip_id[i];
                    xQMax = event_data_object.strip_Q[i];
                }
                else if (event_data_object.strip_xy[i] == 1 && event_data_object.strip_Q[i] > yQMax)
                {
                    hitmap_data_object.y = event_data_object.strip_id[i];
                    yQMax = event_data_object.strip_Q[i];
                }
                else 
                {
                }
            }

			if (xQMax == 0 || yQMax == 0){
				continue;
			}
            else {
                hitmap_tree->Fill();
            }
        }

        // Close the file
        fp->Close();
        std::cout << "Have closed the file:" << event_filename[file_index] << std::endl << "\n" << std::endl;
        delete fp;
    }

    delete[] event_filename;

    // Open spec file
    fp2->cd();
    hitmap_tree->Write();
    fp2->Close();
    delete fp2;
    std::cout << hitmap_filename << " has been written!\n" << std::endl;
    std::cout << "Spectrum calculation has been completed successfully!" << std::endl;
}