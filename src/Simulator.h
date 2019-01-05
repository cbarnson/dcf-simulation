/*
  Simulator class contains the DES code.

  Responsibilities:

  1) knows all Node objects and their corresponding pull_type
  (needed by coroutines to transfer control flow)
  2) knows all events in a priority queue
  3) provides a run() method where events are processed
  4) creates nodes and their pull_type
  5) destroys nodes and pull_type
*/

#ifndef __SIMULATOR_H
#define __SIMULATOR_H

#include <boost/coroutine2/all.hpp>
//#include <boost/heap/priority_queue.hpp>
#include <boost/ptr_container/ptr_container.hpp>
#include "Event.h"
#include "Node.h"

class Simulator;
// Node <-> pull_type association
struct NPT {
  Node node;
  boost::coroutines2::coroutine<void>::pull_type callNode;

  // the constructor constructs the node object and corresponding
  // pull_type as per the coroutine mechanism
  NPT(Simulator *sim, int node_id) : node(sim, node_id), callNode(node) {}
};

// ***************************************************

class Simulator {
  /*bool _free = true;*/

  boost::ptr_list<Event> pq;  // Event queue, ordered by time
  boost::ptr_vector<NPT> nodes;

  float _crtTime;  // current time of the simulation

 public:
  // network access vector, if > 0, the channel is busy and won't be free until
  // _crtTime + nav
  double nav = 0.0;

  // Let's try to store some concrete events.
  Simulator(int nr_nodes = 1);

  // create a node and the associated NPT structure; creation calls
  // the code associated with the node.
  void spawnNode();

  // this is the main routine for processing events
  void run();

  // inserts event ev into the event queue in non-decreasing order
  // by the event time.
  void insertEvent(Event *ev);

  // returns current time of the simulation
  float getCrtTime();

  // Returns the number of nodes spawned in the simulator
  int getNrOfNodes();

  // public helper function
  // calls the pull_type of the coroutine corresponding to node with
  // id node_id. Does nothing if the node_id is invalid.
  void callNode(int node_id);

  void take(double nav_value);
  void release();
};

#endif
