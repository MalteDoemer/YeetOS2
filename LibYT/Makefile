
OBJECTS=\
Verify.o \
StdLib.o \

LibYT.a: $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

# all: $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f LibYT.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)