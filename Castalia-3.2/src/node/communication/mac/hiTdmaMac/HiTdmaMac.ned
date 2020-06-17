//******************************************************************************/

package node.communication.mac.hiTdmaMac;


simple HiTdmaMac like node.communication.mac.iMac {
 parameters: 
	bool collectTraceInfo = default (false);
	int macMaxPacketSize = default (0);
	int macPacketOverhead = default (9);
	int macBufferSize = default (32);

	double timeSlotLength = default (1.0);		// time slot length in ms

 gates:
	output toNetworkModule;
	output toRadioModule;
	input fromNetworkModule;
	input fromRadioModule;
	input fromCommModuleResourceMgr;
}

