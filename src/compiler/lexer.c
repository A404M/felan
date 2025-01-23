#include "lexer.h"

#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

const char *LEXER_TOKEN_STRINGS[] = {
    "LEXER_TOKEN_IDENTIFIER",
    "LEXER_TOKEN_KEYWORD_VOID",
    "LEXER_TOKEN_KEYWORD_PRINT",

    "LEXER_TOKEN_NUMBER",

    "LEXER_TOKEN_SYMBOL",
    "LEXER_TOKEN_SYMBOL_EOL",
    "LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS",
    "LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS",
    "LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET",
    "LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET",
    "LEXER_TOKEN_SYMBOL_FUNCTION_ARROW",
    "LEXER_TOKEN_SYMBOL_COLON",
    "LEXER_TOKEN_SYMBOL_COMMA",

    "LEXER_TOKEN_NONE",
};

const char *LEXER_SYMBOL_STRINGS[] = {
    ";", "(", ")", "{", "}", "->", ":", ",",
};
const LexerToken LEXER_SYMBOL_TOKENS[] = {
    LEXER_TOKEN_SYMBOL_EOL,
    LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS,
    LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS,
    LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET,
    LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET,
    LEXER_TOKEN_SYMBOL_FUNCTION_ARROW,
    LEXER_TOKEN_SYMBOL_COLON,
    LEXER_TOKEN_SYMBOL_COMMA,
};
const size_t LEXER_SYMBOL_SIZE =
    sizeof(LEXER_SYMBOL_TOKENS) / sizeof(*LEXER_SYMBOL_TOKENS);

const char *LEXER_KEYWORD_STRINGS[] = {
    "void",
    "print",
};
const LexerToken LEXER_KEYWORD_TOKENS[] = {
    LEXER_TOKEN_KEYWORD_VOID,
    LEXER_TOKEN_KEYWORD_PRINT,
};
const size_t LEXER_KEYWORD_SIZE =
    sizeof(LEXER_KEYWORD_TOKENS) / sizeof(*LEXER_KEYWORD_TOKENS);

bool lexerNodeArrayIsError(LexerNodeArray array) {
  return LEXER_NODE_ARRAY_ERROR.size == array.size;
}

void lexerNodeArrayPrint(LexerNodeArray array) {
  for (size_t i = 0; i < array.size; ++i) {
    LexerNode node = array.data[i];
    printf("{str=\"%.*s\",token=%s}\n", (int)(node.str_end - node.str_begin),
           node.str_begin, LEXER_TOKEN_STRINGS[node.token]);
  }
}

void lexerNodeArrayDestroy(LexerNodeArray array) { free(array.data); }

LexerNodeArray lexer(char *str) {
  size_t result_size = 0;
  LexerNodeArray result = {
      .data = a404m_malloc(result_size),
      .size = 0,
  };

  LexerToken node_token = LEXER_TOKEN_NONE;
  char *node_str_begin = str;
  char *iter = str;
  for (; *iter != '\0'; ++iter) {
    char c = *iter;
    if (c == '/') {
      ++iter;
      c = *iter;
      if (c == '/') {
        lexerPushClear(&result, &result_size, iter - 1, &node_str_begin,
                       &node_token, LEXER_TOKEN_NONE);
        for (; *iter != '\n'; ++iter) {
          if (*iter == '\0') {
            goto RETURN_SUCCESS;
          }
        }
        continue;
      } else if (c == '*') {
        lexerPushClear(&result, &result_size, iter - 1, &node_str_begin,
                       &node_token, LEXER_TOKEN_NONE);
        ++iter;
        int in = 1;
        for (; in != 0; ++iter) {
          if (*iter == '*' && *(iter + 1) == '/') {
            --in;
          } else if (*iter == '/' && *(iter + 1) == '*') {
            ++in;
          } else if (*iter == '\0') {
            goto RETURN_ERROR;
          }
        }
      }
    }
    if (isSpace(c)) {
      lexerPushClear(&result, &result_size, iter, &node_str_begin, &node_token,
                     LEXER_TOKEN_NONE);
    } else if (isIdentifier(c)) {
      if (node_token != LEXER_TOKEN_IDENTIFIER) {
        lexerPushClear(&result, &result_size, iter, &node_str_begin,
                       &node_token, LEXER_TOKEN_IDENTIFIER);
      }
    } else if (isNumber(c)) {
      if (node_token != LEXER_TOKEN_IDENTIFIER &&
          node_token != LEXER_TOKEN_NUMBER) {
        lexerPushClear(&result, &result_size, iter, &node_str_begin,
                       &node_token, LEXER_TOKEN_NUMBER);
      }
    } else if (isSymbol(c)) {
      if (node_token != LEXER_TOKEN_SYMBOL) {
        lexerPushClear(&result, &result_size, iter, &node_str_begin,
                       &node_token, LEXER_TOKEN_SYMBOL);
      }
    } else if (isSingleSymbol(c)) {
      lexerPushClear(&result, &result_size, iter, &node_str_begin, &node_token,
                     LEXER_TOKEN_SYMBOL);
    } else {
    RETURN_ERROR:
      free(result.data);
      printLog("Unexpected character '%c' at position %ld", c, iter - str);
      return LEXER_NODE_ARRAY_ERROR;
    }
  }
  lexerPushClear(&result, &result_size, iter, &node_str_begin, &node_token,
                 LEXER_TOKEN_NONE);

RETURN_SUCCESS:
  result.data = a404m_realloc(result.data, result.size * sizeof(*result.data));

  return result;
}

void lexerPushClear(LexerNodeArray *array, size_t *array_size, char *iter,
                    char **node_str_begin, LexerToken *node_token,
                    LexerToken token) {
  switch (*node_token) {
  case LEXER_TOKEN_IDENTIFIER: {
    const size_t index =
        searchInStringArray(LEXER_KEYWORD_STRINGS, LEXER_KEYWORD_SIZE,
                            *node_str_begin, iter - *node_str_begin);
    if (index != LEXER_KEYWORD_SIZE) {
      *node_token = LEXER_KEYWORD_TOKENS[index];
    }
  }
    goto PUSH;
  case LEXER_TOKEN_SYMBOL: {
    const size_t index =
        searchInStringArray(LEXER_SYMBOL_STRINGS, LEXER_SYMBOL_SIZE,
                            *node_str_begin, iter - *node_str_begin);
    if (index != LEXER_SYMBOL_SIZE) {
      *node_token = LEXER_SYMBOL_TOKENS[index];
    }
  }
    // goto PUSH;
  PUSH:
  case LEXER_TOKEN_KEYWORD_VOID:
  case LEXER_TOKEN_KEYWORD_PRINT:
  case LEXER_TOKEN_NUMBER:
  case LEXER_TOKEN_SYMBOL_EOL:
  case LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS:
  case LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS:
  case LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET:
  case LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET:
  case LEXER_TOKEN_SYMBOL_FUNCTION_ARROW:
  case LEXER_TOKEN_SYMBOL_COLON:
  case LEXER_TOKEN_SYMBOL_COMMA:
    if (*array_size == array->size) {
      *array_size += 1 + *array_size / 2;
      array->data =
          a404m_realloc(array->data, *array_size * sizeof(*array->data));
    }

    array->data[array->size].token = *node_token;
    array->data[array->size].str_begin = *node_str_begin;
    array->data[array->size].str_end = iter;
    array->data[array->size].parserNode = NULL;

    array->size += 1;

    // goto RETURN_SUCCESS;
  case LEXER_TOKEN_NONE:
    goto RETURN_SUCCESS;
  }
  printLog("Bad token '%d'", *node_token);
  exit(1);
RETURN_SUCCESS:
  *node_str_begin = iter;
  *node_token = token;
}

bool isIdentifier(char c) {
  return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_';
}

bool isNumber(char c) { return '0' <= c && c <= '9'; }

bool isSymbol(char c) {
  switch (c) {
  case '-':
  case '>':
  case '.':
  case '+':
  case '*':
  case '/':
  case '%':
  case '=':
    return true;
  default:
    return false;
  }
}

bool isSingleSymbol(char c) {
  switch (c) {
  case ';':
  case ':':
  case ',':
  case '(':
  case ')':
  case '{':
  case '}':
    return true;
  default:
    return false;
  }
}

bool isSpace(char c) { return isspace(c); }
