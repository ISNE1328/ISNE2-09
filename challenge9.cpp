#include <iostream>
#include <array>
#include <limits>
#include <algorithm>
#include <cstdlib>

using namespace std;

using Board = array<char, 9>;

const char HUMAN = 'X';
const char AI    = 'O';
const char EMPTY = ' ';

const int WINS[8][3] = {
    {0,1,2},{3,4,5},{6,7,8},
    {0,3,6},{1,4,7},{2,5,8},
    {0,4,8},{2,4,6}
};

void printBoard(const Board& b) {
    cout << endl;
    for (int r = 0; r < 3; ++r) {
        cout << " ";
        for (int c = 0; c < 3; ++c) {
            int i = r * 3 + c;
            cout << (b[i] == EMPTY ? char('1' + i) : b[i]);
            if (c < 2) cout << " | ";
        }
        cout << endl;
        if (r < 2) cout << "---+---+---\n";
    }
    cout << endl;
}

char winner(const Board& b) {
    for (const auto& w : WINS)
        if (b[w[0]] != EMPTY && b[w[0]] == b[w[1]] && b[w[1]] == b[w[2]])
            return b[w[0]];
    return EMPTY;
}

bool isFull(const Board& b) {
    return none_of(b.begin(), b.end(), [](char c) { return c == EMPTY; });
}

int minimax(Board& b, int depth, bool aiTurn, int alpha, int beta) {
    char w = winner(b);
    if (w == AI)    return 10 - depth;
    if (w == HUMAN) return depth - 10;
    if (isFull(b))  return 0;

    if (aiTurn) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 9; ++i) {
            if (b[i] != EMPTY) continue;
            b[i] = AI;
            best = max(best, minimax(b, depth + 1, false, alpha, beta));
            b[i] = EMPTY;
            alpha = max(alpha, best);
            if (beta <= alpha) break;
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 9; ++i) {
            if (b[i] != EMPTY) continue;
            b[i] = HUMAN;
            best = min(best, minimax(b, depth + 1, true, alpha, beta));
            b[i] = EMPTY;
            beta = min(beta, best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

int bestMove(Board& b) {
    int bestScore = numeric_limits<int>::min();
    int bestIdx = -1;
    for (int i = 0; i < 9; ++i) {
        if (b[i] != EMPTY) continue;
        b[i] = AI;
        int score = minimax(b, 1, false,
                            numeric_limits<int>::min(),
                            numeric_limits<int>::max());
        b[i] = EMPTY;
        if (score > bestScore) {
            bestScore = score;
            bestIdx = i;
        }
    }
    return bestIdx;
}

int readHumanMove(const Board& b) {
    while (true) {
        cout << "Your move (1-9): ";
        int n;
        if (!(cin >> n)) {
            if (cin.eof()) exit(0);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number from 1 to 9." << endl;
            continue;
        }
        if (n < 1 || n > 9)    { cout << "Out of range.\n"; continue; }
        if (b[n - 1] != EMPTY) { cout << "That square is taken.\n"; continue; }
        return n - 1;
    }
}

int main() {
    cout << "Tic-Tac-Toe: You (X) vs Unbeatable AI (O)" << endl;

    char again = 'y';
    while (again == 'y' || again == 'Y') {
        Board board;
        board.fill(EMPTY);

        char first;
        cout << "Do you want to go first? (y/n): ";
        cin >> first;
        bool humanTurn = (first == 'y' || first == 'Y');

        printBoard(board);

        while (winner(board) == EMPTY && !isFull(board)) {
            if (humanTurn) {
                board[readHumanMove(board)] = HUMAN;
            } else {
                int m = bestMove(board);
                board[m] = AI;
                cout << "AI plays square " << (m + 1) << "." << endl;
            }
            printBoard(board);
            humanTurn = !humanTurn;
        }

        char w = winner(board);
        if (w == AI)         cout << "The AI wins. Better luck next time!" << endl;
        else if (w == HUMAN) cout << "You won?! (This should be impossible.)" << endl;
        else                 cout << "It's a draw." << endl;

        cout << "Play again? (y/n): ";
        cin >> again;
    }
    return 0;
}