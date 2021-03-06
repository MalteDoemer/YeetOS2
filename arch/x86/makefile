
OBJECTS=\
entry.o \


arch.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f arch.a $(OBJECTS) $(OBJECTS:.o=.d)
