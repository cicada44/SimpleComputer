#include <libcommon/common.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct RUNTIME_ERRORS RE = {
    .ERROR_MEM_GET = "[E] ERROR MEMORY GETTING",
    .ERROR_MEM_SET = "[E] ERROR MEMORY SETTING",
    .ERROR_MEM_INIT = "[E] ERROR MEMORY INITIALIZATION",
    .ERROR_OPENING_FILE = "[E] ERROR OPENING FILE",
    .ERROR_FILE_READING = "[E] ERROR READING FILE",
    .ERROR_OPENING_TERM = "[E] ERROR OPENING TERMINAL",
    .ERROR_TERM_OPT = "[E] ERROR TERMINAL SET OPTION",
    .ERROR_REG_GET = "[E] ERROR GETTING REGISTER",
    .ERROR_REG_SET = "[E] ERROR SETTING REGISTER",
    .ERROR_COMMAND_DECODE = "[E] ERROR COMMAND DECODE",
    .ERROR_COMMAND_ENCODE = "[E] ERROR COMMAND ENCODE",
    .ERROR_ANY_TERM = "[E] ERROR TERMINAL",
    .ERROR_ANY_BC = "[E] ERROR BIG CHARACTER",
    .ERROR_READING_KEY = "[E] ERROR READING KEY",
    .ERROR_FILE_WRITING = "[E] ERROR FILE WRITING",
    .ERROR_TERMINAL_INTERFACE = "[E] ERROR TERMINAL INTERFACE"};

void runtime_error_process(const char* const error_msg) {
  mt_clrscr();
  mt_setbgcolor(RED);
  fprintf(stderr, "%s", error_msg);
  sleep(2);
  mt_resetcolor();
}
