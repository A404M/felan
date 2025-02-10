#pragma once

#include <stddef.h>

typedef enum LexerToken {
  LEXER_TOKEN_IDENTIFIER,

  LEXER_TOKEN_KEYWORD_TYPE,
  LEXER_TOKEN_KEYWORD_VOID,
  LEXER_TOKEN_KEYWORD_U64,
  LEXER_TOKEN_KEYWORD_BOOL,
  LEXER_TOKEN_KEYWORD_PRINT_U64,
  LEXER_TOKEN_KEYWORD_RETURN,
  LEXER_TOKEN_KEYWORD_TRUE,
  LEXER_TOKEN_KEYWORD_FALSE,

  LEXER_TOKEN_NUMBER,

  LEXER_TOKEN_SYMBOL,
  LEXER_TOKEN_SYMBOL_EOL,
  LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS,
  LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS,
  LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET,
  LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET,
  LEXER_TOKEN_SYMBOL_FUNCTION_ARROW,
  LEXER_TOKEN_SYMBOL_COLON,
  LEXER_TOKEN_SYMBOL_ASSIGN,
  LEXER_TOKEN_SYMBOL_COMMA,
  LEXER_TOKEN_SYMBOL_PLUS,
  LEXER_TOKEN_SYMBOL_MINUS,
  LEXER_TOKEN_SYMBOL_MULTIPLY,
  LEXER_TOKEN_SYMBOL_DIVIDE,
  LEXER_TOKEN_SYMBOL_MODULO,

  LEXER_TOKEN_NONE,
} LexerToken;

extern const char *LEXER_TOKEN_STRINGS[];

extern const char *LEXER_SYMBOL_STRINGS[];
extern const LexerToken LEXER_SYMBOL_TOKENS[];
extern const size_t LEXER_SYMBOL_SIZE;

extern const char *LEXER_KEYWORD_STRINGS[];
extern const LexerToken LEXER_KEYWORD_TOKENS[];
extern const size_t LEXER_KEYWORD_SIZE;

struct ParserNode;

typedef struct LexerNode {
  char *str_begin;
  char *str_end;
  LexerToken token;
  struct ParserNode *parserNode;
} LexerNode;

typedef struct LexerNodeArray {
  LexerNode *data;
  size_t size;
} LexerNodeArray;

extern const LexerNodeArray LEXER_NODE_ARRAY_ERROR;

extern bool lexerNodeArrayIsError(LexerNodeArray array);
extern void lexerNodeArrayPrint(LexerNodeArray array);
extern void lexerNodeArrayDestroy(LexerNodeArray array);

extern LexerNodeArray lexer(char *str);

extern void lexerPushClear(LexerNodeArray *array, size_t *array_size,
                           char *iter, char **node_str_begin,
                           LexerToken *node_token, LexerToken token);

extern bool isIdentifier(char c);
extern bool isNumber(char c);
extern bool isSymbol(char c);
extern bool isSingleSymbol(char c);
extern bool isSpace(char c);
