.PHONY: all debug

RELEASE_OPTIONS = -O2 -std=c++17
DEBUG_OPTIONS = -g -std=c++17 -D"DEBUG"

all: 
	$(MAKE) -C parser
	g++ main.cpp parser/lex.yy.cc parser/parser.tab.cc -o main.out $(RELEASE_OPTIONS)

debug: 
	$(MAKE) -C parser
	g++ main.cpp parser/lex.yy.cc parser/parser.tab.cc -o main.out $(DEBUG_OPTIONS)	


