#include "ast-tree.h"
#include "compiler/parser.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <stdlib.h>

const char *AST_TREE_TOKEN_STRINGS[] = {
    "AST_TREE_TOKEN_FUNCTION",
    "AST_TREE_TOKEN_KEYWORD_PRINT",
    "AST_TREE_TOKEN_NONE",
};

void astTreePrint(const AstTree *tree, int indent) {
  for (int i = 0; i < indent; ++i)
    printf(" ");
  printf("{token=\"%s\"", AST_TREE_TOKEN_STRINGS[tree->token]);
  switch (tree->token) {
  case AST_TREE_TOKEN_FUNCTION:
    AstTreeScope *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("expressions=[\n");
    for (size_t i = 0; i < metadata->expressions_size; ++i) {
      astTreePrint(&metadata->expressions[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  case AST_TREE_TOKEN_KEYWORD_PRINT:
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_NONE:
  }

  printLog("Bad token '%d'", tree->token);
  exit(1);

RETURN_SUCCESS:
  printf("}");
}

void astTreeRootPrint(const AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    const AstTreeVariable *variable = root->variables.data[i];
    printf("{\nname=\"%.*s\",\nvalue=\n",
           (int)(variable->name_end - variable->name_begin),
           variable->name_begin);
    astTreePrint(variable->value, 1);
    printf("\n},\n");
  }
}

void astTreeDestroy(AstTree tree) {
  switch (tree.token) {
  case AST_TREE_TOKEN_FUNCTION:
    AstTreeScope *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->expressions_size; ++i) {
      astTreeDestroy(metadata->expressions[i]);
    }
  case AST_TREE_TOKEN_KEYWORD_PRINT:
    return;
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", tree.token);
  exit(1);
}

void astTreeDelete(AstTree *tree) {
  astTreeDestroy(*tree);
  free(tree);
}

void astTreeRootDelete(AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    astTreeDelete(root->variables.data[i]->value);
  }
  free(root->variables.data);
  free(root);
}

AstTree *newAstTree(AstTreeToken token, void *metadata) {
  AstTree *result = a404m_malloc(sizeof(*result));
  result->token = token;
  result->metadata = metadata;
  return result;
}

AstTreeRoot *makeAstTree(ParserNode *parsedRoot) {
  if (parsedRoot->token != PARSER_TOKEN_ROOT) {
    return NULL;
  }

  ParserNodeArray *nodes = parsedRoot->metadata;

  AstTreeRoot *root = a404m_malloc(sizeof(*root));

  size_t variables_size = nodes->size;
  root->variables.data =
      a404m_malloc(variables_size * sizeof(*root->variables.data));
  root->variables.size = 0;

  for (size_t i = 0; i < nodes->size; ++i) {
    ParserNode *eol = nodes->data[i];
    if (eol->token != PARSER_TOKEN_SYMBOL_EOL) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    ParserNode *node = (ParserNodeEOLMetadata *)eol->metadata;
    if (node->token != PARSER_TOKEN_CONSTANT) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    }
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
    variable->name_begin = node_metadata->name->str_begin;
    variable->name_end = node_metadata->name->str_end;

    pushVariable(&root->variables, &variables_size, variable);
  }

  for (size_t i = 0; i < nodes->size; ++i) {
    ParserNode *node = (ParserNodeEOLMetadata *)nodes->data[i]->metadata;
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    // TODO: check type
    AstTree *tree = astTreeParse(node_metadata->value, &root->variables, 1);
    if (tree == NULL) {
      goto RETURN_ERROR;
    }

    root->variables.data[i]->value = tree;
  }

  return root;

RETURN_ERROR:
  free(root->variables.data);
  free(root);
  return NULL;
}

void pushVariable(AstTreeVariables *variables, size_t *variables_size,
                  AstTreeVariable *variable) {
  if (*variables_size == variables->size) {
    *variables_size += *variables_size / 2 + 1;
    variables->data = a404m_realloc(variables->data,
                                    *variables_size * sizeof(*variables->data));
  }
  variables->data[variables->size] = variable;
  variables->size += 1;
}

AstTree *astTreeParse(ParserNode *parserNode, AstTreeVariables *variables,
                      size_t variables_size) {
  switch (parserNode->token) {
  case PARSER_TOKEN_KEYWORD_PRINT:
    return newAstTree(AST_TREE_TOKEN_KEYWORD_PRINT, NULL);
  case PARSER_TOKEN_FUNCTION_DEFINITION:
    return astTreeParseFunction(parserNode, variables, variables_size);
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_IDENTIFIER:
  case PARSER_TOKEN_TYPE_VOID:
  case PARSER_TOKEN_TYPE_FUNCTION:
  case PARSER_TOKEN_NONE:
  case PARSER_TOKEN_ROOT:
  }
  printLog("Bad token %d", parserNode->token);
  return NULL;
}

AstTree *astTreeParseFunction(ParserNode *parserNode,
                              AstTreeVariables *variables,
                              size_t variables_size) {
  ParserNodeFunctionDefnitionMetadata *node_metadata = parserNode->metadata;
  ParserNodeArray *body = node_metadata->body->metadata;

  AstTreeScope *scope = a404m_malloc(sizeof(*scope));

  size_t expressions_size = 0;
  scope->expressions =
      a404m_malloc(expressions_size * sizeof(*scope->expressions));
  scope->expressions_size = 0;

  for (size_t i = 0; i < body->size; ++i) {
    ParserNode *eol = body->data[i];
    if (eol->token != PARSER_TOKEN_SYMBOL_EOL) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    ParserNode *node = (ParserNodeEOLMetadata *)eol->metadata;
    AstTree *tree = astTreeParse(node, variables, variables_size);

    if (tree == NULL) {
      goto RETURN_ERROR;
    }

    if (expressions_size == scope->expressions_size) {
      expressions_size += expressions_size / 2 + 1;
      scope->expressions = a404m_realloc(
          scope->expressions, expressions_size * sizeof(*scope->expressions));
    }
    scope->expressions[scope->expressions_size] = *tree;
    scope->expressions_size += 1;

    free(tree);
  }

  scope->expressions =
      a404m_realloc(scope->expressions,
                    scope->expressions_size * sizeof(*scope->expressions));

  AstTree *result = newAstTree(AST_TREE_TOKEN_FUNCTION, scope);

  return result;

RETURN_ERROR:
  free(scope->expressions);
  free(scope);
  return NULL;
}
