#include "Simulator.h"
#include <bits/stdc++.h>
#include <iostream>
#include "SpawnEvent.h"

Simulator::Simulator(int nr_nodes) {
  _crtTime = 0;
  for (int i = 0; i < nr_nodes; i++) pq.push_back(new SpawnEvent(this, 0));
}

// create a node and the associated NPT structure; creation calls
// the node.
void Simulator::spawnNode() {
  // determine the Id of the node by checking how many nodes there
  // already are
  int the_id = nodes.size();
  nodes.push_back(new NPT(this, the_id));  // NB at this moment, control
                                           // is given to the node
                                           // routine; the node should
                                           // give control back to finish
                                           // the insertion up.
}

// this is the main routine for processing events
void Simulator::run() {
  while (!pq.empty()) {  // while there are events
    // get the first event and process it
    Event &crt_event = pq.front();

    double prevTime = _crtTime;
    _crtTime = crt_event.getTime();  // update the crt time of the simulation
    if ((nav > 0) && ((_crtTime - prevTime) > 0)) {
      nav -= (_crtTime - prevTime);
      if (nav < 0) nav = 0;
    }

    // essentially decide which node should be executed next
    crt_event.processEvent();

    pq.pop_front();  // we are done with the event, we don't need its
                     // data structure
  }
  // std::cout << "capacity " << pq.size() << std::endl;
  std::cout << "Simulator DONE!\n";
}

float Simulator::getCrtTime() {
  return _crtTime;
}

int Simulator::getNrOfNodes() {
  return nodes.size();
}

void Simulator::insertEvent(Event *ev) {
  boost::ptr_list<Event>::iterator list_elem = pq.begin();

  // this loop should stop when either list_elem reaches the end of
  // the list or list_elem points to an event with time greater than
  // the even to insert
  while ((list_elem != pq.end()) && (list_elem->getTime() <= ev->getTime())) {
    list_elem++;
  }

  // insert the even in front of the position pointed to by list_elem
  pq.insert(list_elem, ev);
}

void Simulator::callNode(int node_id) {
  // do nothing if the id of the Node that should be awoken is invalid
  if (node_id < 0 || node_id >= getNrOfNodes()) return;
  nodes[node_id].callNode();
  // calls the coroutine for the node (ie
  // wakes up the routine.
}

void Simulator::take(double nav_value) {
  assert(!(nav > 0.0));
  nav = nav_value;
}

void Simulator::release() {
  nav = 0.0;
}
