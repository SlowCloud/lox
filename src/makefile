CC = clang
OBJECTS = *.o
FILES += main.cpp
FILES += Scanner.cpp
FILES += Lox.cpp
FILES += Expr.h
FILES += Lox.h

cmp: $(OBJECTS)
	$(CC) -o main.exe $(OBJECTS)

dbg: $(OBJECTS)
	$(CC) -g -o main.exe $(OBJECTS)

run: cmp
	main.exe

$(OBJECTS): $(FILES)
	$(CC) -c *.cpp

clean:
	del *.o *.exe
