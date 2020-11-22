all:
	flex ../scanner.l
	bison ../compiler.y -d 
	g++ ../driver.cpp lex.yy.cc compiler.tab.cc ../node.cpp -I. -I..