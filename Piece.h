
#ifndef UNTITLED_PIECE_H
#define UNTITLED_PIECE_H
#include <vector>
using namespace std;
int convert(char x, int y);
class Piece {
public:
    Piece(char m, bool c, char x, int y);
    bool get_color();
    virtual char get_type();
    void move(char x, int y);
    char get_x();
    int get_y();
    virtual void update_controlled_squares();
    virtual bool* get_controlled_squares();
    virtual ~Piece();
    bool operator==(const Piece& p)const;
    virtual void update_legal_moves();
    virtual bool* get_legal_moves();
    virtual void update_possible_moves();
    bool pinned=false;
    int pinning_piece_placement=-1;
    bool moved=false;
    bool just_moved2=false;
    bool checked=false;
    int checking_piece_placement[2]={-1, -1};

protected:
    char file;
    int rank;
    char type;
    bool color;
    bool controlled_squares[64];
    bool legal_moves[64];
    bool possible_moves[64];
};

class King : public Piece {
public:
    King(bool c, char x, int y);
    char get_type()override;
    ~King() override;
    void update_controlled_squares()override;
    bool* get_controlled_squares()override;
    void update_legal_moves()override;
    bool* get_legal_moves()override;
    void update_possible_moves()override;
};

class Queen : public Piece {
public:
    Queen(bool c, char x, int y);
    char get_type()override;
    ~Queen() override;
    void update_controlled_squares()override;
    bool* get_controlled_squares()override;
    void update_legal_moves()override;
    bool* get_legal_moves()override;
    void update_possible_moves()override;
};

class Rook : public Piece {
public:
    Rook(bool c, char x, int y);
    char get_type()override;
    ~Rook() override;
    void update_controlled_squares()override;
    bool* get_controlled_squares()override;
    void update_legal_moves()override;
    bool* get_legal_moves()override;
    void update_possible_moves()override;
};

class Bishop : public Piece {
public:
    Bishop(bool c, char x, int y);
    char get_type()override;
    ~Bishop() override;
    void update_controlled_squares()override;
    bool* get_controlled_squares()override;
    void update_legal_moves()override;
    bool* get_legal_moves()override;
    void update_possible_moves()override;
};

class Knight : public Piece {
public:
    Knight(bool c, char x, int y);
    char get_type()override;
    ~Knight() override;
    void update_controlled_squares()override;
    bool* get_controlled_squares()override;
    void update_legal_moves()override;
    bool* get_legal_moves()override;
    void update_possible_moves()override;
};

class Pawn: public Piece {
public:
    Pawn(bool c, char x, int y);
    char get_type()override;
    ~Pawn() override;
    void update_controlled_squares()override;
    bool* get_controlled_squares()override;
    void update_legal_moves()override;
    bool* get_legal_moves()override;
    void update_possible_moves()override;

};
void delete_piece(char x, int y);
void move(int turn, char m, char x, int y, bool c,
          char ambiguous_file, int ambiguous_rank, char n);
inline Piece* WhitePieces[16];
inline Piece* BlackPieces[16];
inline Piece* Board[64];
void detect_pins(King& K);
void detect_checks(King& K);
#endif //UNTITLED_PIECE_H
