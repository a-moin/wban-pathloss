//****************************************************************************
//*  Test MAC module                        *
//                                      *
//*                                                                          *  
//****************************************************************************/

package node.communication.mac.newCastaliaModule;

simple NewCastaliaModule like node.communication.mac.iMac {
 parameters:
	bool collectTraceInfo = default(false);
	int macMaxPacketSize = default(0);
	int macBufferSize = default(16);
	int macPacketOverhead = default(8);			//in bytes
	
	int newParameter1;
	string newParameter2 = default("default value");
	bool newParameter3 = default(false);
 
 gates:
	output toNetworkModule;
	output toRadioModule;
	input fromNetworkModule;
	input fromRadioModule;
	input fromCommModuleResourceMgr;
}

