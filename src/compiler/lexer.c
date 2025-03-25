#include "lexer.h"

#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const char *LEXER_TOKEN_STRINGS[] = {
    "LEXER_TOKEN_IDENTIFIER",

    "LEXER_TOKEN_KEYWORD_TYPE",
    "LEXER_TOKEN_KEYWORD_VOID",
    "LEXER_TOKEN_KEYWORD_I8",
    "LEXER_TOKEN_KEYWORD_U8",
    "LEXER_TOKEN_KEYWORD_I16",
    "LEXER_TOKEN_KEYWORD_U16",
    "LEXER_TOKEN_KEYWORD_I32",
    "LEXER_TOKEN_KEYWORD_U32",
    "LEXER_TOKEN_KEYWORD_I64",
    "LEXER_TOKEN_KEYWORD_U64",
    "LEXER_TOKEN_KEYWORD_F16",
    "LEXER_TOKEN_KEYWORD_F32",
    "LEXER_TOKEN_KEYWORD_F64",
    "LEXER_TOKEN_KEYWORD_F128",
    "LEXER_TOKEN_KEYWORD_BOOL",
    "LEXER_TOKEN_KEYWORD_PRINT_U64",
    "LEXER_TOKEN_KEYWORD_RETURN",
    "LEXER_TOKEN_KEYWORD_TRUE",
    "LEXER_TOKEN_KEYWORD_FALSE",
    "LEXER_TOKEN_KEYWORD_IF",
    "LEXER_TOKEN_KEYWORD_ELSE",
    "LEXER_TOKEN_KEYWORD_WHILE",
    "LEXER_TOKEN_KEYWORD_COMPTIME",
    "LEXER_TOKEN_KEYWORD_NULL",

    "LEXER_TOKEN_NUMBER",

    "LEXER_TOKEN_SYMBOL",
    "LEXER_TOKEN_SYMBOL_EOL",
    "LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS",
    "LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS",
    "LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET",
    "LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET",
    "LEXER_TOKEN_SYMBOL_FUNCTION_ARROW",
    "LEXER_TOKEN_SYMBOL_COLON",
    "LEXER_TOKEN_SYMBOL_ASSIGN",
    "LEXER_TOKEN_SYMBOL_SUM_ASSIGN",
    "LEXER_TOKEN_SYMBOL_SUB_ASSIGN",
    "LEXER_TOKEN_SYMBOL_MULTIPLY_ASSIGN",
    "LEXER_TOKEN_SYMBOL_DIVIDE_ASSIGN",
    "LEXER_TOKEN_SYMBOL_MODULO_ASSIGN",
    "LEXER_TOKEN_SYMBOL_COMMA",
    "LEXER_TOKEN_SYMBOL_PLUS",
    "LEXER_TOKEN_SYMBOL_MINUS",
    "LEXER_TOKEN_SYMBOL_SUM",
    "LEXER_TOKEN_SYMBOL_SUB",
    "LEXER_TOKEN_SYMBOL_MULTIPLY",
    "LEXER_TOKEN_SYMBOL_DIVIDE",
    "LEXER_TOKEN_SYMBOL_MODULO",
    "LEXER_TOKEN_SYMBOL_EQUAL",
    "LEXER_TOKEN_SYMBOL_NOT_EQUAL",
    "LEXER_TOKEN_SYMBOL_GREATER",
    "LEXER_TOKEN_SYMBOL_SMALLER",
    "LEXER_TOKEN_SYMBOL_GREATER_OR_EQUAL",
    "LEXER_TOKEN_SYMBOL_SMALLER_OR_EQUAL",
    "LEXER_TOKEN_SYMBOL_POINTER",
    "LEXER_TOKEN_SYMBOL_ADDRESS",

    "LEXER_TOKEN_NONE",
};

const char *LEXER_SYMBOL_STRINGS[] = {
    ";", "(", ")", "{", "}", "->", ":",  "=",  "+=", "-=", "*=", "/=", "%=",
    ",", "+", "-", "*", "/", "%",  "==", "!=", ">",  ">=", "<",  "<=", "&",
};
const LexerToken LEXER_SYMBOL_TOKENS[] = {
    LEXER_TOKEN_SYMBOL_EOL,
    LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS,
    LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS,
    LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET,
    LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET,
    LEXER_TOKEN_SYMBOL_FUNCTION_ARROW,
    LEXER_TOKEN_SYMBOL_COLON,
    LEXER_TOKEN_SYMBOL_ASSIGN,
    LEXER_TOKEN_SYMBOL_SUM_ASSIGN,
    LEXER_TOKEN_SYMBOL_SUB_ASSIGN,
    LEXER_TOKEN_SYMBOL_MULTIPLY_ASSIGN,
    LEXER_TOKEN_SYMBOL_DIVIDE_ASSIGN,
    LEXER_TOKEN_SYMBOL_MODULO_ASSIGN,
    LEXER_TOKEN_SYMBOL_COMMA,
    LEXER_TOKEN_SYMBOL_PLUS,
    LEXER_TOKEN_SYMBOL_MINUS,
    LEXER_TOKEN_SYMBOL_POINTER,
    LEXER_TOKEN_SYMBOL_DIVIDE,
    LEXER_TOKEN_SYMBOL_MODULO,
    LEXER_TOKEN_SYMBOL_EQUAL,
    LEXER_TOKEN_SYMBOL_NOT_EQUAL,
    LEXER_TOKEN_SYMBOL_GREATER,
    LEXER_TOKEN_SYMBOL_GREATER_OR_EQUAL,
    LEXER_TOKEN_SYMBOL_SMALLER,
    LEXER_TOKEN_SYMBOL_SMALLER_OR_EQUAL,
    LEXER_TOKEN_SYMBOL_ADDRESS,
};
const size_t LEXER_SYMBOL_SIZE =
    sizeof(LEXER_SYMBOL_TOKENS) / sizeof(*LEXER_SYMBOL_TOKENS);

const char *LEXER_KEYWORD_STRINGS[] = {
    "type",   "void", "i8",    "u8",  "i16",  "u16",   "i32",      "u32",
    "i64",    "u64",  "f16",   "f32", "f64",  "f128",  "bool",     "print_u64",
    "return", "true", "false", "if",  "else", "while", "comptime", "null",
};
const LexerToken LEXER_KEYWORD_TOKENS[] = {
    LEXER_TOKEN_KEYWORD_TYPE,     LEXER_TOKEN_KEYWORD_VOID,
    LEXER_TOKEN_KEYWORD_I8,       LEXER_TOKEN_KEYWORD_U8,
    LEXER_TOKEN_KEYWORD_I16,      LEXER_TOKEN_KEYWORD_U16,
    LEXER_TOKEN_KEYWORD_I32,      LEXER_TOKEN_KEYWORD_U32,
    LEXER_TOKEN_KEYWORD_I64,      LEXER_TOKEN_KEYWORD_U64,
    LEXER_TOKEN_KEYWORD_F16,      LEXER_TOKEN_KEYWORD_F32,
    LEXER_TOKEN_KEYWORD_F64,      LEXER_TOKEN_KEYWORD_F128,
    LEXER_TOKEN_KEYWORD_BOOL,     LEXER_TOKEN_KEYWORD_PRINT_U64,
    LEXER_TOKEN_KEYWORD_RETURN,   LEXER_TOKEN_KEYWORD_TRUE,
    LEXER_TOKEN_KEYWORD_FALSE,    LEXER_TOKEN_KEYWORD_IF,
    LEXER_TOKEN_KEYWORD_ELSE,     LEXER_TOKEN_KEYWORD_WHILE,
    LEXER_TOKEN_KEYWORD_COMPTIME, LEXER_TOKEN_KEYWORD_NULL,
};
const size_t LEXER_KEYWORD_SIZE =
    sizeof(LEXER_KEYWORD_TOKENS) / sizeof(*LEXER_KEYWORD_TOKENS);

const LexerNodeArray LEXER_NODE_ARRAY_ERROR = {
    .size = SIZE_MAX,
};

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
        continue;
      } else {
        --iter;
        c = *iter;
      }
    }
    if (isSpace(c)) {
      lexerPushClear(&result, &result_size, iter, &node_str_begin, &node_token,
                     LEXER_TOKEN_NONE);
    } else if (isIdentifier(c)) {
      if (node_token != LEXER_TOKEN_IDENTIFIER &&
          node_token != LEXER_TOKEN_NUMBER) {
        lexerPushClear(&result, &result_size, iter, &node_str_begin,
                       &node_token, LEXER_TOKEN_IDENTIFIER);
      }
    } else if (isNumber(c)) {
      if (node_token != LEXER_TOKEN_IDENTIFIER &&
          node_token != LEXER_TOKEN_NUMBER) {
        lexerPushClear(&result, &result_size, iter, &node_str_begin,
                       &node_token, LEXER_TOKEN_NUMBER);
      }
    } else if (isSymbol(c) || isSingleSymbol(c)) {
      if (node_token != LEXER_TOKEN_SYMBOL || isSingleSymbol(*node_str_begin)) {
        lexerPushClear(&result, &result_size, iter, &node_str_begin,
                       &node_token, LEXER_TOKEN_SYMBOL);
      }
    } else {
    RETURN_ERROR:
      free(result.data);
      printError(iter, iter + 1,
                 "Unexpected character '%c' with code = %d at index %ld", c, c,
                 iter - str);
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
    // fall through
  PUSH:
  case LEXER_TOKEN_KEYWORD_TYPE:
  case LEXER_TOKEN_KEYWORD_VOID:
  case LEXER_TOKEN_KEYWORD_I8:
  case LEXER_TOKEN_KEYWORD_U8:
  case LEXER_TOKEN_KEYWORD_I16:
  case LEXER_TOKEN_KEYWORD_U16:
  case LEXER_TOKEN_KEYWORD_I32:
  case LEXER_TOKEN_KEYWORD_U32:
  case LEXER_TOKEN_KEYWORD_I64:
  case LEXER_TOKEN_KEYWORD_U64:
  case LEXER_TOKEN_KEYWORD_F16:
  case LEXER_TOKEN_KEYWORD_F32:
  case LEXER_TOKEN_KEYWORD_F64:
  case LEXER_TOKEN_KEYWORD_F128:
  case LEXER_TOKEN_KEYWORD_BOOL:
  case LEXER_TOKEN_KEYWORD_PRINT_U64:
  case LEXER_TOKEN_KEYWORD_RETURN:
  case LEXER_TOKEN_KEYWORD_TRUE:
  case LEXER_TOKEN_KEYWORD_FALSE:
  case LEXER_TOKEN_KEYWORD_IF:
  case LEXER_TOKEN_KEYWORD_ELSE:
  case LEXER_TOKEN_KEYWORD_WHILE:
  case LEXER_TOKEN_KEYWORD_COMPTIME:
  case LEXER_TOKEN_KEYWORD_NULL:
  case LEXER_TOKEN_NUMBER:
  case LEXER_TOKEN_SYMBOL_EOL:
  case LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS:
  case LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS:
  case LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET:
  case LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET:
  case LEXER_TOKEN_SYMBOL_FUNCTION_ARROW:
  case LEXER_TOKEN_SYMBOL_COLON:
  case LEXER_TOKEN_SYMBOL_ASSIGN:
  case LEXER_TOKEN_SYMBOL_SUM_ASSIGN:
  case LEXER_TOKEN_SYMBOL_SUB_ASSIGN:
  case LEXER_TOKEN_SYMBOL_MULTIPLY_ASSIGN:
  case LEXER_TOKEN_SYMBOL_DIVIDE_ASSIGN:
  case LEXER_TOKEN_SYMBOL_MODULO_ASSIGN:
  case LEXER_TOKEN_SYMBOL_COMMA:
  case LEXER_TOKEN_SYMBOL_PLUS:
  case LEXER_TOKEN_SYMBOL_MINUS:
  case LEXER_TOKEN_SYMBOL_SUM:
  case LEXER_TOKEN_SYMBOL_SUB:
  case LEXER_TOKEN_SYMBOL_MULTIPLY:
  case LEXER_TOKEN_SYMBOL_DIVIDE:
  case LEXER_TOKEN_SYMBOL_MODULO:
  case LEXER_TOKEN_SYMBOL_EQUAL:
  case LEXER_TOKEN_SYMBOL_NOT_EQUAL:
  case LEXER_TOKEN_SYMBOL_GREATER:
  case LEXER_TOKEN_SYMBOL_SMALLER:
  case LEXER_TOKEN_SYMBOL_GREATER_OR_EQUAL:
  case LEXER_TOKEN_SYMBOL_SMALLER_OR_EQUAL:
  case LEXER_TOKEN_SYMBOL_POINTER:
  case LEXER_TOKEN_SYMBOL_ADDRESS:
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

bool isNumber(char c) { return ('0' <= c && c <= '9') || c == '.'; }

bool isSymbol(char c) {
  switch (c) {
  case '-':
  case '>':
  case '<':
  case '.':
  case '+':
  case '*':
  case '/':
  case '%':
  case '=':
  case '!':
  case '&':
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
