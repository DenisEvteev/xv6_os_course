CC = gcc
CFLAGS = -O2 -ftrapv -fsanitize=address,undefined -Wall -Werror \
	-Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default \
	-Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body \
  	-Wstrict-prototypes -Wold-style-definition -Wmissing-field-initializers \
  	-Wnested-externs -Wno-pointer-sign -std=gnu11 -lm -Wold-style-declaration \
  	-Wmissing-parameter-type -Wlogical-op

OBJS = processing.o run.o parser.o main.o

.PHONY : all
all : main

main : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o : run.h

run.o : run.h

processing.o : processing.h

parser.o : parser.h


.PHONY : clean
clean :
	-rm main
	rm *.o