objects = dict.o main.o helpers.o mystring.o
flags = -Wall -Werror

VMTranslator: $(objects)
	gcc $(flags) -o VMTranslator $^

main.o: main.c
	gcc $(flags) -c main.c
dict.o: dict.c dict.h
	gcc $(flags) -c dict.c
helpers.o: helpers.c helpers.h
	gcc $(flags) -c helpers.c
mystring.o: mystring.c mystring.h
	gcc $(flags) -c mystring.c

.PHONY: clean
clean:
	del *.exe; del *.o;
