#include <iostream>
#include <vector>
#include "Generator.h"
#include "ExpressionTree.h"
#include "UserInterface.h"
#include <time.h>
#include <fstream>
using namespace std;



int main(int argc, char** argv)
{
    Generator gen = Generator({ 5,3,2,1,10,25,25 }, 5);
    ofstream rez("Rezultati.txt");
    ifstream ulaz(argv[1]);
    playGame(rez, ulaz);
    //vector<Node*> vv;
    //nadjiRezultat(gen, vv);
    //ofstream mf("Ulaz.txt");
    //mf.close();
    //gen.print();
    /*vector<int> a{5,3,2,1,10,25};
    vector<char> o{ '+','*','(',')','+' };
    vector<Node*> vc = postfix(a, o);
    
    int c = countResult(vc);
    cout << c << endl;
    */
    return 0;
}