/****************************************************************************
*  	Copyright (c) 2016 - 2017, University of California, Berkeley.		   	*
*  					   													   	*
*  	Author(s): 	Ali Moin, EECS Department, UC Berkeley.						*
*				Pierluigi Nuzzo, EE Department, USC.					   	*
*																			*
*	This file is distributed under the terms in the attached LICENSE file.	*
 ***************************************************************************/

#include "HiStar.h"

Define_Module(HiStar);

void HiStar::startup()
{
	numNodes = getParentModule()->getParentModule()->par("numNodes");
	hubAddress = par("hubAddress");
	packet_rate = par("packet_rate");
	startupDelay = par("startupDelay");
	simTimeLimit = strtod(ev.getConfig()->getConfigValue("sim-time-limit"), NULL);
	finishTime = par("finishTime");	//time before finishing with no new packets generated

	packet_spacing = packet_rate > 0 ? 1 / float (packet_rate) : -1;
	dataSN_tmp = 0;		//seq num with each packet incremental
	dataSN = 0;			//seq num, increments after sent a packet to all channels
	dest = 0;			//dstination address

	sprintf(hubAddressStr, "%d", hubAddress);

	for (int i=0; i<numNodes; i++)
		for (int j=0; j<MAX_SN; j++)
			rxStats[i][j] = 0;

	setTimer(SEND_PACKET, packet_spacing + startupDelay);

	declareOutput("All packet copies received per node");
	declareOutput("Packets sent per node");
	declareOutput("Packets forwarded from node");
	declareOutput("Packets received per node");
}

void HiStar::fromNetworkLayer(ApplicationPacket * genericPacket,
		const char *source, double rssi, double lqi)
{
	
	HiPacket *rcvPacket = check_and_cast<HiPacket*>(genericPacket);
	addr theAddress = rcvPacket->getAddress();
	int sequenceNumber = rcvPacket->getSequenceNumber();

	if (self == hubAddress) {
		trace() << "Hub received!";
		// if final destination is hub:
		if (to_string(theAddress.dst).compare(SELF_NETWORK_ADDRESS) == 0) {
			trace() << "Received packet #" << sequenceNumber << " from node " << theAddress.src << " (last hop: " << source << " )";
			rxStats[theAddress.src][sequenceNumber]++;
		}
		// if hub has to forward the packet
		else {
			addr tmpAddr;
			tmpAddr.src = theAddress.src;
			tmpAddr.dst = theAddress.dst;
			HiPacket *pkt = new HiPacket("HI packet", APPLICATION_PACKET);
			pkt->setAddress(tmpAddr);
			pkt->setData(0);
			pkt->setSequenceNumber(sequenceNumber);
			char dest_str[1];
			sprintf(dest_str, "%d", theAddress.dst);
			toNetworkLayer(pkt, dest_str);
			trace() << "Forwarding packet #" << sequenceNumber << " from node " << theAddress.src << " (last hop: " << source << " ) to node " << theAddress.dst;
			collectOutput("Packets forwarded from node", theAddress.src);
		}
	}
	// if final destication is self (non-hub):
	else if((to_string(theAddress.dst).compare(SELF_NETWORK_ADDRESS) == 0) && (to_string(hubAddress).compare(source) == 0)) {
		trace() << "Received packet #" << sequenceNumber << " from node " << theAddress.src << " (last hop: " << source << " )";
		collectOutput("All packets copies received per node", theAddress.src);
		rxStats[theAddress.src][sequenceNumber]++;
	}

}

void HiStar::timerFiredCallback(int index)
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
			HiPacket *pkt = new HiPacket("HI packet", APPLICATION_PACKET);
			pkt->setAddress(tmpAddr);
			pkt->setData(0);
			pkt->setSequenceNumber(dataSN);
			
			// if it's hub, send it directly to final destination
			if (self == hubAddress) {
				char dest_str[1];
				sprintf(dest_str, "%d", tmpAddr.dst);
				toNetworkLayer(pkt, dest_str);
			}
			else {
				toNetworkLayer(pkt, hubAddressStr);
			}

			
			
			dest = ++dest % numNodes;

			dataSN_tmp++;
			dataSN = dataSN_tmp / (numNodes-1);
			if (simTime() < simTimeLimit - finishTime)
				setTimer(SEND_PACKET, packet_spacing);
			break;
		}
	}
}

void HiStar::finishSpecific()
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
void HiStar::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

