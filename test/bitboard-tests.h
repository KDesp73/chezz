#ifndef BITBOARD_TESTS_H
#define BITBOARD_TESTS_H

#include "board.h"
#include <stddef.h>

int TestCanPawnMove(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_PAWN_MOVE \
    TestCanPawnMove(STARTING_FEN, "c2", "d3", 0), \
    TestCanPawnMove(STARTING_FEN, "a2", "a3", 1), \
    TestCanPawnMove(STARTING_FEN, "a2", "a4", 1), \
    TestCanPawnMove(STARTING_FEN, "e2", "e4", 1), \
    TestCanPawnMove(STARTING_FEN, "a2", "a5", 0), \
    TestCanPawnMove(STARTING_FEN, "h7", "h6", 1), \
    TestCanPawnMove(STARTING_FEN, "h7", "h5", 1), \
    TestCanPawnMove("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "f3", 1), \
    TestCanPawnMove("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "g2", "f3", 1), \
    TestCanPawnMove("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f3", 0), \
    TestCanPawnMove("rnbqkbnr/ppppp1pp/8/8/5p2/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f3", 1), \
    TestCanPawnMove("rnbqkbnr/ppppp1pp/8/8/5p2/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f4", 0), \
    TestCanPawnMove("rnbqkbnr/ppppp1pp/8/8/8/5p2/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "f2", "f4", 0), \
    TestCanPawnMove("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "f7", "e6", 1), \
    TestCanPawnMove("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "d7", "e6", 1), \
    TestCanPawnMove("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "e7", "e6", 0), \
    TestCanPawnMove("rnbqkbnr/ppp1pppp/8/8/3p4/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "d4", "e2", 0), \
    TestCanPawnMove("rnbqkbnr/pppppppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "e7", "e5", 0), \
    TestCanPawnMove("rnbqkbnr/ppp1pppp/8/2PpP3/8/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    TestCanPawnMove("rnbqkbnr/ppp1pppp/8/2PpP3/8/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 1", "c5", "d6", 1)

int TestCanRookMove(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_ROOK_MOVE \
    TestCanRookMove(STARTING_FEN, "a1", "a2", 0), \
    TestCanRookMove(STARTING_FEN, "a1", "a3", 0), \
    TestCanRookMove(STARTING_FEN, "a1", "a3", 0), \
    TestCanRookMove("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a2", 1), \
    TestCanRookMove("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a3", 1), \
    TestCanRookMove("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a4", 1), \
    TestCanRookMove("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a5", 1), \
    TestCanRookMove("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a6", 1), \
    TestCanRookMove("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a7", 1), \
    TestCanRookMove("rnbqkbnr/pppppppp/8/8/5P2/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a8", 0), \
    TestCanRookMove("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "d5", "d6", 1), \
    TestCanRookMove("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "e5", "d6", 0), \
    TestCanRookMove("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "d5", "e5", 1), \
    TestCanRookMove("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "e5", "d5", 1), \
    TestCanRookMove("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "d5", "f5", 0), \
    TestCanRookMove("8/8/8/3Rr3/8/8/8/8 w - - 0 1", "e5", "c5", 0), \
    TestCanRookMove("R6r/8/8/8/8/8/8/8 w - - 0 1", "a8", "h8", 1), \
    TestCanRookMove("R6r/8/8/8/8/8/8/8 w - - 0 1", "a8", "a1", 1), \
    TestCanRookMove("R6r/8/8/8/8/8/8/8 w - - 0 1", "a8", "h1", 0)

int TestCanBishopMove(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_BISHOP_MOVE \
    TestCanBishopMove(STARTING_FEN, "c1", "d2", 0), \
    TestCanBishopMove(STARTING_FEN, "c1", "b2", 0), \
    TestCanBishopMove(STARTING_FEN, "c1", "c3", 0), \
    TestCanBishopMove("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "d2", 1), \
    TestCanBishopMove("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "e3", 1), \
    TestCanBishopMove("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "f4", 1), \
    TestCanBishopMove("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "g5", 1), \
    TestCanBishopMove("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "h6", 1), \
    TestCanBishopMove("rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "a3", 0), \
    TestCanBishopMove("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "d2", 1), \
    TestCanBishopMove("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "e3", 1), \
    TestCanBishopMove("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "f4", 1), \
    TestCanBishopMove("rnbqkbnr/ppppp1pp/8/8/5p2/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1", "c1", "g5", 0), \
    TestCanBishopMove("8/8/8/8/8/3BB3/8/8 w - - 0 1", "d3", "h7", 1), \
    TestCanBishopMove("8/8/8/8/8/3BB3/8/8 w - - 0 1", "d3", "e3", 0), \
    TestCanBishopMove("8/8/8/8/8/3BB3/8/8 w - - 0 1", "d3", "g5", 0)

int TestCanQueenMove(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_QUEEN_MOVE \
    TestCanQueenMove(STARTING_FEN, "d1", "c2", 0), \
    TestCanQueenMove(STARTING_FEN, "d1", "d2", 0), \
    TestCanQueenMove(STARTING_FEN, "d1", "e2", 0), \
    TestCanQueenMove(STARTING_FEN, "d1", "d3", 0), \
    TestCanQueenMove(STARTING_FEN, "d1", "c3", 0), \
    TestCanQueenMove(STARTING_FEN, "d1", "e3", 0), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "h4", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "a4", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "c3", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "e3", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d3", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d6", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d7", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "f6", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "g7", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "a7", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "b6", 1), \
    TestCanQueenMove("rnbqkbnr/pppppppp/8/8/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "d1", 0), \
    TestCanQueenMove("rnbqkbnr/pppp1ppp/8/4p3/3Q4/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", "d4", "f6", 0)

int TestCanKnightMove(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_KNIGHT_MOVE \
    TestCanKnightMove(STARTING_FEN, "b1", "c3", 1), \
    TestCanKnightMove(STARTING_FEN, "b1", "a3", 1), \
    TestCanKnightMove(STARTING_FEN, "b1", "d2", 0), \
    TestCanKnightMove(STARTING_FEN, "g1", "f3", 1), \
    TestCanKnightMove(STARTING_FEN, "g1", "h3", 1), \
    TestCanKnightMove(STARTING_FEN, "g1", "e2", 0), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "d7", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "f7", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "c6", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "c4", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "d3", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "f3", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "g4", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "g6", 1), \
    TestCanKnightMove("rnbqkbnr/pppppppp/8/4N3/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", "e5", "a1", 0)

int TestCanKingMove(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_KING_MOVE \
    TestCanKingMove(STARTING_FEN, "e1", "e2", 0), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "d3", 1), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "c3", 1), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "e3", 1), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "e4", 1), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "c4", 1), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "d5", 0), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "c5", 0), \
    TestCanKingMove("8/8/3k4/8/3K4/8/8/8 w - - 0 1", "d4", "e5", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 1), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 1), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e8", "g8", 1), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e8", "c8", 1), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e1", "g1", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e1", "c1", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e8", "g8", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1", "e8", "c8", 0), \
    TestCanKingMove("3rkr2/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("3rkr2/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("2r1k1r1/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("2r1k1r1/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/1p5p/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/1p5p/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/2p3p1/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("r3k2r/8/8/8/8/8/2p3p1/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("r3k2r/2p3p1/8/8/8/2n3n1/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("r3k2r/2p3p1/8/8/8/2n3n1/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("r3k2r/2p3p1/8/8/8/1n5n/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("r3k2r/2p3p1/8/8/8/1n5n/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("rn2k1nr/2p3p1/8/8/8/4b3/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("rn2k1nr/2p3p1/8/8/8/4b3/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("rn2k1nr/2p3p1/8/8/8/8/4b3/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("rn2k1nr/2p3p1/8/8/8/8/4b3/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("rn2k1nr/2p3p1/8/8/8/8/4q3/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestCanKingMove("rn2k1nr/2p3p1/8/8/8/8/4q3/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("rnbq2nr/2p3p1/8/8/8/8/6k1/R3K2R w KQ - 0 1", "e1", "g1", 0), \
    TestCanKingMove("rnbq2nr/2p3p1/8/8/8/8/2k5/R3K2R w KQ - 0 1", "e1", "c1", 0), \
    TestCanKingMove("rnbq2nr/3p2p1/8/8/8/8/2k5/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestCanKingMove("r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0)

int TestIsPinned(const char* fen, const char* square, _Bool expected);
#define TEST_BITBOARD_IS_PINNED \
    TestIsPinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3P1P2/PPP2N1P/RNBQKB1R w KQkq - 0 1", "f2", 1), \
    TestIsPinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3PBP2/PPP2N1P/RN1QKB1R w KQkq - 0 1", "f2", 1), \
    TestIsPinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3PBP2/PPP2N1P/RN1QKB1R w KQkq - 0 1", "e3", 1), \
    TestIsPinned("rnb1k1nr/pp1ppppp/2p5/4q3/6Pb/3PBP2/PPP1BN1P/RN1QK2R w KQkq - 0 1", "e3", 0), \
    TestIsPinned("rnb1kbnr/pppp1ppp/8/4p3/5P1q/4P1P1/PPPP3P/RNBQKBNR w KQkq - 0 1", "g3", 1), \
    TestIsPinned("rnb1kbnr/pppp1ppp/8/4p3/5P1q/4P1P1/PPPP3P/RNBQKBNR w KQkq - 0 1", "f4", 0), \
    TestIsPinned("rnb1kbnr/pppp1ppp/8/4p3/2K2P1q/4P1P1/PPPP3P/RNBQ1BNR w kq - 0 1", "f4", 1), \
    TestIsPinned("rn2kbnr/pppp1ppp/4b3/3Pp3/2K2P1q/4P1P1/PPP4P/RNBQ1BNR w kq - 0 1", "d5", 1), \
    TestIsPinned("r3kbnr/pppp1ppp/4b3/3Pp3/4nP1q/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "g3", 1), \
    TestIsPinned("r3kbnr/pppp1ppp/4b3/3Pp2q/4nP2/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "g3", 0), \
    TestIsPinned("r3kbnr/pppp1ppp/4b3/3Pp2q/4nP2/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "a1", 0), \
    TestIsPinned("r3kbnr/pppp1ppp/4b3/3Pp2q/4nP2/4P1P1/PPP2K1P/RNBQ1BNR w kq - 0 1", "b1", 0), \
    TestIsPinned("r3kbnr/pppp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/RNBQK1NR w kq - 0 1", "d7", 1), \
    TestIsPinned("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "d7", 1), \
    TestIsPinned("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "a8", 0), \
    TestIsPinned("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "e6", 0), \
    TestIsPinned("r1bk1bnr/pp4pp/2npNp2/4P1B1/3P4/5N2/PPP3PP/R2QKB1R b KQ - 0 11", "f6", 1), \
    TestIsPinned("r1b2bnr/pp2k1pp/2npNP2/6B1/3P4/5N2/PPP3PP/R2QKB1R b KQ - 0 12", "f6", 0)

int TestValidMoves(const char* fen, const char* square, const char* first, ...);
#define TEST_BITBOARD_VALID_MOVES \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "a1", "b1", NULL), \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "a2", "a3", "a4", NULL), \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "c1", "d2", NULL), \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "d1", "d2", "d3", "d4", "e2", "f3", "g4", "h5", NULL), \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "f4", "e5", "f5", NULL), \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "f7", NULL), \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "c7", "a8", "a6", "e6", NULL), \
    TestValidMoves("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR w kq - 0 1", "b5", "a4", "c4", "d3", "e2", "f1", "c6", "d7", "a6", NULL), \
    TestValidMoves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R b Kkq - 0 1", "e8", "f7", "d7", NULL), \
    TestValidMoves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R b Kkq - 0 1", "a7", "a6", "a5", NULL), \
    TestValidMoves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R b Kkq - 0 1", "f6", "f5", NULL), \
    TestValidMoves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R b Kkq - 0 1", "g4", "h3", "f3", "e2", "h5", "f5", "e6", "d7", "c8", NULL), \
    TestValidMoves("r3kb1r/ppB1n1pp/5p2/8/3P2b1/2N4P/PPP1B1P1/2KR3R w Kkq - 0 1", "c3", "a4", "b5", "d5", "e4", "b1", NULL), \
    TestValidMoves("r3kb1r/pp2n1pp/5p2/8/3P2b1/1BN4P/PPP1B1P1/2KR3R b Kkq - 0 1", "e8", "d7", "d8", "c8", NULL), \
    TestValidMoves("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e3", "e4", "e5", "e6", "e7", NULL), \
    TestValidMoves("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "e4", "f5", "g6", NULL), \
    TestValidMoves("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e3", "e4", "e6", "e7", "e8", NULL), \
    TestValidMoves("rnbqkbnr/1p2pppp/p2p4/8/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 1", "e8", "d7", NULL), \
    TestValidMoves("8/5p1p/6k1/6Q1/7P/8/8/3K4 b - - 0 1", "g6", NULL)

int TestMoveIsValid(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_MOVE_IS_VALID \
    TestMoveIsValid("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1", "a2", "a3", 1), \
    TestMoveIsValid("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "f6", "e4", 1), \
    TestMoveIsValid("r1bqkb1r/pppp1ppp/2n5/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "f7", "f6", 0), \
    TestMoveIsValid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "a8", "b8", 0), \
    TestMoveIsValid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "a7", "a6", 0), \
    TestMoveIsValid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "e8", "e7", 1), \
    TestMoveIsValid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R b KQkq - 0 1", "g7", "g6", 1), \
    TestMoveIsValid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R w KQkq - 0 1", "e1", "g1", 1), \
    TestMoveIsValid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R w Qkq - 0 1", "e1", "g1", 0), \
    TestMoveIsValid("r1bqkb1r/pppp2pp/2n2p2/3np2Q/2B1P3/5N2/PPPP1PPP/RNB1K2R w Qkq - 0 1", "e1", "g1", 0), \
    TestMoveIsValid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "d1", 0), \
    TestMoveIsValid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e3", 1), \
    TestMoveIsValid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e4", 1), \
    TestMoveIsValid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e5", 1), \
    TestMoveIsValid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e6", 1), \
    TestMoveIsValid("rnb1kbnr/ppppqppp/8/2p5/2P5/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1", "e2", "e7", 1), \
    TestMoveIsValid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "c4", 0), \
    TestMoveIsValid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "e4", 1), \
    TestMoveIsValid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "f5", 1), \
    TestMoveIsValid("3k4/8/6b1/8/8/3B4/2K5/8 w - - 0 1", "d3", "g6", 1), \
    TestMoveIsValid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "d5", 0), \
    TestMoveIsValid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e3", 1), \
    TestMoveIsValid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e4", 1), \
    TestMoveIsValid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e6", 1), \
    TestMoveIsValid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e7", 1), \
    TestMoveIsValid("3kr3/8/8/4R3/8/8/4K3/8 w - - 0 1", "e5", "e8", 1), \
    TestMoveIsValid("rnbqkbnr/1p2pppp/p2p4/8/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 1", "e8", "d7", 1), \
    TestMoveIsValid("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1), \
    TestMoveIsValid("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR b kq - 0 1", "f7", "f6", 0), \
    TestMoveIsValid("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR b kq - 0 1", "f7", "f5", 0), \
    TestMoveIsValid("8/5p1p/6k1/6Q1/7P/8/8/3K4 b - - 0 1", "g6", "g7", 0), \
    TestMoveIsValid("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 5 4", "e1", "g1", 1)


int TestPawnIsEnpassanting(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_PAWN_IS_ENPASSANTING \
    TestPawnIsEnpassanting("rnbqkbnr/1pppp1pp/p7/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 1", "e5", "f6", 1), \
    TestPawnIsEnpassanting("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    TestPawnIsEnpassanting(STARTING_FEN, "b8", "c6", 0), \
    TestPawnIsEnpassanting("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1)

int TestPawnCanEnpassant(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_PAWN_CAN_ENPASSANT \
    TestPawnCanEnpassant("rnbqkbnr/1pppp1pp/p7/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 1", "e5", "f6", 1), \
    TestPawnCanEnpassant("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    TestPawnCanEnpassant("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1)

int TestMove(const char* fen, const char* from, const char* to, _Bool expected);
#define TEST_BITBOARD_MOVE \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "e4", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "e3", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "e7", "e6", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "e7", "e5", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "g1", "f3", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "b1", "c3", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "g8", "f6", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "b8", "c6", 1), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a2", 0), \
    TestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "a1", "a3", 0), \
    TestMove("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1", "e5", "d6", 1), \
    TestMove("rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", "e5", "d6", 0), \
    TestMove("rnbqkbnr/pppp1pp1/8/4p3/4P1Pp/PP6/2PP1P1P/RNBQKBNR b KQkq g3 0 1", "h4", "g3", 1), \
    TestMove("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 1), \
    TestMove("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 1), \
    TestMove("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8", "g8", 1), \
    TestMove("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", "e8", "c8", 1), \
    TestMove("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestMove("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 1), \
    TestMove("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "d1", 0), \
    TestMove("3rk2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", "e1", "d2", 0), \
    TestMove("r3k2r/8/8/2q5/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestMove("r3k2r/8/8/2q5/8/8/8/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestMove("r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1", "e1", "c1", 0), \
    TestMove("r3k2r/8/8/8/8/6q1/5N2/R3K2R w KQkq - 0 1", "e1", "c1", 1), \
    TestMove("r3k2r/8/8/8/8/6q1/5N2/R3K2R w KQkq - 0 1", "e1", "g1", 0), \
    TestMove("r3k2r/8/8/8/8/6q1/5N2/R3K2R w KQkq - 0 1", "f2", "e4", 0), \
    TestMove("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 5 4", "e1", "g1", 1)

int TestKingInCheck(const char* fen, int color, _Bool expected);
#define TEST_BITBOARD_KING_IN_CHECK \
    TestKingInCheck("r3k2r/8/8/4q3/8/8/8/R3K2R w KQkq - 0 1", 1, 1), \
    TestKingInCheck("r3kbnr/ppNp1ppp/4b3/1B1Pp2q/4nP2/4P1P1/PPP4P/R1BQK1NR b kq - 0 1", 0, 1)

int TestBoardInitFen(const char* fen, _Bool turn, uint8_t castling_rights, char enpassant_square[3], size_t halfmove, size_t fullmove);
#define TEST_BITBOARD_BOARD_INIT_FEN \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1, 0b1111, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", 0, 0b1111, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qkq - 0 1", 1, 0b1110, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kkq - 0 1", 1, 0b1101, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQq - 0 1", 1, 0b1011, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQk - 0 1", 1, 0b0111, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1", 1, 0b0000, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq g3 0 1", 1, 0b1111, "g3", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1, 0b1111, "-", 0, 1), \
    TestBoardInitFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 20 40", 1, 0b1111, "-", 20, 40)

int TestCheckmate(const char* fen, _Bool expected);
#define TEST_BITBOARD_CHECKMATE \
    TestCheckmate("r1bqkb1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1", 1), \
    TestCheckmate("3rkr2/8/4Q3/8/8/8/8/4K3 b - - 0 1", 1), \
    TestCheckmate("8/5p1p/6k1/6Q1/7P/8/8/3K4 b - - 0 1", 1), \
    TestCheckmate("8/8/8/6p1/5qk1/7Q/6K1/8 b - - 0 1", 1), \
    TestCheckmate("6kR/8/5Q2/8/8/8/6K1/8 b - - 0 1", 1), \
    TestCheckmate("6k1/5Q1R/8/8/8/8/6K1/8 b - - 0 1", 1), \
    TestCheckmate("6Q1/5Bpk/5p1p/8/8/8/8/4K3 b - - 0 1", 1), \
    TestCheckmate("4k3/8/2B2Q2/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("7k/5N2/6Q1/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("7k/5Q2/6N1/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("6k1/5p2/5N1Q/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("r3rqk1/6pQ/5pP1/8/8/1pP5/1P6/1K6 b - - 0 1", 1), \
    TestCheckmate("6k1/5pQp/5Pp1/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("4R1k1/5ppp/8/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("r4rk1/6RR/8/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("1R5k1/R7/8/8/8/8/8/6K1 b - - 0 1", 1), \
    TestCheckmate("6kR/5p2/8/8/8/8/8/B6K1 b - - 0 1", 1), \
    TestCheckmate("5rk1/5p1p/8/8/8/8/8/B5RK b - - 0 1", 1), \
    TestCheckmate("rnbqkb1r/ppppp2p/5n2/5ppQ/8/4P3/PPPP1PPP/RNB1KBNR w KQkq - 0 1", 0), \
    TestCheckmate("r1b1kb1r/ppp2ppp/5n2/4q3/8/2N5/PPPB1PPP/R2QKBNR w KQkq - 0 1", 0)


int TestInsufficientMaterial(const char* fen, _Bool expected);
#define TEST_BITBOARD_INSUFFICIENT_MATERIAL \
    TestInsufficientMaterial("8/8/4k3/8/8/2K5/8/8 w - - 0 1", 1), \
    TestInsufficientMaterial("8/8/1b2k3/8/4N3/2K5/8/8 w - - 0 1", 1), \
    TestInsufficientMaterial("8/4k3/8/8/3NN3/2K5/8/8 w - - 0 1", 1), \
    TestInsufficientMaterial("8/2b1k3/8/8/3NN3/2K5/8/8 w - - 0 1", 0), \
    TestInsufficientMaterial("8/2b1k3/8/8/8/2K5/8/8 w - - 0 1", 1), \
    TestInsufficientMaterial("8/4k3/8/6n1/8/2K5/8/8 w - - 0 1", 1)

int TestStalemate(const char* fen, _Bool expected);
#define TEST_BITBOARD_STALEMATE  \
    TestStalemate("6k1/8/5Q1K/8/8/8/8/8 b - - 0 1", 1), \
    TestStalemate("2k5/2p3q1/8/6pK/5p1p/5P1P/6r1/8 w - - 2 48", 1), \
    TestStalemate("3k4/2r4p/4K1p1/6P1/3q4/8/8/8 w - - 4 58", 1), \
    TestStalemate("8/3q4/K7/2q5/3p4/k7/8/8 w - - 12 70", 1), \
    TestStalemate("8/8/8/8/K7/1N6/3Q4/1k6 b - - 28 87", 1), \
    TestStalemate("5K2/3k4/2p2pqp/1p6/1P6/8/8/8 w - - 0 47", 1), \
    TestStalemate("8/8/8/kPK5/P2R4/8/8/8 b - - 6 54", 1), \
    TestStalemate("8/6k1/7p/3r2pK/2p1p1P1/8/8/8 w - - 0 42", 1), \
    TestStalemate("2r5/Kp4p1/p5kp/P7/8/1r6/8/2q5 w - - 2 54", 1), \
    TestStalemate("8/8/8/8/8/5K1k/5Q2/8 b - - 2 61", 1), \
    TestStalemate("r5k1/2p2pp1/2P2r2/pKPq4/P6q/8/8/8 w - - 4 49", 1), \
    TestStalemate("6k1/6P1/6K1/8/8/8/8/8 b - - 2 61", 1), \
    TestStalemate("8/8/2p4K/1p2kq2/6q1/8/8/8 w - - 4 46", 1), \
    TestStalemate("8/6p1/7p/5q1k/8/6K1/4q3/8 w - - 2 55", 1), \
    TestStalemate("8/8/7p/2N4k/5PR1/7P/PBP3P1/3R2K1 b - - 0 37", 1), \
    TestStalemate("3QQQQQ/7R/k7/8/8/8/6K1/8 b - - 2 64", 1), \
    TestStalemate("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", 0)

int TestIsAttackedBy(const char* fen, const char* square, int attacked_by, char* first, ...);
#define TEST_BITBOARD_IS_ATTACKED_BY \
    TestIsAttackedBy("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "b3", 1, "a2", "c2", NULL), \
    TestIsAttackedBy("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "a3", 1, "b2", "b1", NULL), \
    TestIsAttackedBy("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "c3", 1, "b1", "b2", "d2",  NULL), \
    TestIsAttackedBy("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", "c6", 0, "b8", "b7", "d7", NULL), \
    TestIsAttackedBy("3rkr2/8/4Q3/8/8/8/8/4K3 b - - 0 1", "e8", 1, "e6", NULL)


#define TEST_BITBOARD \
    TEST_BITBOARD_PAWN_MOVE, \
    TEST_BITBOARD_ROOK_MOVE, \
    TEST_BITBOARD_BISHOP_MOVE, \
    TEST_BITBOARD_QUEEN_MOVE, \
    TEST_BITBOARD_KNIGHT_MOVE, \
    TEST_BITBOARD_KING_MOVE, \
    TEST_BITBOARD_IS_PINNED, \
    TEST_BITBOARD_PAWN_IS_ENPASSANTING, \
    TEST_BITBOARD_PAWN_CAN_ENPASSANT, \
    TEST_BITBOARD_MOVE, \
    TEST_BITBOARD_KING_IN_CHECK, \
    TEST_BITBOARD_BOARD_INIT_FEN, \
    TEST_BITBOARD_INSUFFICIENT_MATERIAL, \
    TEST_BITBOARD_MOVE_IS_VALID, \
    TEST_BITBOARD_VALID_MOVES, \
    TEST_BITBOARD_STALEMATE, \
    TEST_BITBOARD_CHECKMATE, \
    TEST_BITBOARD_IS_ATTACKED_BY


#endif // BITBOARD_TESTS_H
