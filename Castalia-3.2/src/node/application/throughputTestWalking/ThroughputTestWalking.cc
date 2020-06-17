/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "ThroughputTestWalking.h"

Define_Module(ThroughputTestWalking);

void ThroughputTestWalking::startup()
{
	packet_rate = par("packet_rate");
	recipientAddress = par("nextRecipient").stringValue();
	startupDelay = par("startupDelay");
	txFile = par("txFile");

	packet_spacing = packet_rate > 0 ? 1 / float (packet_rate) : -1;
	dataSN = 0;

	// set the timer for the first transmission window
	if (recipientAddress.compare(SELF_NETWORK_ADDRESS) != 0){
		tx_file.open(txFile);
		if (!tx_file.is_open())
			opp_error("\n[Walking test]:\n Error reading from txFile %s\n", txFile);
		else {
			getline(tx_file, txTime_str);
			setTimer(SEND_PACKET, stof(txTime_str));
		}
	}

	if (packet_spacing > 0 && recipientAddress.compare(SELF_NETWORK_ADDRESS) != 0)
		setTimer(QUEUE_PACKET, packet_spacing + startupDelay);
	else
		trace() << "Not sending packets";

	declareOutput("Packets received per node");
}

void ThroughputTestWalking::fromNetworkLayer(ApplicationPacket * rcvPacket,
		const char *source, double rssi, double lqi)
{
	int sequenceNumber = rcvPacket->getSequenceNumber();

	if (recipientAddress.compare(SELF_NETWORK_ADDRESS) == 0) {
		trace() << "Received packet #" << sequenceNumber << " from node " << source;
		collectOutput("Packets received per node", atoi(source));
	// Packet has to be forwarded to the next hop recipient
	} else {
		ApplicationPacket* fwdPacket = rcvPacket->dup();
		// Reset the size of the packet, otherwise the app overhead will keep adding on
		fwdPacket->setByteLength(0);
		toNetworkLayer(fwdPacket, recipientAddress.c_str());
	}
}

void ThroughputTestWalking::timerFiredCallback(int index)
{
	switch (index) {
		case QUEUE_PACKET:{
			trace() << "Queueing packet #" << dataSN;
			packetBuffer.push(createGenericDataPacket(0, dataSN));
			dataSN++;
			setTimer(QUEUE_PACKET, packet_spacing);
			break;
		}

		case SEND_PACKET:{
			while (!packetBuffer.empty()) {
				trace() << "Sending queued packet";
				toNetworkLayer(packetBuffer.front(), par("nextRecipient"));
				packetBuffer.pop();
			}
			if (getline(tx_file, txTime_str))
				setTimer(SEND_PACKET, stof(txTime_str));
			break;
		}
	}
}

// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void ThroughputTestWalking::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

