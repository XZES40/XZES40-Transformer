/*
 * FILE: charzero.h
 *
 * ZeroToOh
 * In-place Editor converts digit Zero to letter Oh
 *
 * OhToZero
 * In-place Editor converts letter Oh to Zero
 *
 */


/* ZeroToOh
 *
 * Edit buffer in-place, converting digits Zero to letters Oh
 * And returns a pointer to the buffer.
 *
 * theBuf = buffer to edit
 * theLimit = maximum characters in buffer to edit
 * endToken = token character to terminate the edit
 */

char * ZeroToOh(char * theBuf, int theLimit, char endToken);

/* OhToZero
 *
 * Edit buffer in-place, converting letters Oh to digits Zero
 * And returns a pointer to the buffer.
 *
 * theBuf = buffer to edit
 * theLimit = maximum characters in buffer to edit
 * endToken = token character to terminate the edit
 */

char * OhToZero(char * theBuf, int theLimit, char endToken);
