
OBJECTS=\
init.o

kernel.o: $(OBJECTS)
	$(LD) -r $(LD_FLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f kernel.o $(OBJECTS) $(OBJECTS:.o=.d)
