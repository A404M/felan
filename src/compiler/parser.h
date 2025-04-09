#pragma once

#include "compiler/lexer.h"
#include "utils/type.h"
#include <stddef.h>
#include <stdint.h>

typedef enum ParserToken {
  PARSER_TOKEN_ROOT,

  PARSER_TOKEN_IDENTIFIER,
  PARSER_TOKEN_BUILTIN,

  PARSER_TOKEN_VALUE_INT,
  PARSER_TOKEN_VALUE_FLOAT,
  PARSER_TOKEN_VALUE_BOOL,
  PARSER_TOKEN_VALUE_CHAR,

  PARSER_TOKEN_TYPE_TYPE,
  PARSER_TOKEN_TYPE_FUNCTION,
  PARSER_TOKEN_TYPE_VOID,
  PARSER_TOKEN_TYPE_BOOL,
  PARSER_TOKEN_TYPE_I8,
  PARSER_TOKEN_TYPE_U8,
  PARSER_TOKEN_TYPE_I16,
  PARSER_TOKEN_TYPE_U16,
  PARSER_TOKEN_TYPE_I32,
  PARSER_TOKEN_TYPE_U32,
  PARSER_TOKEN_TYPE_I64,
  PARSER_TOKEN_TYPE_U64,
#ifdef FLOAT_16_SUPPORT
  PARSER_TOKEN_TYPE_F16,
#endif
  PARSER_TOKEN_TYPE_F32,
  PARSER_TOKEN_TYPE_F64,
  PARSER_TOKEN_TYPE_F128,

  PARSER_TOKEN_KEYWORD_PUTC,
  PARSER_TOKEN_KEYWORD_RETURN,
  PARSER_TOKEN_KEYWORD_IF,
  PARSER_TOKEN_KEYWORD_WHILE,
  PARSER_TOKEN_KEYWORD_COMPTIME,
  PARSER_TOKEN_KEYWORD_NULL,
  PARSER_TOKEN_KEYWORD_STRUCT,
  PARSER_TOKEN_KEYWORD_UNDEFINED,

  PARSER_TOKEN_CONSTANT,
  PARSER_TOKEN_VARIABLE,

  PARSER_TOKEN_SYMBOL_EOL,
  PARSER_TOKEN_SYMBOL_CURLY_BRACKET,
  PARSER_TOKEN_SYMBOL_PARENTHESIS,
  PARSER_TOKEN_SYMBOL_COMMA,

  PARSER_TOKEN_OPERATOR_ASSIGN,
  PARSER_TOKEN_OPERATOR_SUM_ASSIGN,
  PARSER_TOKEN_OPERATOR_SUB_ASSIGN,
  PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN,
  PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN,
  PARSER_TOKEN_OPERATOR_MODULO_ASSIGN,
  PARSER_TOKEN_OPERATOR_PLUS,
  PARSER_TOKEN_OPERATOR_MINUS,
  PARSER_TOKEN_OPERATOR_SUM,
  PARSER_TOKEN_OPERATOR_SUB,
  PARSER_TOKEN_OPERATOR_MULTIPLY,
  PARSER_TOKEN_OPERATOR_DIVIDE,
  PARSER_TOKEN_OPERATOR_MODULO,
  PARSER_TOKEN_OPERATOR_EQUAL,
  PARSER_TOKEN_OPERATOR_NOT_EQUAL,
  PARSER_TOKEN_OPERATOR_GREATER,
  PARSER_TOKEN_OPERATOR_SMALLER,
  PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL,
  PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL,
  PARSER_TOKEN_OPERATOR_POINTER,
  PARSER_TOKEN_OPERATOR_ADDRESS,
  PARSER_TOKEN_OPERATOR_DEREFERENCE,
  PARSER_TOKEN_OPERATOR_ACCESS,
  PARSER_TOKEN_OPERATOR_LOGICAL_NOT,
  PARSER_TOKEN_OPERATOR_LOGICAL_AND,
  PARSER_TOKEN_OPERATOR_LOGICAL_OR,

  PARSER_TOKEN_FUNCTION_DEFINITION,

  PARSER_TOKEN_FUNCTION_CALL,

  PARSER_TOKEN_NONE,
} ParserToken;

extern const char *PARSER_TOKEN_STRINGS[];

typedef struct ParserOrder {
  bool ltr;
  LexerToken begin;
  LexerToken end;
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

typedef ParserNode ParserNodeSingleChildMetadata;

typedef u64 ParserNodeIntMetadata;

typedef f128 ParserNodeFloatMetadata;

typedef ParserNodeIntMetadata ParserNodeCharMetadata;

typedef struct ParserNodeInfixMetadata {
  ParserNode *left;
  ParserNode *right;
} ParserNodeInfixMetadata;

typedef struct ParserNodeReturnMetadata {
  ParserNode *value;
} ParserNodeReturnMetadata;

typedef bool ParserNodeBoolMetadata;

typedef struct ParserNodeIfMetadata {
  ParserNode *condition;
  ParserNode *ifBody;
  ParserNode *elseBody;
} ParserNodeIfMetadata;

typedef struct ParserNodeWhileMetadata {
  ParserNode *condition;
  ParserNode *body;
} ParserNodeWhileMetadata;

void parserNodePrint(const ParserNode *node, int indent);
void parserNodeDelete(ParserNode *node);

ParserNode *parser(LexerNodeArray lexed);
bool parserNodeArray(LexerNode *begin, LexerNode *end, ParserNode *parent);

ParserNode *newParserNode(ParserToken token, char *str_begin, char *str_end,
                          void *metadata, ParserNode *parent);

ParserNode *parseNode(LexerNode *node, LexerNode *begin, LexerNode *end,
                      ParserNode *parent, bool *conti);

ParserNode *getUntilCommonParent(ParserNode *node, ParserNode *parent);
ParserNode *getUntilCommonParents(ParserNode *node, ParserNode *parent,
                                  ParserNode *parent2);
ParserNode *getNextUsingCommonParent(LexerNode *node, LexerNode *end,
                                     ParserNode *parent);
LexerNode *getNextLexerNodeUsingCommonParent(LexerNode *node, LexerNode *end,
                                             ParserNode *parent);

ParserNode *parserNoMetadata(LexerNode *node, ParserNode *parent,
                             ParserToken token);
ParserNode *parserPutc(LexerNode *node, LexerNode *end, ParserNode *parent);
ParserNode *parserReturn(LexerNode *node, LexerNode *end, ParserNode *parent);
ParserNode *parserNumber(LexerNode *node, ParserNode *parent);
ParserNode *parserChar(LexerNode *node, ParserNode *parent);
ParserNode *parserBoolValue(LexerNode *node, ParserNode *parent);
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
ParserNode *parserBinaryOperator(LexerNode *node, LexerNode *begin,
                                 LexerNode *end, ParserNode *parent,
                                 ParserToken token);
ParserNode *parserBinaryOrLeftOperator(LexerNode *node, LexerNode *begin,
                                       LexerNode *end, ParserNode *parent,
                                       ParserToken token,
                                       LexerToken laterToken);
ParserNode *parserLeftOperator(LexerNode *node, LexerNode *end,
                               ParserNode *parent, ParserToken token);
ParserNode *parserRightOperator(LexerNode *node, LexerNode *begin,
                                ParserNode *parent, ParserToken token);
ParserNode *parserIf(LexerNode *node, LexerNode *end, ParserNode *parent);
ParserNode *parserWhile(LexerNode *node, LexerNode *end, ParserNode *parent);
ParserNode *parserComptime(LexerNode *node, LexerNode *end, ParserNode *parent);
ParserNode *parserStruct(LexerNode *node, LexerNode *end, ParserNode *parent);

bool isAllArguments(const ParserNodeArray *nodes);

bool isExpression(ParserNode *node);
bool isType(ParserNode *node);
bool isValue(ParserNode *node);
