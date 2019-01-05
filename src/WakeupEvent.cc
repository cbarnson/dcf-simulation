#include "WakeupEvent.h"
#include "Simulator.h"
#include <iostream>

WakeupEvent::WakeupEvent(int nid, Simulator * sim, float t): Event(t) {
   _nodeId = nid;
   _simulator = sim;
}

WakeupEvent::~WakeupEvent() {
   //std::cout << "destructor wakeup" << std::endl;
   _simulator = nullptr;
}


void WakeupEvent::processEvent() {
   if (_simulator == nullptr)  // do nothing if no simulator object is available
      return;
   _simulator->callNode(_nodeId);  // wakes up the coroutine for the node
}
