
OBJS=\
setup.o \
tty.o 

kernel.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)

%.o: %.c
	$(CC) -MMD -c $(CFLAGS) -o $@ $<

clean:
	rm -f kernel.o $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)