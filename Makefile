#Commands
MKDIR := mkdir
RMDIR := rm -rf

#Folders to make life easier
BIN 	:= ./bin
OBJ 	:= ./obj
HDR     := ./hdr
SRC 	:= ./src

#Compiler features
CC      := gcc
CFLAGS  := -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align \
		  -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security \
		  -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long  \
		  -Wcast-qual -Wno-suggest-attribute=format -Wpedantic -Werror=declaration-after-statement -I$(HDR) \
		  -Wmissing-declarations
LDFLAGS := -fsanitize=address -fno-omit-frame-pointer
LIBS    := -lm

#Variables
EXE   := ./a.out
HDRS  := $(wildcard $(HDR)/*.h)
SRCS  := $(wildcard $(SRC)/*.c)
OBJS  := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))	
# $@ - the left side of the :
# $^ - the right side of the :
# $< - the first item in the dependencies list
# -c flag says to generate the object file

$(EXE): $(OBJS) | $(BIN)
	$(CC) $^ -o $@ $(LIBS) $(LDFLAGS) 

$(OBJ)/main.o: $(SRC)/main.c $(SRCS) $(HDRS) | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/fill.o: $(SRC)/fill.c $(HDR)/fill.h $(SRC)/matrix.c $(HDR)/matrix.h ${SRC}/error.c $(HDR)/error.h ${SRC}/multiply.c ${HDR}/multiply.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/gauss_inverse.o: $(SRC)/gauss_inverse.c $(HDR)/gauss_inverse.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/matrix.o : $(SRC)/matrix.c $(HDR)/matrix.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/print.o : $(SRC)/print.c $(HDR)/print.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/error.o : $(SRC)/error.c $(HDR)/error.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/solve.o : $(SRC)/solve.c $(SRC)/matrix.c $(HDR)/matrix.h $(SRC)/gauss_inverse.c $(HDR)/gauss_inverse.h $(SRC)/error.c $(HDR)/error.h  $(HDR)/multiply.h ${SRC}/multiply.c $(HDR)/extract.h ${SRC}/extract.c | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)

${OBJ}/multiply.o : ${SRC}/multiply.c $(HDR)/multiply.h | ${OBJ}
	$(CC) -c $< -o $@ $(CFLAGS)

${OBJ}/extract.o : ${SRC}/extract.c $(HDR)/extract.h  | ${OBJ}
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN) $(OBJ):
	$(MKDIR) $@

.PHONY: all clean debug release test

all: clean release test

clean:
	$(RMDIR) $(OBJ) $(BIN) $(EXE)

test:
	./tests/test.sh | tee log.txt && grep Residual log.txt | tee residual.txt

debug: CFLAGS += -g -O0 -D DEBUG
debug: $(EXE) 

release: CFLAGS += -O3 -D RELEASE
release: $(EXE)