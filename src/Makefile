.PHONY: all build install uninstall dvi dist tests gcov_report clang cppcheck leaks valgrind clean test
CC = g++ -std=c++17
CFLAGS = -Wall -Werror -Wextra
CPPCHECKFLAGS = --enable=all --language=c++ --std=c++17 --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=passedByValue --suppress=unmatchedSuppression --suppress=missingInclude --suppress=unusedStructMember

SOURCES :=model/caves.cc \
	model/maze.cc \
	view/caveArea.cpp \
	view/mazeArea.cpp \
	view/renderer.cc \
	view/view.cpp 
SOURCES_H :=controller/controller.h \
	model/caves.h \
	model/maze.h \
	view/renderer.h \
	view/view.h

OS = $(shell uname)
ifeq ($(OS),Darwin)
	CHECKFLAGS = -lgtest
	GCOVFLAGS = --coverage
else
	CHECKFLAGS = -lgtest -lpthread -lrt -lm
	GCOVFLAGS = -fprofile-arcs -ftest-coverage
endif

all: clean build

build:
	@rm -rf ./build
	mkdir -p ./build/; cd ./build/; cmake ../; make;

install: clean uninstall build
	mkdir -p $(HOME)/Desktop/maze/
	cp -rf ./build/maze.app $(HOME)/Desktop/maze/
	make clean
	@open $(HOME)/Desktop/maze/

uninstall:
	rm -rf $(HOME)/Desktop/maze/

dvi:
	mkdir -p ../Documentation
	makeinfo --html -o ../Documentation/ Documentation.texi
	open ../Documentation/index.html

dist: clean
	@mkdir -p ../maze.zip
	@cp -rf ./controller/ ../maze.zip/controller
	@cp -rf ./example ../maze.zip/example
	@cp -rf ./model ../maze.zip/model
	@cp -rf ./view ../maze.zip/view
	@cp -rf ./main.cpp ../maze.zip/
	@cp -rf ./Makefile ../maze.zip/
	@cp -rf ./test.cc ../maze.zip/
	@cp -rf ./Documentation.texi ../maze.zip/
	@cp -rf ./CMakeLists.txt ../maze.zip/
	@cd ..; tar -cvzf maze.tar.gz maze.zip
	@cp ../maze.tar.gz $(HOME)/Desktop
	@rm -rf ../maze.zip
	@rm -rf ../maze.tar.gz
	@open $(HOME)/Desktop/

tests:
	$(CC) $(CFLAGS) test.cc model/maze.cc model/caves.cc $(CHECKFLAGS) -o a.out
	./a.out

gcov_report:
	$(CC) $(CFLAGS) $(GCOVFLAGS) test.cc model/maze.cc model/caves.cc $(CHECKFLAGS) -o gcov_rep.out
	./gcov_rep.out
	lcov -t "gcov_rep" --no-external --output-file=coverag.info --capture --directory .
	genhtml -o ./report coverag.info
	@ open report/index.html

clang:
	@cp ../materials/linters/.clang-format ./
	@clang-format -i controller/*
	@clang-format -i model/*
	@clang-format -i view/*.cpp
	@clang-format -i view/*.cc
	@clang-format -i view/*.h
	@clang-format -i main.cpp
	@clang-format -i test.cc

	clang-format -n controller/*
	clang-format -n model/*
	clang-format -n view/*.cpp
	clang-format -n view/*.cc
	clang-format -n view/*.h
	clang-format -n main.cpp
	clang-format -n test.cc

	@rm .clang-format

cppcheck:
	@cppcheck $(CPPCHECKFLAGS) $(SOURCES)
	@cppcheck $(CPPCHECKFLAGS) $(SOURCES_H)
	@cppcheck $(CPPCHECKFLAGS) main.cpp
	@cppcheck $(CPPCHECKFLAGS) test.cc

leaks:
	CK_FORK=no leaks --atExit -- ./a.out

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./a.out

clean:
	rm -f *.o *.a *.gcno *.gcda *.info *.out
	rm -rf ./report/
	rm -rf ./build/
