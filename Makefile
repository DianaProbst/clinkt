CC=g++
CXXFLAGS=Wall
DEPS=lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp

all:
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) lib/main.cpp -l bcm2835 -o build/program

clean:
	rm build/*

test:
	$(CC) -$(CXXFLAGS) -DTEST -std=c++11 $(DEPS) lib/main.cpp -l bcm2835 -o build/program

clock: examples/clock.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/clock.cpp -l bcm2835 -o build/clock

1D: examples/1dtetris.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/1dtetris.cpp -l bcm2835 -o build/1dtetris

larson: examples/larson.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/larson.cpp -l bcm2835 -o build/larson

changebar: examples/changebar.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/changebar.cpp -l bcm2835 -o build/changebar

smoothbar: examples/smoothbar.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/smoothbar.cpp -l bcm2835 -o build/smoothbar

larson2: examples/larson2.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/larson2.cpp -l bcm2835 -o build/larson2

graph: examples/graph.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/graph.cpp -l bcm2835 -o build/graph

gradientgraph: examples/gradientgraph.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/gradientgraph.cpp -l bcm2835 -o build/gradientgraph

monitorgraph: examples/monitorgraph.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/monitorgraph.cpp -l bcm2835 -o build/monitorgraph

sysmonitor: examples/sysmonitor.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 $(DEPS) examples/sysmonitor.cpp -l bcm2835 -o build/sysmonitor

