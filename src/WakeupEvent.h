/*
	Concrete event used to simulate that the process is sleeping.
*/

#ifndef __WAKEUPEVENT_H
#define __WAKEUPEVENT_H

#include "Event.h"

class Simulator;

class WakeupEvent : public Event {
	int _nodeId;
	Simulator * _simulator;  // ptr to the SImulator
public:
	// nid = the id of the Node that issued the sleep. This is
	// used by the SImulator to determine which Node should be run
	// sim = pointer to the SImulator class. The simulator provides an
	// interface that allows the event to be processed
	// t = the time at which this event should be processed
	WakeupEvent(int nid, Simulator * sim, float t=0);
	~WakeupEvent();
	void processEvent();
};

#endif
