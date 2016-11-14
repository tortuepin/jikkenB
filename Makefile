BIN = ../bin
CC = gcc
OPTION = -g
ARG = -1

all: 1-1 1-2 2-1 3-1

1-1:	1-1.c
	${CC} -o ${BIN}/1-1 1-1.c

1-2:	1-2.c
	${CC} -o ${BIN}/1-2 1-2.c

2-1:	2-1.c
	${CC} -o ${BIN}/2-1 2-1.c

3-1:	3-1.c
	${CC} ${OPTION} -o ${BIN}/3-1 3-1.c

3-2:	3-2.c
	${CC} ${OPTION} -o ${BIN}/3-2 3-2.c

3-3:	3-3.c
	${CC} ${OPTION} -o ${BIN}/3-3 3-3.c

run3-1: 3-1
	${BIN}/3-1

run3-2: 3-2
	${BIN}/3-2

run3-3: 3-3
	${BIN}/3-3 ${ARG}
