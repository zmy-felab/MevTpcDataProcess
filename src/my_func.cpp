#include "my_func.hpp"

// Get file names in a folder.
std::vector<std::string> get_file_names(const std::string& directory) {
    std::vector<std::string> file_names;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            file_names.push_back(entry.path().filename().string());
        }
    }
    return file_names;
}

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