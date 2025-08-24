CXX = g++
CXXFLAGS = -lfltk

falcon : falcon.cpp
	${CXX} ${CXXFLAGS} falcon.cpp -o falcon
clean:
	rm falcon
run:
	./falcon

install:
	sudo cp falcon /usr/local/bin
