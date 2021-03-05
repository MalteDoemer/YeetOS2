OBJS=

filesys.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)

clean:
	rm -f filesys.o $(OBJS) $(OBJS:.o=.d)