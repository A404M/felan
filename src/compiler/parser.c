#include "parser.h"
#include "compiler/lexer.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const char *PARSER_TOKEN_STRINGS[] = {
    "PARSER_TOKEN_ROOT",

    "PARSER_TOKEN_IDENTIFIER",

    "PARSER_TOKEN_TYPE_FUNCTION",
    "PARSER_TOKEN_TYPE_VOID",

    "PARSER_TOKEN_KEYWORD_PRINT",

    "PARSER_TOKEN_CONSTANT",

    "PARSER_TOKEN_SYMBOL_EOL",
    "PARSER_TOKEN_SYMBOL_CURLY_BRACKET",
    "PARSER_TOKEN_SYMBOL_PARENTHESIS",

    "PARSER_TOKEN_FUNCTION_DEFINITION",

    "PARSER_TOKEN_NONE",
};

static constexpr ParserOrder PARSER_ORDER[] = {
    {
        .ltr = true,
        .size = 2,
        .data =
            {
                LEXER_TOKEN_IDENTIFIER,
                LEXER_TOKEN_KEYWORD_VOID,
            },
    },
    {
        .ltr = true,
        .size = 2,
        .data =
            {
                LEXER_TOKEN_SYMBOL_CLOSE_PARENTHESIS,
                LEXER_TOKEN_SYMBOL_CLOSE_CURLY_BRACKET,
            },
    },
    {
        .ltr = false,
        .size = 1,
        .data =
            {
                LEXER_TOKEN_SYMBOL_FUNCTION_ARROW,
            },
    },
    {
        .ltr = true,
        .size = 2,
        .data =
            {
                LEXER_TOKEN_SYMBOL_COLON,
                LEXER_TOKEN_KEYWORD_PRINT,
            },
    },
    {
        .ltr = true,
        .size = 1,
        .data =
            {
                LEXER_TOKEN_SYMBOL_EOL,
            },
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
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_KEYWORD_PRINT:
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_CONSTANT: {
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
  case PARSER_TOKEN_SYMBOL_EOL: {
    const ParserNodeEOLMetadata *metadata = node->metadata;
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
  case PARSER_TOKEN_TYPE_FUNCTION:
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
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_KEYWORD_PRINT:
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_CONSTANT: {
    ParserNodeVariableMetadata *metadata = node->metadata;
    parserNodeDelete(metadata->name);
    parserNodeDelete(metadata->type);
    parserNodeDelete(metadata->value);
    free(metadata);
  }
    goto RETURN_SUCCESS;
  case PARSER_TOKEN_SYMBOL_EOL: {
    ParserNodeEOLMetadata *metadata = node->metadata;
    parserNodeDelete(metadata);
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
  case PARSER_TOKEN_TYPE_FUNCTION:
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
          if (parsedNodes->size == parsedNodes_size) {
            parsedNodes_size += parsedNodes_size / 2 + 1;
            parsedNodes->data =
                a404m_realloc(parsedNodes->data,
                              sizeof(*parsedNodes->data) * parsedNodes_size);
          }
          parsedNodes->data[parsedNodes->size] = parserNode;
          parsedNodes->size += 1;
        }
      }
    }
  }

  size_t inserted = 0;
  for (size_t i = 0; i < parsedNodes->size; ++i) {
    ParserNode *parserNode = parsedNodes->data[i];
    if (parserNode->parent == parent) {
      // TODO: check here
      if (inserted != i) {
        parsedNodes->data[inserted] = parserNode;
      }
      ++inserted;
    }
  }

  parsedNodes->data =
      a404m_realloc(parsedNodes->data, inserted * sizeof(*parsedNodes->data));
  parsedNodes->size = inserted;

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
  case LEXER_TOKEN_KEYWORD_VOID:
    return parserVoid(node, parent);
  case LEXER_TOKEN_KEYWORD_PRINT:
    return parserPrint(node, parent);
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
  case LEXER_TOKEN_NONE:
  case LEXER_TOKEN_NUMBER:
  case LEXER_TOKEN_SYMBOL:
  case LEXER_TOKEN_SYMBOL_OPEN_PARENTHESIS:
  case LEXER_TOKEN_SYMBOL_OPEN_CURLY_BRACKET:
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
  ParserNode *parserNode = newParserNode(
      PARSER_TOKEN_IDENTIFIER, node->str_begin, node->str_end, NULL, parent);
  node->parserNode = parserNode;
  return parserNode;
}

ParserNode *parserVoid(LexerNode *node, ParserNode *parent) {
  ParserNode *parserNode = newParserNode(
      PARSER_TOKEN_TYPE_VOID, node->str_begin, node->str_end, NULL, parent);
  node->parserNode = parserNode;
  return parserNode;
}

ParserNode *parserPrint(LexerNode *node, ParserNode *parent) {
  ParserNode *parserNode = newParserNode(
      PARSER_TOKEN_KEYWORD_PRINT, node->str_begin, node->str_end, NULL, parent);
  node->parserNode = parserNode;
  return parserNode;
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
                    (ParserNodeEOLMetadata *)parserNodeBefore, parent);
  node->parserNode = parserNode;
  if (parserNodeBefore != NULL) {
    parserNodeBefore->parent = parserNode;
  }
  return parserNode;
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
  // TODO: if body is not there then it is a function type not a function
  // definition
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
    // TODO: implement
    printLog("Not implemented");
    return NULL;
  } else {
    body = getUntilCommonParent(bodyNode->parserNode, parent);
  }

  // TODO: check parenthesis
  if (params->token != PARSER_TOKEN_SYMBOL_PARENTHESIS) {
    goto NO_PARAMS;
  } else if (!isType(retType)) {
    goto NO_RETURN_TYPE;
  }

  // TODO: check body

  ParserNodeFunctionDefnitionMetadata *metadata =
      a404m_malloc(sizeof(*metadata));

  metadata->body = body;
  metadata->arguments = params;
  metadata->returnType = retType;

  return params->parent = retType->parent = body->parent = node->parserNode =
             newParserNode(PARSER_TOKEN_FUNCTION_DEFINITION, params->str_begin,
                           body->str_end, metadata, parent);
}

ParserNode *parserVariable(LexerNode *node, LexerNode *begin, LexerNode *end,
                           ParserNode *parent) {
  LexerNode *nameNode = node - 1;
  if (nameNode < begin || nameNode->parserNode == NULL) {
    printLog("No name");
    return NULL;
  }

  ParserNode *name = getUntilCommonParent(nameNode->parserNode, parent);

  if (name->token != PARSER_TOKEN_IDENTIFIER) {
    printLog("Name should be identifier");
    return NULL;
  }

  LexerNode *node1 = node + 1;

  if (node1 >= end || node1->token != LEXER_TOKEN_SYMBOL_COLON) {
    printLog("Bad variable definition");
    return NULL;
  }

  LexerNode *valueNode = node + 2;

  if (valueNode >= end || valueNode->parserNode == NULL) {
    printLog("No initialziation");
    return NULL;
  }

  ParserNode *value = getUntilCommonParent(valueNode->parserNode, parent);

  if (!isValue(value)) {
    printLog("No value");
    return NULL;
  }

  ParserNodeVariableMetadata *metadata = a404m_malloc(sizeof(*metadata));
  metadata->value = value;
  metadata->name = name;
  metadata->type = NULL;

  return name->parent = node1->parserNode = value->parent = node->parserNode =
             newParserNode(PARSER_TOKEN_CONSTANT, name->str_begin,
                           value->str_end, metadata, parent);
}

bool isExpression(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_KEYWORD_PRINT:
    return true;
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    return false;
  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);
}

bool isType(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_TYPE_VOID:
    return true;
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_FUNCTION_DEFINITION:
  case PARSER_TOKEN_KEYWORD_PRINT:
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    return false;
  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);
}

bool isValue(ParserNode *node) {
  switch (node->token) {
  case PARSER_TOKEN_FUNCTION_DEFINITION:
    return true;
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_KEYWORD_PRINT:
  case PARSER_TOKEN_ROOT:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    return false;
  case PARSER_TOKEN_NONE:
  }
  printLog("Bad token '%d'", node->token);
  exit(1);
}
