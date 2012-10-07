SRC=$(wildcard *.cc)
OBJ=$(patsubst %.cc, %.o, $(SRC))

all: main

main: $(OBJ)
	g++ -O2 -std=c++0x -g -o main $(OBJ)

%.o: %.cc
	g++ -O2 -std=c++0x -c -g $^

test:main
	./main dd2380.csc.kth.se $(PORT) $(BOARD)

to_submit.zip:*.h *.cc
	mkdir to_submit
	cp $^ to_submit
	zip $@ to_submit/*
	rm -r to_submit

clean:
	rm main
	rm *.o
