
OBJECTS=\
init.o \
new.o \
Multiboot.o \
Heap.o \

kernel.a: $(OBJECTS)
	$(AR) -rcs $(LD_FLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f kernel.a $(OBJECTS) $(OBJECTS:.o=.d)
