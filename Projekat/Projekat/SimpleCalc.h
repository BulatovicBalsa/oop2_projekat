#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

template<typename T>
class Token {
public:
	char kind;
	T value;
	string name;
	inline Token(char ch) :kind(ch), value(0) { }
	inline Token(char ch, T val) : kind(ch), value(val) { }
	inline Token(char ch, string s) : name(s), kind(ch), value(0) { }
};

template<typename T>
class Token_stream {
	bool full;
	Token<T> buffer;
public:
	vector<T> ponudjeni;

	bool contains(T br);
	inline Token_stream<T>() :full(0), buffer(0) { }

	inline Token<T> get();
	inline void unget(Token<T> t) { buffer = t; full = true; }

	inline void ignore(char);
};

template<typename T> bool Token_stream<T>::contains(T br) {
	T tmp;
	typename vector<T>::iterator iter;
	for (iter = ponudjeni.begin(); iter < ponudjeni.end(); iter++) {
		if (br == *iter) {
			ponudjeni.erase(iter, iter + 1);
			return true;
		}
	}
	return false;
}

const char print = ';';
const char number = '8';

template<typename T>
inline Token<T> Token_stream<T>::get()
{
	if (full) { full = false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
		return Token<T>(ch);
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{	cin.unget();
	T val;
	cin >> val;
	return Token<T>(number, val);
	}
	default:
		throw invalid_argument("Bad token");
	}
}

template<typename T>
inline void Token_stream<T>::ignore(char c)
{
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin >> ch)
		if (ch == c) return;
}



static Token_stream<int> ts;

template<typename T>
inline T expression();

template<typename T>
inline T primary()
{
	Token<T> t = ts.get();
	switch (t.kind) {
	case '(':
	{	T d = expression<T>();
	t = ts.get();
	if (t.kind != ')') throw invalid_argument("'(' expected");
	return d;
	}
	case '-':
		throw invalid_argument("cant make negative numbers");
	case number:
		if (!ts.contains(t.value))
			throw invalid_argument("primary not exist");
		return t.value;
	default:
		throw invalid_argument("primary expected");
	}
}

template<typename T>
inline T term()
{
	T left = primary<T>();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary<T>();
			break;
		case '/':
		{	T d = primary<T>();
		if (d == 0) throw invalid_argument("divide by zero");
		if (left % d != 0) throw invalid_argument("non int division");
		left /= d;
		break;
		}
		default:
			ts.unget(t);
			return left;
		}
	}
}

template<typename T>
inline T expression()
{
	T left = term<T>();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '+':
			left += term<T>();;
			break;
		case '-':
			left -= term<T>();;
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}


template<typename T>
inline T statement()
{
	Token t = ts.get();
	switch (t.kind) {
	default:
		ts.unget(t);
		return expression<T>();
	}
}

inline void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

template<typename T>
inline T calculate()
{
	try {
		//cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t = ts.get();
		ts.unget(t);
		T res = statement<T>();
		t = ts.get();
		if (t.kind != ';')
			throw invalid_argument("invalid expression");
		string kraj;
		getline(cin, kraj);
		if (!kraj.empty())
			throw invalid_argument("invalid expression");
		cout << result << res << endl;
		return res;
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
		return -1;
	}
}

template<typename T>
inline T playerResult(int who, ostream& os, vector<T> ponudjeni) {
label1:
	try {
		ts.ponudjeni = ponudjeni;
		string::reverse_iterator riter;
		cout << "Player " << who << " expression: ";
		string izraz;
		getline(cin, izraz);
		cin.putback('\n');
		for (riter = izraz.rbegin(); riter < izraz.rend(); riter++) {
			cin.putback(*riter);
		}
		T rez1 = calculate<T>();
		izraz.pop_back();
		os << '\t' << izraz;
		return rez1;
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		cin.clear();
		string s;
		getline(cin, s);
		goto label1;
	}
	catch (...) {
		cerr << "idk exception: " << endl;
		cin.clear();
		string s;
		getline(cin, s);
		goto label1;
	}

	return -1;
}