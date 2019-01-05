#include "Node.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include "Simulator.h"
#include "WakeupEvent.h"
#include "config.h"

using std::cout;
using std::endl;

Node::Node(Simulator *sim, int id) {
  _simulator = sim;
  _id = id;
}

Node::~Node() {
  _simulator = nullptr;
}

float Node::getCrtTime() {
  if (_simulator != nullptr)
    return _simulator->getCrtTime();
  else {
    cout << "error, crttime returned 0 in Node" << endl;
    return 0;
  }
}

void Node::sleep(boost::coroutines2::coroutine<void>::push_type &contextSwitch,
                 float deltat) {
  // create the WakeupEvent
  if (deltat <= 0)  // in case the deltat is invalid, do nothing
    return;
  if (_simulator ==
      nullptr)  // in case Node is not bound to a simulator, do nothing
    return;

  Event *ev = new WakeupEvent(_id, _simulator, getCrtTime() + deltat);
  _simulator->insertEvent(ev);
  // we now need to sleep so control flow switches to the simulator.
  // When Simulator processes the packet and calls this node again,
  // execution will resume from here.
  contextSwitch();
}

// the CSMA algorithm should go in here
// This is the MAIN function for the code that runs on the nodes of the network
void Node::operator()(
    boost::coroutines2::coroutine<void>::push_type &contextSwitch) {
  // generate the time of frame arrival
  double frameArrivalDelta, frameArrivalAbsolute;

  frameArrivalDelta = nextTime();
  frameArrivalAbsolute = getCrtTime() + frameArrivalDelta;

  while (frameArrivalAbsolute < SIMULATION_TIME_MAX) {
    // print("arrival at", frameArrivalAbsolute);
    sleep(contextSwitch, frameArrivalDelta);
    // print("wakeup! frame arrived");
    _rcount = CW_MIN_N - 1;  // one less because decre after arrive label

  arrive:
    // print("arrive");
    _rcount++;
    if (_rcount > CW_MAX_N) _rcount = CW_MAX_N;  // bound check
    // _idle = false;
    _bcount = backoffValue(_rcount);
    // print("bcount set to ", _bcount);

  nav:  // busy channel for _simulator.nav time (at least)
    if (_simulator->nav > 0) {
      sleep(contextSwitch, _simulator->nav);
    }

    sleep(contextSwitch, DIFS);
    if (_simulator->nav > 0) goto nav;

    // backoff algorithm
  ba:
    if (_bcount > 0) {
      sleep(contextSwitch, PROP_DELAY);
      if (_simulator->nav > 0) {
        sleep(contextSwitch, PROP_DELAY);
        goto nav;
      }

      sleep(contextSwitch, PROP_DELAY);
      if (_simulator->nav > 0) {
        if (_bcount - 1 == 0) {
          _fcount_f++;
          goto arrive;
        }
        goto nav;
      }
      _bcount--;
      goto ba;
    }

    double takeTime = PROP_DELAY + TX_TIME;  // PROP_DELAY + TX_TIME
    _simulator->take(takeTime);
    sleep(contextSwitch, takeTime);
    _simulator->release();

    // update values for computing this nodes frame arrival rate
    _fcount_s++;

    // compute next
    frameArrivalDelta = nextTime();
    frameArrivalAbsolute = getCrtTime() + frameArrivalDelta;

  }  // end of while

  // simulation output (this node's frame arrival rate in mbits/sec)
  // successful tx: _fcount_s
  // failed tx:     _fcount_f
  double frameArrivalRate =
      (double)_fcount_s * (double)FRAME_SIZE_BITS / (double)SIMULATION_TIME_MAX;
  frameArrivalRate *= 1e-6;
  double frameTxRateAverage =
      ((double)_fcount_s / (double)(_fcount_s + _fcount_f));
  frameTxRateAverage *= frameArrivalRate;
  print("frame arrival rate\t(Mb/s):\t", frameArrivalRate);
  print("avg transmission rate\t(Mb/s):\t", frameTxRateAverage);
}

void Node::print(std::string str) {
  cout << "(node:" << _id << ", time:" << std::setprecision(8) << std::fixed
       << (double)getCrtTime() << ")\t";
  cout << str << endl;
}

void Node::print(std::string str, int i) {
  cout << "(node:" << _id << ", time:" << getCrtTime() << ")\t";
  cout << str << " " << i << endl;
}

void Node::print(std::string str, double i) {
  cout << "(node:" << _id << ", time:" << std::setprecision(8) << std::fixed
       << (double)getCrtTime() << ")\t";
  cout << str << " " << i << endl;
}

double Node::nextTime() {
  // returns time units in seconds
  return (-logf(1.0f - (double)random() / (RAND_MAX + 1.0f)) /
          FRAME_ARRIVAL_RATE);
}

int Node::backoffValue(int n) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::minstd_rand0 generator(seed);
  int window = std::pow(2, n);
  return generator() % window;
}
