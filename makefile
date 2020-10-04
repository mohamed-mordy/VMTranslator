objects = dict.o main.o mystring.o helpers.o

VMTranslator: $(objects)
	gcc -o VMTranslator $(objects)

main.o: main.c
	gcc -c main.c
dict.o: dict.c
	gcc -c dict.c
helpers.o: helpers.c
	gcc -c helpers.c
mystring.o: mystring.c
	gcc -c mystring.c

clean:
	del *.exe; del *.o;
