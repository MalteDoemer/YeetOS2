
OBJECTS=\
entry.o \


arch.o: $(OBJECTS)
	$(LD) -r $(LD_FLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f arch.o $(OBJECTS) $(OBJECTS:.o=.d)
