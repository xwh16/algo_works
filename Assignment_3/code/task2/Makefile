CC=g++
CPPFLAGS=-g
LDFLAGS=-g

task2: task2.o
	g++ $(LDFLAGS) -o task2.out task2.o $(LDLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) *.o