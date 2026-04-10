#include "Piece.h"
#include <stdexcept>
#include <iostream>
using namespace std;
int convert(char x, int y) {
    return x-'a'+8*(y-1);
}

bool is_between(int king, int attacker, int sq) {

    int kf = king % 8, kr = king / 8;
    int af = attacker % 8, ar = attacker / 8;
    int sf = sq % 8, sr = sq / 8;

    // πρέπει να είναι στην ίδια γραμμή
    if (!( (kf==af) || (kr==ar) ||
           (abs(kf-af)==abs(kr-ar)) )) return false;

    // sq πρέπει να είναι στην ίδια γραμμή επίσης
    if (!( (kf==sf) || (kr==sr) ||
           (abs(kf-sf)==abs(kr-sr)) )) return false;

    // και ανάμεσα
    return ( (sf >= min(kf,af) && sf <= max(kf,af)) &&
             (sr >= min(kr,ar) && sr <= max(kr,ar)) );
}

Piece::Piece(char m, bool c, char x, int y):color(c), type(m), file(x), rank(y), controlled_squares{}, legal_moves{} {}
char Piece::get_x(){return file;}
int Piece::get_y(){return rank;}
bool Piece::get_color() {return color;}
Piece::~Piece(){}
void Piece::move(char x, int y) {
    file=x; rank=y;
}

bool Piece::operator==(const Piece &p)const {
    return (p.color==color && p.file==file && p.rank==rank && p.type==type);
}

King::King(bool c, char x, int y):Piece('K',c, x, y){}
char King::get_type() {return type;}
bool* King::get_controlled_squares() {return controlled_squares;}
void King::update_controlled_squares() {
    for (int i=0; i<64; i++) {
        controlled_squares[i]=false;
    }
    for (int i=-1; i<=1; i++) {
        if (1>rank+i || rank+i>8)continue;
        for (int j=-1; j<=1; j++) {
            if ('a'>file+j || file+j>'h')continue;
            if (i==0 && j==0)continue;
            controlled_squares[convert(file+j, rank+i)]=true;
        }
    }
}
bool* King::get_legal_moves() {return legal_moves;}
void King::update_legal_moves() {
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i]){legal_moves[i]=false; continue;}
        if (Board[i]!=nullptr && Board[i]->get_color()==color){legal_moves[i]=false; continue;}
        if (color==0) {
            bool f=false;
            for (int j=0; j<=15; j++){
                if (WhitePieces[j] && WhitePieces[j]->get_controlled_squares()[i])
                {f=true; break;}
            }
            if (f){legal_moves[i]=false; continue;}
        }
        else {
            bool f=false;
            for (int j=0; j<=15; j++){
                if (BlackPieces[j] && BlackPieces[j]->get_controlled_squares()[i])
                {f=true; break;}
            }
            if (f){legal_moves[i]=false; continue;}
        }
        legal_moves[i]=true;
    }

}


Queen::Queen(bool c, char x, int y):Piece('Q',c, x, y){}
char Queen::get_type() {return type;}
bool* Queen::get_controlled_squares() {return controlled_squares;}

void Queen::update_controlled_squares(){
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file-i<'a')break;
        int a=convert(file-i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank+i>8)break;
        int a=convert(file, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file+i>'h')break;
        int a=convert(file+i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (file-i<'a')break;
        int a=convert(file-i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (file+i>'h')break;
        int a=convert(file+i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file-i<'a')break;
        int a=convert(file-i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1)break;
        int a=convert(file, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file+i>'h')break;
        int a=convert(file+i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
}
bool* Queen::get_legal_moves() {return legal_moves;}

void Queen::update_legal_moves() {
    for (int i=0; i<64; i++)
        legal_moves[i]=false;
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i]) continue;
        if (BlackPieces[12]->checked) {

            int attacker = BlackPieces[12]->checking_piece_placement[0];
            int king_sq = convert(BlackPieces[12]->get_x(), BlackPieces[12]->get_y());

            // capture
            if (i == attacker) {
                legal_moves[i] = true;
                continue;
            }

            // block
            if (is_between(king_sq, attacker, i)) {
                legal_moves[i] = true;
                continue;
            }

            legal_moves[i] = false;
            continue;
        }

    }
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i] || Board[i]!=nullptr && Board[i]->get_color()==color)legal_moves[i]=false;
        else legal_moves[i]=true;
    }
}

Rook::Rook(bool c, char x, int y):Piece('R',c, x, y){}
char Rook::get_type() {return type;}
bool* Rook::get_controlled_squares() {return controlled_squares;}

void Rook::update_controlled_squares() {
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    for (int i=1; i<=7; i++) {
        if (rank+i>8)break;
        int a=convert(file, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (file-i<'a')break;
        int a=convert(file-i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (file+i>'h')break;
        int a=convert(file+i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1)break;
        int a=convert(file, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
}
bool* Rook::get_legal_moves() {return legal_moves;}
void Rook::update_legal_moves() {
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i] || Board[i]!=nullptr && Board[i]->get_color()==color)legal_moves[i]=false;
        else legal_moves[i]=true;
    }
}


Bishop::Bishop(bool c, char x, int y):Piece('B',c, x, y){}
char Bishop::get_type() {return type;}
bool* Bishop::get_controlled_squares(){return controlled_squares;}
void Bishop::update_controlled_squares() {
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file-i<'a')break;
        int a=convert(file-i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file+i>'h')break;
        int a=convert(file+i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file-i<'a')break;
        int a=convert(file-i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file+i>'h')break;
        int a=convert(file+i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr)break;
    }
}
bool* Bishop::get_legal_moves() {return legal_moves;}


Knight::Knight(bool c, char x, int y):Piece('N',c, x, y){}
char Knight::get_type() {return type;}
bool* Knight::get_controlled_squares() {return controlled_squares;}
void Knight::update_controlled_squares() {
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    if (rank+1<=8 && file-2>='a')controlled_squares[convert(file-2,rank+1)]=true;
    if (rank+2<=8 && file-1>='a')controlled_squares[convert(file-1,rank+2)]=true;
    if (rank+2<=8 && file+1<='h')controlled_squares[convert(file+1,rank+2)]=true;
    if (rank+1<=8 && file+2<='h')controlled_squares[convert(file+2,rank+1)]=true;
    if (rank-1>=1 && file-2>='a')controlled_squares[convert(file-2,rank-1)]=true;
    if (rank-2>=1 && file-1>='a')controlled_squares[convert(file-1,rank-2)]=true;
    if (rank-2>=1 && file+1<='h')controlled_squares[convert(file+1,rank-2)]=true;
    if (rank-1>=1 && file+2<='h')controlled_squares[convert(file+2,rank-1)]=true;
}
bool* Knight::get_legal_moves() {return legal_moves;}

Pawn::Pawn(bool c, char x, int y):Piece('\0',c, x, y){}
char Pawn::get_type() {return type;}
bool* Pawn::get_controlled_squares() {return controlled_squares;}
void Pawn::update_controlled_squares() {
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    if (color){
        controlled_squares[convert(file-1,rank+1)]=true;
        controlled_squares[convert(file+1,rank+1)]=true;
    }
    else {
        controlled_squares[convert(file-1,rank-1)]=true;
        controlled_squares[convert(file+1,rank-1)]=true;
    }
}
bool* Pawn::get_legal_moves() {return legal_moves;}



void detect_pins(King& K) {
    for (int i=0; i<16; i++) {
        if (WhitePieces[i]!=nullptr && K.get_color())WhitePieces[i]->pinned=false;
        if (BlackPieces[i]!=nullptr && !K.get_color())BlackPieces[i]->pinned=false;
    }
    bool b=false;
    int c;
    for (int i=1; i<=7; i++) {
        if (K.get_y()+i>8 || K.get_x()-i<'a') break;
        int a = convert(K.get_x()-i, K.get_y()+i);
        if (Board[a]!=nullptr) {
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='B' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }
    b=false;
    for (int i=1; i<=7; i++) {
        if (K.get_y()+i>8) break;
        int a = convert(K.get_x(), K.get_y()+i);
        if (Board[a]!=nullptr){
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='R' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }
    b=false;
    for (int i=1; i<=7; i++) {
        if (K.get_y()+i>8 || K.get_x()+i>'h') break;
        int a = convert(K.get_x()+i, K.get_y()+i);
        if (Board[a]!=nullptr) {
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='B' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }

    b=false;
    for (int i=1; i<=7; i++) {
        if (K.get_x()-i<'a') break;
        int a = convert(K.get_x()-i, K.get_y());
        if (Board[a]!=nullptr){
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='R' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }
    b=false;
    for (int i=1; i<=7; i++) {
        if (K.get_x()+i>'h') break;
        int a = convert(K.get_x()+i, K.get_y());
        if (Board[a]!=nullptr){
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='R' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }
    b=false;
    for (int i=1; i<=7; i++) {
        if (K.get_y()-i<1 || K.get_x()-i<'a') break;
        int a = convert(K.get_x()-i, K.get_y()-i);
        if (Board[a]!=nullptr){
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='B' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }
    b=false;
    for (int i=1; i<=7; i++) {
        if (K.get_y()-i<1) break;
        int a = convert(K.get_x(), K.get_y()-i);
        if (Board[a]!=nullptr){
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='R' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }
    b=false;
    for (int i=1; i<=7; i++) {
        if (K.get_y()-i<1 || K.get_x()+i>'h') break;
        int a = convert(K.get_x()+i, K.get_y()-i);
        if (Board[a]!=nullptr) {
            if ((Board[a]->get_color()!=K.get_color() && !b) ||( b && Board[a]->get_color()==K.get_color()))break;
            if (!b) {
                b=true; c=a;
            }
            else {
                if (Board[a]->get_type()=='B' || Board[a]->get_type()=='Q') {
                    Board[c]->pinned=true;
                    Board[c]->pinning_piece_placement=a;
                }
            }
        }
    }
}
void detect_checks(King& K) {
    K.checked = false;
    int j = 0;

    int king_pos = convert(K.get_x(), K.get_y());

    if (K.get_color()) { // white king → check black pieces
        for (int i=0; i<16; i++) {
            if (BlackPieces[i] &&
                BlackPieces[i]->get_controlled_squares()[king_pos]) {

                K.checked = true;
                K.checking_piece_placement[j++] =
                    convert(BlackPieces[i]->get_x(), BlackPieces[i]->get_y());
                }
        }
    }
    else { // black king → check white pieces
        for (int i=0; i<16; i++) {
            if (WhitePieces[i] &&
                WhitePieces[i]->get_controlled_squares()[king_pos]) {

                K.checked = true;
                K.checking_piece_placement[j++] =
                    convert(WhitePieces[i]->get_x(), WhitePieces[i]->get_y());
                }
        }
    }
}

void move(int turn, char m, char x, int y, bool c,
          char ambiguous_file, int ambiguous_rank) {

    int target = convert(x,y);
    bool moved = false;

    Piece** pieces = c ? WhitePieces : BlackPieces;
    Piece** enemy  = c ? BlackPieces : WhitePieces;

    // ================= NORMAL MOVE =================
    for (int i=0; i<16; i++) {
        if (!pieces[i]) continue;

        if (!pieces[i]->get_legal_moves()[target]) continue;

        // ambiguity handling
        if (ambiguous_file!='\0' && ambiguous_rank!=0) {
            if (pieces[i]->get_x()!=ambiguous_file ||
                pieces[i]->get_y()!=ambiguous_rank) continue;
        }
        else if (ambiguous_file!='\0') {
            if (pieces[i]->get_x()!=ambiguous_file) continue;
        }
        else if (ambiguous_rank!=0) {
            if (pieces[i]->get_y()!=ambiguous_rank) continue;
        }

        pieces[i]->move(x,y);
        moved = true;
        break;
    }

    // ================= SPECIAL MOVES =================
    if (moved) return;

    // ================= CASTLING =================
    King* king = nullptr;
    Rook* rook = nullptr;

    // βρίσκουμε king
    for (int i=0; i<16; i++) {
        if (pieces[i] && pieces[i]->get_type()=='K') {
            king = static_cast<King*>(pieces[i]);
            break;
        }
    }

    if (!king) return;

    int ky = king->get_y();

    // ---------- KING SIDE ----------
    if (m=='K' && x=='g') {

        int rook_pos = convert('h', ky);
        if (Board[rook_pos] && Board[rook_pos]->get_type()=='R') {

            rook = static_cast<Rook*>(Board[rook_pos]);

            if (!rook->moved && !king->moved && !king->checked &&
                Board[convert('f',ky)]==nullptr &&
                Board[convert('g',ky)]==nullptr) {

                bool ok = true;

                int squares[] = {convert('f',ky), convert('g',ky)};

                for (int s : squares) {
                    for (int j=0; j<16; j++) {
                        if (enemy[j] &&
                            enemy[j]->get_controlled_squares()[s]) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) break;
                }

                if (ok) {
                    king->move('g',ky);
                    rook->move('f',ky);
                    return;
                }
            }
        }
    }

    // ---------- QUEEN SIDE ----------
    if (m=='K' && x=='c') {

        int rook_pos = convert('a', ky);
        if (Board[rook_pos] && Board[rook_pos]->get_type()=='R') {

            rook = static_cast<Rook*>(Board[rook_pos]);

            if (!rook->moved && !king->moved && !king->checked &&
                Board[convert('b',ky)]==nullptr &&
                Board[convert('c',ky)]==nullptr &&
                Board[convert('d',ky)]==nullptr) {

                bool ok = true;

                int squares[] = {convert('c',ky), convert('d',ky)};

                for (int s : squares) {
                    for (int j=0; j<16; j++) {
                        if (enemy[j] &&
                            enemy[j]->get_controlled_squares()[s]) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) break;
                }

                if (ok) {
                    king->move('c',ky);
                    rook->move('d',ky);
                    return;
                }
            }
        }
    }

    // ================= EN PASSANT =================
    if (m=='\0') {

        int pawn_rank = c ? 5 : 4;     // θέση pawn
        int target_rank = c ? 6 : 3;   // destination

        if (y == target_rank) {

            for (int i=0; i<8; i++) {
                if (!enemy[i]) continue;

                if (enemy[i]->get_type()!='\0') continue;

                if (!enemy[i]->just_moved2) continue;

                if (enemy[i]->get_x() != x) continue;

                // check adjacent pawn
                int left  = x-1;
                int right = x+1;

                if (left >= 'a') {
                    int pos = convert(left, pawn_rank);
                    if (Board[pos] && Board[pos]->get_type()=='\0') {
                        Board[pos]->move(x,y);

                        // remove captured pawn
                        Board[convert(x,pawn_rank)] = nullptr;
                        return;
                    }
                }

                if (right <= 'h') {
                    int pos = convert(right, pawn_rank);
                    if (Board[pos] && Board[pos]->get_type()=='\0') {
                        Board[pos]->move(x,y);

                        Board[convert(x,pawn_rank)] = nullptr;
                        return;
                    }
                }
            }
        }
    }
}

