/*
Concrete event used to create nodes.
 */

#ifndef __SPAWNEVENT_H
#define __SPAWNEVENT_H

#include "Event.h"

class Simulator;

class SpawnEvent : public Event {
	Simulator* _simulator;
	
public:
	SpawnEvent(Simulator * sim, float t=0);

	void processEvent();

	~SpawnEvent() {}
};


#endif
