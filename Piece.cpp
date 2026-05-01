#include "Piece.h"
#include <stdexcept>
#include <iostream>
using namespace std;
int convert(char x, int y) {
    return x-'a'+8*(y-1);
}

bool is_between(int king, int attacker, int sq) {
    if (sq == king || sq == attacker) return false;

    int kf = king % 8, kr = king / 8;
    int af = attacker % 8, ar = attacker / 8;
    int sf = sq % 8, sr = sq / 8;

    int df = af - kf;
    int dr = ar - kr;

    // king και attacker πρέπει να είναι ίδια γραμμή, στήλη ή διαγώνιος
    if (!(df == 0 || dr == 0 || abs(df) == abs(dr))) {
        return false;
    }

    int stepF = (df > 0) - (df < 0);
    int stepR = (dr > 0) - (dr < 0);

    int f = kf + stepF;
    int r = kr + stepR;

    // προχωράμε ακριβώς πάνω στη γραμμή king -> attacker
    while (f != af || r != ar) {
        if (f == sf && r == sr) {
            return true;
        }

        f += stepF;
        r += stepR;
    }

    return false;
}

Piece::Piece(char m, bool c, char x, int y):color(c), type(m), file(x), rank(y), controlled_squares{}, legal_moves{} {}
char Piece::get_x(){return file;}
int Piece::get_y(){return rank;}
bool Piece::get_color() {return color;}
Piece::~Piece(){}
void Piece::move(char x, int y) {
    int old_pos = convert(file, rank);
    int new_pos = convert(x, y);

    // αφαιρείται από παλιά θέση
    Board[old_pos] = nullptr;

    // τοποθετείται στη νέα
    Board[new_pos] = this;

    // update coords
    file = x;
    rank = y;
}

bool Piece::operator==(const Piece &p)const {
    return (p.color==color && p.file==file && p.rank==rank && p.type==type);
}
void Piece::update_controlled_squares() {}

bool* Piece::get_controlled_squares() {
    return controlled_squares;
}

void Piece::update_legal_moves() {}

bool* Piece::get_legal_moves() {
    return legal_moves;
}

char Piece::get_type() {
    return type;
}
void Piece::update_possible_moves() {}

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
        if (!controlled_squares[i]||(Board[i] && Board[i]->get_color()==color)){legal_moves[i]=false; continue;}
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

void King::update_possible_moves(){}
King::~King()=default;

Queen::Queen(bool c, char x, int y):Piece('Q',c, x, y){}
char Queen::get_type() {return type;}
bool* Queen::get_controlled_squares() {return controlled_squares;}

void Queen::update_controlled_squares(){
    Piece** Pieces = color ? BlackPieces : WhitePieces;
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file-i<'a')break;
        int a=convert(file-i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank+i>8)break;
        int a=convert(file, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file+i>'h')break;
        int a=convert(file+i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (file-i<'a')break;
        int a=convert(file-i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (file+i>'h')break;
        int a=convert(file+i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file-i<'a')break;
        int a=convert(file-i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1)break;
        int a=convert(file, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file+i>'h')break;
        int a=convert(file+i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
}
bool* Queen::get_legal_moves() {return legal_moves;}

void Queen::update_legal_moves() {
    for (int i=0; i<64; i++) {legal_moves[i]=false;}
    Piece** Pieces = color ? WhitePieces : BlackPieces;
    if (Pieces[12]->checked && pinned)return;
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i]|| (Board[i] && Board[i]->get_color()==color)){legal_moves[i]=false; continue;}
        if (Pieces[12]->checked) {
            if (Pieces[12]->checking_piece_placement[1]!=-1)return;
            int attacker = Pieces[12]->checking_piece_placement[0];
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (pinned) {
            int attacker = pinning_piece_placement;
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (!controlled_squares[i] || (Board[i] && Board[i]->get_color()==color))legal_moves[i]=false;
        else legal_moves[i]=true;
    }
}

void Queen::update_possible_moves() {}

Queen::~Queen() {}

Rook::Rook(bool c, char x, int y):Piece('R',c, x, y){}
char Rook::get_type() {return type;}
bool* Rook::get_controlled_squares() {return controlled_squares;}

void Rook::update_controlled_squares() {
    Piece** Pieces = color ? BlackPieces : WhitePieces;
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    for (int i=1; i<=7; i++) {
        if (rank+i>8)break;
        int a=convert(file, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (file-i<'a')break;
        int a=convert(file-i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (file+i>'h')break;
        int a=convert(file+i, rank);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1)break;
        int a=convert(file, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
}
bool* Rook::get_legal_moves() {return legal_moves;}
void Rook::update_legal_moves() {
    for (int i=0; i<64; i++) {legal_moves[i]=false;}
    Piece** Pieces = color ? WhitePieces : BlackPieces;
    if (Pieces[12]->checked && pinned)return;
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i]|| (Board[i] && Board[i]->get_color()==color)){legal_moves[i]=false; continue;}
        if (Pieces[12]->checked) {
            if (Pieces[12]->checking_piece_placement[1]!=-1)return;
            int attacker = Pieces[12]->checking_piece_placement[0];
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (pinned) {
            int attacker = pinning_piece_placement;
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (!controlled_squares[i] || (Board[i] && Board[i]->get_color()==color))legal_moves[i]=false;
        else legal_moves[i]=true;
    }
}

void Rook::update_possible_moves() {}

Rook::~Rook() {}


Bishop::Bishop(bool c, char x, int y):Piece('B',c, x, y){}
char Bishop::get_type() {return type;}
bool* Bishop::get_controlled_squares(){return controlled_squares;}
void Bishop::update_controlled_squares() {
    Piece** Pieces = color ? BlackPieces : WhitePieces;
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file-i<'a')break;
        int a=convert(file-i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank+i>8 || file+i>'h')break;
        int a=convert(file+i, rank+i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file-i<'a')break;
        int a=convert(file-i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
    for (int i=1; i<=7; i++) {
        if (rank-i<1 || file+i>'h')break;
        int a=convert(file+i, rank-i);
        controlled_squares[a]=true;
        if (Board[a]!=nullptr && Board[a]!=Pieces[12])break;
    }
}
bool* Bishop::get_legal_moves() {return legal_moves;}
void Bishop::update_legal_moves() {
    for (int i=0; i<64; i++) {legal_moves[i]=false;}
    Piece** Pieces = color ? WhitePieces : BlackPieces;
    if (Pieces[12]->checked && pinned)return;
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i]|| (Board[i] && Board[i]->get_color()==color)){legal_moves[i]=false; continue;}
        if (Pieces[12]->checked) {
            if (Pieces[12]->checking_piece_placement[1]!=-1)return;
            int attacker = Pieces[12]->checking_piece_placement[0];
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (pinned) {
            int attacker = pinning_piece_placement;
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (!controlled_squares[i] || (Board[i] && Board[i]->get_color()==color))legal_moves[i]=false;
        else legal_moves[i]=true;
    }
}
void Bishop::update_possible_moves() {}
Bishop::~Bishop() {}

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
void Knight::update_legal_moves() {
    for (int i=0; i<64; i++) {legal_moves[i]=false;}
    Piece** Pieces = color ? WhitePieces : BlackPieces;
    if (pinned){ return;}
    for (int i=0; i<64; i++) {
        if (!controlled_squares[i] || (Board[i] && Board[i]->get_color()==color)){legal_moves[i]=false; continue;}
        if (Pieces[12]->checked) {
            if (Pieces[12]->checking_piece_placement[1]!=-1)return;
            int attacker = Pieces[12]->checking_piece_placement[0];
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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

        if (!controlled_squares[i] || (Board[i] && Board[i]->get_color()==color))legal_moves[i]=false;
        else legal_moves[i]=true;
    }
}

void Knight::update_possible_moves() {}

Knight::~Knight(){}

Pawn::Pawn(bool c, char x, int y):Piece('\0',c, x, y){}
char Pawn::get_type() {return type;}
bool* Pawn::get_controlled_squares() {return controlled_squares;}
void Pawn::update_controlled_squares() {
    for (int i=0; i<64; i++) controlled_squares[i]=false;
    if (color){
        if (file-1>='a') {
            int a=convert(file-1, rank+1);
            controlled_squares[a]=true;
        }
        if (file+1<='h') {
            int a=convert(file+1, rank+1);
            controlled_squares[a]=true;
        }
    }
    else {
        if (file-1>='a') {
            int a=convert(file-1, rank-1);
            controlled_squares[a]=true;
        }
        if (file+1<='h') {
            int a=convert(file+1, rank-1);
            controlled_squares[a]=true;
        }
    }
}
void Pawn::update_possible_moves() {
    for (int i=0; i<64; i++) possible_moves[i]=false;
    int b=convert(file, rank);
    if (color) {
        if (b+16<64 && !Board[b+16] && !Board[b+8] && get_y()==2)possible_moves[b+16]=true;
        if (!Board[b+8])possible_moves[b+8]=true;
        if (file-1>='a') {
            int a=convert(file-1, rank+1);
            if (Board[a] || (!Board[a] && file!='a' && Board[b-1] && Board[b-1]->get_type()=='\0' && Board[b-1]->just_moved2))possible_moves[a]=true;
        }
        if (file+1<='h'){
            int a=convert(file+1, rank+1);
            if (Board[a] || (!Board[a] && file!='h' && Board[b+1] && Board[b+1]->get_type()=='\0' && Board[b+1]->just_moved2))possible_moves[a]=true;
        }
    }
    else {
        if (b>=16 && !Board[b-16] && !Board[b-8] && get_y()==7)possible_moves[b-16]=true;
        if (!Board[b-8])possible_moves[b-8]=true;
        if (file-1>='a') {
            int a=convert(file-1, rank-1);
            if (Board[a] || (!Board[a] && file !='a' && Board[b-1] && Board[b-1]->get_type()=='\0' && Board[b-1]->just_moved2))possible_moves[a]=true;
        }
        if (file+1<='h') {
            int a=convert(file+1, rank-1);
            if (Board[a] || (!Board[a] && file!='h' && Board[b+1] && Board[b+1]->get_type()=='\0' && Board[b+1]->just_moved2))possible_moves[a]=true;
        }
    }
}
bool* Pawn::get_legal_moves() {return legal_moves;}
void Pawn::update_legal_moves() {
    for (int i=0; i<64; i++) {
        legal_moves[i]=false;
    }
    Piece** Pieces = color ? WhitePieces : BlackPieces;
    if (Pieces[12]->checked && pinned)return;

    for (int i=0; i<64; i++) {
        if (!possible_moves[i]) continue;
        if (Pieces[12]->checked) {
            if (Pieces[12]->checking_piece_placement[1]!=-1)return;
            int attacker = Pieces[12]->checking_piece_placement[0];
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (pinned) {
            int attacker = pinning_piece_placement;
            int king_sq = convert(Pieces[12]->get_x(), Pieces[12]->get_y());

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
        if (!possible_moves[i] || (Board[i] && Board[i]->get_color()==color))legal_moves[i]=false;
        else legal_moves[i]=true;
    }
}



Pawn::~Pawn(){}

void detect_pins(King& K) {
    bool kingColor = K.get_color();

    for (int i = 0; i < 16; i++) {
        if (kingColor) {
            if (WhitePieces[i] != nullptr) {
                WhitePieces[i]->pinned = false;
                WhitePieces[i]->pinning_piece_placement = -1;
            }
        }
        else {
            if (BlackPieces[i] != nullptr) {
                BlackPieces[i]->pinned = false;
                BlackPieces[i]->pinning_piece_placement = -1;
            }
        }
    }

    int kingSq = convert(K.get_x(), K.get_y());

    for (int attackerSq = 0; attackerSq < 64; attackerSq++) {
        if (Board[attackerSq] == nullptr) continue;

        Piece* attacker = Board[attackerSq];


        if (attacker->get_color() == kingColor) continue;

        char type = std::toupper(attacker->get_type());


        if (type != 'B' && type != 'R' && type != 'Q') continue;

        int kf = kingSq % 8;
        int kr = kingSq / 8;
        int af = attackerSq % 8;
        int ar = attackerSq / 8;

        bool sameFile = (kf == af);
        bool sameRank = (kr == ar);
        bool sameDiagonal = (abs(kf - af) == abs(kr - ar));


        if (type == 'B' && !sameDiagonal) continue;
        if (type == 'R' && !(sameFile || sameRank)) continue;
        if (type == 'Q' && !(sameFile || sameRank || sameDiagonal)) continue;

        Piece* pinnedCandidate = nullptr;
        int pinnedSq = -1;
        int piecesBetween = 0;


        for (int sq = 0; sq < 64; sq++) {
            if (!is_between(kingSq, attackerSq, sq)) continue;
            if (Board[sq] == nullptr) continue;

            piecesBetween++;


            if (Board[sq]->get_color() == kingColor) {
                pinnedCandidate = Board[sq];
                pinnedSq = sq;
            }
        }


        if (piecesBetween == 1 && pinnedCandidate != nullptr) {
            pinnedCandidate->pinned = true;
            pinnedCandidate->pinning_piece_placement = attackerSq;
        }
    }
}
void detect_checks(King& K) {
    K.checked = false;
    K.checking_piece_placement[0]=-1;
    K.checking_piece_placement[1]=-1;
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
void delete_piece(char x, int y) {
    int a = convert(x, y);

    if (!Board[a]) return;

    Piece* victim = Board[a];
    bool c = victim->get_color();
    Piece** Pieces = c ? WhitePieces : BlackPieces;

    for (int i=0; i<16; i++) {
        if (Pieces[i] == victim) {
            Pieces[i] = nullptr;   //  πρώτα null
            delete victim;         // μετά delete
            break;
        }
    }

    Board[a] = nullptr;
}

void move(int turn, char m, char x, int y, bool c, char ambiguous_file, int ambiguous_rank, char n) {
        int target = convert(x,y);
        if (target < 0 || target >= 64)
            throw std::out_of_range("Invalid square");

        Piece** pieces = c ? WhitePieces : BlackPieces;
        Piece** enemy  = c ? BlackPieces : WhitePieces;

        bool moved = false;
        int moved_index = -1;

        // ================= NORMAL MOVE =================
        for (int i=0; i<16; i++) {
            if (!pieces[i]) continue;

            // τύπος κομματιού (important fix)
            if ( pieces[i]->get_type()!=m) continue;

            bool* lm = pieces[i]->get_legal_moves();
            if (!lm || !lm[target]) continue;

            // ambiguity
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

            char old_file = pieces[i]->get_x();
            int old_rank = pieces[i]->get_y();
            // ================= EN PASSANT =================
            if (pieces[i]->get_type()=='\0') { // pawn
                // διαγώνια κίνηση σε άδειο τετράγωνο
                if (Board[target] == nullptr && old_file != x) {
                    int dir = pieces[i]->get_color() ? -1 : 1;

                    int captured_rank = y + dir;
                    int captured_pos = convert(x, captured_rank);

                    if (captured_pos >= 0 && captured_pos < 64 &&
                        Board[captured_pos] &&
                        Board[captured_pos]->get_type()=='\0' &&
                        Board[captured_pos]->get_color() != c &&
                        Board[captured_pos]->just_moved2) {

                        // delete enemy pawn
                        delete_piece(x, captured_rank);
                        }
                }
            }

            // capture
            if (Board[target] != nullptr) {
                delete_piece(x, y);
            }

            pieces[i]->move(x,y);
            cout << turn << " " <<pieces[i]->get_type() << x << y << endl;
            pieces[i]->moved = true;

            // reset enemy pawn flags
            for (int k=0; k<16; k++) {
                if (enemy[k]!=nullptr && enemy[k]->get_type()=='\0') {
                    enemy[k]->just_moved2 = false;
                }
            }

            // pawn double move
            if (pieces[i]->get_type()=='\0') {
                if (abs(y - old_rank) == 2)
                    pieces[i]->just_moved2 = true;
                else
                    pieces[i]->just_moved2 = false;
            }

            moved = true;
            moved_index = i;
            break;
        }

        // ================= PROMOTION =================
        if (moved) {
            Piece* p = pieces[moved_index];

            if (p && p->get_type()=='\0') {
                if ((p->get_color() && y==8) ||
                    (!p->get_color() && y==1)) {

                    int pos = convert(x,y);
                    delete pieces[moved_index];
                    pieces[moved_index] = nullptr;

                    Piece* newPiece = nullptr;

                    switch(n) {
                        case 'Q': newPiece = new Queen(c, x, y); break;
                        case 'R': newPiece = new Rook(c, x, y); break;
                        case 'B': newPiece = new Bishop(c, x, y); break;
                        case 'N': newPiece = new Knight(c, x, y); break;
                        default: newPiece = new Queen(c, x, y);
                    }

                    pieces[moved_index] = newPiece;
                    Board[pos] = newPiece;
                    }
            }

            return;
        }

        // ================= CASTLING =================
        King* king = nullptr;

        for (int i=0; i<16; i++) {
            if (pieces[i] && pieces[i]->get_type()=='K') {
                king = static_cast<King*>(pieces[i]);
                break;
            }
        }

        int ky = king->get_y();

        // ---------- KING SIDE ----------
        if (m=='C' && x=='g') {
            int rook_pos = convert('h', ky);

            if (Board[rook_pos] && Board[rook_pos]->get_type()=='R') {

                Rook* rook = static_cast<Rook*>(Board[rook_pos]);

                if (!rook->moved && !king->moved && !king->checked &&
                    Board[convert('f',ky)]==nullptr &&
                    Board[convert('g',ky)]==nullptr) {
                    bool ok = true;

                    int squares[] = {convert('f',ky), convert('g',ky)};

                    for (int s : squares) {
                        for (int j=0; j<16; j++) {
                            if (enemy[j]) {
                                bool* cs = enemy[j]->get_controlled_squares();
                                if (cs && cs[s]) {
                                    ok = false;
                                    break;
                                }
                            }
                        }
                        if (!ok) {break;}
                    }

                    if (ok) {
                        cout << "LO" << endl;
                        king->move('g',ky);
                        rook->move('f',ky);
                        return;
                    }
                    }

            }
        }

        // ---------- QUEEN SIDE ----------
        if (m=='C' && x=='c') {

            int rook_pos = convert('a', ky);

            if (Board[rook_pos] && Board[rook_pos]->get_type()=='R') {

                Rook* rook = static_cast<Rook*>(Board[rook_pos]);

                if (!rook->moved && !king->moved && !king->checked &&
                    Board[convert('b',ky)]==nullptr &&
                    Board[convert('c',ky)]==nullptr &&
                    Board[convert('d',ky)]==nullptr) {

                    bool ok = true;

                    int squares[] = {convert('c',ky), convert('d',ky)};

                    for (int s : squares) {
                        for (int j=0; j<16; j++) {
                            if (enemy[j]!=nullptr) {
                                bool* cs = enemy[j]->get_controlled_squares();
                                if (cs && cs[s]) {
                                    ok = false;
                                    break;
                                }
                            }
                        }
                        if (!ok) break;
                    }

                    if (ok) {
                        cout << "LO LO" <<endl;
                        king->move('c',ky);
                        rook->move('d',ky);
                        return;
                    }
                    }
            }
        }

        // ================= INVALID =================
        cout << "On move " << turn << " ";
        throw std::runtime_error("invalid move!");
    }


