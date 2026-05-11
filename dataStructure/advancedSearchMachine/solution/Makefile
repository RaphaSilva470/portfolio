CC = g++
CFLAGS = -Wall -c
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/buscar.o $(OBJ)/indice.o $(OBJ)/documento.o $(OBJ)/memlog.o
HDRS = $(INC)/documento.h $(INC)/indice.h $(INC)/memlog.h $(INC)/msgassert.h
CFLAGS = -Wall -c -I$(INC)


$(BIN)/buscar: $(OBJS)
	$(CC) -o $(BIN)/buscar $(OBJS) $(LIBS)

$(OBJ)/buscar.o: $(HDRS) $(SRC)/buscar.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/buscar.o $(SRC)/buscar.cpp 

$(OBJ)/documento.o: $(HDRS) $(SRC)/documento.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/documento.o $(SRC)/documento.cpp 

$(OBJ)/indice.o: $(HDRS) $(SRC)/indice.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/indice.o $(SRC)/indice.cpp

$(OBJ)/memlog.o: $(HDRS) $(SRC)/memlog.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/memlog.o $(SRC)/memlog.cpp 
	
clean:
	rm $(OBJS) 
  