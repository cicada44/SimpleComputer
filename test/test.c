#include <ctest.h>
#include <libcomputer/comp.h>
#include <stdlib.h>
#include <string.h>

CTEST(memory_init_tests, init) { ASSERT_EQUAL(sc_memoryInit(), 0); }

CTEST(memory_seting_getting_tests, set_get) {
  for (int x = 25; x != 75; ++x) {
    sc_memorySet(x, x);
  }

  int* accum = malloc(sizeof(int));

  for (int x = 0; x != 100; ++x) {
    int actual = sc_memoryGet(x, accum);
    if (x > 24 && x < 75) {
      ASSERT_EQUAL(*accum, x);
    } else {
      ASSERT_EQUAL(*accum, 0);
    }
    ASSERT_EQUAL(actual, 0);
  }

  free(accum);
}

CTEST(memory_save_load_tests, binary_saving_loading) {
  char* mem_path1 = "test/test_memory_snapshots/mem1";
  char* mem_path2 = "test/test_memory_snapshots/mem2";

  for (int x = 0; x != 100; ++x) {
    sc_memorySet(x, 0);
  }

  sc_memorySave(mem_path2);

  sc_memorySet(10, 1);

  sc_memorySave(mem_path1);

  sc_memoryInit();

  sc_memoryLoad(mem_path1);

  int* accum = malloc(sizeof(int));

  for (int x = 0; x != 100; ++x) {
    sc_memoryGet(x, accum);
    if (x == 10) {
      ASSERT_EQUAL(*accum, 1);
    } else {
      ASSERT_EQUAL(*accum, 0);
    }
  }

  sc_memoryLoad(mem_path2);

  for (int x = 0; x != 100; ++x) {
    sc_memoryGet(x, accum);
    ASSERT_EQUAL(*accum, 0);
  }

  free(accum);
}

CTEST(memory_register_tests, register_seting_getting) {
  for (int x = 0; x != 5; x++) {
    sc_regSet(x + 1, 1);
  }

  int* actual = malloc(sizeof(int));

  for (int x = 0; x != 5; x++) {
    sc_regGet(x + 1, actual);
    ASSERT_EQUAL(1, *actual);
  }

  for (int x = 0; x > -5; --x) {
    ASSERT_EQUAL(-1, sc_regGet(x, actual));
  }

  sc_regInit();

  for (int x = 0; x > -5; --x) {
    sc_regGet(x + 1, actual);
    ASSERT_EQUAL(*actual, 0);
  }
}

CTEST(memory_commands, commands_encoding_decoding) {
  int* encode_value = malloc(sizeof(int));

  sc_commandEncode(0x33, 0x59, encode_value);
  ASSERT_EQUAL(6617, *encode_value);

  sc_commandEncode(0xa, 0xa, encode_value);
  ASSERT_EQUAL(1290, *encode_value);

  sc_commandEncode(0x3c, 0x55, encode_value);
  ASSERT_EQUAL(7765, *encode_value);
}
