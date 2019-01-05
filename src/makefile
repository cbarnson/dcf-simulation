CC=g++11
CCFLAGS = -Wall -std=c++11 -I /home/lib4210/boost-1.63/include/
LN=g++11
LNFLAGS= -L /home/lib4210/boost-1.63/lib/
LIBS= -lboost_coroutine -lboost_context -lboost_system -lboost_thread -lboost_chrono

MODULES=Node SpawnEvent main Simulator WakeupEvent config

# the executable
des: $(addsuffix .o, $(MODULES))
	$(LN) $(LNFLAGS) -o $@ $^ $(LIBS)

%.o : %.cc
	$(CC) $(CCFLAGS) -c -o $@ $<
	$(CC) $(CCFLAGS) -MM -MP -MT $@ $< > $(basename $@).d

.PHONY : clean
clean :
	rm -f *.o *~ *.d des

# add the dependency info to make
-include $(addsuffix .d,$(MODULES))



