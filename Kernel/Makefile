
OBJECTS=\
Main.o \
Kheap.o \
Panic.o \


Kernel.a: $(OBJECTS)
	$(AR) -rcs $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f Kernel.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)