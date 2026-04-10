#include "Chessboard_setup.h"
void setup() {
    for (int i=0; i<=1; i++) {
        char f='a';
        for (int j=0; j<=7; j++) {
            if (i==0) {
                BlackPieces[j]=new Pawn(false, f, 7);
                Board[48+j]=BlackPieces[j]; f++;
            }
            else {
                WhitePieces[j]=new Pawn(true, f, 2);
                Board[8+j]=WhitePieces[j]; f++;
            }
        }
    }
    for (int i=0; i<=1; i++) {
        if (i==0){
            Board[56]=new Rook(false, 'a', 8);
            Board[57]=new Knight(false, 'b', 8);
            Board[58]=new Bishop(false, 'c', 8);
            Board[59]=new Queen(false, 'd', 8);
            Board[60]=new King(false, 'e', 8);
            Board[61]=new Bishop(false, 'f', 8);
            Board[62]=new Knight(false, 'g', 8);
            Board[63]=new Rook(false, 'h', 8);
            BlackPieces[8]=Board[56];
            BlackPieces[9]=Board[57];
            BlackPieces[10]=Board[58];
            BlackPieces[11]=Board[59];
            BlackPieces[12]=Board[60];
            BlackPieces[13]=Board[61];
            BlackPieces[14]=Board[62];
            BlackPieces[15]=Board[63];
        }
        else {
            Board[0]=new Rook(true, 'a', 1);
            Board[1]=new Knight(true, 'b', 1);
            Board[2]=new Bishop(true, 'c', 1);
            Board[3]=new Queen(true, 'd', 1);
            Board[4]=new King(true, 'e', 1);
            Board[5]=new Bishop(true, 'f', 1);
            Board[6]=new Knight(true, 'g', 1);
            Board[7]=new Rook(true, 'h', 1);
            WhitePieces[8]=Board[0];
            WhitePieces[9]=Board[1];
            WhitePieces[10]=Board[2];
            WhitePieces[11]=Board[3];
            WhitePieces[12]=Board[4];
            WhitePieces[13]=Board[5];
            WhitePieces[14]=Board[6];
            WhitePieces[15]=Board[7];
        }
    }
}