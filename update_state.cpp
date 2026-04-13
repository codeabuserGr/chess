#include "update_state.h"

void update_board_state(bool c) {
    Piece** Pieces = c ? WhitePieces : BlackPieces;
    Piece** enemy_Pieces = !c ? WhitePieces : BlackPieces;
    for (int i=0;i<16;i++) {
        if (Pieces[i]) {
            Pieces[i]->update_controlled_squares();
            Pieces[i]->update_possible_moves();
        }
        if (enemy_Pieces[i]) {
            enemy_Pieces[i]->update_controlled_squares();
            enemy_Pieces[i]->update_possible_moves();
        }
        detect_checks(*dynamic_cast<King*>(Pieces[12]));
        detect_pins(*dynamic_cast<King*>(Pieces[12]));
        for (int j=0;j<16;j++) {
            if (Pieces[j]) {
                Pieces[j]->update_legal_moves();
            }
        }
    }
}
