#include "Unpacker_Lattice_TDC.h"
#include <iostream>
#include "Event.h"
#include "Hit.h"

using namespace std;

ClassImp(Unpacker_Lattice_TDC);

Unpacker_Lattice_TDC::Unpacker_Lattice_TDC(string bT, string bA, string hA, int cN, int o, int r, string mR, bool dec, bool dbg) : UnpackingModule(bT, bA, hA, cN, o, r, mR, dec, dbg) {
  
  channelNumber = cN;
  
  leadTimes = new UInt_t*[channelNumber];
  trailTimes = new UInt_t*[channelNumber];
  leadMult = new UInt_t[channelNumber];
  trailMult = new UInt_t[channelNumber];
  
  for(int i = 0; i < channelNumber; i++) {
    leadTimes[i] = new UInt_t[1000];
    trailTimes[i] = new UInt_t[1000];
    leadMult[i] = 0;
    trailMult[i] = 0;
  }
  
  cerr<<"HPTDC_HR: Creating Unpacker_Lattice_TDC for board type: "<<bT<<" board address "<<bA<<" hub address "<<hA<<" number of channels "<<channelNumber<<endl;
}

Unpacker_Lattice_TDC::~Unpacker_Lattice_TDC() {
  for(int i = 0; i < channelNumber; i++) {
    delete [] leadTimes[i];
    delete [] trailTimes[i];
  }
  
  delete [] leadMult;
  delete [] trailMult;
  delete [] leadTimes;
  delete [] trailTimes;
}

void Unpacker_Lattice_TDC::Clear() {
  for(int i = 0; i < channelNumber; i++) {
    delete [] leadTimes[i];
    delete [] trailTimes[i];
    leadTimes[i] = new UInt_t[1000];
    trailTimes[i] = new UInt_t[1000];
    leadMult[i] = 0;
    trailMult[i] = 0;
  }
}

void Unpacker_Lattice_TDC::ProcessEvent(UInt_t* data) {
  
  UInt_t data_i = 0;
  if(GetInvertBytes() == true) {
    data_i = ReverseHex((*data));
  }
  else {
    data_i = (*data);
  }
  
  UInt_t header = (data_i >> 28) & 0xf;
  UInt_t channel;
  UInt_t time;
  
  switch (header) {
    
    case 2:
      break;
      
    case 3:
      break;
      
    case 4:
      channel = (data_i >> 19) & 0x1f;
      time = data_i & 0x7ffff;
      leadTimes[channel][leadMult[channel]] = time;
      //if(debugMode == true)
	//cerr<<"HPTDC_HR: LeadTime found on channel "<<channel<<" with value "<<time<<endl;
      leadMult[channel]++;
      break;
      
    case 5:
      channel = (data_i >> 19) & 0x1f;
      time = data_i & 0x7ffff;
      trailTimes[channel][trailMult[channel]] = time;
      //if(debugMode == true)
	//cerr<<"HPTDC_HR: TrailTime found on channel "<<channel<<" with value "<<time<<endl;
      trailMult[channel]++;
      break;
    
  }
}