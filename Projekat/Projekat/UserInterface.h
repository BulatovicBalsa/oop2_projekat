#pragma once
#include <vector>
#include <fstream>
using namespace std;

void playGame(ostream& out, ifstream& ulaz);
void isValidExpression(int poz, vector<int>, vector<int>& brs, vector<char>& ops);
vector<int>::iterator findBr(vector<int>& brs, int target);

