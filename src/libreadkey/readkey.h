/* Library for working with reading input from user.
   All functions return SUCCESS or FAIL (defined in common.h).  */

#pragma once

// Readable characters.
enum keys {
  LOAD,
  SAVE,
  RUN,
  STEP,
  RESET,
  F5,
  F6,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  ENTER,
  OTHER
};

/* Read sequence from terminal and write first pressed key to k.  */
int rk_readkey(enum keys* k);

/* Save actual terminal settings. */
int rk_termsave(const char* const file_name);

/* Load actual terminal settings. */
int rk_termrestore(const char* const file_name);

/* Switch between terminal states. */
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);
