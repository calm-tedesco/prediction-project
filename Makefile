BIN = ./bin
OBJ = ./obj
SRC = ./src
INC = ./include
LIB = ./lib
DOC = ./doc
CXX = g++

CPPFLAGS = -I$(INC) -std=c++11


all: $(BIN)/ejercicio1

ejercicio: $(BIN)/ejercicio1
	$(BIN)/ejercicio1 data/Meteologica_vacante_ProgC_ProblemaDatos_20190903.txt

# ************ Generación de documentación ******************
documentacion: 
	doxygen $(DOC)/Doxyfile

# ************ Compilación de módulos ************
$(BIN)/ejercicio1: $(OBJ)/main.o $(LIB)/libutilidades.a
	$(CXX) $(OBJ)/main.o -L$(LIB) -lutilidades -o $(BIN)/ejercicio1

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CXX) -c $(SRC)/main.cpp -o $(OBJ)/main.o $(CPPFLAGS)

$(LIB)/libutilidades.a: $(OBJ)/deming.o
	ar rvs $(LIB)/libutilidades.a $(OBJ)/deming.o

$(OBJ)/deming.o: $(SRC)/deming.cpp
	$(CXX) -c $(SRC)/deming.cpp -o $(OBJ)/deming.o $(CPPFLAGS)

# ************ Limpieza ************
clean:
	echo "Limpieza de archivos objeto y biblioteca..."
	rm $(OBJ)/*.o $(LIB)/*.a

mrproper: clean
	echo "Limpieza de ejecutables..."
	rm $(BIN)/*
