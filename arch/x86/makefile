
OBJS=\
setup.o \
entry.o \

arch.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)

%.o: %.c
	$(CC) -MMD -c $(CFLAGS) -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -f arch.o $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)