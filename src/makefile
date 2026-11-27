CC = clang
OBJECTS = *.o
FILES += main.cpp
FILES += Scanner.cpp
FILES += Lox.cpp

cmp: $(OBJECTS)
	$(CC) $(OBJECTS) -o main.exe

dbg: $(OBJECTS)
	$(CC) $(OBJECTS) -g -o main.exe

run: cmp
	main.exe

$(OBJECTS): $(FILES)
	$(CC) -c *.cpp

clean:
	del *.o *.exe
