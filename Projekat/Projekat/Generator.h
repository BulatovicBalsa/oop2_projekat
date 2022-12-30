#pragma once
#include <vector>
#include <iostream>
using namespace std;

class Generator
{
public:
	inline Generator(vector<int> postavka, int length);
	inline ~Generator();
	void print();
	inline vector<vector<int>> getBrojevi() { return brojevi; }
	inline vector<vector<char>> getOperacije() { return operacije; }
	inline int getResenje() { return resenje; }
private:
	vector<vector<int>> brojevi;
	vector<vector<char>> operacije;
	int resenje;
};

inline Generator::Generator(vector<int> postavka, int length) : brojevi(), operacije(), resenje(0)
{
	char opers[6] = { '+', '-', '*', '/', '(', ')' };
	resenje = postavka[6];
	postavka.pop_back();
	for (int i = 0; i < 6; i++) {
		if (length == 1) {
			vector<int> novi = { postavka[i] };
			brojevi.push_back(novi);
		}
		else {
			for (int j = 0; j < 6; j++) {
				if (i == j)
					continue;
				if (length == 2) {
					vector<int> novi = { postavka[i], postavka[j] };
					brojevi.push_back(novi);
				}
				else {
					for (int k = 0; k < 6; k++) {
						if (i == k || j == k)
							continue;
						if (length == 3) {
							vector<int> novi = { postavka[i], postavka[j], postavka[k] };
							brojevi.push_back(novi);
						}
						else {
							for (int m = 0; m < 6; m++) {
								if (m == k || m == j || m == i)
									continue;
								if (length == 4) {
									vector<int> novi = { postavka[i], postavka[j], postavka[k], postavka[m] };
									brojevi.push_back(novi);
								}
								else {
									for (int n = 0; n < 6; n++) {
										if (n == m || n == k || n == j || n == i)
											continue;
										if (length == 5) {
											vector<int> novi = { postavka[i], postavka[j], postavka[k], postavka[m], postavka[n] };
											brojevi.push_back(novi);
										}
										else {
											for (int z = 0; z < 6; z++) {
												if (z == n || z == m || z == k || z == j || z == i)
													continue;
												vector<int> novi = { postavka[i], postavka[j], postavka[k], postavka[m], postavka[n], postavka[z] };
												brojevi.push_back(novi);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (length == 1) {
		return;
	}
	for (int i = 0; i < 6; i++) {
		if (length == 2) {
			vector<char> novi = { opers[i] };
			operacije.push_back(novi);
		}
		else {
			for (int j = 0; j < 6; j++) {
				if (length == 3) {
					vector<char> novi = { opers[i], opers[j] };
					operacije.push_back(novi);
				}
				else {
					for (int k = 0; k < 6; k++) {
						if (length == 4) {
							vector<char> novi = { opers[i], opers[j], opers[k] };
							operacije.push_back(novi);
						}
						else {
							for (int l = 0; l < 6; l++) {
								if (length == 5) {
									vector<char> novi = { opers[i], opers[j], opers[k], opers[l] };
									operacije.push_back(novi);
								}
								else {
									for (int x = 0; x < 6; x++) {
										vector<char> novi = { opers[i], opers[j], opers[k], opers[l], opers[x] };
										operacije.push_back(novi);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

inline Generator::~Generator()
{
}

inline void Generator::print()
{
	int i = 0;
	vector<int>::iterator iter;
	for (vector<int> var : brojevi)
	{
		i++;
		cout << "(";
		for (int x : var) {
			cout << x << " ";
		}
		cout << "), ";
	}
	cout << endl;

	i = 0;
	for (vector<char> vc : operacije) {
		i++;
		cout << "(";
		for (char c : vc) {
			cout << c << " ";
		}
		cout << "), ";
	}
	cout << endl;
	cout << "ukupno: " << i << endl;
}
