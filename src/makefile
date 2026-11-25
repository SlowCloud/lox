CC = clang
OBJECTS = main.o

dbg: $(OBJECTS)
	$(CC) -g -o main.exe $(OBJECTS)

cp: $(OBJECTS)
	$(CC) -o main.exe $(OBJECTS)

run: cp
	main.exe

main.o: main.cpp
	$(CC) -c main.cpp
