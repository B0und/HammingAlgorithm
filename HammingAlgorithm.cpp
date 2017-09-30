// HammingAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HAMM.h"

using std::cout;
using std::endl;


int main()
 {
	//std::vector<int> word; = { 0,1,1,0,1,0,0,0,0,1,1,1,1,1 };
	//HAMM h1(word);
	HAMM h1;
	h1.set_word();
	cout << "word is : \n";
	h1.show_word();
	h1.encrypt();
	cout << "code word is : \n";
	h1.show_codeword();
	h1.set_codeword_mistake(2);
	//h1.set_codeword_mistake(10);
	cout << "code word with mistake is : \n";
	h1.show_codeword();
	h1.decipher();
	std::cin.clear();
	std::cin.get();
    return 0;
	
}

