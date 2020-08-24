# All Targets
all: splflix

# Tool invocations
splflix: bin/Session.o bin/Action.o bin/User.o bin/main.o bin/Watchable.o
	@echo 'Building target: splflix'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/splflix bin/Session.o bin/Action.o bin/User.o bin/main.o bin/Watchable.o
	@echo 'Finished building target: splflix'
	@echo ' '

# Depends on the source and header files
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp

bin/User.o: src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

#Clean the build directory
clean: 
	rm -f bin/*
