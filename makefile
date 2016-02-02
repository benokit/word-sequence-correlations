CCFLAGS = -O3

build: wc2main wc3main

wc3main: wc3main.o wordPositions.o optionsParser.o
	g++ $(CCFLAGS) -o wc3 wc3main.o wordPositions.o optionsParser.o

wc2main: wc2main.o wordPositions.o optionsParser.o
	g++ $(CCFLAGS) -o wc2 wc2main.o wordPositions.o optionsParser.o

wordPositions.o: wordPositions.cpp wordPositions.h
	g++ -c $(CCFLAGS) -std=c++11 wordPositions.cpp

optionsParser.o: optionsParser.cpp optionsParser.h
	g++ -c $(CCFLAGS) -std=c++11 optionsParser.cpp

wc3main.o: wc3main.cpp threeWordsCorrelations.h
	g++ -c $(CCFLAGS) -std=c++11 wc3main.cpp 

wc2main.o: wc2main.cpp twoWordsCorrelations.h
	g++ -c $(CCFLAGS) -std=c++11 wc2main.cpp 

clean:
	rm *.o
