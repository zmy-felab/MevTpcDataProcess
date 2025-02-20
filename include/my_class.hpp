#ifndef _my_class_h
#define _my_class_h

#include "../include/RtypesCore.h"
#include "main.hpp"

class pulse_data
{
private:

public:
    Int_t Packet_Size;
    Int_t FE_ID;
    Int_t CH_ID;
    Int_t course;
    Int_t Q;
    Int_t T;
    Int_t c1;
    Int_t c2;
    Int_t Num;
    Int_t Stop;
    Long64_t Time_Stamp;
    Long64_t Event_Num;
    Int_t ADC_Data[wave_length];
};

class event_data
{
private:
    
public:
    Int_t strip_num;
    Int_t FE_ID[channel_num];
    Int_t CH_ID[channel_num];
    Int_t strip_xy[channel_num];
    Int_t strip_id[channel_num];
    Int_t event_id;
    Double_t strip_amplitude[channel_num];
    Double_t strip_timing[channel_num];
};

class spectrum_data
{
private:

public:
	Int_t event_id;
    Double_t Q;
};

#endif