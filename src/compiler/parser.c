#include "parser.h"

#include "compiler/lexer.h"
#include "utils/file.h"
#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const char *PARSER_TOKEN_STRINGS[] = {
    "PARSER_TOKEN_ROOT",

    "PARSER_TOKEN_IDENTIFIER",
    "PARSER_TOKEN_BUILTIN",

    "PARSER_TOKEN_VALUE_INT",
    "PARSER_TOKEN_VALUE_FLOAT",
    "PARSER_TOKEN_VALUE_BOOL",
    "PARSER_TOKEN_VALUE_CHAR",
    "PARSER_TOKEN_VALUE_STRING",

    "PARSER_TOKEN_TYPE_TYPE",
    "PARSER_TOKEN_TYPE_FUNCTION",
    "PARSER_TOKEN_TYPE_VOID",
    "PARSER_TOKEN_TYPE_BOOL",
    "PARSER_TOKEN_TYPE_I8",
    "PARSER_TOKEN_TYPE_U8",
    "PARSER_TOKEN_TYPE_I16",
    "PARSER_TOKEN_TYPE_U16",
    "PARSER_TOKEN_TYPE_I32",
    "PARSER_TOKEN_TYPE_U32",
    "PARSER_TOKEN_TYPE_I64",
    "PARSER_TOKEN_TYPE_U64",
#ifdef FLOAT_16_SUPPORT
    "PARSER_TOKEN_TYPE_F16",
#endif
    "PARSER_TOKEN_TYPE_F32",
    "PARSER_TOKEN_TYPE_F64",
    "PARSER_TOKEN_TYPE_F128",
    "PARSER_TOKEN_TYPE_CODE",

    "PARSER_TOKEN_KEYWORD_PUTC",
    "PARSER_TOKEN_KEYWORD_RETURN",
    "PARSER_TOKEN_KEYWORD_IF",
    "PARSER_TOKEN_KEYWORD_WHILE",
    "PARSER_TOKEN_KEYWORD_COMPTIME",
    "PARSER_TOKEN_KEYWORD_NULL",
    "PARSER_TOKEN_KEYWORD_STRUCT",
    "PARSER_TOKEN_KEYWORD_UNDEFINED",

    "PARSER_TOKEN_CONSTANT",
    "PARSER_TOKEN_VARIABLE",

    "PARSER_TOKEN_SYMBOL_EOL",
    "PARSER_TOKEN_SYMBOL_CURLY_BRACKET",
    "PARSER_TOKEN_SYMBOL_BRACKET_LEFT",
    "PARSER_TOKEN_SYMBOL_BRACKET_RIGHT",
    "PARSER_TOKEN_SYMBOL_PARENTHESIS",
    "PARSER_TOKEN_SYMBOL_COMMA",

    "PARSER_TOKEN_OPERATOR_ASSIGN",
    "PARSER_TOKEN_OPERATOR_SUM_ASSIGN",
    "PARSER_TOKEN_OPERATOR_SUB_ASSIGN",
    "PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN",
    "PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN",
    "PARSER_TOKEN_OPERATOR_MODULO_ASSIGN",
    "PARSER_TOKEN_OPERATOR_PLUS",
    "PARSER_TOKEN_OPERATOR_MINUS",
    "PARSER_TOKEN_OPERATOR_SUM",
    "PARSER_TOKEN_OPERATOR_SUB",
    "PARSER_TOKEN_OPERATOR_MULTIPLY",
    "PARSER_TOKEN_OPERATOR_DIVIDE",
    "PARSER_TOKEN_OPERATOR_MODULO",
    "PARSER_TOKEN_OPERATOR_EQUAL",
    "PARSER_TOKEN_OPERATOR_NOT_EQUAL",
    "PARSER_TOKEN_OPERATOR_GREATER",
    "PARSER_TOKEN_OPERATOR_SMALLER",
    "PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL",
    "PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL",
    "PARSER_TOKEN_OPERATOR_POINTER",
    "PARSER_TOKEN_OPERATOR_ADDRESS",
    "PARSER_TOKEN_OPERATOR_DEREFERENCE",
    "PARSER_TOKEN_OPERATOR_ACCESS",
    "PARSER_TOKEN_OPERATOR_LOGICAL_NOT",
    "PARSER_TOKEN_OPERATOR_LOGICAL_AND",
    "PARSER_TOKEN_OPERATOR_LOGICAL_OR",

    "PARSER_TOKEN_FUNCTION_DEFINITION",

    "PARSER_TOKEN_FUNCTION_CALL",

    "PARSER_TOKEN_NONE",
};

static const ParserOrder PARSER_ORDER[] = {
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER0,
        .end = LEXER_TOKEN_ORDER1,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER1,
        .end = LEXER_TOKEN_ORDER2,
    },
    {
        .ltr = false,
        .begin = LEXER_TOKEN_ORDER2,
        .end = LEXER_TOKEN_ORDER3,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER3,
        .end = LEXER_TOKEN_ORDER4,
    },
    {
        .ltr = false,
        .begin = LEXER_TOKEN_ORDER4,
        .end = LEXER_TOKEN_ORDER5,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER5,
        .end = LEXER_TOKEN_ORDER6,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER6,
        .end = LEXER_TOKEN_ORDER7,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER7,
        .end = LEXER_TOKEN_ORDER8,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER8,
        .end = LEXER_TOKEN_ORDER9,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER9,
        .end = LEXER_TOKEN_ORDER10,
    },
    {
        .ltr = false,
        .begin = LEXER_TOKEN_ORDER10,
        .end = LEXER_TOKEN_ORDER11,
    },
    {
        .ltr = false,
        .begin = LEXER_TOKEN_ORDER11,
        .end = LEXER_TOKEN_ORDER12,
    },
    {
        .ltr = true,
        .begin = LEXER_TOKEN_ORDER12,
        .end = LEXER_TOKEN_ORDER13,
    },
    {
        .ltr = false,
        .begin = LEXER_TOKEN_ORDER13,
        .end = LEXER_TOKEN_END_ORDERS,
    },
};

static const size_t PARSER_ORDER_SIZE =
    sizeof(PARSER_ORDER) / sizeof(*PARSER_ORDER);

#ifdef PRINT_COMPILE_TREE
void parserNodePrint(const ParserNode *node, int indent) {
  for (int i = 0; i < indent; ++i)
    printf(" ");

  if (node == NULL) {
    printf("null");
    return;
  }

  printf("{token=\"%s\"", PARSER_TOKEN_STRINGS[node->token]);

  switch (node->token) {
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS: {
    const ParserNodeArray *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("children=[\n");
    for (size_t i = 0; i < metadata->size; ++i) {
      parserNodePrint(metadata->data[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
  case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT: {
    const ParserNodeBracketMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("operand=\n");
    parserNodePrint(metadata->operand, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("params=[\n");
    for (size_t i = 0; i < metadata->params->size; ++i) {
      parserNodePrint(metadata->params->data[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_BUILTIN:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_BOOL:
  case PARSER_TOKEN_TYPE_I8:
  case PARSER_TOKEN_TYPE_U8:
  case PARSER_TOKEN_TYPE_I16:
  case PARSER_TOKEN_TYPE_U16:
  case PARSER_TOKEN_TYPE_I32:
  case PARSER_TOKEN_TYPE_U32:
  case PARSER_TOKEN_TYPE_I64:
  case PARSER_TOKEN_TYPE_U64:
#ifdef FLOAT_16_SUPPORT
  case PARSER_TOKEN_TYPE_F16:
#endif
  case PARSER_TOKEN_TYPE_F32:
  case PARSER_TOKEN_TYPE_F64:
  case PARSER_TOKEN_TYPE_F128:
  case PARSER_TOKEN_TYPE_CODE:
  case PARSER_TOKEN_KEYWORD_NULL:
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_INT: {
    ParserNodeIntMetadata *metadata = node->metadata;
    printf(",operand=%ld", *metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_FLOAT: {
    ParserNodeFloatMetadata *metadata = node->metadata;
    printf(",operand=%Lf", *metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_BOOL: {
    ParserNodeBoolMetadata *metadata = node->metadata;
    printf(",value=%b", *metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_CHAR: {
    ParserNodeCharMetadata *metadata = node->metadata;
    printf(",value=%c", (char)*metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_STRING: {
    ParserNodeStringMetadata *metadata = node->metadata;
    printf(",value=%.*s", (int)(metadata->end - metadata->begin),
           metadata->begin);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE: {
    const ParserNodeVariableMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("name=\n");
    parserNodePrint(metadata->name, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("type=\n");
    parserNodePrint(metadata->type, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("value=\n");
    parserNodePrint(metadata->value, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
  case PARSER_TOKEN_KEYWORD_STRUCT:
  case PARSER_TOKEN_OPERATOR_POINTER:
  case PARSER_TOKEN_OPERATOR_ADDRESS:
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
  case PARSER_TOKEN_OPERATOR_PLUS:
  case PARSER_TOKEN_OPERATOR_MINUS:
  case PARSER_TOKEN_KEYWORD_PUTC:
  case PARSER_TOKEN_KEYWORD_COMPTIME:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_SYMBOL_EOL: {
    const ParserNodeSingleChildMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("child=\n");
    parserNodePrint(metadata, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_KEYWORD_RETURN: {
    const ParserNodeReturnMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("value=\n");
    parserNodePrint(metadata->value, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_FUNCTION_DEFINITION: {
    const ParserNodeFunctionDefnitionMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("arguments=\n");
    parserNodePrint(metadata->arguments, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("returnType=\n");
    parserNodePrint(metadata->returnType, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("body=\n");
    parserNodePrint(metadata->body, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_TYPE_FUNCTION: {
    const ParserNodeTypeFunctionMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("arguments=\n");
    parserNodePrint(metadata->arguments, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("returnType=\n");
    parserNodePrint(metadata->returnType, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_FUNCTION_CALL: {
    const ParserNodeFunctionCall *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("function=\n");
    parserNodePrint(metadata->function, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("params=[\n");
    for (size_t i = 0; i < metadata->params->size; ++i) {
      parserNodePrint(metadata->params->data[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
  case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
  case PARSER_TOKEN_OPERATOR_ACCESS:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
  case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM:
  case PARSER_TOKEN_OPERATOR_SUB:
  case PARSER_TOKEN_OPERATOR_MULTIPLY:
  case PARSER_TOKEN_OPERATOR_DIVIDE:
  case PARSER_TOKEN_OPERATOR_MODULO:
  case PARSER_TOKEN_OPERATOR_EQUAL:
  case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
  case PARSER_TOKEN_OPERATOR_GREATER:
  case PARSER_TOKEN_OPERATOR_SMALLER:
  case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    const ParserNodeInfixMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("left=\n");
    parserNodePrint(metadata->left, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("right=\n");
    parserNodePrint(metadata->right, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_KEYWORD_IF: {
    ParserNodeIfMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("condition=\n");
    parserNodePrint(metadata->condition, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("ifBody=\n");
    parserNodePrint(metadata->ifBody, indent + 1);
    printf("\n,");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("elseBody=\n");
    parserNodePrint(metadata->elseBody, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_KEYWORD_WHILE: {
    ParserNodeIfMetadata *metadata = node->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("condition=\n");
    parserNodePrint(metadata->condition, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("ifBody=\n");
    parserNodePrint(metadata->ifBody, indent + 1);
    printf("\n,");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("elseBody=\n");
    parserNodePrint(metadata->elseBody, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_NONE:
  }
  UNREACHABLE;

RETURN_SUCCESS:
  printf("}");
}
#endif

void parserNodeDelete(ParserNode *node) {
  if (node == NULL) {
    return;
  }
  switch (node->token) {
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS: {
    ParserNodeArray *metadata = node->metadata;
    for (size_t i = 0; i < metadata->size; ++i) {
      parserNodeDelete(metadata->data[i]);
    }
    free(metadata->data);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
  case PARSER_TOKEN_SYMBOL_BRACKET_LEFT: {
    ParserNodeBracketMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->operand);
    for (size_t i = 0; i < metadata->params->size; ++i) {
      parserNodeDelete(metadata->params->data[i]);
    }
    free(metadata->params->data);
    free(metadata->params);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_BUILTIN:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_BOOL:
  case PARSER_TOKEN_TYPE_I8:
  case PARSER_TOKEN_TYPE_U8:
  case PARSER_TOKEN_TYPE_I16:
  case PARSER_TOKEN_TYPE_U16:
  case PARSER_TOKEN_TYPE_I32:
  case PARSER_TOKEN_TYPE_U32:
  case PARSER_TOKEN_TYPE_I64:
  case PARSER_TOKEN_TYPE_U64:
#ifdef FLOAT_16_SUPPORT
  case PARSER_TOKEN_TYPE_F16:
#endif
  case PARSER_TOKEN_TYPE_F32:
  case PARSER_TOKEN_TYPE_F64:
  case PARSER_TOKEN_TYPE_F128:
  case PARSER_TOKEN_TYPE_CODE:
  case PARSER_TOKEN_KEYWORD_NULL:
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_BOOL: {
    ParserNodeBoolMetadata *metadata = node->metadata;
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_INT: {
    ParserNodeIntMetadata *metadata = node->metadata;
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_FLOAT: {
    ParserNodeFloatMetadata *metadata = node->metadata;
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_CHAR: {
    ParserNodeCharMetadata *metadata = node->metadata;
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_STRING: {
    ParserNodeStringMetadata *metadata = node->metadata;
    free(metadata->begin);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE: {
    ParserNodeVariableMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->name);
    parserNodeDelete(metadata->type);
    parserNodeDelete(metadata->value);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
  case PARSER_TOKEN_KEYWORD_STRUCT:
  case PARSER_TOKEN_OPERATOR_POINTER:
  case PARSER_TOKEN_OPERATOR_ADDRESS:
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
  case PARSER_TOKEN_OPERATOR_PLUS:
  case PARSER_TOKEN_OPERATOR_MINUS:
  case PARSER_TOKEN_KEYWORD_PUTC:
  case PARSER_TOKEN_KEYWORD_COMPTIME:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_SYMBOL_EOL: {
    ParserNodeSingleChildMetadata *metadata = node->metadata;
    parserNodeDelete(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_KEYWORD_RETURN: {
    ParserNodeReturnMetadata *metadata = node->metadata;
    if (metadata->value != NULL) {
      parserNodeDelete(metadata->value);
    }
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_FUNCTION_DEFINITION: {
    ParserNodeFunctionDefnitionMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->arguments);
    parserNodeDelete(metadata->returnType);
    parserNodeDelete(metadata->body);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_TYPE_FUNCTION: {
    ParserNodeTypeFunctionMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->arguments);
    parserNodeDelete(metadata->returnType);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_FUNCTION_CALL: {
    ParserNodeFunctionCall *metadata = node->metadata;
    parserNodeDelete(metadata->function);
    for (size_t i = 0; i < metadata->params->size; ++i) {
      parserNodeDelete(metadata->params->data[i]);
    }
    free(metadata->params->data);
    free(metadata->params);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
  case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
  case PARSER_TOKEN_OPERATOR_ACCESS:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
  case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM:
  case PARSER_TOKEN_OPERATOR_SUB:
  case PARSER_TOKEN_OPERATOR_MULTIPLY:
  case PARSER_TOKEN_OPERATOR_DIVIDE:
  case PARSER_TOKEN_OPERATOR_MODULO:
  case PARSER_TOKEN_OPERATOR_EQUAL:
  case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
  case PARSER_TOKEN_OPERATOR_GREATER:
  case PARSER_TOKEN_OPERATOR_SMALLER:
  case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    ParserNodeInfixMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->left);
    parserNodeDelete(metadata->right);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_KEYWORD_IF: {
    ParserNodeIfMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->condition);
    parserNodeDelete(metadata->ifBody);
    parserNodeDelete(metadata->elseBody);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_KEYWORD_WHILE: {
    ParserNodeWhileMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->condition);
    parserNodeDelete(metadata->body);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_NONE:
  }
  UNREACHABLE;

RETURN_SUCCESS:
  free(node);
}

ParserNode *newParserNode(ParserToken token, char *str_begin, char *str_end,
                          void *metadata, ParserNode *parent) {
  ParserNode *parserNode = a404m_malloc(sizeof(*parserNode));
  parserNode->token = token;
  parserNode->str_begin = str_begin;
  parserNode->str_end = str_end;
  parserNode->metadata = metadata;
  parserNode->parent = parent;
  return parserNode;
}

ParserNode *parserFromPath(const char *filePath) {
  char *code = readWholeFile(filePath);
  if (code == NULL) {
    return NULL;
  }

  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    return NULL;
  }

  ParserNode *root = parser(lexed);
  lexerNodeArrayDestroy(lexed);

  return root;
}

ParserNode *parser(LexerNodeArray lexed) {
  ParserNode *root = newParserNode(PARSER_TOKEN_ROOT, NULL, NULL, NULL, NULL);
  if (parserNodeArray(lexed.data, lexed.data + lexed.size, root)) {
    return root;
  } else {
    free(root);
    return NULL;
  }
}

bool parserNodeArray(LexerNode *begin, LexerNode *end, ParserNode *parent) {
  size_t parsedNodes_size = 0;

  ParserNodeArray *parsedNodes = a404m_malloc(sizeof(*parsedNodes));
  parsedNodes->data =
      a404m_malloc(parsedNodes_size * sizeof(*parsedNodes->data));
  parsedNodes->size = 0;

  for (size_t orders_index = 0; orders_index < PARSER_ORDER_SIZE;
       ++orders_index) {
    const ParserOrder orders = PARSER_ORDER[orders_index];

    for (LexerNode *iter = orders.ltr ? begin : end - 1;
         iter < end && iter >= begin; orders.ltr ? ++iter : --iter) {
      if (iter->parserNode == NULL && orders.begin <= iter->token &&
          iter->token < orders.end) {
        bool conti = false;
        ParserNode *parserNode = parseNode(iter, begin, end, parent, &conti);
        if (parserNode == NULL && !conti) {
          goto RETURN_ERROR;
        }
      }
    }
  }

  for (LexerNode *iter = begin; iter < end; ++iter) {
    ParserNode *pNode = iter->parserNode;
    if (pNode == NULL) {
      printError(iter->str_begin, iter->str_end, "Bad child");
      goto RETURN_ERROR;
    } else if (pNode->parent != parent) {
      continue;
    }

    for (size_t i = 0; i < parsedNodes->size; ++i) {
      if (parsedNodes->data[i] == pNode) {
        goto CONTINUE;
      }
    }
    if (parsedNodes->size == parsedNodes_size) {
      parsedNodes_size += parsedNodes_size / 2 + 1;
      parsedNodes->data = a404m_realloc(
          parsedNodes->data, sizeof(*parsedNodes->data) * parsedNodes_size);
    }
    parsedNodes->data[parsedNodes->size] = pNode;
    parsedNodes->size += 1;
  CONTINUE:
  }

  parsedNodes->data = a404m_realloc(
      parsedNodes->data, parsedNodes->size * sizeof(*parsedNodes->data));

  parent->metadata = parsedNodes;

  return true;

RETURN_ERROR:
  for (size_t i = 0; i < parsedNodes->size; ++i) {
    free(parsedNodes->data[i]);
  }
  free(parsedNodes->data);
  free(parsedNodes);
  return false;
}

ParserNode *parseNode(LexerNode *node, LexerNode *begin, LexerNode *end,
                      ParserNode *parent, bool *conti) {
  switch (node->token) {
  case LEXER_TOKEN_IDENTIFIER:
    return parserNoMetadata(node, parent, PARSER_TOKEN_IDENTIFIER);
  case LEXER_TOKEN_BUILTIN:
    return parserNoMetadata(node, parent, PARSER_TOKEN_BUILTIN);
  case LEXER_TOKEN_KEYWORD_TYPE:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_TYPE);
  case LEXER_TOKEN_KEYWORD_VOID:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_VOID);
  case LEXER_TOKEN_KEYWORD_I8:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_I8);
  case LEXER_TOKEN_KEYWORD_U8:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_U8);
  case LEXER_TOKEN_KEYWORD_I16:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_I16);
  case LEXER_TOKEN_KEYWORD_U16:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_U16);
  case LEXER_TOKEN_KEYWORD_I32:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_I32);
  case LEXER_TOKEN_KEYWORD_U32:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_U32);
  case LEXER_TOKEN_KEYWORD_I64:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_I64);
  case LEXER_TOKEN_KEYWORD_U64:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_U64);
  case LEXER_TOKEN_KEYWORD_BOOL:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_BOOL);
#ifdef FLOAT_16_SUPPORT
  case LEXER_TOKEN_KEYWORD_F16:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_F16);
#endif
  case LEXER_TOKEN_KEYWORD_F32:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_F32);
  case LEXER_TOKEN_KEYWORD_F64:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_F64);
  case LEXER_TOKEN_KEYWORD_F128:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_F128);
  case LEXER_TOKEN_KEYWORD_CODE:
    return parserNoMetadata(node, parent, PARSER_TOKEN_TYPE_CODE);
  case LEXER_TOKEN_KEYWORD_NULL:
    return parserNoMetadata(node, parent, PARSER_TOKEN_KEYWORD_NULL);
  case LEXER_TOKEN_KEYWORD_UNDEFINED:
    return parserNoMetadata(node, parent, PARSER_TOKEN_KEYWORD_UNDEFINED);
  case LEXER_TOKEN_KEYWORD_PUTC:
    return parserPutc(node, end, parent);
  case LEXER_TOKEN_KEYWORD_RETURN:
    return parserReturn(node, end, parent);
  case LEXER_TOKEN_KEYWORD_TRUE:
  case LEXER_TOKEN_KEYWORD_FALSE:
    return parserBoolValue(node, parent);
  case LEXER_TOKEN_SYMBOL_EOL:
    return parserEol(node, begin, parent);
  case LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS:
    return parserParenthesis(node, begin, parent);
  case LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET:
    return parserCurlyBrackets(node, begin, parent);
  case LEXER_TOKEN_SYMBOL_CLOSE_BRACKET:
    return parserBracketsRight(node, begin, parent, conti);
  case LEXER_TOKEN_SYMBOL_CLOSE_BRACKET_LEFT:
    return parserBracketsLeft(node, begin, end, parent);
  case LEXER_TOKEN_SYMBOL_FUNCTION_ARROW:
    return parserFunction(node, begin, end, parent);
  case LEXER_TOKEN_SYMBOL_COLON:
    return parserVariable(node, begin, end, parent);
  case LEXER_TOKEN_SYMBOL_COMMA:
    return parserComma(node, begin, parent);
  case LEXER_TOKEN_NUMBER:
    return parserNumber(node, parent);
  case LEXER_TOKEN_CHAR:
    return parserChar(node, parent);
  case LEXER_TOKEN_STRING:
    return parserString(node, parent);
  case LEXER_TOKEN_SYMBOL_ASSIGN:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_ASSIGN);
  case LEXER_TOKEN_SYMBOL_SUM_ASSIGN:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_SUM_ASSIGN);
  case LEXER_TOKEN_SYMBOL_SUB_ASSIGN:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_SUB_ASSIGN);
  case LEXER_TOKEN_SYMBOL_MULTIPLY_ASSIGN:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN);
  case LEXER_TOKEN_SYMBOL_DIVIDE_ASSIGN:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN);
  case LEXER_TOKEN_SYMBOL_MODULO_ASSIGN:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_MODULO_ASSIGN);
  case LEXER_TOKEN_SYMBOL_SUM:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_SUM);
  case LEXER_TOKEN_SYMBOL_SUB:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_SUB);
  case LEXER_TOKEN_SYMBOL_MULTIPLY:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_MULTIPLY);
  case LEXER_TOKEN_SYMBOL_DIVIDE:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_DIVIDE);
  case LEXER_TOKEN_SYMBOL_MODULO:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_MODULO);
  case LEXER_TOKEN_SYMBOL_EQUAL:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_EQUAL);
  case LEXER_TOKEN_SYMBOL_NOT_EQUAL:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_NOT_EQUAL);
  case LEXER_TOKEN_SYMBOL_GREATER:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_GREATER);
  case LEXER_TOKEN_SYMBOL_SMALLER:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_SMALLER);
  case LEXER_TOKEN_SYMBOL_GREATER_OR_EQUAL:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL);
  case LEXER_TOKEN_SYMBOL_SMALLER_OR_EQUAL:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL);
  case LEXER_TOKEN_SYMBOL_ACCESS:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_ACCESS);
  case LEXER_TOKEN_SYMBOL_LOGICAL_AND:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_LOGICAL_AND);
  case LEXER_TOKEN_SYMBOL_LOGICAL_OR:
    return parserBinaryOperator(node, begin, end, parent,
                                PARSER_TOKEN_OPERATOR_LOGICAL_OR);
  case LEXER_TOKEN_SYMBOL_LOGICAL_NOT:
    return parserLeftOperator(node, end, parent,
                              PARSER_TOKEN_OPERATOR_LOGICAL_NOT);
  case LEXER_TOKEN_SYMBOL_PLUS: {
    ParserNode *result = parserBinaryOrLeftOperator(node, begin, end, parent,
                                                    PARSER_TOKEN_OPERATOR_PLUS,
                                                    LEXER_TOKEN_SYMBOL_SUM);
    *conti = result == NULL;
    return result;
  }
  case LEXER_TOKEN_SYMBOL_MINUS: {
    ParserNode *result = parserBinaryOrLeftOperator(node, begin, end, parent,
                                                    PARSER_TOKEN_OPERATOR_MINUS,
                                                    LEXER_TOKEN_SYMBOL_SUB);
    *conti = result == NULL;
    return result;
  }
  case LEXER_TOKEN_SYMBOL_POINTER: {
    ParserNode *result = parserBinaryOrLeftOperator(
        node, begin, end, parent, PARSER_TOKEN_OPERATOR_POINTER,
        LEXER_TOKEN_SYMBOL_MULTIPLY);
    *conti = result == NULL;
    return result;
  }
  case LEXER_TOKEN_SYMBOL_ADDRESS: {
    ParserNode *result =
        parserLeftOperator(node, end, parent, PARSER_TOKEN_OPERATOR_ADDRESS);
    *conti = result == NULL;
    return result;
  }
  case LEXER_TOKEN_SYMBOL_DEREFERENCE: {
    ParserNode *result = parserRightOperator(node, begin, parent,
                                             PARSER_TOKEN_OPERATOR_DEREFERENCE);
    *conti = result == NULL;
    return result;
  }
  case LEXER_TOKEN_KEYWORD_IF:
    return parserIf(node, end, parent);
  case LEXER_TOKEN_KEYWORD_WHILE:
    return parserWhile(node, end, parent);
  case LEXER_TOKEN_KEYWORD_COMPTIME:
    return parserComptime(node, end, parent);
  case LEXER_TOKEN_KEYWORD_STRUCT:
    return parserStruct(node, end, parent);
  case LEXER_TOKEN_KEYWORD_ELSE:
  case LEXER_TOKEN_SYMBOL:
  case LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS:
  case LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET:
  case LEXER_TOKEN_SYMBOL_OPEN_BRACKET:
  case LEXER_TOKEN_NONE:
    break;
  }
  UNREACHABLE;
}

ParserNode *getUntilCommonParent(ParserNode *node, ParserNode *parent) {
  while (node != NULL && node != parent && node->parent != parent) {
    if (node == node->parent) {
      return NULL;
    }

    node = node->parent;
  }
  return node;
}

ParserNode *getUntilCommonParents(ParserNode *node, ParserNode *parent,
                                  ParserNode *parent2) {
  while (node != NULL && node != parent && node->parent != parent &&
         node->parent != parent2) {
    if (node == node->parent) {
      return NULL;
    }

    node = node->parent;
  }
  return node;
}

ParserNode *getNextUsingCommonParent(LexerNode *node, LexerNode *end,
                                     ParserNode *parent) {
  ParserNode *parsed = getUntilCommonParent(node->parserNode, parent);
  if (parsed == NULL) {
    return NULL;
  }

  while (true) {
    node += 1;
    if (node >= end) {
      return NULL;
    }

    ParserNode *newParsed = getUntilCommonParent(node->parserNode, parsed);

    if (newParsed == NULL) {
      return getUntilCommonParent(node->parserNode, parent);
    }
  }

  return NULL;
}

LexerNode *getNextLexerNodeUsingCommonParent(LexerNode *node, LexerNode *end,
                                             ParserNode *parent) {
  ParserNode *parsed = getUntilCommonParent(node->parserNode, parent);
  if (parsed == NULL) {
    return NULL;
  }

  while (true) {
    node += 1;
    if (node >= end) {
      return NULL;
    }

    ParserNode *newParsed = getUntilCommonParent(node->parserNode, parsed);

    if (newParsed == NULL) {
      return node;
    }
  }

  return NULL;
}

ParserNode *parserNoMetadata(LexerNode *node, ParserNode *parent,
                             ParserToken token) {
  return node->parserNode =
             newParserNode(token, node->str_begin, node->str_end, NULL, parent);
}

ParserNode *parserPutc(LexerNode *node, LexerNode *end, ParserNode *parent) {
  LexerNode *afterNode = node + 1;
  if (afterNode >= end) {
    printError(node->str_begin, node->str_end, "No param");
    return NULL;
  } else if (afterNode->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "Bad param");
    return NULL;
  }

  ParserNode *operand = getUntilCommonParent(afterNode->parserNode, parent);
  if (operand == NULL) {
    printError(node->str_begin, node->str_end, "Bad param");
    return NULL;
  }

  return operand->parent = node->parserNode = newParserNode(
             PARSER_TOKEN_KEYWORD_PUTC, node->str_begin, node->str_end,
             (ParserNodeSingleChildMetadata *)operand, parent);
}

ParserNode *parserReturn(LexerNode *node, LexerNode *end, ParserNode *parent) {
  LexerNode *afterNode = node + 1;
  ParserNode *operand;
  if (afterNode >= end || afterNode->parserNode == NULL) {
    operand = NULL;
  } else {
    operand = getUntilCommonParent(afterNode->parserNode, parent);
    if (operand == NULL) {
      printError(node->str_begin, node->str_end, "No param");
      return NULL;
    }
  }

  ParserNodeReturnMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->value = operand;

  node->parserNode = newParserNode(PARSER_TOKEN_KEYWORD_RETURN, node->str_begin,
                                   node->str_end, metadata, parent);
  if (operand != NULL) {
    operand->parent = node->parserNode;
  }

  return node->parserNode;
}

ParserNode *parserNumber(LexerNode *node, ParserNode *parent) {
  ParserNode *parserNode;
  switch (*node->str_begin) {
  case '0':
    if (node->str_end - node->str_begin > 1) {
      printError(node->str_begin, node->str_end, "Not implemented");
      return NULL;
    }
    // fall through
  default: {
    bool success;
    u64 value = decimalToU64(node->str_begin, node->str_end, &success);
    if (success) {
      ParserNodeIntMetadata *metadata = a404m_malloc(sizeof(*metadata));
      *metadata = value;
      parserNode = newParserNode(PARSER_TOKEN_VALUE_INT, node->str_begin,
                                 node->str_end, metadata, parent);
    } else {
      ParserNodeFloatMetadata *metadata = a404m_malloc(sizeof(*metadata));
      *metadata = numberToFloat(node->str_begin, node->str_end, &success);

      if (success) {
        parserNode = newParserNode(PARSER_TOKEN_VALUE_FLOAT, node->str_begin,
                                   node->str_end, metadata, parent);
      } else {
        printError(node->str_begin, node->str_end, "Error in parsing number");
        return NULL;
      }
    }
  }
  }
  return node->parserNode = parserNode;
}

ParserNode *parserChar(LexerNode *node, ParserNode *parent) {
  const size_t size = node->str_end - 1 - node->str_begin - 1;
  ParserNodeCharMetadata *metadata = a404m_malloc(sizeof(*metadata));
  char c = node->str_begin[1];
  if (size == 0) {
    printError(node->str_begin, node->str_end,
               "Bad character: Character can't be empty");
    goto RETURN_ERROR;
  } else if (size == 1) {
    // knowingly left empty
  } else if (size == 2 && c == '\\') {
    bool success;
    c = escapeChar(node->str_begin + 2, node->str_end, &success);
    if (!success) {
      printError(node->str_begin, node->str_end, "Bad escape character");
      goto RETURN_ERROR;
    }
  } else {
    printError(node->str_begin, node->str_end, "Bad character");
    goto RETURN_ERROR;
  }
  *metadata = c;
  return node->parserNode =
             newParserNode(PARSER_TOKEN_VALUE_CHAR, node->str_begin,
                           node->str_end, metadata, parent);
RETURN_ERROR:
  free(metadata);
  return NULL;
}

ParserNode *parserString(LexerNode *node, ParserNode *parent) {
  ParserNodeStringMetadata *metadata = a404m_malloc(sizeof(*metadata));

  metadata->begin = metadata->end = a404m_malloc(
      sizeof(*metadata->begin) * (node->str_end - node->str_begin));

  for (char *iter = node->str_begin + 1; iter < node->str_end - 1; ++iter) {
    if (*iter == '\\') {
      iter += 1;
      bool success;
      *metadata->end = escapeChar(iter, node->str_end, &success);
      if (!success) {
        printError(node->str_begin, node->str_end, "Bad escape character");
        goto RETURN_ERROR;
      }
    } else {
      *metadata->end = *iter;
    }
    metadata->end += 1;
  }

  size_t size = metadata->end - metadata->begin;
  metadata->begin = a404m_realloc(metadata->begin, size * (*metadata->begin));
  metadata->end = metadata->begin + size;

  return node->parserNode =
             newParserNode(PARSER_TOKEN_VALUE_STRING, node->str_begin,
                           node->str_end, metadata, parent);
RETURN_ERROR:
  free(metadata->begin);
  free(metadata);
  return NULL;
}

ParserNode *parserBoolValue(LexerNode *node, ParserNode *parent) {
  ParserNodeBoolMetadata *metadata = a404m_malloc(sizeof(*metadata));
  *metadata = node->token == LEXER_TOKEN_KEYWORD_TRUE;
  return node->parserNode =
             newParserNode(PARSER_TOKEN_VALUE_BOOL, node->str_begin,
                           node->str_end, metadata, parent);
}

ParserNode *parserEol(LexerNode *node, LexerNode *begin, ParserNode *parent) {
  LexerNode *nodeBefore = node - 1;
  ParserNode *parserNodeBefore;
  if (nodeBefore < begin) {
    parserNodeBefore = NULL;
  } else if (nodeBefore->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "Bad EOL after %s",
               LEXER_TOKEN_STRINGS[nodeBefore->token]);
    return NULL;
  } else {
    parserNodeBefore = getUntilCommonParent(nodeBefore->parserNode, parent);
    if (parserNodeBefore == NULL || !isExpression(parserNodeBefore)) {
      printError(node->str_begin, node->str_end, "Bad EOL after %s",
                 LEXER_TOKEN_STRINGS[nodeBefore->token]);
      return NULL;
    }
  }
  ParserNode *parserNode =
      newParserNode(PARSER_TOKEN_SYMBOL_EOL, node->str_begin, node->str_end,
                    (ParserNodeSingleChildMetadata *)parserNodeBefore, parent);
  node->parserNode = parserNode;
  if (parserNodeBefore != NULL) {
    parserNodeBefore->parent = parserNode;
  }
  return parserNode;
}

ParserNode *parserComma(LexerNode *node, LexerNode *begin, ParserNode *parent) {
  LexerNode *nodeBefore = node - 1;
  ParserNode *parserNodeBefore;
  if (nodeBefore < begin || nodeBefore->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "Bad Comma after %s",
               LEXER_TOKEN_STRINGS[nodeBefore->token]);
    return NULL;
  } else {
    parserNodeBefore = getUntilCommonParent(nodeBefore->parserNode, parent);
    if (parserNodeBefore == NULL || !isExpression(parserNodeBefore)) {
      printError(node->str_begin, node->str_end, "Bad Comma after %s",
                 LEXER_TOKEN_STRINGS[nodeBefore->token]);
      return NULL;
    }
  }
  return node->parserNode = parserNodeBefore->parent = newParserNode(
             PARSER_TOKEN_SYMBOL_COMMA, node->str_begin, node->str_end,
             (ParserNodeSingleChildMetadata *)parserNodeBefore, parent);
}

ParserNode *parserParenthesis(LexerNode *closing, LexerNode *begin,
                              ParserNode *parent) {
  ParserNode *parserNode = a404m_malloc(sizeof(*parserNode));
  parserNode->parent = parent;

  LexerNode *opening = NULL;

  for (LexerNode *iter = closing - 1; iter >= begin; --iter) {

    if (iter->parserNode != NULL) {
      ParserNode *pNode =
          getUntilCommonParents(iter->parserNode, parent, parserNode);
      if (pNode == NULL) {
        printLog(pNode->str_begin, pNode->str_end, "Bad node");
        return NULL;
      } else {
        pNode->parent = parserNode;
      }
    } else if (iter->token == LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS) {
      opening = iter;
      break;
    }
  }

  if (opening == NULL) {
    printError(closing->str_begin, closing->str_end,
               "No opening for parenthesis");
    return NULL;
  }
  opening->parserNode = parserNode;
  closing->parserNode = parserNode;

  LexerNode *beforeNode = opening - 1;
  ParserNode *before;
  if (beforeNode < begin || beforeNode->parserNode == NULL ||
      (before = getUntilCommonParent(beforeNode->parserNode, parent)) == NULL ||
      !isExpression(before)) {
    before = NULL;
  } else {
    before->parent = parserNode;
  }

  if (before == NULL) {
    parserNode->token = PARSER_TOKEN_SYMBOL_PARENTHESIS;
    parserNode->str_begin = opening->str_begin;
    parserNode->str_end = closing->str_end;

    if (parserNodeArray(opening + 1, closing, parserNode)) {
      return parserNode;
    } else {
      free(parserNode);
      return NULL;
    }
  } else {
    parserNode->token = PARSER_TOKEN_FUNCTION_CALL;
    parserNode->str_begin = before->str_begin;
    parserNode->str_end = closing->str_end;

    if (parserNodeArray(opening + 1, closing, parserNode)) {
      ParserNodeFunctionCall *metadata = malloc(sizeof(*metadata));
      metadata->function = before;
      metadata->params = parserNode->metadata;
      parserNode->metadata = metadata;
      return parserNode;
    } else {
      free(parserNode);
      return NULL;
    }
  }
}

ParserNode *parserBracketsRight(LexerNode *closing, LexerNode *begin,
                                ParserNode *parent, bool *conti) {
  LexerNode *opening = NULL;

  for (LexerNode *iter = closing - 1; iter >= begin; --iter) {
    if (iter->parserNode == NULL &&
        iter->token == LEXER_TOKEN_SYMBOL_OPEN_BRACKET) {
      opening = iter;
      break;
    }
  }

  if (opening == NULL) {
    printError(closing->str_begin, closing->str_end, "No opening for bracket");
    return NULL;
  }

  LexerNode *beforeNode = opening - 1;
  ParserNode *before;
  if (beforeNode < begin || beforeNode->parserNode == NULL ||
      (before = getUntilCommonParent(beforeNode->parserNode, parent)) == NULL ||
      !isExpression(before)) {
    closing->token = LEXER_TOKEN_SYMBOL_CLOSE_BRACKET_LEFT;
    *conti = true;
    return NULL;
  }

  ParserNode *parserNode = a404m_malloc(sizeof(*parserNode));
  parserNode->parent = parent;

  before->parent = parserNode;

  for (LexerNode *iter = closing - 1; iter >= begin; --iter) {
    if (iter->parserNode != NULL) {
      ParserNode *pNode =
          getUntilCommonParents(iter->parserNode, parent, parserNode);
      if (pNode == NULL) {
        printLog(pNode->str_begin, pNode->str_end, "Bad node");
        return NULL;
      } else {
        pNode->parent = parserNode;
      }
    } else if (iter == opening) {
      break;
    }
  }

  opening->parserNode = parserNode;
  closing->parserNode = parserNode;

  parserNode->token = PARSER_TOKEN_SYMBOL_BRACKET_RIGHT;
  parserNode->str_begin = before->str_begin;
  parserNode->str_end = closing->str_end;

  if (parserNodeArray(opening + 1, closing, parserNode)) {
    ParserNodeBracketMetadata *metadata = malloc(sizeof(*metadata));
    metadata->operand = before;
    metadata->params = parserNode->metadata;
    parserNode->metadata = metadata;
    return parserNode;
  } else {
    free(parserNode);
    return NULL;
  }
}

ParserNode *parserBracketsLeft(LexerNode *closing, LexerNode *begin,
                               LexerNode *end, ParserNode *parent) {
  LexerNode *afterNode = closing + 1;
  ParserNode *after;
  if (afterNode >= end || afterNode->parserNode == NULL ||
      (after = getUntilCommonParent(afterNode->parserNode, parent)) == NULL ||
      !isExpression(after)) {
    printLog(closing->str_begin, closing->str_end,
             "Bad bracket can't be parsed");
    return NULL;
  }

  ParserNode *parserNode = a404m_malloc(sizeof(*parserNode));
  parserNode->parent = parent;

  after->parent = parserNode;

  LexerNode *opening = NULL;

  for (LexerNode *iter = closing - 1; iter >= begin; --iter) {

    if (iter->parserNode != NULL) {
      ParserNode *pNode =
          getUntilCommonParents(iter->parserNode, parent, parserNode);
      if (pNode == NULL) {
        printLog(pNode->str_begin, pNode->str_end, "Bad node");
        return NULL;
      } else {
        pNode->parent = parserNode;
      }
    } else if (iter->token == LEXER_TOKEN_SYMBOL_OPEN_BRACKET) {
      opening = iter;
      break;
    }
  }

  if (opening == NULL) {
    printError(closing->str_begin, closing->str_end, "No opening for bracket");
    return NULL;
  }
  opening->parserNode = parserNode;
  closing->parserNode = parserNode;

  parserNode->token = PARSER_TOKEN_SYMBOL_BRACKET_LEFT;
  parserNode->str_begin = opening->str_begin;
  parserNode->str_end = after->str_end;

  if (parserNodeArray(opening + 1, closing, parserNode)) {
    ParserNodeBracketMetadata *metadata = malloc(sizeof(*metadata));
    metadata->operand = after;
    metadata->params = parserNode->metadata;
    parserNode->metadata = metadata;
    return parserNode;
  } else {
    free(parserNode);
    return NULL;
  }
}

ParserNode *parserCurlyBrackets(LexerNode *closing, LexerNode *begin,
                                ParserNode *parent) {
  ParserNode *parserNode = a404m_malloc(sizeof(*parserNode));
  parserNode->parent = parent;
  parserNode->token = PARSER_TOKEN_SYMBOL_CURLY_BRACKET;

  LexerNode *opening = NULL;

  for (LexerNode *iter = closing - 1; iter >= begin; --iter) {
    if (iter->parserNode != NULL) {
      ParserNode *pNode =
          getUntilCommonParents(iter->parserNode, parent, parserNode);
      if (pNode == NULL) {
        printError(iter->str_begin, iter->str_end, "Bad node");
        return NULL;
      } else {
        pNode->parent = parserNode;
      }
    } else if (iter->token == LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET) {
      opening = iter;
      break;
    }
  }

  if (opening == NULL) {
    printError(closing->str_begin, closing->str_end,
               "No opening for curly brackets");
    return NULL;
  }

  parserNode->str_begin = opening->str_begin;
  parserNode->str_end = closing->str_end;

  opening->parserNode = closing->parserNode = parserNode;

  if (parserNodeArray(opening + 1, closing, parserNode)) {
    return parserNode;
  } else {
    free(parserNode);
    return NULL;
  }
}

ParserNode *parserFunction(LexerNode *node, LexerNode *begin, LexerNode *end,
                           ParserNode *parent) {
  LexerNode *paramsNode = node - 1;
  LexerNode *retTypeNode = node + 1;
  if (paramsNode < begin || paramsNode->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "No params");
    return NULL;
  } else if (retTypeNode >= end || retTypeNode->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "No return type");
    return NULL;
  }
  ParserNode *params = getUntilCommonParent(paramsNode->parserNode, parent);
  ParserNode *retType = getUntilCommonParent(retTypeNode->parserNode, parent);

  LexerNode *bodyNode =
      getNextLexerNodeUsingCommonParent(retTypeNode, end, parent);

  ParserNode *body;
  if (bodyNode == NULL || bodyNode >= end || bodyNode->parserNode == NULL) {
    body = NULL;
  } else {
    body = bodyNode->parserNode;
    if (body == NULL || body->token != PARSER_TOKEN_SYMBOL_CURLY_BRACKET) {
      printError(node->str_begin, node->str_end, "Bad body");
      return NULL;
    }
    ParserNodeArray *bodyArray = body->metadata;
    for (size_t i = 0; i < bodyArray->size; ++i) {
      switch (bodyArray->data[i]->token) {
      case PARSER_TOKEN_SYMBOL_EOL:
      case PARSER_TOKEN_KEYWORD_IF:
      case PARSER_TOKEN_KEYWORD_WHILE:
        continue;
      case PARSER_TOKEN_ROOT:
      case PARSER_TOKEN_IDENTIFIER:
      case PARSER_TOKEN_BUILTIN:
      case PARSER_TOKEN_VALUE_INT:
      case PARSER_TOKEN_VALUE_FLOAT:
      case PARSER_TOKEN_VALUE_BOOL:
      case PARSER_TOKEN_VALUE_CHAR:
      case PARSER_TOKEN_VALUE_STRING:
      case PARSER_TOKEN_TYPE_TYPE:
      case PARSER_TOKEN_TYPE_FUNCTION:
      case PARSER_TOKEN_TYPE_VOID:
      case PARSER_TOKEN_TYPE_BOOL:
      case PARSER_TOKEN_TYPE_I8:
      case PARSER_TOKEN_TYPE_U8:
      case PARSER_TOKEN_TYPE_I16:
      case PARSER_TOKEN_TYPE_U16:
      case PARSER_TOKEN_TYPE_I32:
      case PARSER_TOKEN_TYPE_U32:
      case PARSER_TOKEN_TYPE_I64:
      case PARSER_TOKEN_TYPE_U64:
#ifdef FLOAT_16_SUPPORT
      case PARSER_TOKEN_TYPE_F16:
#endif
      case PARSER_TOKEN_TYPE_F32:
      case PARSER_TOKEN_TYPE_F64:
      case PARSER_TOKEN_TYPE_F128:
      case PARSER_TOKEN_TYPE_CODE:
      case PARSER_TOKEN_KEYWORD_NULL:
      case PARSER_TOKEN_KEYWORD_UNDEFINED:
      case PARSER_TOKEN_KEYWORD_PUTC:
      case PARSER_TOKEN_KEYWORD_RETURN:
      case PARSER_TOKEN_KEYWORD_STRUCT:
      case PARSER_TOKEN_CONSTANT:
      case PARSER_TOKEN_VARIABLE:
      case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
      case PARSER_TOKEN_SYMBOL_PARENTHESIS:
      case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
      case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
      case PARSER_TOKEN_SYMBOL_COMMA:
      case PARSER_TOKEN_OPERATOR_ACCESS:
      case PARSER_TOKEN_OPERATOR_ASSIGN:
      case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
      case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
      case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
      case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
      case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
      case PARSER_TOKEN_OPERATOR_POINTER:
      case PARSER_TOKEN_OPERATOR_ADDRESS:
      case PARSER_TOKEN_OPERATOR_DEREFERENCE:
      case PARSER_TOKEN_OPERATOR_PLUS:
      case PARSER_TOKEN_OPERATOR_MINUS:
      case PARSER_TOKEN_OPERATOR_SUM:
      case PARSER_TOKEN_OPERATOR_SUB:
      case PARSER_TOKEN_OPERATOR_MULTIPLY:
      case PARSER_TOKEN_OPERATOR_DIVIDE:
      case PARSER_TOKEN_OPERATOR_MODULO:
      case PARSER_TOKEN_OPERATOR_EQUAL:
      case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
      case PARSER_TOKEN_OPERATOR_GREATER:
      case PARSER_TOKEN_OPERATOR_SMALLER:
      case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
      case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
      case PARSER_TOKEN_FUNCTION_DEFINITION:
      case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
      case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
      case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
      case PARSER_TOKEN_FUNCTION_CALL:
      case PARSER_TOKEN_KEYWORD_COMPTIME:
        printError(bodyArray->data[i]->str_begin, bodyArray->data[i]->str_end,
                   "Maybe forgot a ; with %s",
                   PARSER_TOKEN_STRINGS[bodyArray->data[i]->token]);
        return NULL;
      case PARSER_TOKEN_NONE:
      }
      UNREACHABLE;
    }
  }

  if (params->token != PARSER_TOKEN_SYMBOL_PARENTHESIS) {
    printError(node->str_begin, node->str_end, "No params %s",
               PARSER_TOKEN_STRINGS[params->token]);
    return NULL;
  } else if (!isType(retType)) {
    printError(node->str_begin, node->str_end, "No return type");
    return NULL;
  } else if (!isAllArguments(params->metadata)) {
    printError(params->str_begin, params->str_end, "Bad arguments");
    return NULL;
  }

  if (body != NULL) {
    ParserNodeFunctionDefnitionMetadata *metadata =
        a404m_malloc(sizeof(*metadata));

    metadata->body = body;
    metadata->arguments = params;
    metadata->returnType = retType;

    return params->parent = retType->parent = body->parent = node->parserNode =
               newParserNode(PARSER_TOKEN_FUNCTION_DEFINITION,
                             params->str_begin, body->str_end, metadata,
                             parent);
  } else {
    ParserNodeTypeFunctionMetadata *metadata = a404m_malloc(sizeof(*metadata));

    metadata->arguments = params;
    metadata->returnType = retType;

    return params->parent = retType->parent = node->parserNode =
               newParserNode(PARSER_TOKEN_TYPE_FUNCTION, params->str_begin,
                             retType->str_end, metadata, parent);
  }
}

ParserNode *parserVariable(LexerNode *node, LexerNode *begin, LexerNode *end,
                           ParserNode *parent) {
  ParserNode *variableNode = a404m_malloc(sizeof(*variableNode));
  variableNode->parent = parent;

  LexerNode *nameNode = node - 1;
  if (nameNode < begin || nameNode->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "No name");
    goto RETURN_ERROR;
  }

  ParserNode *name = getUntilCommonParent(nameNode->parserNode, parent);

  if (name == NULL) {
    printError(node->str_begin, node->str_end,
               "Name should be identifier but got nothing");
    return NULL;
  } else if (name->token != PARSER_TOKEN_IDENTIFIER) {
    printError(name->str_begin, name->str_end,
               "Name should be identifier but got '%s'",
               PARSER_TOKEN_STRINGS[name->token]);
    return NULL;
  } else {
    name->parent = variableNode;
    variableNode->str_begin = name->str_begin;
  }

  LexerNode *node1 = node + 1;
  ParserNode *type;
  ParserNode *value;

  ParserToken token = PARSER_TOKEN_VARIABLE;

  if (node1 >= end) {
    printError(node->str_begin, node->str_end, "Bad variable definition");
    return NULL;
  } else if (node1->token == LEXER_TOKEN_SYMBOL_COLON) {
    type = NULL;
    node1->parserNode = variableNode;
    token = PARSER_TOKEN_CONSTANT;
  } else if (node1->token == LEXER_TOKEN_SYMBOL_ASSIGN) {
    type = NULL;
    node1->parserNode = variableNode;
  } else if (node1->parserNode == NULL) {
    printError(node1->str_begin, node1->str_end,
               "Bad variable type with token '%s' %d",
               LEXER_TOKEN_STRINGS[node1->token], node1->token);
    return NULL;
  } else {
    type = getUntilCommonParent(node1->parserNode, parent);
    if (type == NULL || !isType(type)) {
      printError(node1->str_begin, node1->str_end, "Bad variable type");
      return NULL;
    }
    type->parent = variableNode;
    for (++node1; node1 < end && getUntilCommonParent(node1->parserNode,
                                                      parent) == variableNode;
         ++node1) {
    }
    if (node1 < end && node1->token == LEXER_TOKEN_SYMBOL_COLON) {
      node1->parserNode = variableNode;
      token = PARSER_TOKEN_CONSTANT;
    } else if (node1 < end && node1->token == LEXER_TOKEN_SYMBOL_ASSIGN) {
      node1->parserNode = variableNode;
    } else {
      node1 = NULL;
      value = NULL;
    }
  }

  if (node1 != NULL) {
    LexerNode *valueNode = node1 + 1;
    if (valueNode >= end || valueNode->parserNode == NULL) {
      printError(name->str_begin, name->str_end, "No value");
      return NULL;
    } else {
      value = getUntilCommonParent(valueNode->parserNode, parent);
      if (!isValue(value)) {
        printError(name->str_begin, name->str_end, "Bad value");
        return NULL;
      }
      value->parent = variableNode;
    }
  }

  ParserNodeVariableMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->value = value;
  metadata->name = name;
  metadata->type = type;

  variableNode->metadata = metadata;

  if (value != NULL) {
    variableNode->str_end = value->str_end;
  } else {
    variableNode->str_end = type->str_end;
  }

  variableNode->token = token;
  return node->parserNode = variableNode;

RETURN_ERROR:
  free(variableNode);
  return NULL;
}

ParserNode *parserBinaryOperator(LexerNode *node, LexerNode *begin,
                                 LexerNode *end, ParserNode *parent,
                                 ParserToken token) {
  LexerNode *leftNode = node - 1;
  LexerNode *rightNode = node + 1;

  if (leftNode < begin || rightNode >= end) {
    printError(node->str_begin, node->str_end, "Bad binary operator");
    return NULL;
  }

  ParserNode *left = getUntilCommonParent(leftNode->parserNode, parent);
  ParserNode *right = getUntilCommonParent(rightNode->parserNode, parent);

  if (left == NULL || right == NULL) {
    printError(node->str_begin, node->str_end, "Bad binary operator");
    return NULL;
  }

  ParserNodeInfixMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->left = left;
  metadata->right = right;

  return left->parent = right->parent = node->parserNode = newParserNode(
             token, left->str_begin, right->str_end, metadata, parent);
}

ParserNode *parserBinaryOrLeftOperator(LexerNode *node, LexerNode *begin,
                                       LexerNode *end, ParserNode *parent,
                                       ParserToken token,
                                       LexerToken laterToken) {
  LexerNode *leftNode = node - 1;
  LexerNode *rightNode = node + 1;

  if ((leftNode >= begin && leftNode->parserNode != NULL) || rightNode >= end ||
      rightNode->parserNode == NULL) {
    node->token = laterToken;
    return NULL;
  }

  ParserNode *right = getUntilCommonParent(rightNode->parserNode, parent);

  if (right == NULL) {
    node->token = laterToken;
    return NULL;
  }

  return right->parent = node->parserNode =
             newParserNode(token, node->str_begin, right->str_end,
                           (ParserNodeSingleChildMetadata *)right, parent);
}

ParserNode *parserLeftOperator(LexerNode *node, LexerNode *end,
                               ParserNode *parent, ParserToken token) {
  LexerNode *rightNode = node + 1;

  if (rightNode >= end || rightNode->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "No operand found");
    return NULL;
  }

  ParserNode *right = getUntilCommonParent(rightNode->parserNode, parent);

  if (right == NULL) {
    printError(node->str_begin, node->str_end, "No operand found");
    return NULL;
  }

  return right->parent = node->parserNode =
             newParserNode(token, node->str_begin, right->str_end,
                           (ParserNodeSingleChildMetadata *)right, parent);
}

ParserNode *parserRightOperator(LexerNode *node, LexerNode *begin,
                                ParserNode *parent, ParserToken token) {
  LexerNode *leftNode = node - 1;

  if (leftNode < begin || leftNode->parserNode == NULL) {
    printError(node->str_begin, node->str_end, "No operand found");
    return NULL;
  }

  ParserNode *left = getUntilCommonParent(leftNode->parserNode, parent);

  if (left == NULL) {
    printError(node->str_begin, node->str_end, "No operand found");
    return NULL;
  }

  return left->parent = node->parserNode =
             newParserNode(token, node->str_begin, left->str_end,
                           (ParserNodeSingleChildMetadata *)left, parent);
}

ParserNode *parserIf(LexerNode *node, LexerNode *end, ParserNode *parent) {
  LexerNode *conditionNode = node + 1;
  if (conditionNode >= end) {
    printError(node->str_begin, node->str_end, "If has no condition");
    return NULL;
  }

  ParserNode *condition =
      getUntilCommonParent(conditionNode->parserNode, parent);

  if (condition == NULL) {
    printError(conditionNode->str_begin, conditionNode->str_end,
               "If has bad condition");
    return NULL;
  }

  LexerNode *bodyNode =
      getNextLexerNodeUsingCommonParent(conditionNode, end, parent);
  ParserNode *body = getUntilCommonParent(bodyNode->parserNode, parent);

  if (body == NULL) {
    printError(node->str_begin, node->str_end, "If has bad body");
    return NULL;
  }

  ParserNodeIfMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->condition = condition;
  metadata->ifBody = body;

  LexerNode *elseNode =
      getNextLexerNodeUsingCommonParent(bodyNode, end, parent);
  if (elseNode == NULL || elseNode->token != LEXER_TOKEN_KEYWORD_ELSE) {
    metadata->elseBody = NULL;
    return condition->parent = body->parent = node->parserNode =
               newParserNode(PARSER_TOKEN_KEYWORD_IF, node->str_begin,
                             body->str_end, metadata, parent);
  }

  LexerNode *elseBodyNode = elseNode + 1;

  if (elseBodyNode >= end || elseBodyNode->parserNode == NULL) {
    printError(elseNode->str_begin, elseNode->str_end, "Else has bad body");
    return NULL;
  }

  ParserNode *elseBody = getUntilCommonParent(elseBodyNode->parserNode, parent);

  if (elseBody == NULL) {
    printError(elseNode->str_begin, elseNode->str_end, "Else has bad body");
    return NULL;
  }

  metadata->elseBody = elseBody;

  return elseBody->parent = elseNode->parserNode = condition->parent =
             body->parent = node->parserNode =
                 newParserNode(PARSER_TOKEN_KEYWORD_IF, node->str_begin,
                               elseBody->str_end, metadata, parent);
}

ParserNode *parserWhile(LexerNode *node, LexerNode *end, ParserNode *parent) {
  LexerNode *conditionNode = node + 1;
  if (conditionNode >= end) {
    printError(node->str_begin, node->str_end, "While has no condition");
    return NULL;
  }

  ParserNode *condition =
      getUntilCommonParent(conditionNode->parserNode, parent);

  if (condition == NULL) {
    printError(conditionNode->str_begin, conditionNode->str_end,
               "While has bad condition");
    return NULL;
  }

  LexerNode *bodyNode =
      getNextLexerNodeUsingCommonParent(conditionNode, end, parent);
  ParserNode *body = getUntilCommonParent(bodyNode->parserNode, parent);

  if (body == NULL) {
    printError(node->str_begin, node->str_end, "While has bad body");
    return NULL;
  }

  ParserNodeWhileMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->condition = condition;
  metadata->body = body;

  return condition->parent = body->parent = node->parserNode =
             newParserNode(PARSER_TOKEN_KEYWORD_WHILE, node->str_begin,
                           body->str_end, metadata, parent);
}

ParserNode *parserComptime(LexerNode *node, LexerNode *end,
                           ParserNode *parent) {
  LexerNode *next = node + 1;
  if (next >= end) {
    printError(node->str_begin, node->str_end, "Comptime has no operand");
    return NULL;
  }

  ParserNode *operand = getUntilCommonParent(next->parserNode, parent);

  if (operand == NULL) {
    printError(node->str_begin, node->str_end, "Comptime has bad operand");
    return NULL;
  }

  return operand->parent = node->parserNode = newParserNode(
             PARSER_TOKEN_KEYWORD_COMPTIME, node->str_begin, operand->str_end,
             (ParserNodeSingleChildMetadata *)operand, parent);
}

ParserNode *parserStruct(LexerNode *node, LexerNode *end, ParserNode *parent) {
  LexerNode *next = node + 1;
  if (next >= end) {
    printError(node->str_begin, node->str_end, "Bad struct without a body");
    return NULL;
  }

  ParserNode *operand = getUntilCommonParent(next->parserNode, parent);

  if (operand == NULL) {
    printError(node->str_begin, node->str_end, "Bad struct without a body");
    return NULL;
  } else if (operand->token != PARSER_TOKEN_SYMBOL_CURLY_BRACKET) {
    printError(node->str_begin, node->str_end, "Bad body");
    return NULL;
  }

  return operand->parent = node->parserNode = newParserNode(
             PARSER_TOKEN_KEYWORD_STRUCT, node->str_begin, operand->str_end,
             (ParserNodeSingleChildMetadata *)operand, parent);
}

bool isAllArguments(const ParserNodeArray *nodes) {
  for (size_t i = 0; i < nodes->size; ++i) {
    const ParserNode *node = nodes->data[i];
    if (node->token != PARSER_TOKEN_SYMBOL_COMMA && i + 1 != nodes->size) {
      return false;
    }
  }
  return true;
}

bool isExpression(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_BUILTIN:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
  case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_FUNCTION_CALL:
  case PARSER_TOKEN_KEYWORD_PUTC:
  case PARSER_TOKEN_KEYWORD_RETURN:
  case PARSER_TOKEN_OPERATOR_ACCESS:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
  case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
  case PARSER_TOKEN_OPERATOR_POINTER:
  case PARSER_TOKEN_OPERATOR_ADDRESS:
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
  case PARSER_TOKEN_OPERATOR_PLUS:
  case PARSER_TOKEN_OPERATOR_MINUS:
  case PARSER_TOKEN_OPERATOR_SUM:
  case PARSER_TOKEN_OPERATOR_SUB:
  case PARSER_TOKEN_OPERATOR_MULTIPLY:
  case PARSER_TOKEN_OPERATOR_DIVIDE:
  case PARSER_TOKEN_OPERATOR_MODULO:
  case PARSER_TOKEN_OPERATOR_EQUAL:
  case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
  case PARSER_TOKEN_OPERATOR_GREATER:
  case PARSER_TOKEN_OPERATOR_SMALLER:
  case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
  case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
  case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
  case PARSER_TOKEN_VALUE_INT:
  case PARSER_TOKEN_VALUE_FLOAT:
  case PARSER_TOKEN_VALUE_BOOL:
  case PARSER_TOKEN_VALUE_CHAR:
  case PARSER_TOKEN_VALUE_STRING:
  case PARSER_TOKEN_KEYWORD_IF:
  case PARSER_TOKEN_KEYWORD_WHILE:
  case PARSER_TOKEN_KEYWORD_COMPTIME:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_BOOL:
  case PARSER_TOKEN_TYPE_I8:
  case PARSER_TOKEN_TYPE_U8:
  case PARSER_TOKEN_TYPE_I16:
  case PARSER_TOKEN_TYPE_U16:
  case PARSER_TOKEN_TYPE_I32:
  case PARSER_TOKEN_TYPE_U32:
  case PARSER_TOKEN_TYPE_I64:
  case PARSER_TOKEN_TYPE_U64:
#ifdef FLOAT_16_SUPPORT
  case PARSER_TOKEN_TYPE_F16:
#endif
  case PARSER_TOKEN_TYPE_F32:
  case PARSER_TOKEN_TYPE_F64:
  case PARSER_TOKEN_TYPE_F128:
  case PARSER_TOKEN_TYPE_CODE:
  case PARSER_TOKEN_KEYWORD_NULL:
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
  case PARSER_TOKEN_KEYWORD_STRUCT:
    return true;
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_COMMA:
    return false;
  case PARSER_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isType(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_I8:
  case PARSER_TOKEN_TYPE_U8:
  case PARSER_TOKEN_TYPE_I16:
  case PARSER_TOKEN_TYPE_U16:
  case PARSER_TOKEN_TYPE_I32:
  case PARSER_TOKEN_TYPE_U32:
  case PARSER_TOKEN_TYPE_I64:
  case PARSER_TOKEN_TYPE_U64:
#ifdef FLOAT_16_SUPPORT
  case PARSER_TOKEN_TYPE_F16:
#endif
  case PARSER_TOKEN_TYPE_F32:
  case PARSER_TOKEN_TYPE_F64:
  case PARSER_TOKEN_TYPE_F128:
  case PARSER_TOKEN_TYPE_CODE:
  case PARSER_TOKEN_TYPE_BOOL:
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_BUILTIN:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
  case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
  case PARSER_TOKEN_FUNCTION_CALL:
  case PARSER_TOKEN_KEYWORD_IF:
  case PARSER_TOKEN_KEYWORD_COMPTIME:
  case PARSER_TOKEN_OPERATOR_POINTER:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_KEYWORD_STRUCT:
  case PARSER_TOKEN_OPERATOR_ACCESS:
    return true;
  case PARSER_TOKEN_OPERATOR_ADDRESS:
  case PARSER_TOKEN_KEYWORD_NULL:
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE:
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_VALUE_INT:
  case PARSER_TOKEN_VALUE_FLOAT:
  case PARSER_TOKEN_VALUE_BOOL:
  case PARSER_TOKEN_VALUE_CHAR:
  case PARSER_TOKEN_VALUE_STRING:
  case PARSER_TOKEN_KEYWORD_PUTC:
  case PARSER_TOKEN_KEYWORD_RETURN:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
  case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
  case PARSER_TOKEN_OPERATOR_PLUS:
  case PARSER_TOKEN_OPERATOR_MINUS:
  case PARSER_TOKEN_OPERATOR_SUM:
  case PARSER_TOKEN_OPERATOR_SUB:
  case PARSER_TOKEN_OPERATOR_MULTIPLY:
  case PARSER_TOKEN_OPERATOR_DIVIDE:
  case PARSER_TOKEN_OPERATOR_MODULO:
  case PARSER_TOKEN_OPERATOR_EQUAL:
  case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
  case PARSER_TOKEN_OPERATOR_GREATER:
  case PARSER_TOKEN_OPERATOR_SMALLER:
  case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
  case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
  case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
  case PARSER_TOKEN_KEYWORD_WHILE:
    return false;
  case PARSER_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isValue(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_FUNCTION_CALL:
  case PARSER_TOKEN_VALUE_INT:
  case PARSER_TOKEN_VALUE_FLOAT:
  case PARSER_TOKEN_VALUE_BOOL:
  case PARSER_TOKEN_VALUE_CHAR:
  case PARSER_TOKEN_VALUE_STRING:
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_BUILTIN:
  case PARSER_TOKEN_OPERATOR_ACCESS:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
  case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
  case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
  case PARSER_TOKEN_OPERATOR_POINTER:
  case PARSER_TOKEN_OPERATOR_ADDRESS:
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
  case PARSER_TOKEN_OPERATOR_PLUS:
  case PARSER_TOKEN_OPERATOR_MINUS:
  case PARSER_TOKEN_OPERATOR_SUM:
  case PARSER_TOKEN_OPERATOR_SUB:
  case PARSER_TOKEN_OPERATOR_MULTIPLY:
  case PARSER_TOKEN_OPERATOR_DIVIDE:
  case PARSER_TOKEN_OPERATOR_MODULO:
  case PARSER_TOKEN_OPERATOR_EQUAL:
  case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
  case PARSER_TOKEN_OPERATOR_GREATER:
  case PARSER_TOKEN_OPERATOR_SMALLER:
  case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
  case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
  case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_BOOL:
  case PARSER_TOKEN_TYPE_I8:
  case PARSER_TOKEN_TYPE_U8:
  case PARSER_TOKEN_TYPE_I16:
  case PARSER_TOKEN_TYPE_U16:
  case PARSER_TOKEN_TYPE_I32:
  case PARSER_TOKEN_TYPE_U32:
  case PARSER_TOKEN_TYPE_I64:
  case PARSER_TOKEN_TYPE_U64:
#ifdef FLOAT_16_SUPPORT
  case PARSER_TOKEN_TYPE_F16:
#endif
  case PARSER_TOKEN_TYPE_F32:
  case PARSER_TOKEN_TYPE_F64:
  case PARSER_TOKEN_TYPE_F128:
  case PARSER_TOKEN_TYPE_CODE:
  case PARSER_TOKEN_KEYWORD_NULL:
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
  case PARSER_TOKEN_KEYWORD_IF:
  case PARSER_TOKEN_KEYWORD_COMPTIME:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
  case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
  case PARSER_TOKEN_KEYWORD_STRUCT:
    return true;
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE:
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_KEYWORD_PUTC:
  case PARSER_TOKEN_KEYWORD_RETURN:
  case PARSER_TOKEN_KEYWORD_WHILE:
    return false;
  case PARSER_TOKEN_NONE:
  }
  UNREACHABLE;
}

char escapeChar(char *begin, char *end, bool *success) {
  (void)end;
  switch (*begin) {
  case 'a':
    *success = true;
    return '\a';
  case 'b':
    *success = true;
    return '\b';
  case 'e':
    *success = true;
    return '\e';
  case 'f':
    *success = true;
    return '\f';
  case 'n':
    *success = true;
    return '\n';
  case 'r':
    *success = true;
    return '\r';
  case 't':
    *success = true;
    return '\t';
  case 'v':
    *success = true;
    return '\v';
  case '\\':
    *success = true;
    return '\\';
  case '\'':
    *success = true;
    return '\'';
  case '"':
    *success = true;
    return '\"';
  default:
    *success = false;
    return 0;
  }
}
