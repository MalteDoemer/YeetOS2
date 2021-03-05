
OBJS=memcpy.o

lib.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) -MMD -c $(CFLAGS) -o $@ $<

clean:
	rm -f lib.a $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)