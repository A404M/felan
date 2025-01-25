#pragma once

#include "compiler/lexer.h"
#include <stddef.h>

typedef enum ParserToken {
  PARSER_TOKEN_ROOT,

  PARSER_TOKEN_IDENTIFIER,

  PARSER_TOKEN_TYPE_FUNCTION,
  PARSER_TOKEN_TYPE_VOID,

  PARSER_TOKEN_KEYWORD_PRINT,

  PARSER_TOKEN_CONSTANT,

  PARSER_TOKEN_SYMBOL_EOL,
  PARSER_TOKEN_SYMBOL_CURLY_BRACKET,
  PARSER_TOKEN_SYMBOL_PARENTHESIS,
  PARSER_TOKEN_SYMBOL_COMMA,

  PARSER_TOKEN_FUNCTION_DEFINITION,

  PARSER_TOKEN_FUNCTION_CALL,

  PARSER_TOKEN_NONE,
} ParserToken;

extern const char *PARSER_TOKEN_STRINGS[];

typedef struct ParserOrder {
  bool ltr;
  size_t size;
  LexerToken data[8];
} ParserOrder;

typedef struct ParserNode {
  ParserToken token;
  char *str_begin;
  char *str_end;
  void *metadata;
  struct ParserNode *parent;
} ParserNode;

typedef struct ParserNodeTypeFunctionMetadata {
  ParserNode *arguments;
  ParserNode *returnType;
} ParserNodeTypeFunctionMetadata;

typedef struct ParserNodeVariableMetadata {
  ParserNode *name;
  ParserNode *type;
  ParserNode *value;
} ParserNodeVariableMetadata;

typedef struct ParserNodeFunctionDefnitionMetadata {
  ParserNode *arguments;
  ParserNode *returnType;
  ParserNode *body;
} ParserNodeFunctionDefnitionMetadata;

typedef struct ParserNodeArray {
  ParserNode **data;
  size_t size;
} ParserNodeArray;

typedef struct ParserNodeFunctionCall {
  ParserNode *function;
  ParserNodeArray *params;
} ParserNodeFunctionCall;

typedef ParserNode ParserNodeEOLMetadata;

void parserNodePrint(const ParserNode *node, int indent);
void parserNodeDelete(ParserNode *node);

ParserNode *parser(LexerNodeArray lexed);
bool parserNodeArray(LexerNode *begin, LexerNode *end, ParserNode *parent);

ParserNode *newParserNode(ParserToken token, char *str_begin, char *str_end,
                          void *metadata, ParserNode *parent);

ParserNode *parseNode(LexerNode *node, LexerNode *begin, LexerNode *end,
                      ParserNode *parent);

ParserNode *getUntilCommonParent(ParserNode *node, ParserNode *parent);

ParserNode *parserIdentifier(LexerNode *node, ParserNode *parent);
ParserNode *parserVoid(LexerNode *node, ParserNode *parent);
ParserNode *parserPrint(LexerNode *node, ParserNode *parent);
ParserNode *parserEol(LexerNode *node, LexerNode *begin, ParserNode *parent);
ParserNode *parserComma(LexerNode *node, LexerNode *begin, ParserNode *parent);
ParserNode *parserParenthesis(LexerNode *closing, LexerNode *begin,
                              ParserNode *parent);
ParserNode *parserCurlyBrackets(LexerNode *closing, LexerNode *begin,
                                ParserNode *parent);
ParserNode *parserFunction(LexerNode *node, LexerNode *begin, LexerNode *end,
                           ParserNode *parent);
ParserNode *parserVariable(LexerNode *node, LexerNode *begin, LexerNode *end,
                           ParserNode *parent);

bool isAllArguments(const ParserNodeArray *nodes);

bool isExpression(ParserNode *node);
bool isType(ParserNode *node);
bool isValue(ParserNode *node);
