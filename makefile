all: BC CC

BC: BetweennessCent.cpp
	g++ -Wall -o  BetweennessCent BetweennessCent.cpp -lm -O2 -ffast-math -march=native

CC: ClusteringCoeff.cpp
	g++ -Wall -o  ClusteringCoeff ClusteringCoeff.cpp -lm -O2 -ffast-math -march=native

clean:
	rm ClusteringCoeff BetweennessCent
