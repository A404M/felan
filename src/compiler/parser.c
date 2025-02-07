#include "parser.h"
#include "compiler/lexer.h"
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

    "PARSER_TOKEN_VALUE_U64",

    "PARSER_TOKEN_TYPE_TYPE",
    "PARSER_TOKEN_TYPE_FUNCTION",
    "PARSER_TOKEN_TYPE_VOID",
    "PARSER_TOKEN_TYPE_U64",

    "PARSER_TOKEN_KEYWORD_PRINT_U64",
    "PARSER_TOKEN_KEYWORD_RETURN",

    "PARSER_TOKEN_CONSTANT",
    "PARSER_TOKEN_VARIABLE",

    "PARSER_TOKEN_SYMBOL_EOL",
    "PARSER_TOKEN_SYMBOL_CURLY_BRACKET",
    "PARSER_TOKEN_SYMBOL_PARENTHESIS",
    "PARSER_TOKEN_SYMBOL_COMMA",

    "PARSER_TOKEN_OPERATOR_ASSIGN",
    "PARSER_TOKEN_OPERATOR_SUM",

    "PARSER_TOKEN_FUNCTION_DEFINITION",

    "PARSER_TOKEN_FUNCTION_CALL",

    "PARSER_TOKEN_NONE",
};

#define ORDER_ARRAY(...)                                                       \
  .size = sizeof((LexerToken[]){__VA_ARGS__}) / sizeof(LexerToken),            \
  .data = {__VA_ARGS__}

static constexpr ParserOrder PARSER_ORDER[] = {
    {
        .ltr = true,
        ORDER_ARRAY(LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET, ),
    },
    {
        .ltr = true,
        ORDER_ARRAY(LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS,
                    LEXER_TOKEN_IDENTIFIER, LEXER_TOKEN_KEYWORD_TYPE,
                    LEXER_TOKEN_KEYWORD_VOID, LEXER_TOKEN_KEYWORD_U64,
                    LEXER_TOKEN_NUMBER, ),
    },
    {
        .ltr = false,
        ORDER_ARRAY(LEXER_TOKEN_SYMBOL_FUNCTION_ARROW, ),
    },
    {
        .ltr = true,
        ORDER_ARRAY(LEXER_TOKEN_SYMBOL_PLUS, ),
    },
    {
        .ltr = true,
        ORDER_ARRAY(LEXER_TOKEN_SYMBOL_COLON, ),
    },
    {
        .ltr = false,
        ORDER_ARRAY(LEXER_TOKEN_SYMBOL_ASSIGN, ),
    },
    {
        .ltr = true,
        ORDER_ARRAY(LEXER_TOKEN_KEYWORD_RETURN,
                    LEXER_TOKEN_KEYWORD_PRINT_U64, ),
    },
    {
        .ltr = true,
        ORDER_ARRAY(LEXER_TOKEN_SYMBOL_EOL, LEXER_TOKEN_SYMBOL_COMMA, ),
    },
};

static constexpr size_t PARSER_ORDER_SIZE =
    sizeof(PARSER_ORDER) / sizeof(*PARSER_ORDER);

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
    printf(",children=[\n");
    for (size_t i = 0; i < metadata->size; ++i) {
      parserNodePrint(metadata->data[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_U64:
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_VALUE_U64: {
    ParserNodeU64Metadata metadata = (ParserNodeU64Metadata)node->metadata;
    printf(",operand=%lu", metadata);
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
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
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
    printf("]\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM: {
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
  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);

RETURN_SUCCESS:
  printf("}");
}

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
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_U64:
  case PARSER_TOKEN_VALUE_U64:
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
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
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
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM: {
    ParserNodeInfixMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->left);
    parserNodeDelete(metadata->right);
  }
    goto RETURN_SUCCESS;

  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);

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

      if (iter->parserNode != NULL) {
        continue;
      }

      for (size_t order_index = 0; order_index < orders.size; ++order_index) {
        const LexerToken order = orders.data[order_index];
        if (iter->token == order) {
          ParserNode *parserNode = parseNode(iter, begin, end, parent);
          if (parserNode == NULL) {
            goto RETURN_ERROR;
          }
        }
      }
    }
  }

  for (LexerNode *iter = begin; iter < end; ++iter) {
    ParserNode *pNode = iter->parserNode;
    if (pNode->parent != parent) {
      continue;
    } else if (pNode == NULL) {
      printLog("Bad child");
      goto RETURN_ERROR;
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
                      ParserNode *parent) {
  switch (node->token) {
  case LEXER_TOKEN_IDENTIFIER:
    return parserIdentifier(node, parent);
  case LEXER_TOKEN_KEYWORD_TYPE:
    return parserType(node, parent);
  case LEXER_TOKEN_KEYWORD_VOID:
    return parserVoid(node, parent);
  case LEXER_TOKEN_KEYWORD_U64:
    return parserU64(node, parent);
  case LEXER_TOKEN_KEYWORD_PRINT_U64:
    return parserPrintU64(node, end, parent);
  case LEXER_TOKEN_KEYWORD_RETURN:
    return parserReturn(node, end, parent);
  case LEXER_TOKEN_SYMBOL_EOL:
    return parserEol(node, begin, parent);
  case LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS:
    return parserParenthesis(node, begin, parent);
  case LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET:
    return parserCurlyBrackets(node, begin, parent);
  case LEXER_TOKEN_SYMBOL_FUNCTION_ARROW:
    return parserFunction(node, begin, end, parent);
  case LEXER_TOKEN_SYMBOL_COLON:
    return parserVariable(node, begin, end, parent);
  case LEXER_TOKEN_SYMBOL_COMMA:
    return parserComma(node, begin, parent);
  case LEXER_TOKEN_NUMBER:
    return parserNumber(node, parent);
  case LEXER_TOKEN_SYMBOL_ASSIGN:
    return parserAssign(node, begin, end, parent);
  case LEXER_TOKEN_SYMBOL_PLUS:
    return parserPlus(node, begin, end, parent);
  case LEXER_TOKEN_SYMBOL:
  case LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS:
  case LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET:
  case LEXER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  return NULL;
}

ParserNode *getUntilCommonParent(ParserNode *node, ParserNode *parent) {
  while (node != NULL && node->parent != parent) {
    node = node->parent;
  }
  return node;
}

ParserNode *parserIdentifier(LexerNode *node, ParserNode *parent) {
  return node->parserNode =
             newParserNode(PARSER_TOKEN_IDENTIFIER, node->str_begin,
                           node->str_end, NULL, parent);
}

ParserNode *parserType(LexerNode *node, ParserNode *parent) {
  return node->parserNode =
             newParserNode(PARSER_TOKEN_TYPE_TYPE, node->str_begin,
                           node->str_end, NULL, parent);
}

ParserNode *parserVoid(LexerNode *node, ParserNode *parent) {
  return node->parserNode =
             newParserNode(PARSER_TOKEN_TYPE_VOID, node->str_begin,
                           node->str_end, NULL, parent);
}

ParserNode *parserU64(LexerNode *node, ParserNode *parent) {
  return node->parserNode =
             newParserNode(PARSER_TOKEN_TYPE_U64, node->str_begin,
                           node->str_end, NULL, parent);
}

ParserNode *parserPrintU64(LexerNode *node, LexerNode *end,
                           ParserNode *parent) {
  LexerNode *afterNode = node + 1;
  if (afterNode >= end) {
    printLog("No param");
    return NULL;
  } else if (afterNode->parserNode == NULL) {
    printLog("Bad param");
    return NULL;
  }

  ParserNode *operand = getUntilCommonParent(afterNode->parserNode, parent);
  if (operand == NULL) {
    printLog("No param");
    return NULL;
  }

  return operand->parent = node->parserNode = newParserNode(
             PARSER_TOKEN_KEYWORD_PRINT_U64, node->str_begin, node->str_end,
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
      printLog("No param");
      return NULL;
    }
  }

  ParserNodeReturnMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->value = operand;

  return operand->parent = node->parserNode =
             newParserNode(PARSER_TOKEN_KEYWORD_RETURN, node->str_begin,
                           node->str_end, metadata, parent);
}

ParserNode *parserNumber(LexerNode *node, ParserNode *parent) {
  ParserNode *parserNode;
  switch (*node->str_begin) {
  case '0':
    printLog("Not implemented");
    return NULL;
  default: {
    bool success;
    uint64_t value = decimalToU64(node->str_begin, node->str_end, &success);
    if (!success) {
      printLog("Error in parsing number");
      return NULL;
    }
    parserNode =
        newParserNode(PARSER_TOKEN_VALUE_U64, node->str_begin, node->str_end,
                      (void *)(ParserNodeU64Metadata)value, parent);
  }
  }
  return node->parserNode = parserNode;
}

ParserNode *parserEol(LexerNode *node, LexerNode *begin, ParserNode *parent) {
  LexerNode *nodeBeore = node - 1;
  ParserNode *parserNodeBefore;
  if (nodeBeore < begin) {
    parserNodeBefore = NULL;
  } else if (nodeBeore->parserNode == NULL) {
    printLog("Bad EOL after %s", LEXER_TOKEN_STRINGS[nodeBeore->token]);
    return NULL;
  } else {
    parserNodeBefore = getUntilCommonParent(nodeBeore->parserNode, parent);
    if (parserNodeBefore == NULL || !isExpression(parserNodeBefore)) {
      printLog("Bad EOL");
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
    printLog("Bad Comma after %s", LEXER_TOKEN_STRINGS[nodeBefore->token]);
    return NULL;
  } else {
    parserNodeBefore = getUntilCommonParent(nodeBefore->parserNode, parent);
    if (parserNodeBefore == NULL || !isExpression(parserNodeBefore)) {
      printLog("Bad Comma");
      return NULL;
    }
  }
  return node->parserNode = parserNodeBefore->parent = newParserNode(
             PARSER_TOKEN_SYMBOL_COMMA, node->str_begin, node->str_end,
             (ParserNodeSingleChildMetadata *)parserNodeBefore, parent);
}

ParserNode *parserParenthesis(LexerNode *closing, LexerNode *begin,
                              ParserNode *parent) {
  LexerNode *opening = NULL;

  for (LexerNode *iter = closing - 1; iter >= begin; --iter) {
    if (iter->parserNode == NULL &&
        iter->token == LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS) {
      opening = iter;
      break;
    }
  }

  if (opening == NULL) {
    printLog("No opening for parenthesis");
    return NULL;
  }

  LexerNode *beforeNode = opening - 1;
  ParserNode *before;
  if (beforeNode < begin || beforeNode->parserNode == NULL ||
      (before = getUntilCommonParent(beforeNode->parserNode, parent)) == NULL ||
      !isExpression(before)) {
    before = NULL;
  }

  if (before == NULL) {
    ParserNode *parserNode =
        newParserNode(PARSER_TOKEN_SYMBOL_PARENTHESIS, opening->str_begin,
                      closing->str_end, NULL, parent);

    opening->parserNode = parserNode;
    closing->parserNode = parserNode;
    if (parserNodeArray(opening + 1, closing, parserNode)) {
      return parserNode;
    } else {
      free(parserNode);
      return NULL;
    }
  } else {
    ParserNode *parserNode =
        newParserNode(PARSER_TOKEN_FUNCTION_CALL, before->str_begin,
                      closing->str_end, NULL, parent);

    before->parent = parserNode;
    opening->parserNode = parserNode;
    closing->parserNode = parserNode;
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

ParserNode *parserCurlyBrackets(LexerNode *closing, LexerNode *begin,
                                ParserNode *parent) {
  LexerNode *opening = NULL;

  for (LexerNode *iter = closing - 1; iter >= begin; --iter) {
    if (iter->parserNode == NULL &&
        iter->token == LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET) {
      opening = iter;
      break;
    }
  }

  if (opening == NULL) {
    printLog("No opening for curly brackets");
    return NULL;
  }

  ParserNode *parserNode =
      newParserNode(PARSER_TOKEN_SYMBOL_CURLY_BRACKET, opening->str_begin,
                    closing->str_end, NULL, parent);

  opening->parserNode = parserNode;
  closing->parserNode = parserNode;
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
  LexerNode *bodyNode = node + 2;
  if (paramsNode < begin || paramsNode->parserNode == NULL) {
  NO_PARAMS:
    printLog("No params");
    return NULL;
  } else if (retTypeNode >= end || retTypeNode->parserNode == NULL) {
  NO_RETURN_TYPE:
    printLog("No return type");
    return NULL;
  }
  ParserNode *params = getUntilCommonParent(paramsNode->parserNode, parent);
  ParserNode *retType = getUntilCommonParent(retTypeNode->parserNode, parent);
  ParserNode *body;

  if (bodyNode >= end || bodyNode->parserNode == NULL) {
    body = NULL;
  } else {
    body = getUntilCommonParent(bodyNode->parserNode, parent);
    if (body == NULL || body->token != PARSER_TOKEN_SYMBOL_CURLY_BRACKET) {
      printLog("Bad body");
      return NULL;
    }
    ParserNodeArray *bodyArray = body->metadata;
    for (size_t i = 0; i < bodyArray->size; ++i) {
      if (bodyArray->data[i]->token != PARSER_TOKEN_SYMBOL_EOL) {
        printLog("Bad body %s",
                 PARSER_TOKEN_STRINGS[bodyArray->data[i]->token]);
        return NULL;
      }
    }
  }

  if (params->token != PARSER_TOKEN_SYMBOL_PARENTHESIS) {
    goto NO_PARAMS;
  } else if (!isType(retType)) {
    goto NO_RETURN_TYPE;
  } else if (!isAllArguments(params->metadata)) {
    printLog("Bad arguments");
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
    printLog("No name");
    goto RETURN_ERROR;
  }

  ParserNode *name = getUntilCommonParent(nameNode->parserNode, parent);

  if (name->token != PARSER_TOKEN_IDENTIFIER) {
    printLog("Name should be identifier but got '%s'",
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
    printLog("Bad variable definition");
    return NULL;
  } else if (node1->token == LEXER_TOKEN_SYMBOL_COLON) {
    type = NULL;
    node1->parserNode = variableNode;
    token = PARSER_TOKEN_CONSTANT;
  } else if (node1->token == LEXER_TOKEN_SYMBOL_ASSIGN) {
    type = NULL;
    node1->parserNode = variableNode;
  } else if (node1->parserNode == NULL) {
    printLog("Bad variable type with token '%s' %d",
             LEXER_TOKEN_STRINGS[node1->token], node1->token);
    return NULL;
  } else {
    type = getUntilCommonParent(node1->parserNode, parent);
    if (type == NULL || !isType(type)) {
      printLog("Bad variable type");
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
    } else if (node1 < end && node1->token == LEXER_TOKEN_SYMBOL_COLON) {
      node1->parserNode = variableNode;
    } else {
      node1 = NULL;
      value = NULL;
    }
  }

  if (node1 != NULL) {
    LexerNode *valueNode = node1 + 1;
    if (valueNode >= end || valueNode->parserNode == NULL) {
      printLog("No value for '%.*s'", (int)(name->str_end - name->str_begin),
               name->str_begin);
      return NULL;
    } else {
      value = getUntilCommonParent(valueNode->parserNode, parent);
      if (!isValue(value)) {
        printLog("No value");
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

ParserNode *parserAssign(LexerNode *node, LexerNode *begin, LexerNode *end,
                         ParserNode *parent) {
  LexerNode *leftNode = node - 1;
  LexerNode *rightNode = node + 1;

  if (leftNode < begin || rightNode >= end) {
    printLog("Bad assign");
    return NULL;
  }

  ParserNode *left = getUntilCommonParent(leftNode->parserNode, parent);
  ParserNode *right = getUntilCommonParent(rightNode->parserNode, parent);

  if (left == NULL || right == NULL) {
    printLog("Bad assign");
    return NULL;
  }

  ParserNodeInfixMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->left = left;
  metadata->right = right;

  return left->parent = right->parent = node->parserNode =
             newParserNode(PARSER_TOKEN_OPERATOR_ASSIGN, left->str_begin,
                           right->str_end, metadata, parent);
}

ParserNode *parserPlus(LexerNode *node, LexerNode *begin, LexerNode *end,
                       ParserNode *parent) {
  LexerNode *leftNode = node - 1;
  LexerNode *rightNode = node + 1;

  if (leftNode < begin || rightNode >= end) {
    printLog("Bad plus");
    return NULL;
  }

  ParserNode *left = getUntilCommonParent(leftNode->parserNode, parent);
  ParserNode *right = getUntilCommonParent(rightNode->parserNode, parent);

  if (left == NULL || right == NULL) {
    printLog("Bad plus");
    return NULL;
  }

  ParserNodeInfixMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->left = left;
  metadata->right = right;

  return left->parent = right->parent = node->parserNode =
             newParserNode(PARSER_TOKEN_OPERATOR_SUM, left->str_begin,
                           right->str_end, metadata, parent);
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
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_FUNCTION_CALL:
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
  case PARSER_TOKEN_KEYWORD_RETURN:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM:
    return true;
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_U64:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_VALUE_U64:
    return false;
  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);
}

bool isType(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_U64:
    return true;
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_FUNCTION_CALL:
  case PARSER_TOKEN_VALUE_U64:
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
  case PARSER_TOKEN_KEYWORD_RETURN:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM:
    return false;
  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);
}

bool isValue(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_FUNCTION_CALL:
  case PARSER_TOKEN_VALUE_U64:
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_OPERATOR_ASSIGN:
  case PARSER_TOKEN_OPERATOR_SUM:
    return true;
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_TYPE_TYPE:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_U64:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_VARIABLE:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
  case PARSER_TOKEN_KEYWORD_RETURN:
    return false;
  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);
}
