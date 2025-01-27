#include "ast-tree.h"
#include "compiler/parser.h"
#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *AST_TREE_TOKEN_STRINGS[] = {
    "AST_TREE_TOKEN_FUNCTION",

    "AST_TREE_TOKEN_KEYWORD_PRINT_U64",

    "AST_TREE_TOKEN_TYPE_FUNCTION",
    "AST_TREE_TOKEN_TYPE_VOID",
    "AST_TREE_TOKEN_TYPE_U64",

    "AST_TREE_TOKEN_FUNCTION_CALL",
    "AST_TREE_TOKEN_VARIABLE",
    "AST_TREE_TOKEN_VALUE_U64",

    "AST_TREE_TOKEN_VARIABLE_DEFINE",

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
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
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
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = tree->metadata;
    printf(",variable.name=%.*s",
           (int)(metadata->name_end - metadata->name_begin),
           metadata->name_begin);
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
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_VALUE_U64:
    return;
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64: {
    AstTreeSingleChild *metadata = tree.metadata;
    astTreeDelete(metadata);
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
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VARIABLE: {
    // AstTreeIdentifier *metadata = tree.metadata; // not needed
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
    if (node->token != PARSER_TOKEN_CONSTANT) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    }
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
    variable->name_begin = node_metadata->name->str_begin;
    variable->name_end = node_metadata->name->str_end;

    pushVariable(&root->variables, variable);
  }

  for (size_t i = 0; i < nodes->size; ++i) {
    ParserNode *node =
        (ParserNodeSingleChildMetadata *)nodes->data[i]->metadata;
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    AstTree *type;
    if (node_metadata->type != NULL) {
      AstTreeVariables *variables = &root->variables;
      type = astTreeParse(node_metadata->type, &variables, 1);
      if (type == NULL) {
        goto RETURN_ERROR;
      }
    } else {
      type = NULL; // TODO: change this to find type
    }

    switch (node_metadata->value->token) {
    case PARSER_TOKEN_VALUE_U64:
    case PARSER_TOKEN_FUNCTION_DEFINITION:
      goto AFTER_SWITCH;
    case PARSER_TOKEN_ROOT:
    case PARSER_TOKEN_IDENTIFIER:
    case PARSER_TOKEN_TYPE_FUNCTION:
    case PARSER_TOKEN_TYPE_VOID:
    case PARSER_TOKEN_TYPE_U64:
    case PARSER_TOKEN_KEYWORD_PRINT_U64:
    case PARSER_TOKEN_CONSTANT:
    case PARSER_TOKEN_SYMBOL_EOL:
    case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    case PARSER_TOKEN_SYMBOL_PARENTHESIS:
    case PARSER_TOKEN_SYMBOL_COMMA:
    case PARSER_TOKEN_FUNCTION_CALL:
    case PARSER_TOKEN_NONE:
      printLog("Should not be here");
      goto RETURN_ERROR;
    }
    printLog("Bad token %d", node_metadata->value->token);
    goto RETURN_ERROR;

  AFTER_SWITCH:
    AstTreeVariables *variables = &root->variables;
    AstTree *value = astTreeParse(node_metadata->value, &variables, 1);
    if (value == NULL) {
      goto RETURN_ERROR;
    }

    // TODO: then delete type != NULL
    if (type != NULL && !hasTypeOf(value, type)) {
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

void pushVariable(AstTreeVariables *variables, AstTreeVariable *variable) {
  size_t variables_size =
      a404m_malloc_usable_size(variables->data) / sizeof(*variables->data);
  if (variables_size == variables->size) {
    variables_size += variables_size / 2 + 1;
    variables->data = a404m_realloc(variables->data,
                                    variables_size * sizeof(*variables->data));
  }
  variables->data[variables->size] = variable;
  variables->size += 1;
}

AstTreeVariable *getVariable(AstTreeVariables **variables,
                             size_t variables_size, char *name_begin,
                             char *name_end) {
  for (size_t i = 0; i < variables_size; ++i) {
    AstTreeVariables vars = *variables[i];
    for (size_t j = vars.size - 1; j != (size_t)-1ULL; --j) {
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
  case PARSER_TOKEN_TYPE_FUNCTION:
    return astTreeParseTypeFunction(parserNode, variables, variables_size);
  case PARSER_TOKEN_TYPE_VOID:
    return newAstTree(AST_TREE_TOKEN_TYPE_VOID, NULL);
  case PARSER_TOKEN_TYPE_U64:
    return newAstTree(AST_TREE_TOKEN_TYPE_U64, NULL);
  case PARSER_TOKEN_FUNCTION_CALL:
    return astTreeParseFunctionCall(parserNode, variables, variables_size);
  case PARSER_TOKEN_IDENTIFIER:
    return astTreeParseIdentifier(parserNode, variables, variables_size);
  case PARSER_TOKEN_VALUE_U64:
    return newAstTree(
        AST_TREE_TOKEN_VALUE_U64,
        (void *)(AstTreeU64)(ParserNodeU64Metadata)parserNode->metadata);
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
    return astTreeParsePrintU64(parserNode, variables, variables_size);
  case PARSER_TOKEN_CONSTANT:
    return astTreeParseConstant(parserNode, variables, variables_size);
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

    pushVariable(&function->arguments, argument);
  }

  for (size_t i = 0; i < body->size; ++i) {
    ParserNode *eol = body->data[i];
    if (eol->token != PARSER_TOKEN_SYMBOL_EOL) {
      printLog("Unexpected %s", PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    ParserNode *node = (ParserNodeSingleChildMetadata *)eol->metadata;
    AstTree *tree = astTreeParse(node, variables, variables_size);

    if (tree == NULL) {
      goto RETURN_ERROR;
    } else if (tree->token == AST_TREE_TOKEN_VARIABLE_DEFINE) {
      astTreeDelete(tree);
      continue;
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

  return newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, typeFunction);

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
  // TODO: check types here

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));
  metadata->function = function;

  metadata->parameters =
      a404m_malloc(sizeof(*metadata->parameters) * node_metadata->params->size);
  metadata->parameters_size = node_metadata->params->size;

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    printLog("Not impelemented yet");
    exit(1);
    if ((metadata->parameters[i] = astTreeParse(node_metadata->params->data[i],
                                                variables, variables_size)) ==
            NULL ||
        hasTypeOf(metadata->parameters[i], function->metadata)) {
      goto RETURN_ERROR;
    }
  }

  return newAstTree(AST_TREE_TOKEN_FUNCTION_CALL, metadata);
RETURN_ERROR:
  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    astTreeDelete(metadata->parameters[i]);
  }
  free(metadata->parameters);
  return NULL;
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
  return newAstTree(AST_TREE_TOKEN_VARIABLE, var);
}

AstTree *astTreeParsePrintU64(ParserNode *parserNode,
                              AstTreeVariables **variables,
                              size_t variables_size) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTree *operand = astTreeParse(node_metadata, variables, variables_size);
  if (operand == NULL) {
    return NULL;
  }

  // TODO: check type to be u64

  return newAstTree(AST_TREE_TOKEN_KEYWORD_PRINT_U64,
                    (AstTreeSingleChild *)operand);
}

AstTree *astTreeParseConstant(ParserNode *parserNode,
                              AstTreeVariables **variables,
                              size_t variables_size) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->value == NULL || node_metadata->type == NULL ||
      node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
    printLog("Not yet supported");
    return NULL;
  }

  AstTree *type = astTreeParse(node_metadata->type, variables, variables_size);
  if (type == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *value =
      astTreeParse(node_metadata->value, variables, variables_size);
  if (value == NULL) {
    goto RETURN_ERROR;
  }

  AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
  variable->type = type;
  variable->value = value;
  variable->name_begin = node_metadata->name->str_begin;
  variable->name_end = node_metadata->name->str_end;

  pushVariable(variables[variables_size - 1], variable);

  return newAstTree(AST_TREE_TOKEN_VARIABLE_DEFINE, variable);
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
      if (typeMetadata->arguments_size != valueMetadata->arguments.size) {
        return false;
      }
      for (size_t i = 0; i < typeMetadata->arguments_size; ++i) {
        if (!typeIsEqual(typeMetadata->arguments[i],
                         valueMetadata->arguments.data[i]->type)) {
          return false;
        }
      }
      return typeIsEqual(typeMetadata->returnType, valueMetadata->returnType);
    }
    case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
    case AST_TREE_TOKEN_VALUE_U64:
    case AST_TREE_TOKEN_TYPE_FUNCTION:
    case AST_TREE_TOKEN_TYPE_VOID:
    case AST_TREE_TOKEN_TYPE_U64:
    case AST_TREE_TOKEN_FUNCTION_CALL:
    case AST_TREE_TOKEN_VARIABLE:
    case AST_TREE_TOKEN_VARIABLE_DEFINE:
      return false;
    case AST_TREE_TOKEN_NONE:
    }
    goto ERROR;
  case AST_TREE_TOKEN_TYPE_U64:
    return value->token == AST_TREE_TOKEN_VALUE_U64;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    return false;
  case AST_TREE_TOKEN_NONE:
  }
ERROR:
  printLog("Bad token '%d'", type->token);
  exit(1);
}

bool typeIsEqual(AstTree *type0, AstTree *type1) {
  switch (type0->token) {
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_VALUE_U64:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    return false;
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_U64:
    return type1->token == type0->token;
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    if (type1->token != AST_TREE_TOKEN_TYPE_FUNCTION) {
      return false;
    }
    printLog("Not implemented yet");
    exit(1);
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
