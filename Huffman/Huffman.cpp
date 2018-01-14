// Huffman.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int main()
{
	char text[TEXTLENGTH];
	char num[CODELENGTH];
	gets_s(text);
	HuffTree<char> hufftree(text);
	hufftree.encode();
	cout << endl;
	gets_s(num);
	hufftree.decode(num);
	system("pause");
    return 0;
}

