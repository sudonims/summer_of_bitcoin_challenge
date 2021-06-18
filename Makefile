SRCS = choose_transaction.cpp
OBJS = $(SRCS: .cpp = .o)
OUT = choose_transaction

all: $(OBJS)
	g++ $(OBJS) -o $(OUT)
	./choose_transaction

verify: 
	g++ verify.cpp -o verify
	./verify

clean:
	rm $(OUT) verify
