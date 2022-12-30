#include "UserInterface.h"
#include "ExpressionTree.h"
#include "SimpleCalc.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "Generator.h"
using namespace std;


void playGame(ostream& out, ifstream& ulaz) {
	//ifstream ulaz("Ulaz.txt");
	int x;
	int ukupno_pobeda1 = 0;
	int ukupno_pobeda2 = 0;
	int ukupno_rundi = 0;

	while (ulaz >> x) {
		vector<int> brojevi(7);
		cout << "Ponudjeni brojevi: ";
		brojevi[0] = x;
		for (int i = 1; i < 7; i++) {
			ulaz >> x;
			brojevi[i] = x;
			if (i != 6)
				cout << x << " ";
			else {
				cout << endl << "Trazeni broj: " << x << endl;
			}
		}
		int resenje = brojevi[6];
		ukupno_rundi++;
		out << ukupno_rundi << ".\t";
		for (int bb : brojevi) {
			out << bb << " ";
		}
		vector<int> brojevitmp = brojevi;
		brojevitmp.pop_back();
		int rez1 = playerResult(1, out, brojevitmp);
		int rez2 = 0;
		if (rez1 != resenje) {
			rez2 = playerResult(2, out, brojevitmp);
		}
		if (rez1 != resenje && rez2 != resenje) {
			vector<Node*> vc;
			vector<Node*> min_vc;
			int minComp = 0;
			for (int j = 2; j <= 6; j++) {
				Generator gen(brojevi, j);

				int rez3;
				nadjiRezultat(gen, vc);
				rez3 = countResult(vc);
				if (rez3 == resenje) {
					minComp = rez3;
					min_vc = vc;
					break;
				}
				if (abs(resenje - minComp) > rez3) {
					minComp = rez3;
					min_vc = vc;
				}
			}
			printExpression(out, min_vc);
			
		}
		else {
			out << "\tXXXXXXXX";
		}
		if (abs(rez2 - resenje) < abs(rez1 - resenje)) {
			out << "\t" << 2 << endl;
			ukupno_pobeda2++;
		}
		else {
			out << "\t\t" << 1 << endl;
			ukupno_pobeda1++;
		}
		
	}
	out << endl;
	out << "Ukupno rundi: " << ukupno_rundi << "\tigrac 1: " << ukupno_pobeda1 << "\tigrac2: " << ukupno_pobeda2;
	if (ukupno_pobeda1 < ukupno_pobeda2)
		out << "\tPobednik je igrac 2";
	else
		if (ukupno_pobeda2 < ukupno_pobeda1)
			out << "\tPobednik je igrac 1";
		else
			out << "\tNereseno";
}


vector<int>::iterator findBr(vector<int>& brs, int target) {
	vector<int>::iterator iter = brs.begin();
	for (iter; iter < brs.end(); iter++) {
		if (*iter == target)
			return iter;
	}
	return iter;

}

void isValidExpression(int poz, vector<int> ponudjeni, vector<int>& brs, vector<char>& ops)
{
	bool greska = false;
	bool kraj = false;
	while (!kraj)
	{
		kraj = true;
		brs.clear();
		ops.clear();
		if (greska) {
			greska = false;
			cout << "Invlaid expression!" << endl;
			string s;
			cin.clear();
			cin.ignore();
			getline(cin, s);

		}
		cout << "Player " << poz << " expression: ";
		int i = 0;
		char x;
		while (cin >> x)
		{
			if (x == ';') {
				break;
			}
			cin.putback(x);
			i++;
			if (i % 2 == 1) {
				int br;
				cin >> br;
				if (cin.fail()) {
					greska = true;
					kraj = false;
					break;
				}
				vector<int>::iterator iter = findBr(ponudjeni, br);
				if (iter == ponudjeni.end()) {
					greska = true;
					kraj = false;
					break;
				}
				brs.push_back(br);
				ponudjeni.erase(iter, iter + 1);
			}
			else {
				if (i > 11) {
					greska = true;
					kraj = false;
					break;
				}
			
				char c;
			
				cin >> c;

				if (c != '+' && c != '-' && c != '*' && c != '/' && c != '(' && c != ')') {
					greska = true;
					kraj = false;
					break;
				}
				ops.push_back(c);
			}
		}
	}
}
