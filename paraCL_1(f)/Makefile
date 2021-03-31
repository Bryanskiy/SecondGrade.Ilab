.PHONY: all debug

RELEASE_OPTS = -O2

DEBUG_OPS = -D"DEBUG" -g -fno-elide-constructors

all:
	$(MAKE) -C parser
	g++ driver/driver.cpp node_interface/node.cpp parser/compiler.tab.cc parser/lex.yy.cc main.cpp -o main.out $(RELEASE_OPTS)

debug:
	$(MAKE) -C parser
	g++ driver/driver.cpp node_interface/node.cpp parser/compiler.tab.cc parser/lex.yy.cc main.cpp -o main.out $(DEBUG_OPS)