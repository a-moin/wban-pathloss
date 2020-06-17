#include "NewCastaliaModule.h"

Define_Module(NewCastaliaModule);

void NewCastaliaModule::fromNetworkLayer(cPacket *pkt, int dst) {
	return;
}

void NewCastaliaModule::fromRadioLayer(cPacket *pkt, double rssi, double lqi) {
	// if the incoming packet is not BaselineBAN, retu
	return;
}