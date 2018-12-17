CC=g++
CPPFLAGS=-g
LDFLAGS=-g

task1: task1.o
	g++ $(LDFLAGS) -o task1.out task1.o $(LDLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) *.o
	$(RM) *.out