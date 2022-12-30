#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include "Generator.h"
#include "MyStack.h"
using namespace std;


class Node
{
public:
	inline Node();
	int getTip() { return tip; }
	void setTip(int t) { tip = t; }
	virtual string ToS() = 0;
	virtual ~Node() {};
protected:
	int tip;
};

inline Node::Node():tip(0) { }

class Operacija : public Node
{
public:
	inline Operacija(char c);
	inline Operacija() :Node(), oper() { tip = 2; };
	char getOper() { return oper; }
	Operacija* operator = (Operacija o) {
		oper = o.oper;
		return this;
	}
	string ToS() {
		char ss[2];
		ss[0] = oper;
		ss[1] = '\0';
		string t = string(ss);
		return t;
	}
private:
	char oper;
};

inline Operacija::Operacija(char c) : Node(), oper(c) { tip = 2; }


class Broj : public Node
{
public:
	inline Broj(int t);
	int getBroj() { return broj; }
	string ToS() { return to_string(broj); }
private:
	int broj;
};

inline Broj::Broj(int t) : Node(), broj(t) { tip = 1; }

class Izraz : public Node {
public:
	string izraz;
	Izraz(string i) : izraz(i), Node() { tip = 4; }
	string ToS() { return izraz; }
};

inline vector<Node*> postfix(vector<int> brojevi, vector<char> operacije) {
	map<char, int> mapa;
	mapa.insert({ '+', 1 });
	mapa.insert({ '-', 1 });
	mapa.insert({ '*', 2 });
	mapa.insert({ '/', 2 });
	mapa.insert({ '(', 3 });
	mapa.insert({ ')', 3 });

	MyStack<Operacija> stek = MyStack<Operacija>();
	vector<Node*> rez;
	Operacija* op = NULL;
	
	for (int i = 0; i < brojevi.size(); i++) { //6
		Broj* br = new Broj(brojevi[i]);
		bool kraj = false;

		if (i < brojevi.size()-1) { //5
			op = new Operacija(operacije[i]);
		}
		else
		{
			kraj = true;
		}
		rez.push_back(br);

		if (kraj) {
			kraj = false;
			while (!stek.isEmpty())
			{
				op = new Operacija(stek.pop());
				rez.push_back(op);
			}
			return rez;
		}
		
		if (stek.isEmpty()) {
			stek.push(*op);
			delete op;
			continue;
		}
		if (mapa[op->getOper()] > mapa[stek.top().getOper()]) {
			stek.push(*op);
			delete op;
		}
		else {
			Operacija tmp = *op;
			delete op;
			if ((mapa[tmp.getOper()] == mapa[stek.top().getOper()])) {
				op = new Operacija(stek.pop());
				rez.push_back(op);
				stek.push(tmp);
			}
			else {
				while (!stek.isEmpty())
				{
					op = new Operacija(stek.pop());
					//*op = stek.pop();
					rez.push_back(op);
				}
				stek.push(tmp);
			}
		}
		
	}
	return rez;
}

inline void infix(vector<Node*> postfix, vector<int>& brojevi, vector<char>& operacije) {
	operacije.clear();
	brojevi.clear();
	bool inside = false;
	vector<Node*>::iterator iter;
	vector<char> tmp;
	for (iter = postfix.begin(); iter < postfix.end(); iter++) {
		tmp.clear();
		while ((*iter)->getTip() == 1)
		{
			brojevi.push_back(static_cast<Broj*>(*iter)->getBroj());
			iter++;
			if (iter == postfix.end()) {
				iter--;
				break;
			}
		}
		while ((*iter)->getTip() == 2) {
			inside = true;
			tmp.push_back(static_cast<Operacija*>(*iter)->getOper());
			iter++;
			if (iter == postfix.end()) {
				break;
			}
		}
		if(inside)
			iter--;
		inside = false;
		for (int i = tmp.size() - 1; i > -1; i--) {
			operacije.push_back(tmp[i]);
		}

	}
}

inline ostream& PE(ostream& os, vector<Node*> postfix) {
	vector<int> brojevi;
	vector<char> operacije;
	infix(postfix, brojevi, operacije);
	vector<char>::iterator citer;
	vector<int>::iterator inter = brojevi.begin();
	os << "\t";
	int brojac = 0;
	int pocetak = 0;
	for (int i = 0; i < operacije.size(); i++) {
		os << '(';
	}
	for (citer = operacije.begin(); citer < operacije.end(); citer++) {
		brojac = 0;
		if (*citer != '(' && *citer != ')') {
			os << *inter;
			if (pocetak != 0)
				os << ')';
			else
				pocetak = 1;
			os << *citer;
			inter++;
			continue;
		}
		while ((*citer == '(') || (*citer == ')'))
		{
			brojac++;
			citer++;
			if (citer == operacije.end())
				break;
		}
		for (int i = 0; i < brojac; i++) {
			os << '(';
		}
		citer -= brojac;
		brojac = 0;
		while (*citer == '(' || *citer == ')') {
			if (brojac == 0) {
				os << *inter;
				inter++;
			}
			if (*citer == '(') {
				os << '+';
			}
			else {
				os << '-';
			}
			os << *inter;
			os << ')';
			inter++;
			citer++;
			brojac++;
		}
		if (brojac != 0) {
			os << ')';
			inter--;
			os << *citer;
		}

		//os << *inter;
		//os << *citer;
		inter++;
	}

	if (brojac == 0)
		os << *inter;
	os << endl;
	return os;
}

inline vector<Node*>::iterator findOper(vector<Node*>& postfix) {
	vector<Node*>::iterator iter = postfix.begin();
	for (iter; iter < postfix.end(); iter++) {
		if ((*iter)->getTip() == 2)
			return iter;
	}
	return postfix.end();
}

inline void clearVector(vector<Node*>& vc) {
	for (Node* v : vc) {
		delete v;
	}
}

inline ostream& printExpression(ostream& os, vector<Node*> postfix) {
	vector<Node*> novi_izrazi;
	bool prvi = true;
	os << '\t';
	while (postfix.size() != 1) {
		bool zagrade = false;
		vector<Node*>::iterator iter = findOper(postfix);
		string s;
		string iz1 = (*(iter - 2))->ToS();
		string iz2 = (*(iter - 1))->ToS();
		string oper = (*(iter))->ToS();

		if (oper == "(") {
			oper = "+";
		}
		if (oper == ")") {
			oper = "-";
		}

		s = "(" + iz1 + oper + iz2 + ")";
		Izraz* izraz = new Izraz(s);

		novi_izrazi.push_back(izraz);
		postfix.insert(iter + 1, izraz);

		iter = findOper(postfix);
		postfix.erase(iter - 2, iter + 1);
	}
	string s = postfix[0]->ToS();
	if (s[s.length() - 1] == ')') {
		s.pop_back();
		s.erase(s.begin());
	}
	os << s; // << endl;
	clearVector(novi_izrazi);

	return os;
}



inline int countResult(vector<Node*> postfix) {
	Node* n = NULL;

	vector<Node*> novi_brojevi(8);
	bool prvi = true;
	while (postfix.size() != 1) {
		vector<Node*>::iterator iter = findOper(postfix);
		vector<Node*>::iterator iter1 = iter - 1;
		vector<Node*>::iterator iter2 = iter - 2;
		char op = static_cast<Operacija*>(*iter)->getOper();
		int a = static_cast<Broj*>(*iter2)->getBroj();
		int b = static_cast<Broj*>(*iter1)->getBroj();
		int c = 0;
		switch (op)
		{
		case '(':
		case '+':
			c = a + b;
			break;
		case ')':
		case '-':
			c = a - b;
			break;
		case '*':
			c = a * b;
			break;
		case '/':
			if (b == 0) {
				clearVector(novi_brojevi);
				return -1;
			}
			if (a % b != 0) {
				clearVector(novi_brojevi);
				return -1;
			}
			c = a / b;
			break;
		}
		
		//if((*iter2)->getTip())

		n = new Broj(c);
		
		novi_brojevi.push_back(n);
		

		postfix.insert(iter + 1, n);
		iter = findOper(postfix);
		
		
		postfix.erase(iter - 2, iter + 1);

	}
	int res = static_cast<Broj*>(postfix[0])->getBroj();
	clearVector(novi_brojevi);
	//preresult = postfix;
	return res;
}

inline void printVecNode(vector<Node*> vc) {
	vector <Node*>::iterator iter;
	for (iter = vc.begin(); iter != vc.end(); iter++) {
		Node* v = *iter;
		if (v->getTip() == 1) {
			cout << static_cast<Broj*>(v)->getBroj() << " ";
		}
		else
		{
			cout << static_cast<Operacija*>(v)->getOper() << " ";
		}
	}
	cout << endl;
}


inline bool nadjiRezultat(Generator gen, vector<Node*>& vv) {
	vector<Node*> minv;
	int minr = -1;
	vector<vector<int>> brojevi = gen.getBrojevi();
	vector<vector<char>> operacije = gen.getOperacije();
	for (vector<int> brs : brojevi) {
		for (int x : brs) {
			if (x == gen.getResenje()) {
				Broj* b = new Broj(x);
				vv = vector<Node*>({ b });
				printExpression(cout, vv);
				return true;
			}
		}
		
		for (vector<char> ops : operacije) {
			//cout << brojac2 << '\r';
			vector<Node*> vc = postfix(brs, ops);
			int c = countResult(vc);
			if (c == gen.getResenje()) {
				//printExpression(cout, vc);
				//clearVector(vc);
				vv = vc;
				clearVector(minv);
				return true;
			}
			if (abs(gen.getResenje() - c) < abs(gen.getResenje() - minr)) {
				minr = c;
				clearVector(minv);
				minv = vc;
			}
			else {
				clearVector(vc);
			}
		}
	}
	vv = minv;
	return false;
}