**CPSC4210/5210 ASSIGNMENT 1 - REPORT**
Cody Barnson 

**IMPLEMENTATION OF DCF**

The overloaded Node::operator() contains the majority of the implementation of the DCF algorithm.  It can be summarized as follows:

* compute the time delta for the frame arrival time for this Node object
* while time delta + current time < simulation time max, do
* * call sleep with time delta (creates a WakeupEvent for time delta + current time)
* * initialize the contention window size
* * arrive (goto label)
* * * update the contention window size
* * * compute the random backoff value using the current cw range
* * nav (goto label)
* * * if channel is busy (computed by checking if the Simulator object's “nav” value is > 0) then sleep for nav time
* * * sleep for DIFS time
* * * * if channel is busy then goto nav

* * ba (goto label)
* * * if backoff counter > 0, then
* * * * sleep for HALF of slot time (i.e. PROP_DELAY time)
* * * * if channel is busy, then sleep for PROP_DELAY, then goto nav
* * * * sleep for PROP_DELAY
* * * * if channel is busy, then
* * * * * if backoff counter – 1 is equal to 0, then goto arrive, else goto nav
* * * * decrement backoff counter, then goto ba

* * transmit frame
* * * call Simulator::take (sets nav value) with time equal to PROP_DELAY + TX_TIME
* * * sleep for PROP_DELAY + TX_TIME
* * * call Simulator::release (returns nav value to 0)

* All frames, for this Node and for this simulation time max, have been transmitted; output some simulation details:
* * set frameArrivalRate = (# of frames successfully transmitted * # of bits in a frame / simulation time max) * 1e-6
* * * value is Mb/s for this Node's frame arrival rate
* * set frameTxRateAverage = (# of frames successfully transmitted / (# of frames successfully transmitted + # offrames unsuccessfully transmitted)) * frameArrivalRate
* * * value is Mb/s for this Node's average transmission rate (of successful frames)


The DCF algorithm implemented provides a model to simulate the behaviour of a wireless network. Frame transmission is handled at each Node.  Before transmission can occur, the Node must have a frame to transmit, sense the channel is idle for time equal to DIFS (done by checking the Simulator object's “nav” member variable; if > 0, channel is busy, otherwise channel is free; i.e. carrier sense), exhaust its backoff counter to 0, and not experience a collision with another Node.  A collision is said to occur when two or more stations (i.e. Node objects) attempt to send a frame in the same time slot.

Cody Barnson 001172313- This particular implementation roughly approximates the collision behaviour by allowing the first station to complete its transmission, but all those who attempt to transmit after will see a busy channel, and their contention windows will double (exponential backoff).  Since this does not simulate the basic access mechanism, in which all colliding frames would be lost and need to be retransmitted, this implementation more closely resembles that of RTS/CTS, with the first “request” is granted and the others are denied.  Without making the implementation considerably more complicated (e.g. adding an ACK mechanism), I determined this was suitable for an approximate simulation.

**SIMULATION OUTPUT**

For each scenario specified (n nodes; phi, 0 < phi < 1), a simulation will be run and each node will output its frame arrival rate and average transmission rate (of successful frames) in Mb/s that resulted from the simulation.  (See the included output.txt for simulation data)


- PARAMETERS

- Given parameters:

- Diameter of wireless network = 200m,

- Speed of light = 3 * 10^8 m/s

- Bit rate or bandwidth of channel = 54 * 10^6 bits/sec

- Frame size = 64000 bits

- SIFS = 10 * 10^-6 sec

- Computed from the given parameters:

- Propagation delay = distance / wave propagation speed = 200m / 3 * 10^8 m/s

- Transmission delay = frame size / bandwidth = 64000 bits / 54 * 10^6 bits/sec

- Slot time = 2 * Propagation delay

- DIFS = SIFS + (2 * Slot time)

- Frame arrival rate:

- We know the frame arrival rate should obey a Poisson distribution.  Assuming perfect coordination, we

- want the total number of frames that can pass through the channel.  We will then run a simulation for

- using a frame arrival rate that is a fraction of this total (ie. The set {0.25, 0.5, 0.9}).

- We know that the total channel time (or cycle time) used by a node for a given packet transmission is,

- total time = transmission delay + propagation delay + slot time * number of slots

Cody Barnson 001172313- The slot time is the time for a signal to propagation from a sending node, to a receiver, and back to the

- sender (ie. 2 * propagation delay).

- Thus,

- total time =  transmission delay + propagation delay + ( propagation delay * 2 * number of slots)

- For a wireless network of n nodes, (n > 1), the probability of a successful transmission is as follows,

- psucc = nc1 + p + (1 – p) ^ (n – 1),

- where p is the probability that a given node has a frame it wants to transmit.

- Taking the derviative of psucc with respect to p,

- dpsucc/dp = 0 => p = 1/n, so psucc = pmax

- Substituting,

- pmax = n + 1/n + (1 – 1/n) ^ (n – 1)

- Taking the limit of pmax as n approaches infinity results in a pmax of 1/e.  Given the probability of

- pmax = 1/e, we want the average number of attempts (of transmission) we should expect before success

- (ie. Successful transmission).

- 1 / pmax = 1 / (1/e) = e

- Thus, (tta = total time on average)

- tta = transmission delay + propagation delay + ( propagation delay * 2 * e)

- So for a given time we wish to run our simulation, say t, we can get the total number of frames

- generated on average in time t by dividing the total time into t.  Using the following values (given in

- assignment),

- transmission time = bit rate of channel / packet size = 8000 * 8 bits / 54 * 10^6 bits/sec

- propagation delay = distance / wave propagation speed = 200 m / 3 * 10^8 m/s

- So for a simulation time of t = 1 second, the total number of frames generated on average is, (tga = total

- number of frames generated on average)

- tga = t / ( tta )

- tga = 1 / 0.001189476 = 840.7063 frames

- We enforce the frame arrival rate in the following way:  for each Node, in the overloaded

- Node::operator() method, we use the inverse of the exponential distribution function (described at

- http://preshing.com/20111007/how-to-generate-random-timings-for-a-poisson-process/; sections

- “Writing a Simulation”, and “The Implementation”) to compute the delta time until the next frame

- arrival (for this node).  Using a while loop, we ensure that this process is repeated until the current time

- of the simulation plus the time delta is beyond the max time of our simulation (1 second, in this case).

Cody Barnson 001172313- OTHER ASSUMPTIONS

- We are given the diameter of the network as part of the assignment (200m).  From this, we will assume

- that any two nodes in the network are 200m apart from each other.  This influences the propagation

- delay (i.e. distance / wave propagation speed).

- RESOURCES

- http://preshing.com/20111007/how-to-generate-random-timings-for-a-poisson-process/

- Bianchi, G. (2000). Performance Analysis of the IEEE 802.11 Distributed Coordination Function.

- IEEE Journal, 18, 1-13.