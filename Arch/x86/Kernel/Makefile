
OBJECTS=\
Arch.o \


Kernel.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f Kernel.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)