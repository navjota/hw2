

CC = gcc
CFLAGS = -Wall

CSRC = hw2.c quadtree.c
HSRC = quadtree.h
OBJ = $(CSRC:.c=.o)

%o:%c $(HSRC)
	$(CC) $(CFLAGS) -o hw2 $<
#target

.PHONY: clean
.PHONY: test

#target rules
hw2: $(OBJ)
	$(CC) $(CFLAGS) -o hw2 $(OBJ)

clean: 
	rm -f $(OBJ)

test: 
	test.dat $(OBJ)