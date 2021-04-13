
OBJECTS=\


LibC.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f LibC.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)