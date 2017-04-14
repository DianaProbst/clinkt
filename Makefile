CC=g++
CXXFLAGS=Wall

all:
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp lib/main.cpp -l bcm2835 -o build/program

test:
	$(CC) -$(CXXFLAGS) -DTEST -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp lib/main.cpp -l bcm2835 -o build/program

clock: examples/clock.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/clock.cpp -l bcm2835 -o build/clock

1D: examples/1dtetris.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/1dtetris.cpp -l bcm2835 -o build/1dtetris

larson: examples/larson.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/larson.cpp -l bcm2835 -o build/larson

changebar: examples/changebar.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/changebar.cpp -l bcm2835 -o build/changebar

smoothbar: examples/smoothbar.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/smoothbar.cpp -l bcm2835 -o build/smoothbar

larson2: examples/larson2.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/larson2.cpp -l bcm2835 -o build/larson2

graph: examples/graph.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/graph.cpp -l bcm2835 -o build/graph

gradientgraph: examples/gradientgraph.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/gradientgraph.cpp -l bcm2835 -o build/gradientgraph

monitorgraph: examples/monitorgraph.cpp
	$(CC) -$(CXXFLAGS) -std=c++11 lib/clinkt.cpp lib/pixel.cpp lib/low_level.cpp examples/monitorgraph.cpp -l bcm2835 -o build/monitorgraph
