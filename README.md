# Lobster Simulation

This is a simulation software for the Lobster Robot Motor Program.
It allows user to verify that the DTM parameters are set correctly to generate walking pattern.

# pc2dsp
	C Code to init and calculate Discrete Time Mapped Neurons.
	Calculated values and associated Params are stored in global Arrays that are accessible by Objective C classes
	
**Modulatible Synapse** (Need fixing to enable Heterosynaptics Facilitation and Presynaptic Inhibition)

Function: *void calcModulatedCurrents(double \*I, struct structSynapses \*params,struct structSynapses \*modParams,*
                           *double xPost,double xModPost, double spikes, double modSpikes)* 
			   
Network of Three cells: Pre (Presynaptic cell), Post (Postsynaptic)  and M (Modulation)

The third neuron can either facilitate or decrease synaptic transmission between Presynaptic cell and postsynaptic cell.

Make Xrp for M positive.

Compute I_syn for M.

Normalize I_syn for M to 1 and convert it g_syn for Pre to create **heterosynaptic facilitation**.

Invert g_syn for Pre to be negative to create **presynaptic inhibition**.
			
**Modulation**
Call the above function, with the right pre, post and M neurons to create the network:

Example of Synaptic Inhibition from Command Neuron to Bifunctional motor Neurons, via CPG.

*calcModulatedCurrents( &iExcSegStanceRet[iSide][iSeg], &pExcSegStanceRet[iSide][iSeg], &pInhIntBStance[iSide],      cellRetractor[iSide][iSeg].x, cellStance[iSide][iSeg].x, cellB[iSide].spike, cellStance[iSide][iSeg].spike);*

If Heterosynaptic Faciliation works (it doesn't seem to work yet), then all the Command Neurons will facilitate ModCom neuron to get the CPG to fire at three different levels (L,m, H).
	
Need to wire Command Neurons (Forward, Backward, Leading, Trailing, Posture) to the ModCom neurons, and make sure heterosynaptic facilitation works.

Hence, we need to call calcModulatedCurrent from Command Neuron to CPG Neurons, via ModCom Neurons.

# traceSelector
	TraceSelector.xib (Associated GUI)
	First pop up Window that allow users to select from a drop down list what neurons to view in the Oscilloscope

# Waveforms
	Objective C class that follow what users choose in the TraceSelector Class, 
	access the global C classes that store all the calculated neurons currents and associated params, 
	and put them in an Objective C NSArray so the remaining Obj C classes can have access too

# AppDelegate
	A class where everything is shared. 
	Basically, all Obj C class can call this class, 
	and access all the values and objects, and call all function from this class, and vice versa. 
	It’s a way for classes in Obj C to communicate and share info

# OscilloscopeController
	AnalysisWindowController.xib (Associated GUI)
	This class control the Oscilloscope window that we see once Neurons are selected, and ready to be displayed
	It gives functionality to all the buttons, drop down list, texts in the associated GUI

# OscilloscopeView
	This class does the drawing of the traces. 
	It accesses the Obj C NSArray that stores the Neuron Currents, 
	then it paints onto the Oscilloscope that values as a plot, 
	and it also put the name of the traces there. 
	Basically, it plots a graph as it receives new values

# SynapseController
	SynapseController.xib (Associated GUI)
	This class controls the Synapse Window,
	where users can manually edit the synapse types and change the params in real time
	It gives functionality to all the buttons, drop down list, texts in the associated GUI

# CommandStateViewController
	Window.xib (Associated GUI)
	This class controls the Command State Window, 
	where users can choose to turn on different command neurons, 
	change the speed of each command (For instance, walk Forward Left fast, or Backward Right Slow)
	It gives functionality to all the buttons, drop down list, texts in the associated GUI
