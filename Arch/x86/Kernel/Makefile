
OBJECTS=\
misc.o \
isr.o \
Arch.o \
CPU.o \
GDT.o \
IDT.o \
Interrupts.o \
SerialDebug.o \

Kernel.a: $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

# all: $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f Kernel.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)