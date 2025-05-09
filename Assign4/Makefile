# Compiler and flags
CXX = g++
CXXFLAGS = -c -g -Wall -std=c++17

# Executable name
PROGRAM = main.out

# Source files
CPPFILES = FinalAssignment.cpp Pattern.cpp RabinKarp.cpp Logger.cpp Linear.cpp Chaining.cpp

# Object files
OBJFILES = $(CPPFILES:.cpp=.o)

# Default rule
all: $(PROGRAM)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Link object files into final executable
$(PROGRAM): $(OBJFILES)
	$(CXX) -o $(PROGRAM) $(OBJFILES)

# Clean rule
clean:
	rm -f $(PROGRAM) *.o frequencies.txt logger.txt

# Run rule with example input/output
run:
	./$(PROGRAM) "A_Scandal_In_Bohemia.txt" "frequencies.txt"