/*
  test_utf8_casefold.c - Unit test harness for UTF-8 Unicode case folding in mairix (#29)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Declarations from mairix.h */
void utf8_casefold_inplace(char *str);
unsigned int unicode_casefold_codepoint(unsigned int cp);

static void test_casefold_pair(const char *input, const char *expected)
{
    char buf[256];
    strncpy(buf, input, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    utf8_casefold_inplace(buf);

    if (strcmp(buf, expected) == 0) {
        printf("[PASS] '%s' -> '%s'\n", input, buf);
    } else {
        printf("[FAIL] '%s' -> '%s' (expected '%s')\n", input, buf, expected);
        exit(1);
    }
}

int main(void)
{
    printf("=== RUNNING UTF-8 CASE FOLDING UNIT TESTS (MAIRIX #29) ===\n");

    /* 1. Latin-1 Supplement with Umlauts & Accents */
    test_casefold_pair("ÜBUNG", "übung");
    test_casefold_pair("CAFÉ", "café");
    test_casefold_pair("NAÏVE", "naïve");
    test_casefold_pair("RÉSUMÉ", "résumé");
    test_casefold_pair("ÅNGSTRÖM", "ångström");

    /* 2. Cyrillic */
    test_casefold_pair("МОСКВА", "москва");
    test_casefold_pair("ПРИВЕТ", "привет");
    test_casefold_pair("САНКТ-ПЕТЕРБУРГ", "санкт-петербург");

    /* 3. Greek */
    test_casefold_pair("ΑΘΗΝΑ", "αθηνα");
    test_casefold_pair("ΕΛΛΑΔΑ", "ελλαδα");

    /* 4. Latin Extended-A */
    test_casefold_pair("ČESKÁ", "česká");
    test_casefold_pair("ŁÓDŹ", "łódź");

    /* 5. Substring & Query Equivalence Simulation */
    char indexed_token[64] = "ÜBUNG";
    char query_term[64]    = "übung";
    utf8_casefold_inplace(indexed_token);
    utf8_casefold_inplace(query_term);
    assert(strcmp(indexed_token, query_term) == 0);
    printf("[PASS] Query 'übung' matches indexed 'ÜBUNG' after folding\n");

    char indexed_cyrillic[64] = "МОСКВА";
    char query_cyrillic[64]   = "москва";
    utf8_casefold_inplace(indexed_cyrillic);
    utf8_casefold_inplace(query_cyrillic);
    assert(strcmp(indexed_cyrillic, query_cyrillic) == 0);
    printf("[PASS] Query 'москва' matches indexed 'МОСКВА' after folding\n");

    char indexed_cafe[64] = "CAFÉ";
    char query_cafe[64]   = "café";
    utf8_casefold_inplace(indexed_cafe);
    utf8_casefold_inplace(query_cafe);
    assert(strcmp(indexed_cafe, query_cafe) == 0);
    printf("[PASS] Query 'café' matches indexed 'CAFÉ' after folding\n");

    printf("\n>>> ALL UTF-8 UNICODE CASE FOLDING TESTS PASSED (100%%) <<<\n");
    return 0;
}
