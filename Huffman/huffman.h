#pragma once

#ifndef HUFFMAN_TREE_
#define HUFFMAN_TREE_
#include "stdafx.h"
#define NODE 100
#define TEXTLENGTH 1000
#define CODELENGTH 10000

template <typename E> class HuffNode {
private:
	int wgt; //weight
	bool isLeaf;
	E it;
	HuffNode<E>* lc;
	HuffNode<E>* rc;
	bool isMoveFromFore;
public:
	~HuffNode() {}
	HuffNode(const int& w, const E& e) { //leaf
		wgt = w;   it = e;  isLeaf = 1;
		lc = rc = NULL;
		isMoveFromFore = 0;
	}
	HuffNode(HuffNode<E>* l, HuffNode<E>* r) {
		lc = l;  rc = r;
		wgt = l->weight() + r->weight();
		isLeaf = 0;
		isMoveFromFore = 0;
	}
	HuffNode(HuffNode<E>* t) {
		wgt = t->weight();
		isLeaf = t->judgeLeaf();
		it = t->val();
		lc = t->getLeft();
		rc = t->getRight(); 
		isMoveFromFore = 0;
	}
	bool judgeMove() {
		return isMoveFromFore;
	}
	void setMove() {
		isMoveFromFore = 1;
	}
	HuffNode() {
	}
	bool judgeLeaf() {
		return isLeaf;
	}
	E val() {
		return it;
	}
	HuffNode<E>* getLeft() const {
		return lc;
	}
	HuffNode<E>* getRight() const {
		return rc;
	}
	void setWeight(int w) {
		wgt = w;
	}
	int weight() {
		return wgt;
	}
};

template<typename E>
struct information {
	int freq;
	E it;
};

template <typename E>
class forest {
private:
	information<E> s[NODE];
	HuffNode<E>* Root;
	HuffNode<E>** treeArray;
	int numOfInformation;
public:
	forest() {}
	forest(E st[TEXTLENGTH]) {
		initialInformation(st);
		sortInformation();
		int count = numOfInformation;
		treeArray = new HuffNode<E>*[NODE];
		for (int i = 0; i < count; i++)
			treeArray[i] = new HuffNode<E>(s[i].freq, s[i].it);
	}
	void initialInformation(E text[TEXTLENGTH]) {
		numOfInformation = 0;
		bool apear = 0; int i, k;
		for (i = 0; text[i] != '\0'; i++) {
			for (k = 0; k < numOfInformation; k++)
				if (text[i] == s[k].it) {
					s[k].freq++;
					break;
				}
			if (k == numOfInformation) {
				s[numOfInformation].it = text[i];
				s[numOfInformation].freq = 1;
				numOfInformation++;
			}
		}
	}
	void sortInformation() {
		for (int i = 0; i < numOfInformation; i++) {
			int lowindex = i;
			for (int k = numOfInformation - 1; k > i; k--)
				if (s[k].freq < s[lowindex].freq)
					lowindex = k;
			swap(s[i], s[lowindex]);
		}
	}
	void sortHuff(int k) {
		HuffNode<E>* temp1 = treeArray[k];
		HuffNode<E>* temp2 = NULL;
		for (int i = k + 1; i < numOfInformation; i++) {
			temp2 = treeArray[i];
			if (temp2->judgeMove())
				continue;
			if (temp1->weight() <= temp2->weight())
				break;
			swap(treeArray[i], treeArray[k]);
			k = i;
		}
	}
	HuffNode<E>* buidHuff() {
		int count = numOfInformation;
		HuffNode<E>* temp1 = NULL;
		HuffNode<E>* temp2 = NULL;
		while (count > 1) {
			int final = 0;
			for (int i = 0; i < numOfInformation; i++) {
				temp1 = treeArray[i];
				if (!temp1->judgeMove()) {
					temp1->setMove();
					for (int k = i + 1; k < numOfInformation; k++) {
						temp2 = treeArray[k];
						if (!temp2->judgeMove()) {
							treeArray[k] = new HuffNode<E>(temp1, temp2);
							final = k;
							count--;
							break;
						}
					}
					break;
				}
			}
			Root = treeArray[final];
			sortHuff(final);
		}
		return Root;
	}
	~forest() {}
};

template <typename E>
struct helpCode {
	char num[50];
	E it;
};

template <typename E>
class HuffTree {
private:
	forest<E>* fore;
	HuffNode<E>* Root;
	E textStore[TEXTLENGTH];
	helpCode<E> letter[NODE];
	int numOfCode;
	int textCodeLenth;
public:
	HuffTree(E s[TEXTLENGTH]) {
		int i;
		Root = NULL;
		numOfCode = 0;
		textCodeLenth = 0;
		for (i = 0; s[i] != '\0'; i++)
			textStore[i] = s[i];
		textStore[i] = '\0';
		fore = new forest<E>(s);
	}
	void findCodeHelp(HuffNode<E>* root, int i, char codeNum[50], E& key) {
		if (root->judgeLeaf()) {
			if (root->val() != key) 
				return;
			int k;
			for (k = 0; k < i; k++)
				letter[numOfCode].num[k] = codeNum[k];
			letter[numOfCode].num[k] = '\0';
			letter[numOfCode].it = key;
			numOfCode++;
			return;
		}
		else {
			if (root->getLeft() != NULL) {
				codeNum[i] = '0';
				findCodeHelp(root->getLeft(), i + 1, codeNum, key);
			}
			if (root->getRight() != NULL) {
				codeNum[i] = '1';
				findCodeHelp(root->getRight(), i + 1, codeNum, key);
			}
			i++;
		}
		return;
	}
	void printHelp(char num[50]) {
		for (int i = 0; num[i] != '\0'; i++)
			cout << num[i];
	}

	void findCode(HuffNode<E>* root, E & it) {
		for (int i = 0; i < numOfCode; i++)
			if (letter[i].it == it) {
				printHelp(letter[i].num);
				return;
			}
		char num[50];
		findCodeHelp(Root, 0, num, it);
		printHelp(letter[numOfCode - 1].num);
	}
	
	void encode() {
		Root = fore->buidHuff();
		for (int i = 0; textStore[i] != '\0'; i++) 
			findCode(Root, textStore[i]);
	}
	void decodeHelp(HuffNode<E>* root, int i, char decodeText[CODELENGTH]) {
		if (root->judgeLeaf()) {
			cout << root->val();
			if (decodeText[i] != '\0')
				decodeHelp(Root, i, decodeText);
			return;
		}
		if (decodeText[i] == '1')
			decodeHelp(root->getRight(), ++i, decodeText);
		else
			decodeHelp(root->getLeft(), ++i, decodeText);
		return;
	}
	void decode(char decodeText[CODELENGTH]) {
		int i = 0;
		decodeHelp(Root, 0, decodeText);
	}
	~HuffTree() {}
	HuffNode<E>* root() {
		return Root;
	}
};


#endif // !HUFFMAN_TREE_
