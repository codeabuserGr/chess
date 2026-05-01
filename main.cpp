#include <iostream>
#include "ChessBoard_setup.h"
#include <fstream>
#include <string>
#include "update_state.h"
using namespace std;

int main() {
    try {
        setup();
        update_board_state(false);
        update_board_state(true);
        ifstream file("game.txt");
        if (!file) {
            cout << "File not found!" << endl;
            return 1;
        }
        string turn, white_move, black_move;
        int i = 1;
        while (file >> turn >> white_move >> black_move) {
            string m = turn;
            m.pop_back();
            if (stoll(m) != i || turn.back() != '.') {
                throw std::runtime_error("Move " + m + ": Invalid Syntax!");
            }


            if (white_move[0] >= 'a' && white_move[0] <= 'h' && (
                    (white_move.back() < 'A' || white_move.back() > 'Z') || (
                            (white_move[white_move.size() - 2] < 'A' || white_move[white_move.size() - 2] > 'Z') &&
                            white_move.back() == '+'))) {
                //not promotion
                if (white_move[1] == 'x' && (white_move[2] >= 'a' && white_move[2] <= 'h') && (
                        white_move[3] - '0' >= 1 && white_move[3] - '0' <= 8))
                    move(
                            stoll(m), '\0', white_move[2], white_move[3] - '0', true, white_move[0], 0, '\0');
                else if ((white_move[1] - '0' >= 1 && white_move[1] - '0' <= 8)) {
                    move(stoll(m), '\0', white_move[0], white_move[1] - '0', true, '\0', 0, '\0');
                }
            } else if (white_move[0] >= 'a' && white_move[0] <= 'h') {
                //promotion
                if (white_move[1] == 'x' && (white_move[2] >= 'a' && white_move[2] <= 'h') && (
                        white_move[3] - '0' >= 1 && white_move[3] - '0' <= 8))
                    move(
                            stoll(m), '\0', white_move[2], white_move[3] - '0', true, white_move[0], 0, white_move[5]);
                else if ((white_move[1] - '0' >= 1 && white_move[1] - '0' <= 8)) {
                    move(stoll(m), '\0', white_move[0], white_move[1] - '0', true, '\0', 0, white_move[3]);
                }
            } else if (white_move[0] == 'N' || white_move[0] == 'B' || white_move[0] == 'Q' || white_move[0] == 'R' ||
                       white_move[0] == 'K') {
                if (white_move[1] >= 'a' && white_move[1] <= 'h' && (
                        (white_move[2] >= 'a' && white_move[2] <= 'h') || white_move[2] == 'x')) {
                    //piece ambiguous file
                    if (white_move[2] == 'x' && (white_move[3] >= 'a' && white_move[3] <= 'h') && (
                            white_move[4] - '0' >= 1 && white_move[4] - '0' <= 8))
                        move(
                                stoll(m), white_move[0], white_move[3], white_move[4] - '0', true, white_move[1], 0,
                                '\0');
                    else if (white_move[2] >= 'a' && white_move[2] <= 'h')
                        move(
                                stoll(m), white_move[0], white_move[2], white_move[3] - '0', true, white_move[1], 0,
                                '\0');
                } else if (white_move[1] - '0' >= 1 && white_move[1] - '0' <= 8) {
                    //piece ambiguous rank
                    if (white_move[2] == 'x' && (white_move[3] >= 'a' && white_move[3] <= 'h') && (
                            white_move[4] - '0' >= 1 && white_move[4] - '0' <= 8)) {
                        move(
                                stoll(m), white_move[0], white_move[3], white_move[4] - '0', true, '\0',
                                white_move[1] - '0',
                                '\0');
                    } else if (white_move[2] >= 'a' && white_move[2] <= 'h')
                        move(
                                stoll(m), white_move[0], white_move[2], white_move[3] - '0', true, '\0',
                                white_move[1] - '0',
                                '\0');
                } else if (white_move != "O-O" && white_move != "O-O-O") {
                    //piece regular
                    if (white_move[1] == 'x' && (white_move[2] >= 'a' && white_move[2] <= 'h') && (
                            white_move[3] - '0' >= 1 && white_move[3] - '0' <= 8)) {
                        move(stoll(m), white_move[0], white_move[2], white_move[3] - '0', true, '\0', 0, '\0');
                    } else if ((white_move[1] >= 'a' && white_move[1] <= 'h') && (
                            white_move[2] - '0' >= 1 && white_move[2] - '0' <= 8)) {
                        move(stoll(m), white_move[0], white_move[1], white_move[2] - '0', true, '\0', 0, '\0');
                    }
                }
            } else if (white_move == "O-O") { move(stoll(m), 'C', 'g', 1, true, '\0', 0, '\0'); }
            else if (
                    white_move == "O-O-O")
                move(stoll(m), 'C', 'c', 1, true, '\0', 0, '\0');
            else {
                throw std::runtime_error("Move " + m + ": Invalid Move!");
            }
            update_board_state(true);
            update_board_state(false);
            bool l = false;
            for (int j = 0; j < 16; j++) {
                if (l)break;
                for (int z = 0; z < 64; z++) {
                    if (!BlackPieces[j])break;
                    if (BlackPieces[j]->get_legal_moves()[z] ) {
                        l = true;
                        break;
                    }
                }
            }
            if (!l && BlackPieces[12]->checked) {
                cout << "Checkmate. 1-0";
                return 0;
            }
            if (!l) {
                cout << "Stalemate. 0.5-0.5";
                return 0;
            }

            if (black_move[0] >= 'a' && black_move[0] <= 'h' && (
                    (black_move.back() < 'A' || black_move.back() > 'Z') || (
                            (black_move[black_move.size() - 2] < 'A' || black_move[black_move.size() - 2] > 'Z') &&
                            black_move.back() == '+'))) {
                //not promotion
                if (black_move[1] == 'x' && (black_move[2] >= 'a' && black_move[2] <= 'h') && (
                        black_move[3] - '0' >= 1 && black_move[3] - '0' <= 8))
                    move(stoll(m), '\0', black_move[2], black_move[3] - '0', false, black_move[0], 0, '\0');
                else if ((black_move[1] - '0' >= 1 && black_move[1] - '0' <= 8)) {
                    move(stoll(m), '\0', black_move[0], black_move[1] - '0', false, '\0', 0, '\0');
                }
            } else if (black_move[0] >= 'a' && black_move[0] <= 'h') {
                //promotion
                if (black_move[1] == 'x' && (black_move[2] >= 'a' && black_move[2] <= 'h') && (
                        black_move[3] - '0' >= 1 && black_move[3] - '0' <= 8))
                    move(stoll(m), '\0', black_move[2], black_move[3] - '0', false, black_move[0], 0, black_move[5]);
                else if ((black_move[1] - '0' >= 1 && black_move[1] - '0' <= 8)) {
                    move(stoll(m), '\0', black_move[0], black_move[1] - '0', false, '\0', 0, black_move[3]);
                }
            } else if (black_move[0] == 'N' || black_move[0] == 'B' || black_move[0] == 'Q' || black_move[0] == 'R' ||
                       black_move[0] == 'K') {
                if (black_move[1] >= 'a' && black_move[1] <= 'h' && (
                        (black_move[2] >= 'a' && black_move[2] <= 'h') || black_move[2] == 'x')) {
                    //piece ambiguous file
                    if (black_move[2] == 'x' && (black_move[3] >= 'a' && black_move[3] <= 'h') && (
                            black_move[4] - '0' >= 1 && black_move[4] - '0' <= 8))
                        move(stoll(m), black_move[0], black_move[3], black_move[4] - '0', false, black_move[1], 0,
                             '\0');
                    else if (black_move[2] >= 'a' && black_move[2] <= 'h')
                        move(stoll(m), black_move[0], black_move[2], black_move[3] - '0', false, black_move[1], 0,
                             '\0');
                } else if (black_move[1] - '0' >= 1 && black_move[1] - '0' <= 8) {
                    //piece ambiguous rank
                    if (black_move[2] == 'x' && (black_move[3] >= 'a' && black_move[3] <= 'h') && (
                            black_move[4] - '0' >= 1 && black_move[4] - '0' <= 8))
                        move(stoll(m), black_move[0], black_move[3], black_move[4] - '0', false, '\0',
                             black_move[1] - '0',
                             '\0');
                    else if (black_move[2] >= 'a' && black_move[2] <= 'h')
                        move(stoll(m), black_move[0], black_move[2], black_move[3] - '0', false, '\0',
                             black_move[1] - '0',
                             '\0');
                } else if (black_move != "O-O" && black_move != "O-O-O") {
                    //piece regular
                    if (black_move[1] == 'x' && (black_move[2] >= 'a' && black_move[2] <= 'h') && (
                            black_move[3] - '0' >= 1 && black_move[3] - '0' <= 8)) {
                        move(stoll(m), black_move[0], black_move[2], black_move[3] - '0', false, '\0', 0, '\0');
                    } else if ((black_move[1] >= 'a' && black_move[1] <= 'h') && (
                            black_move[2] - '0' >= 1 && black_move[2] - '0' <= 8)) {
                        move(stoll(m), black_move[0], black_move[1], black_move[2] - '0', false, '\0', 0, '\0');
                    }
                }
            } else if (black_move == "O-O") {
                move(stoll(m), 'C', 'g', 8, false, '\0', 0, '\0');
            } else if (black_move == "O-O-O") {
                move(stoll(m), 'C', 'c', 8, false, '\0', 0, '\0');
            } else {
                throw std::runtime_error("Move " + m + ": Invalid Move!");
            }
            update_board_state(true);
            update_board_state(false);
            l = false;
            for (int j = 0; j < 16; j++) {
                if (l)break;
                for (int z = 0; z < 64; z++) {
                    if (!WhitePieces[j])break;
                    if (WhitePieces[j]->get_legal_moves()[z]) {
                        if (i==69) cout << WhitePieces[j]->get_type() <<z << " ";
                        if (i==69 && (WhitePieces[j]->get_type()=='N' || WhitePieces[j]->get_type()=='R')&& z==1)break;
                        l = true;
                        break;
                    }
                }
            }
            if (!l && WhitePieces[12]->checked) {
                cout << "Checkmate. 0-1";
                return 0;
            }
            if (!l) {
                cout << "Stalemate. 0.5-0.5";
                return 0;
            }
            if (i==69) {
                for (int j = 0; j < 16; j++) {
                    if (WhitePieces[j]) {
                        Piece *w = WhitePieces[j];
                        cout << w->get_type() << w->get_x() << w->get_y() << " ";
                    }
                }
                for (int j = 0; j < 16; j++) {
                    if (BlackPieces[j]) {
                        Piece *w = BlackPieces[j];
                        cout << w->get_type() << w->get_x() << w->get_y() << " ";
                    }
                }
            }
            i++;
        }
    } catch (const std::exception &e) {
        cout << e.what() << endl;
    }
}