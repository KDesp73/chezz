#include "common.h"

int has_castling_rights(state_t state, uint8_t castling_rights)
{
    return state.castling_rights & castling_rights;
}

void revoke_castling_rights(state_t* state, uint8_t castling_rights)
{
    state->castling_rights &= ~castling_rights;
}

