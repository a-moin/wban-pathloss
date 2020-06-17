/****************************************************************************
*  	Copyright (c) 2016 - 2017, University of California, Berkeley.		   	*
*  					   													   	*
*  	Author(s): 	Ali Moin, EECS Department, UC Berkeley.						*
*				Pierluigi Nuzzo, EE Department, USC.					   	*
*																			*
*	This file is distributed under the terms in the attached LICENSE file.	*
 ***************************************************************************/

#include "HiMesh.h"

Define_Module(HiMesh);

void HiMesh::startup()
{
	numNodes = getParentModule()->getParentModule()->par("numNodes");
	packet_rate = par("packet_rate");
	startupDelay = par("startupDelay");
	simTimeLimit = strtod(ev.getConfig()->getConfigValue("sim-time-limit"), NULL);
	finishTime = par("finishTime");	//time before finishing with no new packets generated

	packet_spacing = packet_rate > 0 ? 1 / float (packet_rate) : -1;
	dataSN_tmp = 0;		//seq num with each packet incremental
	dataSN = 0;			//seq num, increments after sent a packet to all channels
	dest = 0;			//dstination address

	for (int i=0; i<numNodes; i++)
		for (int j=0; j<MAX_SN; j++)
			rxStats[i][j] = 0;

	setTimer(SEND_PACKET, packet_spacing + startupDelay);

	// set the radio to sleep mode during initializtion
	//toNetworkLayer(createRadioCommand(SET_STATE,SLEEP));
	//setTimer(START_RX, 0.2);

	declareOutput("All packet copies received per node");
	declareOutput("Packets sent per node");
	declareOutput("Packets forwarded from node");
	declareOutput("Packets received per node");
}

void HiMesh::fromNetworkLayer(ApplicationPacket * genericPacket,
		const char *source, double rssi, double lqi)
{
	
	HiPacket *rcvPacket = check_and_cast<HiPacket*>(genericPacket);
	addr theAddress = rcvPacket->getAddress();
	int sequenceNumber = rcvPacket->getSequenceNumber();

	if (to_string(theAddress.dst).compare(SELF_NETWORK_ADDRESS) == 0) {
		trace() << "Received packet #" << sequenceNumber << " from node " << theAddress.src << " (last hop: " << source << " )";
		collectOutput("All packets copies received per node", theAddress.src);
		rxStats[theAddress.src][sequenceNumber]++;
	// Packet has to be forwarded to the next hop, if it's not been at this node befoore (including the source)
	} else if (theAddress.rt_hstr[self] == 0 && theAddress.ttl > 0){
		// Forwarding the packet
		addr tmpAddr;
		tmpAddr.src = theAddress.src;
		tmpAddr.dst = theAddress.dst;
		tmpAddr.ttl = theAddress.ttl - 1;
		for (int i=0; i<numNodes; i++)					// initialize the routing history
				tmpAddr.rt_hstr[i] = theAddress.rt_hstr[i];
		tmpAddr.rt_hstr[self] = 1;
		HiPacket *pkt = new HiPacket("HI packet", APPLICATION_PACKET);
		pkt->setAddress(tmpAddr);
		pkt->setData(0);
		pkt->setSequenceNumber(sequenceNumber);
		toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);

		trace() << "Forwarding packet #" << sequenceNumber << " from node " << theAddress.src << " (last hop: " << source << " ) to node " << theAddress.dst;
		collectOutput("Packets forwarded from node", theAddress.src);
	}
}

void HiMesh::timerFiredCallback(int index)
{
	switch (index) {
		case SEND_PACKET:{
			if (dest == self) dest = (++dest) % numNodes;

			// do {
			// 	dest = rand() % numNodes;
			// 	recipientAddress = to_string(dest);
			// } while (recipientAddress.compare(SELF_NETWORK_ADDRESS) == 0);
			
			trace() << "Sending packet #" << dataSN << " to node " << dest;
			collectOutput("Packets sent per node", dest);
			
			// Generating a new packet
			addr tmpAddr;
			tmpAddr.src = (unsigned short)self;
			tmpAddr.dst = (unsigned short)dest;
			tmpAddr.ttl = TTL;
			for (int i=0; i<numNodes; i++)					// initialize the routing history
				tmpAddr.rt_hstr[i] = (i==self) ? 1 : 0;
			HiPacket *pkt = new HiPacket("HI packet", APPLICATION_PACKET);
			pkt->setAddress(tmpAddr);
			pkt->setData(0);
			pkt->setSequenceNumber(dataSN);
			toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);
			
			dest = ++dest % numNodes;

			dataSN_tmp++;
			dataSN = dataSN_tmp / (numNodes-1);
			if (simTime() < simTimeLimit - finishTime)
				setTimer(SEND_PACKET, packet_spacing);
			break;
		}
		// case START_RX:{
		// 	toNetworkLayer(createRadioCommand(SET_STATE,RX));
		// 	break;
		// }
	}
}

void HiMesh::finishSpecific()
{
	// unsigned short received[numNodes];

	// for (int i=0; i<numNodes; i++){
	// 	received[i] = 0;
	// 	for (int j=0; j<10; j++) {
	// 		if (rxStats[i][j] != 0)
	// 			received[i]++; 
	// 	}
	// 	trace() << "receved from node " << i << ": " << received[i];
	// }

	for (int i=0; i<numNodes; i++){
		for (int j=0; j<MAX_SN; j++) {
			if (i != self) {
				trace() << "rxStats: " << " From Node: " << i << " SN: " << j << " Received: " << rxStats[i][j];
				if (rxStats[i][j] > 0) collectOutput("Packets received per node", i);
			}

		}
	}
}


// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void HiMesh::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

