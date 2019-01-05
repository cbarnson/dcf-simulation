/*
Abstract Event class; it is stored in the event queue which is a
priority queue; the simulator extracts the first event and transfers
the flow of control via boost::coroutines to the Node that generated
the event.
*/

#ifndef __EVENT_H
#define __EVENT_H

class Event {
  float _time;  // the time when this event will happen

 public:
  // sets the time when the event should occur.
  Event(float t = 0) { _time = t; }

  virtual void processEvent() = 0;

  // returns the time of the event
  float getTime() { return _time; }

  virtual ~Event() {}
};

#endif
