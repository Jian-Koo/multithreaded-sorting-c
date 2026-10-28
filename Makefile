CC = gcc
CFLAGS = -Wall -pedantic -Werror
RM = /bin/rm
OBJ = sss.o SortingFunction.o
EXEC = sss

ifdef CUBE
CFLAGS += -D CUBE
CUBE : clean $(EXEC)
endif

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

%.o : %.c 
	$(CC) $(CFLAGS) -c $<

clean : 
	$(RM) $(EXEC) $(OBJ)

SRCS = sss.c SortingFunction.c

depend : 
	$(RM) .depend
	makedepend -f- -- $(CFLAGS) -- $(SRCS) > .depend 2>/dev/null

-include .depend
