#include "ast-tree.h"

#include "compiler/parser.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AstTree AST_TREE_TYPE_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_TYPE,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_VOID_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_VOID,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_U64_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U64,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_VOID_VALUE = {
    .token = AST_TREE_TOKEN_NONE,
    .metadata = NULL,
    .type = &AST_TREE_VOID_TYPE,
};

const char *AST_TREE_TOKEN_STRINGS[] = {
    "AST_TREE_TOKEN_FUNCTION",

    "AST_TREE_TOKEN_KEYWORD_PRINT_U64",

    "AST_TREE_TOKEN_TYPE_TYPE",
    "AST_TREE_TOKEN_TYPE_FUNCTION",
    "AST_TREE_TOKEN_TYPE_VOID",
    "AST_TREE_TOKEN_TYPE_U64",

    "AST_TREE_TOKEN_FUNCTION_CALL",
    "AST_TREE_TOKEN_VARIABLE",
    "AST_TREE_TOKEN_VARIABLE_DEFINE",
    "AST_TREE_TOKEN_VALUE_U64",

    "AST_TREE_TOKEN_OPERATOR_ASSIGN",
    "AST_TREE_TOKEN_OPERATOR_SUM",

    "AST_TREE_TOKEN_NONE",
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
    for (size_t i = 0; i < metadata->arguments.size; ++i) {
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("{name=%.*s,\n",
             (int)(metadata->arguments.data[i]->name_end -
                   metadata->arguments.data[i]->name_begin),
             metadata->arguments.data[i]->name_begin);
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("type=\n");
      astTreePrint(metadata->arguments.data[i]->type, indent + 2);
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
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
    AstTreeSingleChild *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("child=\n");
    astTreePrint(metadata, indent + 1);
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = tree->metadata;
    if (metadata->value != NULL) {
      printf(",\n");
      for (int i = 0; i < indent; ++i)
        printf(" ");
      printf("value=\n");
      astTreePrint(metadata->value, indent + 1);
    }
  }
    goto RETURN_SUCCESS;

  case AST_TREE_TOKEN_VALUE_U64: {
    AstTreeU64 metadata = (AstTreeU64)tree->metadata;
    printf(",value=%lu", metadata);
  }
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
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("function=\n");
    astTreePrint(metadata->function, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("paramters=[\n");
    for (size_t i = 0; i < metadata->parameters_size; ++i) {
      astTreePrint(metadata->parameters[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = tree->metadata;
    printf(",variable.name=%.*s",
           (int)(metadata->name_end - metadata->name_begin),
           metadata->name_begin);
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreeInfix *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("left=\n");
    astTreePrint(&metadata->left, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("right=\n");
    astTreePrint(&metadata->right, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
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
    printf("\n}\n");
  }
}

void astTreeDestroy(AstTree tree) {
  switch (tree.token) {
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
      astTreeDestroy(metadata->scope.expressions[i]);
    }
    for (size_t i = 0; i < metadata->scope.variables.size; ++i) {
      astTreeVariableDelete(metadata->scope.variables.data[i]);
    }
    for (size_t i = 0; i < metadata->arguments.size; ++i) {
      astTreeVariableDelete(metadata->arguments.data[i]);
    }
    astTreeDelete(metadata->returnType);
    free(metadata->scope.variables.data);
    free(metadata->scope.expressions);
    free(metadata->arguments.data);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    return;
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
    AstTreeSingleChild *metadata = tree.metadata;
    astTreeDelete(metadata);
  }
    return;
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = tree.metadata;
    if (metadata->value != NULL) {
      astTreeDelete(metadata->value);
    }
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_TYPE_FUNCTION: {
    AstTreeTypeFunction *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      astTreeDelete(metadata->arguments[i]);
    }
    astTreeDelete(metadata->returnType);
    free(metadata->arguments);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree.metadata;
    astTreeDelete(metadata->function);
    for (size_t i = 0; i < metadata->parameters_size; ++i) {
      astTreeDelete(metadata->parameters[i]);
    }
    free(metadata->parameters);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_VARIABLE: {
    // AstTreeIdentifier *metadata = tree.metadata; // not needed
  }
    return;
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreeInfix *metadata = tree.metadata;
    astTreeDestroy(metadata->left);
    astTreeDestroy(metadata->right);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", tree.token);
  exit(1);
}

void astTreeVariableDestroy(AstTreeVariable variable) {
  if (variable.value != NULL) {
    astTreeDelete(variable.value);
  }
  if (variable.type != NULL) {
    astTreeDelete(variable.type);
  }
}

void astTreeVariableDelete(AstTreeVariable *variable) {
  astTreeVariableDestroy(*variable);
  free(variable);
}

void astTreeDelete(AstTree *tree) {
  if (tree != &AST_TREE_TYPE_TYPE && tree != &AST_TREE_VOID_TYPE &&
      tree != &AST_TREE_U64_TYPE) {
    return;
  }
  astTreeDestroy(*tree);
  free(tree);
}

void astTreeRootDelete(AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    astTreeVariableDestroy(*root->variables.data[i]);
    free(root->variables.data[i]);
  }
  free(root->variables.data);
  free(root);
}

AstTree *newAstTree(AstTreeToken token, void *metadata, AstTree *type) {
  AstTree *result = a404m_malloc(sizeof(*result));
  result->token = token;
  result->metadata = metadata;
  result->type = type;
  return result;
}

AstTree *copyAstTree(AstTree *tree) {
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
    return &AST_TREE_TYPE_TYPE;
  case AST_TREE_TOKEN_TYPE_VOID:
    return &AST_TREE_VOID_TYPE;
  case AST_TREE_TOKEN_TYPE_U64:
    return &AST_TREE_U64_TYPE;
  case AST_TREE_TOKEN_VALUE_U64:
    return newAstTree(tree->token, (void *)(AstTreeU64)tree->metadata,
                      &AST_TREE_U64_TYPE);
  case AST_TREE_TOKEN_VARIABLE:
    return newAstTree(tree->token, tree->metadata, copyAstTree(tree->type));
  case AST_TREE_TOKEN_TYPE_FUNCTION: {
    AstTreeTypeFunction *metadata = tree->metadata;
    AstTreeTypeFunction *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->returnType = copyAstTree(metadata->returnType);
    new_metadata->arguments_size = metadata->arguments_size;
    new_metadata->arguments =
        a404m_malloc(sizeof(*new_metadata) * new_metadata->arguments_size);
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      new_metadata->arguments[i] = copyAstTree(metadata->arguments[i]);
    }
    return newAstTree(tree->token, new_metadata, &AST_TREE_TYPE_TYPE);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", tree->token);
  exit(1);
}

AstTreeRoot *makeAstTree(ParserNode *parsedRoot) {
  if (parsedRoot->token != PARSER_TOKEN_ROOT) {
    return NULL;
  }

  ParserNodeArray *nodes = parsedRoot->metadata;

  AstTreeRoot *root = a404m_malloc(sizeof(*root));

  root->variables.data =
      a404m_malloc(nodes->size * sizeof(*root->variables.data));
  root->variables.size = 0;

  for (size_t i = 0; i < nodes->size; ++i) {
    ParserNode *eol = nodes->data[i];
    if (eol->token != PARSER_TOKEN_SYMBOL_EOL) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    ParserNode *node = (ParserNodeSingleChildMetadata *)eol->metadata;
    if (node->token != PARSER_TOKEN_CONSTANT &&
        node->token != PARSER_TOKEN_VARIABLE) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    }
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
    variable->name_begin = node_metadata->name->str_begin;
    variable->name_end = node_metadata->name->str_end;
    variable->isConst = node->token == PARSER_TOKEN_CONSTANT;

    if (!pushVariable(&root->variables, variable)) {
      astTreeVariableDelete(variable);
      goto RETURN_ERROR;
    }
  }

  AstTreeVariables *variables = &root->variables;
  constexpr size_t variables_size = 1;

  for (size_t i = 0; i < nodes->size; ++i) {
    ParserNode *node =
        (ParserNodeSingleChildMetadata *)nodes->data[i]->metadata;
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    if (node->token != PARSER_TOKEN_CONSTANT &&
        node->token != PARSER_TOKEN_VARIABLE) {
      printLog("Only variables are allowed here");
      goto RETURN_ERROR;
    }

    if (node_metadata->value == NULL) {
      printLog("Variables must be initialized");
      goto RETURN_ERROR;
    }

    switch (node_metadata->value->token) {
    case PARSER_TOKEN_VALUE_U64:
    case PARSER_TOKEN_FUNCTION_DEFINITION:
      goto AFTER_SWITCH;
    case PARSER_TOKEN_ROOT:
    case PARSER_TOKEN_IDENTIFIER:
    case PARSER_TOKEN_TYPE_TYPE:
    case PARSER_TOKEN_TYPE_FUNCTION:
    case PARSER_TOKEN_TYPE_VOID:
    case PARSER_TOKEN_TYPE_U64:
    case PARSER_TOKEN_KEYWORD_PRINT_U64:
    case PARSER_TOKEN_KEYWORD_RETURN:
    case PARSER_TOKEN_CONSTANT:
    case PARSER_TOKEN_VARIABLE:
    case PARSER_TOKEN_SYMBOL_EOL:
    case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    case PARSER_TOKEN_SYMBOL_PARENTHESIS:
    case PARSER_TOKEN_SYMBOL_COMMA:
    case PARSER_TOKEN_FUNCTION_CALL:
    case PARSER_TOKEN_OPERATOR_ASSIGN:
    case PARSER_TOKEN_OPERATOR_SUM:
      printLog("Should not be here");
      goto RETURN_ERROR;
    case PARSER_TOKEN_NONE:
    }
    printLog("Bad token %d", node_metadata->value->token);
    goto RETURN_ERROR;

  AFTER_SWITCH:

    AstTree *value =
        astTreeParse(node_metadata->value, &variables, variables_size);
    if (value == NULL) {
      goto RETURN_ERROR;
    }

    AstTree *type;
    if (node_metadata->type != NULL) {
      type = astTreeParse(node_metadata->type, &variables, variables_size);
      if (type == NULL) {
        goto RETURN_ERROR;
      }
    } else {
      type = NULL;
    }

    root->variables.data[i]->type = type;
    root->variables.data[i]->value = value;
  }

  if (!setAllTypesRoot(root) || !astTreeCleanRoot(root)) {
    goto RETURN_ERROR;
  }

  return root;

RETURN_ERROR:
  free(root->variables.data);
  free(root);
  return NULL;
}

bool pushVariable(AstTreeVariables *variables, AstTreeVariable *variable) {
  for (size_t j = 0; j < variables->size; ++j) {
    char *var_begin = variables->data[j]->name_begin;
    char *var_end = variables->data[j]->name_end;

    if (variable->name_end - variable->name_begin == var_end - var_begin &&
        strncmp(var_begin, variable->name_begin,
                variable->name_end - variable->name_begin) == 0) {
      printLog("Variable exists");
      return false;
    }
  }

  size_t variables_size =
      a404m_malloc_usable_size(variables->data) / sizeof(*variables->data);
  if (variables_size == variables->size) {
    variables_size += variables_size / 2 + 1;
    variables->data = a404m_realloc(variables->data,
                                    variables_size * sizeof(*variables->data));
  }
  variables->data[variables->size] = variable;
  variables->size += 1;
  return true;
}

AstTreeVariable *getVariable(AstTreeVariables **variables,
                             size_t variables_size, char *name_begin,
                             char *name_end) {
  for (size_t i = variables_size - 1; i != (size_t)-1ULL; --i) {
    AstTreeVariables vars = *variables[i];
    for (size_t j = 0; j < vars.size; ++j) {
      char *var_begin = vars.data[j]->name_begin;
      char *var_end = vars.data[j]->name_end;

      if (name_end - name_begin == var_end - var_begin &&
          strncmp(var_begin, name_begin, name_end - name_begin) == 0) {
        return vars.data[j];
      }
    }
  }

  return NULL;
}

AstTree *astTreeParse(ParserNode *parserNode, AstTreeVariables **variables,
                      size_t variables_size) {
  switch (parserNode->token) {
  case PARSER_TOKEN_FUNCTION_DEFINITION:
    return astTreeParseFunction(parserNode, variables, variables_size);
  case PARSER_TOKEN_TYPE_TYPE:
    return &AST_TREE_TYPE_TYPE;
  case PARSER_TOKEN_TYPE_FUNCTION:
    return astTreeParseTypeFunction(parserNode, variables, variables_size);
  case PARSER_TOKEN_TYPE_VOID:
    return &AST_TREE_VOID_TYPE;
  case PARSER_TOKEN_TYPE_U64:
    return &AST_TREE_U64_TYPE;
  case PARSER_TOKEN_FUNCTION_CALL:
    return astTreeParseFunctionCall(parserNode, variables, variables_size);
  case PARSER_TOKEN_IDENTIFIER:
    return astTreeParseIdentifier(parserNode, variables, variables_size);
  case PARSER_TOKEN_VALUE_U64:
    return newAstTree(
        AST_TREE_TOKEN_VALUE_U64,
        (void *)(AstTreeU64)(ParserNodeU64Metadata)parserNode->metadata,
        &AST_TREE_U64_TYPE);
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
    return astTreeParsePrintU64(parserNode, variables, variables_size);
  case PARSER_TOKEN_KEYWORD_RETURN:
    return astTreeParseReturn(parserNode, variables, variables_size);
  case PARSER_TOKEN_OPERATOR_ASSIGN:
    return astTreeParseAssign(parserNode, variables, variables_size);
  case PARSER_TOKEN_OPERATOR_SUM:
    return astTreeParseSum(parserNode, variables, variables_size);
  case PARSER_TOKEN_VARIABLE:
    return astTreeParseVariable(parserNode, variables, variables_size);
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_SYMBOL_EOL:
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_NONE:
  case PARSER_TOKEN_ROOT:
  }
  printLog("Bad token %d", parserNode->token);
  return NULL;
}

AstTree *astTreeParseFunction(ParserNode *parserNode,
                              AstTreeVariables **p_variables,
                              size_t p_variables_size) {
  ParserNodeFunctionDefnitionMetadata *node_metadata = parserNode->metadata;
  ParserNodeArray *node_arguments = node_metadata->arguments->metadata;
  ParserNodeArray *body = node_metadata->body->metadata;

  size_t expressions_size = 0;
  AstTreeScope scope = {
      .expressions =
          a404m_malloc(expressions_size * sizeof(*scope.expressions)),
      .expressions_size = 0,
      .variables.data = a404m_malloc(0),
      .variables.size = 0,
  };

  AstTreeFunction *function = a404m_malloc(sizeof(*function));

  function->arguments.data = a404m_malloc(0);
  function->arguments.size = 0;

  size_t variables_size = p_variables_size + 2;
  AstTreeVariables *variables[variables_size];
  for (size_t i = 0; i < p_variables_size; ++i) {
    variables[i] = p_variables[i];
  }
  variables[variables_size - 2] = &function->arguments;
  variables[variables_size - 1] = &scope.variables;

  if ((function->returnType = astTreeParse(node_metadata->returnType, variables,
                                           variables_size)) == NULL) {
    goto RETURN_ERROR;
  }

  for (size_t i = 0; i < node_arguments->size; ++i) {
    ParserNode *arg = node_arguments->data[i];
    if (arg->token == PARSER_TOKEN_SYMBOL_COMMA) {
      arg = (ParserNodeSingleChildMetadata *)arg->metadata;
    }

    ParserNodeVariableMetadata *arg_metadata = arg->metadata;
    if (arg_metadata->value != NULL) {
      printLog("arguments can't have default values (for now)");
      goto RETURN_ERROR;
    }

    AstTree *type = astTreeParse(arg_metadata->type, variables, variables_size);
    if (type == NULL) {
      goto RETURN_ERROR;
    }

    AstTreeVariable *argument = a404m_malloc(sizeof(*argument));
    argument->value = NULL;
    argument->type = type;
    argument->name_begin = arg_metadata->name->str_begin;
    argument->name_end = arg_metadata->name->str_end;
    argument->isConst = false; // all arguments are not constants

    if (!pushVariable(&function->arguments, argument)) {
      astTreeVariableDelete(argument);
      goto RETURN_ERROR;
    }
  }

  for (size_t i = 0; i < body->size; ++i) {
    ParserNode *eol = body->data[i];
    if (eol->token != PARSER_TOKEN_SYMBOL_EOL) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    ParserNode *node = (ParserNodeSingleChildMetadata *)eol->metadata;

    if (node->token == PARSER_TOKEN_CONSTANT) {
      if (!astTreeParseConstant(node, variables, variables_size)) {
        goto RETURN_ERROR;
      }
    } else {
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
  }

  scope.expressions = a404m_realloc(
      scope.expressions, scope.expressions_size * sizeof(*scope.expressions));

  function->scope = scope;

  AstTree *result = newAstTree(AST_TREE_TOKEN_FUNCTION, function, NULL);

  return result;

RETURN_ERROR:
  free(function);
  free(scope.expressions);
  return NULL;
}

AstTree *astTreeParseTypeFunction(ParserNode *parserNode,
                                  AstTreeVariables **variables,
                                  size_t variables_size) {
  ParserNodeTypeFunctionMetadata *metadata = parserNode->metadata;
  ParserNodeArray *node_arguments = metadata->arguments->metadata;

  AstTreeTypeFunction *typeFunction = a404m_malloc(sizeof(*typeFunction));

  size_t arguments_size = 0;
  typeFunction->arguments =
      a404m_malloc(arguments_size * sizeof(*typeFunction->arguments));
  typeFunction->arguments_size = 0;

  for (size_t i = 0; i < node_arguments->size; ++i) {
    ParserNode *argument = node_arguments->data[i];

    if (argument->token == PARSER_TOKEN_SYMBOL_COMMA) {
      argument = (ParserNodeSingleChildMetadata *)argument->metadata;
    }

    ParserNodeVariableMetadata *arg_metadata = argument->metadata;
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

  return newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, typeFunction,
                    &AST_TREE_TYPE_TYPE);

RETURN_ERROR:
  return NULL;
}

AstTree *astTreeParseFunctionCall(ParserNode *parserNode,
                                  AstTreeVariables **variables,
                                  size_t variables_size) {
  ParserNodeFunctionCall *node_metadata = parserNode->metadata;
  AstTree *function =
      astTreeParse(node_metadata->function, variables, variables_size);
  if (function == NULL) {
    return NULL;
  }
  if (function->token != AST_TREE_TOKEN_VARIABLE) {
    printLog("Not yet supported");
    return NULL;
  }

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));
  metadata->function = function;

  metadata->parameters =
      a404m_malloc(sizeof(*metadata->parameters) * node_metadata->params->size);
  metadata->parameters_size = node_metadata->params->size;

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    ParserNode *param = node_metadata->params->data[i];
    if (param->token == PARSER_TOKEN_SYMBOL_COMMA) {
      param = (ParserNodeSingleChildMetadata *)param->metadata;
    }
    metadata->parameters[i] =
        astTreeParse(param, variables, variables_size);
  }

  return newAstTree(AST_TREE_TOKEN_FUNCTION_CALL, metadata, NULL);
}

AstTree *astTreeParseIdentifier(ParserNode *parserNode,
                                AstTreeVariables **variables,
                                size_t variables_size) {
  AstTreeVariable *var = getVariable(
      variables, variables_size, parserNode->str_begin, parserNode->str_end);
  if (var == NULL) {
    printLog("Variable not found");
    return NULL;
  }
  return newAstTree(AST_TREE_TOKEN_VARIABLE, var, NULL);
}

AstTree *astTreeParsePrintU64(ParserNode *parserNode,
                              AstTreeVariables **variables,
                              size_t variables_size) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTree *operand = astTreeParse(node_metadata, variables, variables_size);
  if (operand == NULL) {
    return NULL;
  }

  return newAstTree(AST_TREE_TOKEN_KEYWORD_PRINT_U64,
                    (AstTreeSingleChild *)operand, NULL);
}

AstTree *astTreeParseReturn(ParserNode *parserNode,
                            AstTreeVariables **variables,
                            size_t variables_size) {
  ParserNodeReturnMetadata *node_metadata = parserNode->metadata;

  AstTree *value;
  if (node_metadata->value == NULL) {
    value = NULL;
  } else {
    value = astTreeParse(node_metadata->value, variables, variables_size);
    if (value == NULL) {
      return NULL;
    }
  }

  AstTreeReturn *metadata = a404m_malloc(sizeof(*metadata));
  metadata->value = value;

  return newAstTree(AST_TREE_TOKEN_KEYWORD_RETURN, metadata, NULL);
}

AstTree *astTreeParseAssign(ParserNode *parserNode,
                            AstTreeVariables **variables,
                            size_t variables_size) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *left = astTreeParse(node_metadata->left, variables, variables_size);
  if (left == NULL) {
    return NULL;
  }
  AstTree *right =
      astTreeParse(node_metadata->right, variables, variables_size);
  if (right == NULL) {
    return NULL;
  }

  AstTreeInfix *metadata = a404m_malloc(sizeof(*metadata));

  metadata->left = *left;
  metadata->right = *right;

  free(left);
  free(right);

  return newAstTree(AST_TREE_TOKEN_OPERATOR_ASSIGN, metadata, NULL);
}

AstTree *astTreeParseSum(ParserNode *parserNode, AstTreeVariables **variables,
                         size_t variables_size) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *left = astTreeParse(node_metadata->left, variables, variables_size);
  if (left == NULL) {
    return NULL;
  }
  AstTree *right =
      astTreeParse(node_metadata->right, variables, variables_size);
  if (right == NULL) {
    return NULL;
  }

  AstTreeInfix *metadata = a404m_malloc(sizeof(*metadata));

  metadata->left = *left;
  metadata->right = *right;

  free(left);
  free(right);

  return newAstTree(AST_TREE_TOKEN_OPERATOR_SUM, metadata, NULL);
}

bool astTreeParseConstant(ParserNode *parserNode, AstTreeVariables **variables,
                          size_t variables_size) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->value == NULL ||
      node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
    printLog("Not supported");
    return NULL;
  }

  AstTree *value =
      astTreeParse(node_metadata->value, variables, variables_size);
  if (value == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *type;
  if (node_metadata->type == NULL) {
    type = NULL;
  } else {
    type = astTreeParse(node_metadata->type, variables, variables_size);
    if (type == NULL) {
      goto RETURN_ERROR;
    }
  }

  AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
  variable->type = type;
  variable->value = value;
  variable->name_begin = node_metadata->name->str_begin;
  variable->name_end = node_metadata->name->str_end;
  variable->isConst = true;

  if (!pushVariable(variables[variables_size - 1], variable)) {
    astTreeVariableDelete(variable);
    goto RETURN_ERROR;
  }

  return true;
RETURN_ERROR:
  return false;
}

AstTree *astTreeParseVariable(ParserNode *parserNode,
                              AstTreeVariables **variables,
                              size_t variables_size) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->value == NULL ||
      node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
    printLog("Not supported");
    return NULL;
  }

  AstTree *value =
      astTreeParse(node_metadata->value, variables, variables_size);
  if (value == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *type;
  if (node_metadata->type == NULL) {
    type = NULL;
  } else {
    type = astTreeParse(node_metadata->type, variables, variables_size);
    if (type == NULL) {
      goto RETURN_ERROR;
    }
  }

  AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
  variable->type = type;
  variable->value = value;
  variable->name_begin = node_metadata->name->str_begin;
  variable->name_end = node_metadata->name->str_end;
  variable->isConst = false;

  if (!pushVariable(variables[variables_size - 1], variable)) {
    astTreeVariableDelete(variable);
    goto RETURN_ERROR;
  }

  return newAstTree(AST_TREE_TOKEN_VARIABLE_DEFINE, variable, NULL);
RETURN_ERROR:
  return NULL;
}

AstTreeFunction *getFunction(AstTree *value) {
  switch (value->token) {
  case AST_TREE_TOKEN_FUNCTION:
    return value->metadata;
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = value->metadata;
    AstTreeFunction *function = metadata->function->metadata;
    return getFunction(function->returnType);
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = value->metadata;
    if (metadata->value->token == AST_TREE_TOKEN_FUNCTION) {
      return metadata->value->metadata;
    } else {
      return NULL;
    }
  }
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
    return NULL;
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", value->token);
  exit(1);
}

bool isConst(AstTree *value) {
  switch (value->token) {
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_VALUE_U64:
    return true;
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
    return false;
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = value->metadata;
    return metadata->isConst;
  }
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", value->token);
  exit(1);
}

AstTree *makeTypeOf(AstTree *value) {
  switch (value->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
    return &AST_TREE_TYPE_TYPE;
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = value->metadata;
    AstTreeFunction *function = metadata->function->metadata;
    return copyAstTree(function->returnType);
  }
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *function = value->metadata;

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = function->arguments.size;
    type_metadata->arguments = a404m_malloc(function->arguments.size *
                                            sizeof(*type_metadata->arguments));
    type_metadata->returnType = copyAstTree(function->returnType);

    for (size_t i = 0; i < function->arguments.size; ++i) {
      type_metadata->arguments[i] =
          copyAstTree(function->arguments.data[i]->type);
    }

    return newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                      &AST_TREE_TYPE_TYPE);
  }
  case AST_TREE_TOKEN_VALUE_U64:
    return &AST_TREE_U64_TYPE;
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = value->metadata;
    return copyAstTree(variable->type);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM: {
    AstTreeInfix *metadata = value->metadata;
    return copyAstTree(metadata->left.type);
  }
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", value->token);
  exit(1);
}

bool typeIsEqual(const AstTree *type0, const AstTree *type1) {
  switch (type0->token) {
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
    return false;
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
    return type1->token == type0->token;
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    if (type1->token != AST_TREE_TOKEN_TYPE_FUNCTION) {
      return false;
    }
    AstTreeTypeFunction *type0_metadata = type0->metadata;
    AstTreeTypeFunction *type1_metadata = type1->metadata;
    if (!typeIsEqual(type0_metadata->returnType->type,
                     type1_metadata->returnType->type)) {
      return false;
    }
    if (type0_metadata->arguments_size != type1_metadata->arguments_size) {
      return false;
    }
    for (size_t i = 0; i < type0_metadata->arguments_size; ++i) {
      if (!setAllTypes(type0_metadata->arguments[i], NULL)) {
        return false;
      }
    }
    return true;
  case AST_TREE_TOKEN_FUNCTION_CALL:
    printLog("Not implemented yet");
    exit(1);
  case AST_TREE_TOKEN_VARIABLE:
    return type1->token == AST_TREE_TOKEN_VARIABLE &&
           type0->metadata == type1->metadata;
  case AST_TREE_TOKEN_NONE:
    break;
  }
  printLog("Bad token '%d'", type0->token);
  exit(1);
}

bool setAllTypesRoot(AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    if (!setTypesAstVariable(variable)) {
      return false;
    }
  }

  return true;
}

bool setAllTypes(AstTree *tree, AstTreeFunction *function) {
  if (tree->type != NULL) {
    return true;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    return true;
  case AST_TREE_TOKEN_FUNCTION:
    return setTypesFunction(tree);
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
    return setTypesPrintU64(tree);
  case AST_TREE_TOKEN_KEYWORD_RETURN:
    return setTypesReturn(tree, function);
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    return setTypesTypeFunction(tree);
  case AST_TREE_TOKEN_FUNCTION_CALL:
    return setTypesFunctionCall(tree);
  case AST_TREE_TOKEN_VARIABLE:
    return setTypesVariable(tree);
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
    return setTypesOperatorAssign(tree);
  case AST_TREE_TOKEN_OPERATOR_SUM:
    return setTypesOperatorSum(tree);
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", tree->token);
  exit(1);
}

bool setTypesFunction(AstTree *tree) {
  AstTreeFunction *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->arguments.size; ++i) {
    AstTreeVariable *variable = metadata->arguments.data[i];
    if (!setTypesAstVariable(variable)) {
      return false;
    }
  }

  if (!setAllTypes(metadata->returnType, NULL)) {
    return false;
  }

  tree->type = makeTypeOf(tree);

  for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
    AstTree *expression = &metadata->scope.expressions[i];
    if (!setAllTypes(expression, metadata)) {
      return false;
    }
  }

  for (size_t i = 0; i < metadata->scope.variables.size; ++i) {
    if (!setTypesAstVariable(metadata->scope.variables.data[i])) {
      return false;
    }
  }

  return true;
}

bool setTypesPrintU64(AstTree *tree) {
  AstTreeSingleChild *metadata = tree->metadata;
  if (!setAllTypes(metadata, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->type, &AST_TREE_U64_TYPE)) {
    printLog("Type mismatch");
    return false;
  } else {
    tree->type = &AST_TREE_VOID_TYPE;
    return true;
  }
}

bool setTypesReturn(AstTree *tree, AstTreeFunction *function) {
  AstTreeReturn *metadata = tree->metadata;
  if (metadata->value != NULL) {
    if (!setAllTypes(metadata->value, NULL)) {
      return false;
    } else if (!typeIsEqual(metadata->value->type, function->returnType)) {
      printLog("Type mismatch");
      return false;
    }
  }
  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesTypeFunction(AstTree *tree) {
  AstTreeTypeFunction *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->arguments_size; ++i) {
    printLog("Not yet supported");
    return false;
  }

  if (!setAllTypes(metadata->returnType, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->returnType->type, &AST_TREE_TYPE_TYPE)) {
    printLog("Type mismatch");
    return false;
  }

  tree->type = &AST_TREE_TYPE_TYPE;
  return true;
}

bool setTypesFunctionCall(AstTree *tree) {
  AstTreeFunctionCall *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    if (!setAllTypes(metadata->parameters[i], NULL)) {
      return false;
    }
  }

  if (metadata->function->token != AST_TREE_TOKEN_VARIABLE) {
    printLog("Not yet supported");
    return false;
  } else if (!setAllTypes(metadata->function, NULL)) {
    return false;
  }

  AstTreeFunction *function = getFunction(metadata->function);
  if (function == NULL ||
      function->arguments.size != metadata->parameters_size) {
    printLog("Arguments doesn't match %ld != %ld", function->arguments.size,
             metadata->parameters_size);
    return NULL;
  }

  tree->type = copyAstTree(function->returnType);
  return true;
}

bool setTypesVariable(AstTree *tree) {
  AstTreeVariable *metadata = tree->metadata;
  if (setTypesAstVariable(metadata)) {
    tree->type = copyAstTree(metadata->type);
    return true;
  } else {
    return false;
  }
}

bool setTypesOperatorAssign(AstTree *tree) {
  AstTreeInfix *infix = tree->metadata;
  if (!setTypesAstInfix(infix)) {
    return false;
  } else if (!typeIsEqual(infix->left.type, infix->right.type)) {
    printLog("Type mismatch");
    return false;
  } else if (isConst(&infix->left)) {
    printLog("Constants can't be assigned");
    return false;
  } else {
    tree->type = copyAstTree(infix->left.type);
    return true;
  }
}

bool setTypesOperatorSum(AstTree *tree) {
  AstTreeInfix *infix = tree->metadata;
  if (!setTypesAstInfix(infix)) {
    return false;
  } else if (!typeIsEqual(infix->left.type, infix->right.type)) {
    printLog("Type mismatch");
    return false;
  } else {
    tree->type = copyAstTree(infix->left.type);
    return true;
  }
}

bool setTypesAstVariable(AstTreeVariable *variable) {
  if (variable->value != NULL && !setAllTypes(variable->value, NULL)) {
    return false;
  }

  if (variable->type == NULL) {
    variable->type = copyAstTree(variable->value->type);
  }

  if (variable->type == NULL) {
    return false;
  } else if (variable->value != NULL &&
             !typeIsEqual(variable->value->type, variable->type)) {
    printLog("Type mismatch");
    return false;
  }

  return true;
}

bool setTypesAstInfix(AstTreeInfix *infix) {
  return setAllTypes(&infix->left, NULL) && setAllTypes(&infix->right, NULL);
}

bool astTreeCleanRoot(AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    astTreeClean(root->variables.data[i]->value);
    astTreeClean(root->variables.data[i]->type);
  }
  return true;
}

bool astTreeClean(AstTree *tree) {
  switch (tree->token) {
  case AST_TREE_TOKEN_VARIABLE:
    return astTreeCleanVariable(tree);
  case AST_TREE_TOKEN_FUNCTION:
    return astTreeCleanFunction(tree);
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
    return true;
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token '%d'", tree->token);
  exit(1);
}

bool astTreeCleanFunction(AstTree *tree) {
  AstTreeFunction *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->arguments.size; ++i) {
    if (metadata->arguments.data[i]->value != NULL &&
        !astTreeClean(metadata->arguments.data[i]->value)) {
      return false;
    }
  }

  if (!astTreeClean(metadata->returnType)) {
    return false;
  }

  for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
    AstTree *expression = &metadata->scope.expressions[i];
    if (!astTreeClean(expression)) {
      return false;
    }
  }

  for (size_t i = 0; i < metadata->scope.variables.size; ++i) {
    if (!astTreeCleanAstVariable(metadata->scope.variables.data[i])) {
      return false;
    }
  }

  return true;
}

bool astTreeCleanVariable(AstTree *tree) {
  return astTreeCleanAstVariable(tree->metadata);
}

bool astTreeCleanAstVariable(AstTreeVariable *variable) {
  if (!astTreeClean(variable->value) || !astTreeClean(variable->type)) {
    return false;
  }
  AstTree *value = variable->value;

  while (value->token == AST_TREE_TOKEN_VARIABLE) {
    AstTreeVariable *value_metadata = value->metadata;
    if (!value_metadata->isConst) {
      break;
    }
    AstTree *newValue = value_metadata->value;
    astTreeDelete(value);
    value = newValue;
  }

  if (variable->value != value) {
    variable->value = copyAstTree(value);
  }

  if (variable->isConst && !isConst(value)) {
    return false;
  }

  return true;
}

size_t astTreeTypeSize(AstTree tree) {
  switch (tree.token) {
  case AST_TREE_TOKEN_TYPE_U64:
    return 8;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}
