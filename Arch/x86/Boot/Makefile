
OBJECTS=\
entry.o \

Boot.a: $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

# all: $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f Boot.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)