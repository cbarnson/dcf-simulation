#include <iostream>
#include "SpawnEvent.h"
#include "Simulator.h"


SpawnEvent::SpawnEvent(Simulator * sim, float t):Event(t) {
	_simulator = sim;  // remember the simulator so that we can create nodes 
}


void SpawnEvent::processEvent() {
	if (_simulator != nullptr)
		_simulator->spawnNode();  // create the node
}

