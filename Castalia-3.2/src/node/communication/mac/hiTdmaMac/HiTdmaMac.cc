/****************************************************************************
*  	Copyright (c) 2016 - 2017, University of California, Berkeley.		   	*
*  					   													   	*
*  	Author(s): 	Ali Moin, EECS Department, UC Berkeley.						*
*				Pierluigi Nuzzo, EE Department, USC.					   	*
*																			*
*	This file is distributed under the terms in the attached LICENSE file.	*
 ***************************************************************************/

#include "HiTdmaMac.h"

Define_Module(HiTdmaMac);

void HiTdmaMac::startup()
{
	numNodes = getParentModule()->getParentModule()->getParentModule()->par("numNodes");
	timeSlotLength = par("timeSlotLength");	// in ms

	setTimer(SYNC, 0.00001);

	declareOutput("HiTdmaMac packet breakdown");

}

void HiTdmaMac::timerFiredCallback(int timer)
{
	switch (timer) {

		case SYNC:{
			
			if ( simTime().inUnit(-3) % numNodes == self ) {
				setTimer(SYNC, (numNodes - 1) * timeSlotLength / 1000);
				trace() << "Time slot start";
				sendData();
			}
			else {
				setTimer(SYNC, 0.00001);

			}
			break;
		}

		default:{
			trace() << "WARNING: unknown timer callback " << timer;
		}
	}
}



void HiTdmaMac::fromNetworkLayer(cPacket * netPkt, int destination)
{
	MacPacket *macPkt = new MacPacket("BypassRouting packet", MAC_LAYER_PACKET);
	// *first* encapsulate the packet the then set its dest and source fields.
	encapsulatePacket(macPkt, netPkt);
	macPkt->setSource(SELF_MAC_ADDRESS);
	macPkt->setDestination(destination);
	//collectOutput("TunableMAC packet breakdown", "Received from App");

	/* We always try to buffer the packet first */
	if (bufferPacket(macPkt)) {

	} else {
		/* bufferPacket() failed, buffer is full
		 * FULL_BUFFER control msg sent by virtualMAC code
		 */
		collectOutput("HiTdmaMac packet breakdown", "Overflown");
		trace() << "WARNING buffer overflow";
	}
}


void HiTdmaMac::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
	MacPacket *macPkt = dynamic_cast <MacPacket*>(pkt);
	if (macPkt == NULL)
		return;
	if (macPkt->getDestination() == SELF_MAC_ADDRESS ||
	    macPkt->getDestination() == BROADCAST_MAC_ADDRESS)
	{
		toNetworkLayer(decapsulatePacket(macPkt));
	}
}


void HiTdmaMac::sendData()
{
	if (!TXBuffer.empty()) {
		trace() << "MAC TX Buffer size: " << TXBuffer.size();
		toRadioLayer(TXBuffer.front()->dup());
		toRadioLayer(createRadioCommand(SET_STATE, TX));
		TXBuffer.pop();
	}
}

