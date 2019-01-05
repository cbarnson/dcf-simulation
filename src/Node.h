/*
  Node is the class representing a running network node in the
  simulator; we use coroutines to shift execution to Node::operator () back
  and forth from the Simulator::run()
*/

#ifndef __NODE_H
#define __NODE_H

#include <boost/coroutine2/all.hpp>
#include <cmath>  // pow
#include <string>
#include "config.h"

class Simulator;

class Node {
  int _id;
  Simulator *_simulator;

  int _rcount = 1;    // retransmission count
  int _bcount = 0;    // backoff interval value
  int _fcount_s = 0;  // frames sent (success)
  int _fcount_f = 0;  // frames sent (failed)

 public:
  // sim = pointer to the simulator object
  // id = the ide of the Node. The ID is the index in the vector from
  // the simulator object that stores info about the node.
  Node(Simulator *sim = nullptr, int id = 0);
  ~Node();

  // The CSMA algorithm should go in here.
  // This is the MAIN function for the code that runs on the nodes of the
  // network. The push_type argument is used to switch execution back to the
  // simulator
  void operator()(boost::coroutines2::coroutine<void>::push_type &);

  // returns the current time for the simulation, or 0 if the Node is
  // not bound to a simulator object
  float getCrtTime();

  // helper routines useful for operator () implementing the CSMA algorithm
 private:
  void print(std::string);
  void print(std::string, int);
  void print(std::string, double);

  // routine for waiting a given number of time units
  // contextSwitch: function used in switching execution to the
  // Simulator object
  // deltat = number of time units to sleep
  // OBS: the procedure returns when Simulator's current time reaches
  // the wake-up time, eg when the simulator processes the event
  // corresponding to the wake-up of the node.
  void sleep(boost::coroutines2::coroutine<void>::push_type &contextSwitch,
             float deltat);

  // returns the time of the next frame arrival for this node based on a
  // cumulative distribution function F(x) = 1 - e^(-rate*x)
  // using a rate of 1/40 we will get a value approximately in the range
  // of 0 to 478, averaging approx 44.  This value should represent
  // milliseconds since we are asked to compute frame arrivals over a period
  // of 1 second.  The next arrival will be computed to be current time +
  // nextTime() after a frame has completed transmission successfully
  double nextTime();
  int backoffValue(int n);
};

#endif
