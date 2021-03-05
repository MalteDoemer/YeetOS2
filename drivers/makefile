
OBJS=\
vga_text.o

drivers.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) -MMD -c $(CFLAGS) -o $@ $<

clean:
	rm -f drivers.a $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)