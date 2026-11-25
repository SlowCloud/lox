CC = clang
OBJECTS = generateAst.o

generate: compile
	generateAst.exe ..

compile: $(OBJECTS)
	$(CC) -o generateAst.exe $(OBJECTS)

generateAst.o: generateAst.cpp
	$(CC) -c generateAst.cpp
