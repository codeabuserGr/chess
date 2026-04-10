#include <iostream>
#include "ChessBoard_setup.h"
#include <fstream>
using namespace std;
int main() {
    setup();
    ifstream file("game.txt");

    if (!file) {
        cout << "File not found!" << endl;
        return 1;
    }
    string turn, white_move, black_move;
    int i=1;
    while (file >> turn >> white_move >> black_move) {
        if (turn[0]!=i || turn[1]!='.')throw("Invalid Syntax!");

    }
}