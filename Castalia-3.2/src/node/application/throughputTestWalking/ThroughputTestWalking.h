/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#ifndef _THROUGHPUTTESTWALKING_H_
#define _THROUGHPUTTESTWALKING_H_

#include "VirtualApplication.h"
#include "queue"

using namespace std;

enum ThroughputTestTimers {
	QUEUE_PACKET = 1,
	SEND_PACKET = 2
};

class ThroughputTestWalking: public VirtualApplication {
 private:
	double packet_rate;
	string recipientAddress;
	double startupDelay;

	float packet_spacing;
	int dataSN;
	queue<ApplicationPacket *> packetBuffer;
	const char *txFile;
	ifstream tx_file;
	string txTime_str;

 protected:
	void startup();
	void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
	void handleRadioControlMessage(RadioControlMessage *);
	void timerFiredCallback(int);
};

#endif				// _THROUGHPUTTEST_APPLICATIONMODULE_H_
