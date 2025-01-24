#include "ast-tree.h"
#include "compiler/parser.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <stdlib.h>

const char *AST_TREE_TOKEN_STRINGS[] = {
    "AST_TREE_TOKEN_FUNCTION",  "AST_TREE_TOKEN_KEYWORD_PRINT",
    "AST_TREE_TOKEN_NONE",      "AST_TREE_TOKEN_TYPE_FUNCTION",
    "AST_TREE_TOKEN_TYPE_VOID",
};

void astTreePrint(const AstTree *tree, int indent) {
  for (int i = 0; i < indent; ++i)
    printf(" ");
  printf("{token=\"%s\"", AST_TREE_TOKEN_STRINGS[tree->token]);
  switch (tree->token) {
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("arguments=[\n");
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("{name=%.*s,\n",
             (int)(metadata->arguments[i].name_end -
                   metadata->arguments[i].name_begin),
             metadata->arguments[i].name_begin);
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("type=\n");
      astTreePrint(metadata->arguments[i].type, indent + 2);
      printf("\n");
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("},\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("],\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("expressions=[\n");
    for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
      astTreePrint(&metadata->scope.expressions[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_KEYWORD_PRINT:
  case AST_TREE_TOKEN_TYPE_VOID:
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_TYPE_FUNCTION: {
    AstTreeTypeFunction *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("arguments=[\n");
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      astTreePrint(metadata->arguments[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
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
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeScope *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->expressions_size; ++i) {
      astTreeDestroy(metadata->expressions[i]);
    }
    free(metadata->expressions);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_KEYWORD_PRINT:
  case AST_TREE_TOKEN_TYPE_VOID:
    return;
  case AST_TREE_TOKEN_TYPE_FUNCTION: {
    AstTreeTypeFunction *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      astTreeDelete(metadata->arguments[i]);
    }
    free(metadata->arguments);
    free(metadata->returnType);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_NONE:
    break;
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
    free(root->variables.data[i]);
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

    AstTree *type = astTreeParse(node_metadata->type, &root->variables, 1);
    if (type == NULL) {
      goto RETURN_ERROR;
    }

    AstTree *value = astTreeParse(node_metadata->value, &root->variables, 1);
    if (value == NULL) {
      goto RETURN_ERROR;
    }

    if (!hasTypeOf(value, type)) {
      printLog("Type mismatch");
      goto RETURN_ERROR;
    }

    root->variables.data[i]->type = type;
    root->variables.data[i]->value = value;
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
  case PARSER_TOKEN_TYPE_FUNCTION:
    return astTreeParseTypeFunction(parserNode, variables, variables_size);
  case PARSER_TOKEN_TYPE_VOID:
    return newAstTree(AST_TREE_TOKEN_TYPE_VOID, NULL);
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_IDENTIFIER:
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
  ParserNodeArray *node_arguments = node_metadata->arguments->metadata;
  ParserNodeArray *body = node_metadata->body->metadata;

  size_t expressions_size = 0;
  AstTreeScope scope = {
      .expressions =
          a404m_malloc(expressions_size * sizeof(*scope.expressions)),
      .expressions_size = 0,
      .variables = NULL,
      .variables_size = 0,
  };

  AstTreeFunction *function = a404m_malloc(sizeof(*function));

  size_t arguments_size = 0;
  function->arguments =
      a404m_malloc(arguments_size * sizeof(*function->arguments));
  function->arguments_size = 0;

  if ((function->returnType = astTreeParse(node_metadata->returnType, variables,
                                           variables_size)) == NULL) {
    goto RETURN_ERROR;
  }

  for (size_t i = 0; i < node_arguments->size; ++i) {
    ParserNode *arg = node_arguments->data[i];
    ParserNodeVariableMetadata *arg_metadata = arg->metadata;
    if (arg_metadata->value != NULL) {
      printLog("arguments can't have default values (for now)");
      goto RETURN_ERROR;
    }

    AstTree *type = astTreeParse(arg_metadata->type, variables, variables_size);
    if (type == NULL) {
      goto RETURN_ERROR;
    }

    if (function->arguments_size == arguments_size) {
      arguments_size += arguments_size / 2 + 1;
      function->arguments = a404m_realloc(
          function->arguments, arguments_size * sizeof(*function->arguments));
    }

    function->arguments[function->arguments_size].value = NULL;
    function->arguments[function->arguments_size].type = type;
    function->arguments[function->arguments_size].name_begin =
        arg_metadata->name->str_begin;
    function->arguments[function->arguments_size].name_end =
        arg_metadata->name->str_end;
    function->arguments_size += 1;
  }

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

    if (expressions_size == scope.expressions_size) {
      expressions_size += expressions_size / 2 + 1;
      scope.expressions = a404m_realloc(
          scope.expressions, expressions_size * sizeof(*scope.expressions));
    }
    scope.expressions[scope.expressions_size] = *tree;
    scope.expressions_size += 1;

    free(tree);
  }

  scope.expressions = a404m_realloc(
      scope.expressions, scope.expressions_size * sizeof(*scope.expressions));

  function->scope = scope;

  AstTree *result = newAstTree(AST_TREE_TOKEN_FUNCTION, function);

  return result;

RETURN_ERROR:
  free(function);
  free(scope.expressions);
  return NULL;
}

AstTree *astTreeParseTypeFunction(ParserNode *parserNode,
                                  AstTreeVariables *variables,
                                  size_t variables_size) {
  ParserNodeTypeFunctionMetadata *metadata = parserNode->metadata;
  ParserNodeArray *node_arguments = metadata->arguments->metadata;

  AstTreeTypeFunction *typeFunction = a404m_malloc(sizeof(*typeFunction));

  size_t arguments_size = 0;
  typeFunction->arguments =
      a404m_malloc(arguments_size * sizeof(*typeFunction->arguments));
  typeFunction->arguments_size = 0;

  for (size_t i = 0; i < node_arguments->size; ++i) {
    ParserNodeVariableMetadata *arg_metadata =
        node_arguments->data[i]->metadata;
    if (arg_metadata->value != NULL) {
      printLog("arguments can't have default values (for now)");
      goto RETURN_ERROR;
    }

    AstTree *type = astTreeParse(arg_metadata->type, variables, variables_size);
    if (type == NULL) {
      goto RETURN_ERROR;
    }

    if (typeFunction->arguments_size == arguments_size) {
      arguments_size += arguments_size / 2 + 1;
      typeFunction->arguments =
          a404m_realloc(typeFunction->arguments,
                        arguments_size * sizeof(*typeFunction->arguments));
    }

    typeFunction->arguments[typeFunction->arguments_size] = type;
    typeFunction->arguments_size += 1;
  }

  if ((typeFunction->returnType = astTreeParse(metadata->returnType, variables,
                                               variables_size)) == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *tree = a404m_malloc(sizeof(*tree));
  tree->metadata = typeFunction;
  tree->token = AST_TREE_TOKEN_TYPE_FUNCTION;
  return tree;

RETURN_ERROR:
  return NULL;
}

bool hasTypeOf(AstTree *value, AstTree *type) {
  switch (type->token) {
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    AstTreeTypeFunction *typeMetadata = type->metadata;
    switch (value->token) {
    case AST_TREE_TOKEN_FUNCTION: {
      AstTreeFunction *valueMetadata = value->metadata;
      if (typeMetadata->arguments_size != valueMetadata->arguments_size) {
        return false;
      }
      for (size_t i = 0; i < typeMetadata->arguments_size; ++i) {
        if (!typeIsEqual(typeMetadata->arguments[i],
                         valueMetadata->arguments[i].type)) {
          return false;
        }
      }
      return typeIsEqual(typeMetadata->returnType, valueMetadata->returnType);
    }
    case AST_TREE_TOKEN_KEYWORD_PRINT:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
      return false;
    case AST_TREE_TOKEN_NONE:
    }
    goto ERROR;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT:
  case AST_TREE_TOKEN_TYPE_VOID:
    return false;
  case AST_TREE_TOKEN_NONE:
  }
ERROR:
  printLog("Bad token '%d'", type->token);
  exit(1);
}

bool typeIsEqual(AstTree *type0, AstTree *type1) { return true; }
