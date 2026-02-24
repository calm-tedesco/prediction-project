BIN = ./bin
OBJ = ./obj
SRC = ./src
INC = ./include
LIB = ./lib
DOC = ./doc
CXX = g++

CPPFLAGS = -I$(INC) -std=c++11


all: $(BIN)/exercise1

exercise: $(BIN)/exercise1
	$(BIN)/exercise1 ./data/wind_data.txt

# ************ Doc generation ******************
doc: 
	doxygen $(DOC)/Doxyfile

# ************ Modules compilation ************
$(BIN)/exercise1: $(OBJ)/main.o $(LIB)/libutilities.a
	$(CXX) $(OBJ)/main.o -L$(LIB) -lutilities -o $(BIN)/exercise1

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CXX) -c $(SRC)/main.cpp -o $(OBJ)/main.o $(CPPFLAGS)

$(LIB)/libutilities.a: $(OBJ)/deming.o
	ar rvs $(LIB)/libutilities.a $(OBJ)/deming.o

$(OBJ)/deming.o: $(SRC)/deming.cpp
	$(CXX) -c $(SRC)/deming.cpp -o $(OBJ)/deming.o $(CPPFLAGS)

# ************ Cleaning ************
clean:
	echo "Cleaning object files and libraries..."
	rm $(OBJ)/*.o $(LIB)/*.a

mrproper: clean
	echo "Cleaning executable..."
	rm $(BIN)/*
