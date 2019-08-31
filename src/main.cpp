#include <iostream>
#include "include/lexer.h"

#include <queue>

using namespace std;

int main(){

	Lexer lx;

	queue<string> tokens = lx.getTokenFromFile("test/hello_world.lx");

	int x = tokens.size();
	
	for(int i = 0; i < x; i++){
		cout << tokens.front() << endl;
		tokens.pop();
	}


	return 0;
}