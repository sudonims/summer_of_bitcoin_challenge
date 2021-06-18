SRCS = choose_transaction1.cpp
OBJS = $(SRCS: .cpp = .o)

all: $(OBJS)
	g++ $(OBJS) -o choose_transaction
	./choose_transaction

.cpp.o:
	g++ -c $<

clean:
	rm *.o
	rm *~
