all:
	$(MAKE) -C parser
	g++ driver/driver.cpp node_interface/node.cpp parser/compiler.tab.cc parser/lex.yy.cc main.cpp -o main.out -g