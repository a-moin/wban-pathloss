#include "VirtualMac.h"

class NewCastaliaModule : public VirtualMac {
	int beaconPeriodLength;

	void fromNetworkLayer(cPacket*, int);
	void fromRadioLayer(cPacket*,double,double);
};