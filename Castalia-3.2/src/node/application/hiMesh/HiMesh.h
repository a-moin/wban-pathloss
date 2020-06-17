/****************************************************************************
*  	Copyright (c) 2016 - 2017, University of California, Berkeley.		   	*
*  					   													   	*
*  	Author(s): 	Ali Moin, EECS Department, UC Berkeley.						*
*				Pierluigi Nuzzo, EE Department, USC.					   	*
*																			*
*	This file is distributed under the terms in the attached LICENSE file.	*
 ***************************************************************************/


#ifndef _HIMESH_H_
#define _HIMESH_H_

#include "VirtualApplication.h"
#include "HiPacket_m.h"

using namespace std;

enum HiMeshTimers {
	SEND_PACKET = 1
	//START_RX = 2
};

#define MAX_NODES 10
#define MAX_SN 40000
#define TTL 2

class HiMesh: public VirtualApplication {
 private:
	unsigned short numNodes;
	double packet_rate;
	string recipientAddress;
	double startupDelay;
	double simTimeLimit;
	double finishTime;

	float packet_spacing;
	int dataSN_tmp;
	int dataSN;
	unsigned short dest;

	unsigned short rxStats[MAX_NODES][MAX_SN];	//statistics of received SN's from each node

 protected:
	void startup();
	void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
	void handleRadioControlMessage(RadioControlMessage *);
	void timerFiredCallback(int);
	void finishSpecific();
};

#endif				// _STARBANTEST_APPLICATIONMODULE_H_
