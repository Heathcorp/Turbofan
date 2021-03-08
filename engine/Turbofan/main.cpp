#include <iostream>

int main() {

    return 0;
}

struct BoardState {
    unsigned int rank8 : 32;
    unsigned int rank7 : 32;
    unsigned int rank6 : 32;
    unsigned int rank5 : 32;
    unsigned int rank4 : 32;
    unsigned int rank3 : 32;
    unsigned int rank2 : 32;
    unsigned int rank1 : 32;

}

int FENToBit