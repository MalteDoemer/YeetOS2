
OBJECTS=\
KernelTests.o \
GerneralTests.o \
KheapTests.o \
StdLibTests.o \
StringTests.o \

# allow tests to access private members
C_FLAGS := -fno-access-control $(C_FLAGS)
CXX_FLAGS := -fno-access-control $(CXX_FLAGS)

KernelTests.a: $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

# all: $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f KernelTests.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)