/****************************************************************************
*  	Copyright (c) 2016 - 2017, University of California, Berkeley.		   	*
*  					   													   	*
*  	Author(s): 	Ali Moin, EECS Department, UC Berkeley.						*
*				Pierluigi Nuzzo, EE Department, USC.					   	*
*																			*
*	This file is distributed under the terms in the attached LICENSE file.	*
 ***************************************************************************/

#ifndef HITDMAMACMODULE
#define HITDMAMACMODULE

#include "VirtualMac.h"

using namespace std;

enum HiTdmaMacTimers {
	SYNC = 1
};

class HiTdmaMac: public VirtualMac {
 private:
	unsigned short numNodes;
	double timeSlotLength;

 protected:
	void startup();
	void fromNetworkLayer(cPacket *, int);
	void fromRadioLayer(cPacket *, double, double);
	void timerFiredCallback(int);
	void sendData();
};

#endif				//HITDMAMACMODULE
