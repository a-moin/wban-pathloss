# WBAN Path Loss Framework

This repository is a framework for generating realisitc wireless channel path loss patterns and using them to simulate wireless body area networks (WBANs). It consists of two main components:
1) Path loss generator: This MATLAB code receives a BVH motion capture file (e.g. from mocap.cs.cmu.edu) for arbitrary movements as the input, and generates a realistic path loss time sereies between a pair of nodes as the output. The algorithm is discussed in our paper [1].
2) Simulation framework: We have modified the Castalia network simulator [2] to accept the generated path loss file as its wireless channel model.

## System requirements
- MATLAB (MathWorks, Inc.). Tested on version R2019a.
- MATLAB geom3d free toolbox [3]
- Castalia Network Simulator; available at: https://github.com/boulis/Castalia
	- Castalia is based on OMNeT++ simulator, so you need to install it first as discussed in Castalia's installation manual.
	- The current version of Castalia only works with OMNeT's older version 4.x (been tested with 4.6).

## Installation guide
- Install Castalia
- The following folders should be added to the default Castalia installation. You can either replace the whole "Castalia-3.2" folder with the one provided in this repo, or copy only these folders if you already had Castallia installed with your own settings
	- `Castalia-3.2/src/CastaliaMessages.h`
	- `Castalia-3.2/src/wirelessChannel/WirelessChannel.cc`
	- `Castalia-3.2/src/wirelessChannel/WirelessChannel.h`
	- `Castalia-3.2/src/wirelessChannel/WirelessChannel.ned`
	- `Castalia-3.2/src/wirelessChannel/WirelessChannelMessages.msg`
	- `Castalia-3.2/src/node/application/throughputTestWalking`
	- `Castalia-3.2/Simulations/HIchan`
	- `Castalia-3.2/Simulations/Parameters/WirelessChannel/BANmodels`

## Problems?
If you face any problems or discover any bugs, please let us know: *MyLastName AT berkeley DOT edu*

## References
[1] Moin, et al. "Adaptive Body Area Networks Using Kinematicsand Biosignals", IEEE Journal of Biomedical and Health Informatics, 2020.
[2] https://github.com/boulis/Castalia
[3] David Legland (2020). geom3d (https://www.mathworks.com/matlabcentral/fileexchange/24484-geom3d), MATLAB Central File Exchange. Retrieved June 17, 2020.