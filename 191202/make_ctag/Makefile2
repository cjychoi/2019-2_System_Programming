SRCS = main.c factorial.c hello.c
OBJS = $(SRCS:.c=.o)
TARGET = hello
CFLAG = -Wall -g 
LFLAGS = -L/home/test/lib -L../lib
INCLUDES = -I../include
LIBS = -lm 
CC = gcc 

all: ${TARGET}

hello: ${OBJS}
	  ${CC} ${CFLAGS} -o $@ ${OBJS} ${LFLAGS} ${LIBS}

.c.o: 
	${CC} ${CFLAGS} ${INCLUDES} -c $<

clean: 
	   rm -f ${OBJS} ${TARGET}


