#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

// some UTF-8 strings for testing
// this is actually the Unicode Katakana range, 30A0-30FF
#define KATAKANA_UTF8_01 "\xe3\x82\xa0\xe3\x82\xa1\xe3\x82\xa2\xe3\x82\xa3\xe3\x82\xa4\xe3\x82\xa5\xe3\x82\xa6\xe3\x82\xa7"
#define KATAKANA_UTF8_02 "\xe3\x82\xa8\xe3\x82\xa9\xe3\x82\xaa\xe3\x82\xab\xe3\x82\xac\xe3\x82\xad\xe3\x82\xae\xe3\x82\xaf"
#define KATAKANA_UTF8_03 "\xe3\x82\xb0\xe3\x82\xb1\xe3\x82\xb2\xe3\x82\xb3\xe3\x82\xb4\xe3\x82\xb5\xe3\x82\xb6\xe3\x82\xb7"
#define KATAKANA_UTF8_04 "\xe3\x82\xb8\xe3\x82\xb9\xe3\x82\xba\xe3\x82\xbb\xe3\x82\xbc\xe3\x82\xbd\xe3\x82\xbe\xe3\x82\xbf"
#define KATAKANA_UTF8_05 "\xe3\x83\x80\xe3\x83\x81\xe3\x83\x82\xe3\x83\x83\xe3\x83\x84\xe3\x83\x85\xe3\x83\x86\xe3\x83\x87"
#define KATAKANA_UTF8_06 "\xe3\x83\x88\xe3\x83\x89\xe3\x83\x8a\xe3\x83\x8b\xe3\x83\x8c\xe3\x83\x8d\xe3\x83\x8e\xe3\x83\x8f"
#define KATAKANA_UTF8_07 "\xe3\x83\x90\xe3\x83\x91\xe3\x83\x92\xe3\x83\x93\xe3\x83\x94\xe3\x83\x95\xe3\x83\x96\xe3\x83\x97"
#define KATAKANA_UTF8_08 "\xe3\x83\x98\xe3\x83\x99\xe3\x83\x9a\xe3\x83\x9b\xe3\x83\x9c\xe3\x83\x9d\xe3\x83\x9e\xe3\x83\x9f"
#define KATAKANA_UTF8_09 "\xe3\x83\xa0\xe3\x83\xa1\xe3\x83\xa2\xe3\x83\xa3\xe3\x83\xa4\xe3\x83\xa5\xe3\x83\xa6\xe3\x83\xa7"
#define KATAKANA_UTF8_10 "\xe3\x83\xa8\xe3\x83\xa9\xe3\x83\xaa\xe3\x83\xab\xe3\x83\xac\xe3\x83\xad\xe3\x83\xae\xe3\x83\xaf"
#define KATAKANA_UTF8_11 "\xe3\x83\xb0\xe3\x83\xb1\xe3\x83\xb2\xe3\x83\xb3\xe3\x83\xb4\xe3\x83\xb5\xe3\x83\xb6\xe3\x83\xb7"
#define KATAKANA_UTF8_12 "\xe3\x83\xb8\xe3\x83\xb9\xe3\x83\xba\xe3\x83\xbb\xe3\x83\xbc\xe3\x83\xbd\xe3\x83\xbe\xe3\x83\xbf"

// some wchar_t (UTF-32) strings for testing
// again, this is the Katakana range
#define KATAKANA_UTF32_01 L"\u30a0\u30a1\u30a2\u30a3\u30a4\u30a5\u30a6\u30a7"
#define KATAKANA_UTF32_02 L"\u30a8\u30a9\u30aa\u30ab\u30ac\u30ad\u30ae\u30af"
#define KATAKANA_UTF32_03 L"\u30b0\u30b1\u30b2\u30b3\u30b4\u30b5\u30b6\u30b7"
#define KATAKANA_UTF32_04 L"\u30b8\u30b9\u30ba\u30bb\u30bc\u30bd\u30be\u30bf"
#define KATAKANA_UTF32_05 L"\u30c0\u30c1\u30c2\u30c3\u30c4\u30c5\u30c6\u30c7"
#define KATAKANA_UTF32_06 L"\u30c8\u30c9\u30ca\u30cb\u30cc\u30cd\u30ce\u30cf"
#define KATAKANA_UTF32_07 L"\u30d0\u30d1\u30d2\u30d3\u30d4\u30d5\u30d6\u30d7"
#define KATAKANA_UTF32_08 L"\u30d8\u30d9\u30da\u30db\u30dc\u30dd\u30de\u30df"
#define KATAKANA_UTF32_09 L"\u30e0\u30e1\u30e2\u30e3\u30e4\u30e5\u30e6\u30e7"
#define KATAKANA_UTF32_10 L"\u30e8\u30e9\u30ea\u30eb\u30ec\u30ed\u30ee\u30ef"
#define KATAKANA_UTF32_11 L"\u30f0\u30f1\u30f2\u30f3\u30f4\u30f5\u30f6\u30f7"
#define KATAKANA_UTF32_12 L"\u30f8\u30f9\u30fa\u30fb\u30fc\u30fd\u30fe\u30ff"

// print a block of memory in a formatted hex dump
// useful for comparing output of functions that may not be ASCII
void print_memory_char(char* buf, int length) {
  char hexbuf[16*3+1];

  int i, j;
  for(i = 0; i < length; i += 16) {
    hexbuf[0] = 0;
    for(j = i; j < length && j < (i + 16); ++j) {
      char hexchar[4];
      snprintf(hexchar, 4, "%02hhx ", buf[j]);
      strcat(hexbuf, hexchar);
    }

    printf("%-50s", hexbuf);
    for(j = i; j < length && j < (i + 16); ++j) {
      char c = buf[j];
      printf("%c", (c >= 32 && c <= 126) ? c : '.');
    }
    printf("\n");
  }
}

void print_memory_wchar(wchar_t* buf, int length) {
  char hexbuf[16*3+1];

  int i, j;
  for(i = 0; i < length; i += 4) {
    hexbuf[0] = 0;
    for(j = i; j < length && j < (i + 4); ++j) {
      char hexchar[13];
      wchar_t quad = buf[j];
      char e1 = (quad & 0x000000ff);
      char e2 = (quad & 0x0000ff00) >> 8;
      char e3 = (quad & 0x00ff0000) >> 16;
      char e4 = (quad & 0xff000000) >> 24;
      snprintf(hexchar, 13, "%02hhx %02hhx %02hhx %02hhx ", e1, e2, e3, e4);
      strcat(hexbuf, hexchar);
    }

    printf("%-50s", hexbuf);
    for(j = i; j < length && j < (i + 4); ++j) {
      wchar_t quad = buf[j];
      char e1 = (quad & 0x000000ff);
      char e2 = (quad & 0x0000ff00) >> 8;
      char e3 = (quad & 0x00ff0000) >> 16;
      char e4 = (quad & 0xff000000) >> 24;
      printf("%c", (e1 >= 32 && e1 <= 126) ? e1 : '.');
      printf("%c", (e2 >= 32 && e2 <= 126) ? e2 : '.');
      printf("%c", (e3 >= 32 && e3 <= 126) ? e3 : '.');
      printf("%c", (e4 >= 32 && e4 <= 126) ? e4 : '.');
    }
    printf("\n");
  }
}

#define TEST_FORMATTER_CHAR(fmtstring, ...) do {                                            \
    snprintf(outbuf, bufsize, fmtstring, ## __VA_ARGS__);                                   \
    printf("Result: <%s>\nMemory dump:\n", outbuf);                                         \
    print_memory_char(outbuf, strlen(outbuf));                                              \
    printf("\n");                                                                           \
  } while(0);

#define TEST_FORMATTER_WCHAR(fmtstring, ...) do {                                           \
    swprintf(woutbuf, bufsize, fmtstring, ## __VA_ARGS__);                                  \
    printf("Result: <%ls>\nMemory dump:\n", /*(wchar_t*)outbuf*/ L"N/A");                   \
    print_memory_wchar(woutbuf, wcslen(woutbuf));                                           \
    printf("\n");                                                                           \
  } while(0);

void test_snprintf() {
  const size_t bufsize = 1024;
  char outbuf[bufsize];
  wchar_t woutbuf[bufsize];

  // Substitute ASCII (< 128) strings into ASCII strings, using both char and wchar
  printf("Substitute ASCII char* string into ASCII char* string:\n");
  TEST_FORMATTER_CHAR("Lorem %s dolor sit amet,", "ipsum");

  printf("Substitute ASCII wchar_t* string into ASCII char* string:\n");
  TEST_FORMATTER_CHAR("consectetur %ls elit,", L"adipisicing");

  printf("Substitute ASCII char* string into ASCII wchar_t* string:\n");
  TEST_FORMATTER_WCHAR(L"sed do eiusmod %s incididunt ut", "tempor");

  printf("Substitute ASCII wchar_t* string into ASCII wchar_t* string:\n");
  TEST_FORMATTER_WCHAR(L"labore et %ls magna aliqua.", L"dolore");

  // Substitute ASCII characters into ASCII strings, using both char and wchar
  printf("Substitute ASCII char into ASCII char* string:\n");
  TEST_FORMATTER_CHAR("Ut enim ad %cinim veniam,", 'm');

  printf("Substitute ASCII wchar_t into ASCII char* string:\n");
  TEST_FORMATTER_CHAR("quis nostrud e%lcercitation ullamco", L'x');  

  printf("Substitute ASCII char into ASCII wchar_t* string:\n");
  TEST_FORMATTER_WCHAR(L"laboris n%csi ut aliquip", 'i');  

  printf("Substitute ASCII wchar_t into ASCII wchar_t* string:\n");
  TEST_FORMATTER_WCHAR(L"ex ea c%lcmmodo consequat.", L'o');  

  // Substitute UTF-8/ASCII char strings
  printf("Substitute ASCII char string into UTF-8 char string:\n");
  TEST_FORMATTER_CHAR(KATAKANA_UTF8_01 " %s " KATAKANA_UTF8_02, "Hello");

  printf("Substitute UTF-8 char string into UTF-8 char string:\n");
  TEST_FORMATTER_CHAR(KATAKANA_UTF8_03 " %s " KATAKANA_UTF8_05, KATAKANA_UTF8_04);

  printf("Substitute UTF-8 char string into ASCII char string:\n");
  TEST_FORMATTER_CHAR("Foo %s bar", KATAKANA_UTF8_06);

  // Subsitute UTF-32 strings
  printf("Substitute UTF-32 char string into UTF-32 wchar_t string:\n");
  TEST_FORMATTER_WCHAR(KATAKANA_UTF32_07 L" %ls " KATAKANA_UTF32_09, KATAKANA_UTF32_08);

  // Cannot substitute UTF-32 into UTF-8 or vice versa

  // Substitute NULL
  printf("Substitute NULL char string into char string:\n");
  TEST_FORMATTER_CHAR("Null string:<%s>", (char*)NULL);

  printf("Substitute NULL wchar_t string into char string:\n");
  TEST_FORMATTER_CHAR("Null string:<%ls>", (wchar_t*)NULL);

  printf("Substitute NULL char string into wchar_t string:\n");
  TEST_FORMATTER_WCHAR(L"Null string:<%s>", (char*)NULL);

  printf("Substitute NULL wchar_t string into wchar_t string:\n");
  TEST_FORMATTER_WCHAR(L"Null string:<%ls>", (wchar_t*)NULL);
}

int main() {
  // Some libc implementations (Mac OS X) seem to have problems with the running
  // wide character strings through swprintf if you are using the default "C"
  // locale.
  //
  // This doesn't affect Emscripten directly, but in order to get the
  // reference output, it is nice to be able to run this on another platform.
  setlocale(LC_ALL, "");

  test_snprintf();
  return EXIT_SUCCESS;
}
