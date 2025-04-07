#include "ast-tree.h"

#include "compiler/parser.h"
#include "runner/runner.h"
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

AstTree AST_TREE_BOOL_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_BOOL,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_I8_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I8,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_U8_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U8,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_I16_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I16,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_U16_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U16,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_I32_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I32,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_U32_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U32,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_I64_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I64,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_U64_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U64,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_F16_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F16,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_F32_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F32,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_F64_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F64,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_F128_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F128,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};

AstTree AST_TREE_VOID_VALUE = {
    .token = AST_TREE_TOKEN_VALUE_VOID,
    .metadata = NULL,
    .type = &AST_TREE_VOID_TYPE,
};

const char *AST_TREE_TOKEN_STRINGS[] = {
    "AST_TREE_TOKEN_FUNCTION",

    "AST_TREE_TOKEN_KEYWORD_PRINT_U64",
    "AST_TREE_TOKEN_KEYWORD_RETURN",
    "AST_TREE_TOKEN_KEYWORD_IF",
    "AST_TREE_TOKEN_KEYWORD_WHILE",
    "AST_TREE_TOKEN_KEYWORD_COMPTIME",
    "AST_TREE_TOKEN_KEYWORD_STRUCT",

    "AST_TREE_TOKEN_TYPE_FUNCTION",
    "AST_TREE_TOKEN_TYPE_TYPE",
    "AST_TREE_TOKEN_TYPE_VOID",
    "AST_TREE_TOKEN_TYPE_I8",
    "AST_TREE_TOKEN_TYPE_U8",
    "AST_TREE_TOKEN_TYPE_I16",
    "AST_TREE_TOKEN_TYPE_U16",
    "AST_TREE_TOKEN_TYPE_I32",
    "AST_TREE_TOKEN_TYPE_U32",
    "AST_TREE_TOKEN_TYPE_I64",
    "AST_TREE_TOKEN_TYPE_U64",
    "AST_TREE_TOKEN_TYPE_F16",
    "AST_TREE_TOKEN_TYPE_F32",
    "AST_TREE_TOKEN_TYPE_F64",
    "AST_TREE_TOKEN_TYPE_F128",
    "AST_TREE_TOKEN_TYPE_BOOL",
    "AST_TREE_TOKEN_VALUE_VOID",

    "AST_TREE_TOKEN_FUNCTION_CALL",
    "AST_TREE_TOKEN_VARIABLE",
    "AST_TREE_TOKEN_VARIABLE_DEFINE",
    "AST_TREE_TOKEN_VALUE_NULL",
    "AST_TREE_TOKEN_VALUE_UNDEFINED",
    "AST_TREE_TOKEN_VALUE_INT",
    "AST_TREE_TOKEN_VALUE_FLOAT",
    "AST_TREE_TOKEN_VALUE_BOOL",
    "AST_TREE_TOKEN_VALUE_OBJECT",

    "AST_TREE_TOKEN_OPERATOR_ASSIGN",
    "AST_TREE_TOKEN_OPERATOR_PLUS",
    "AST_TREE_TOKEN_OPERATOR_MINUS",
    "AST_TREE_TOKEN_OPERATOR_SUM",
    "AST_TREE_TOKEN_OPERATOR_SUB",
    "AST_TREE_TOKEN_OPERATOR_MULTIPLY",
    "AST_TREE_TOKEN_OPERATOR_DIVIDE",
    "AST_TREE_TOKEN_OPERATOR_MODULO",
    "AST_TREE_TOKEN_OPERATOR_EQUAL",
    "AST_TREE_TOKEN_OPERATOR_NOT_EQUAL",
    "AST_TREE_TOKEN_OPERATOR_GREATER",
    "AST_TREE_TOKEN_OPERATOR_SMALLER",
    "AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL",
    "AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL",
    "AST_TREE_TOKEN_OPERATOR_POINTER",
    "AST_TREE_TOKEN_OPERATOR_ADDRESS",
    "AST_TREE_TOKEN_OPERATOR_DEREFERENCE",
    "AST_TREE_TOKEN_OPERATOR_ACCESS",

    "AST_TREE_TOKEN_SCOPE",

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
      astTreePrint(metadata->scope.expressions[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
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
  case AST_TREE_TOKEN_VALUE_INT: {
    AstTreeInt *metadata = tree->metadata;
    printf(",value=%lu", *metadata);
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_VALUE_FLOAT: {
    AstTreeFloat *metadata = (AstTreeFloat *)tree->metadata;
    printf(",value=%Lf", *metadata);
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_VALUE_BOOL: {
    AstTreeBool *metadata = tree->metadata;
    printf(",value=%b", *metadata);
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_VALUE_OBJECT: {
    AstTreeObject *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("[");
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *variable = metadata->variables.data[i];
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("{name=%.*s,value=\n",
             (int)(variable->name_end - variable->name_begin),
             variable->name_begin);
      astTreePrint(variable->value, indent + 2);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_TYPE_FUNCTION: {
    AstTreeTypeFunction *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("arguments=[\n");
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      AstTreeTypeFunctionArgument arg = metadata->arguments[i];
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("{\n");
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      if (arg.name_begin != arg.name_end) {
        printf("name=\"%.*s\",\n", (int)(arg.name_end - arg.name_begin),
               arg.name_begin);
      }
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("type=\n");
      astTreePrint(arg.type, indent + 2);
      printf(",\n");
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
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
      AstTreeFunctionCallParam param = metadata->parameters[i];
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("{name:\"%.*s\",\n", (int)(param.nameEnd - param.nameBegin),
             param.nameBegin);
      astTreePrint(param.value, indent + 2);
      printf("\n");
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      printf("},\n");
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
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
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
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("condition=\n");
    astTreePrint(metadata->condition, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("ifBody=\n");
    astTreePrint(metadata->ifBody, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("elseBody=\n");
    astTreePrint(metadata->elseBody, indent + 1);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("condition=\n");
    astTreePrint(metadata->condition, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("body=\n");
    astTreePrint(metadata->body, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("expressions=[\n");
    for (size_t i = 0; i < metadata->expressions_size; ++i) {
      astTreePrint(metadata->expressions[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("variables=[\n");
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      astTreeVariablePrint(metadata->variables.data[i], indent + 1);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i) {
      printf(" ");
    }
    printf("]");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("left=\n");
    astTreePrint(metadata->object, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("right=%.*s",
           (int)(metadata->member.name.end - metadata->member.name.begin),
           metadata->member.name.begin);
    printf("\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_NONE:
  }

  UNREACHABLE;

RETURN_SUCCESS:
  printf("}");
}

void astTreeVariablePrint(const AstTreeVariable *variable, int indent) {
  for (int i = 0; i < indent; ++i) {
    printf(" ");
  }
  printf("{\nname=\"%.*s\",\nvalue=\n",
         (int)(variable->name_end - variable->name_begin),
         variable->name_begin);
  if (variable->value == NULL) {
    printf("null");
  } else {
    astTreePrint(variable->value, indent);
  }
  printf("\n}");
}

void astTreeRootPrint(const AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    astTreeVariablePrint(root->variables.data[i], 0);
    printf("\n");
  }
}

void astTreeDestroy(AstTree tree) {
  if (tree.type != NULL) {
    astTreeDelete(tree.type);
  }
  switch (tree.token) {
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
      astTreeDelete(metadata->scope.expressions[i]);
    }
    for (size_t i = 0; i < metadata->scope.variables.size; ++i) {
      astTreeVariableDelete(metadata->scope.variables.data[i]);
    }
    for (size_t i = 0; i < metadata->arguments.size; ++i) {
      astTreeVariableDelete(metadata->arguments.data[i]);
    }
    astTreeDelete(metadata->returnType);
    free(metadata->scope.expressions);
    free(metadata->scope.variables.data);
    free(metadata->arguments.data);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    return;
  case AST_TREE_TOKEN_VALUE_BOOL: {
    AstTreeBool *metadata = tree.metadata;
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_VALUE_INT: {
    AstTreeInt *metadata = tree.metadata;
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_VALUE_FLOAT: {
    AstTreeFloat *metadata = tree.metadata;
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_VALUE_OBJECT: {
    AstTreeObject *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *variable = metadata->variables.data[i];
      astTreeVariableDelete(variable);
    }
    free(metadata->variables.data);
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
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
      AstTreeTypeFunctionArgument arg = metadata->arguments[i];
      astTreeDelete(arg.type);
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
      astTreeDelete(metadata->parameters[i].value);
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
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeInfix *metadata = tree.metadata;
    astTreeDestroy(metadata->left);
    astTreeDestroy(metadata->right);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = tree.metadata;
    astTreeDelete(metadata->condition);
    astTreeDelete(metadata->ifBody);
    if (metadata->elseBody != NULL) {
      astTreeDelete(metadata->elseBody);
    }
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = tree.metadata;
    astTreeDelete(metadata->condition);
    astTreeDelete(metadata->body);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->expressions_size; ++i) {
      astTreeDelete(metadata->expressions[i]);
    }
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      astTreeVariableDelete(metadata->variables.data[i]);
    }
    free(metadata->expressions);
    free(metadata->variables.data);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = tree.metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      astTreeVariableDelete(metadata->variables.data[i]);
    }
    free(metadata->variables.data);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = tree.metadata;
    astTreeDelete(metadata->object);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_NONE:
  }
  printLog("token = %d", tree.token);
  UNREACHABLE;
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
  if (astTreeShouldDelete(tree)) {
    astTreeDestroy(*tree);
    free(tree);
  }
}

bool astTreeShouldDelete(AstTree *tree) {
  return AST_TREE_TOKEN_STATIC_VARS_BEGIN > tree->token ||
         tree->token > AST_TREE_TOKEN_STATIC_VARS_END;
}

void astTreeRootDelete(AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    astTreeVariableDelete(root->variables.data[i]);
  }
  free(root->variables.data);
  free(root);
}

AstTree *newAstTree(AstTreeToken token, void *metadata, AstTree *type,
                    char *str_begin, char *str_end) {
  AstTree *result = a404m_malloc(sizeof(*result));
  result->token = token;
  result->metadata = metadata;
  result->type = type;
  result->str_begin = str_begin;
  result->str_end = str_end;
  return result;
}

AstTree *copyAstTree(AstTree *tree) {
  return copyAstTreeBack(tree, NULL, NULL, 0);
}

AstTree *copyAstTreeBack(AstTree *tree, AstTreeVariables oldVariables[],
                         AstTreeVariables newVariables[],
                         size_t variables_size) {
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_VALUE_VOID:
    return tree;
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
    return newAstTree(
        tree->token, NULL,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  case AST_TREE_TOKEN_VALUE_BOOL: {
    AstTreeBool *metadata = tree->metadata;
    AstTreeBool *newMetadata = a404m_malloc(sizeof(*newMetadata));
    *newMetadata = *metadata;
    return newAstTree(
        tree->token, newMetadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_INT: {
    AstTreeInt *metadata = tree->metadata;
    AstTreeInt *newMetadata = a404m_malloc(sizeof(*newMetadata));
    *newMetadata = *metadata;
    return newAstTree(
        tree->token, newMetadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_FLOAT: {
    AstTreeFloat *metadata = tree->metadata;
    AstTreeFloat *newMetadata = a404m_malloc(sizeof(*newMetadata));
    *newMetadata = *metadata;
    return newAstTree(
        tree->token, newMetadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_OBJECT: {
    AstTreeObject *metadata = tree->metadata;
    AstTreeObject *newMetadata = a404m_malloc(sizeof(*newMetadata));

    newMetadata->variables = copyAstTreeVariables(
        metadata->variables, oldVariables, newVariables, variables_size);

    return newAstTree(
        tree->token, newMetadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE: {
    AstTreeVariable *variable = tree->metadata;
    for (size_t i = 0; i < variables_size; ++i) {
      for (size_t j = 0; j < oldVariables[i].size; ++j) {
        if (oldVariables[i].data[j] == variable) {
          variable = newVariables[i].data[j];
        }
      }
    }
    return newAstTree(
        tree->token, variable,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_TYPE_FUNCTION: {
    AstTreeTypeFunction *metadata = tree->metadata;
    AstTreeTypeFunction *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->arguments_size = metadata->arguments_size;
    new_metadata->arguments = a404m_malloc(sizeof(*new_metadata->arguments) *
                                           new_metadata->arguments_size);
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      AstTreeTypeFunctionArgument arg = metadata->arguments[i];
      new_metadata->arguments[i].str_begin = arg.str_begin;
      new_metadata->arguments[i].str_end = arg.str_end;
      new_metadata->arguments[i].name_begin = arg.name_begin;
      new_metadata->arguments[i].name_end = arg.name_end;
      new_metadata->arguments[i].type =
          copyAstTreeBack(arg.type, oldVariables, newVariables, variables_size);
    }
    new_metadata->returnType = copyAstTreeBack(
        metadata->returnType, oldVariables, newVariables, variables_size);
    return newAstTree(tree->token, new_metadata, &AST_TREE_TYPE_TYPE,
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeInfix *metadata = tree->metadata;
    AstTreeInfix *new_metadata = a404m_malloc(sizeof(*new_metadata));
    AstTree *left = copyAstTreeBack(&metadata->left, oldVariables, newVariables,
                                    variables_size);
    AstTree *right = copyAstTreeBack(&metadata->right, oldVariables,
                                     newVariables, variables_size);
    new_metadata->left = *left;
    new_metadata->right = *right;
    free(left);
    free(right);
    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *metadata = tree->metadata;
    AstTreeFunction *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->arguments = copyAstTreeVariables(
        metadata->arguments, oldVariables, newVariables, variables_size);

    size_t new_variables_size = variables_size + 2;
    AstTreeVariables new_oldVariables[new_variables_size];
    AstTreeVariables new_newVariables[new_variables_size];
    for (size_t i = 0; i < variables_size; ++i) {
      new_oldVariables[i] = oldVariables[i];
      new_newVariables[i] = newVariables[i];
    }
    new_oldVariables[new_variables_size - 2] = metadata->arguments;
    new_newVariables[new_variables_size - 2] = new_metadata->arguments;

    new_oldVariables[new_variables_size - 1].data = NULL;
    new_oldVariables[new_variables_size - 1].size = 0;
    new_newVariables[new_variables_size - 1].data = NULL;
    new_newVariables[new_variables_size - 1].size = 0;

    new_metadata->returnType =
        copyAstTreeBack(metadata->returnType, new_oldVariables,
                        new_newVariables, new_variables_size);

    new_metadata->scope.variables =
        copyAstTreeVariables(metadata->scope.variables, new_oldVariables,
                             new_newVariables, new_variables_size);
    new_metadata->scope.expressions_size = metadata->scope.expressions_size;
    new_metadata->scope.expressions =
        a404m_malloc(new_metadata->scope.expressions_size *
                     sizeof(*new_metadata->scope.expressions));

    new_oldVariables[new_variables_size - 1] = metadata->scope.variables;
    new_newVariables[new_variables_size - 1] = new_metadata->scope.variables;

    for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
      new_metadata->scope.expressions[i] =
          copyAstTreeBack(metadata->scope.expressions[i], new_oldVariables,
                          new_newVariables, new_variables_size);
    }

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, new_oldVariables,
                                      new_newVariables, new_variables_size),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *metadata = tree->metadata;
    AstTreeSingleChild *new_metadata =
        copyAstTreeBack(metadata, oldVariables, newVariables, variables_size);

    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = tree->metadata;
    AstTreeReturn *new_metadata = a404m_malloc(sizeof(*new_metadata));
    if (metadata->value != NULL) {
      new_metadata->value = copyAstTreeBack(metadata->value, oldVariables,
                                            newVariables, variables_size);
    } else {
      new_metadata->value = NULL;
    }

    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree->metadata;
    AstTreeFunctionCall *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->function = copyAstTreeBack(metadata->function, oldVariables,
                                             newVariables, variables_size);

    new_metadata->parameters_size = metadata->parameters_size;
    new_metadata->parameters = a404m_malloc(metadata->parameters_size *
                                            sizeof(*new_metadata->parameters));
    for (size_t i = 0; i < metadata->parameters_size; ++i) {
      new_metadata->parameters[i].nameBegin = metadata->parameters[i].nameBegin;
      new_metadata->parameters[i].nameEnd = metadata->parameters[i].nameEnd;
      new_metadata->parameters[i].value =
          copyAstTreeBack(metadata->parameters[i].value, oldVariables,
                          newVariables, variables_size);
    }
    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = tree->metadata;
    AstTreeIf *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->condition = copyAstTreeBack(metadata->condition, oldVariables,
                                              newVariables, variables_size);
    new_metadata->ifBody = copyAstTreeBack(metadata->ifBody, oldVariables,
                                           newVariables, variables_size);
    if (metadata->elseBody != NULL) {
      new_metadata->elseBody = copyAstTreeBack(metadata->elseBody, oldVariables,
                                               newVariables, variables_size);
    } else {
      new_metadata->elseBody = NULL;
    }
    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = tree->metadata;
    AstTreeWhile *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->condition = copyAstTreeBack(metadata->condition, oldVariables,
                                              newVariables, variables_size);
    new_metadata->body = copyAstTreeBack(metadata->body, oldVariables,
                                         newVariables, variables_size);
    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = tree->metadata;
    AstTreeScope *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->variables = copyAstTreeVariables(
        metadata->variables, oldVariables, newVariables, variables_size);
    new_metadata->expressions_size = metadata->expressions_size;
    new_metadata->expressions = a404m_malloc(
        new_metadata->expressions_size * sizeof(*new_metadata->expressions));

    const size_t new_variables_size = variables_size + 1;
    AstTreeVariables new_oldVariables[new_variables_size];
    AstTreeVariables new_newVariables[new_variables_size];
    for (size_t i = 0; i < variables_size; ++i) {
      new_oldVariables[i] = oldVariables[i];
      new_newVariables[i] = newVariables[i];
    }

    new_oldVariables[new_variables_size - 1] = metadata->variables;
    new_newVariables[new_variables_size - 1] = new_metadata->variables;

    for (size_t i = 0; i < metadata->expressions_size; ++i) {
      new_metadata->expressions[i] =
          copyAstTreeBack(metadata->expressions[i], new_oldVariables,
                          new_newVariables, new_variables_size);
    }

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, new_oldVariables,
                                      new_newVariables, new_variables_size),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = tree->metadata;
    AstTreeStruct *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->variables = copyAstTreeVariables(
        metadata->variables, oldVariables, newVariables, variables_size);
    new_metadata->id = metadata->id;

    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = tree->metadata;
    AstTreeAccess *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->object = copyAstTreeBack(metadata->object, oldVariables,
                                           newVariables, variables_size);
    new_metadata->member = metadata->member;

    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token %d", tree->token);
  UNREACHABLE;
}

AstTreeVariables copyAstTreeVariables(AstTreeVariables variables,
                                      AstTreeVariables oldVariables[],
                                      AstTreeVariables newVariables[],
                                      size_t variables_size) {
  AstTreeVariables result = {
      .data = a404m_malloc(variables.size * sizeof(*variables.data)),
      .size = variables.size,
  };

  size_t new_variables_size = variables_size + 1;
  AstTreeVariables new_oldVariables[new_variables_size];
  AstTreeVariables new_newVariables[new_variables_size];
  for (size_t i = 0; i < variables_size; ++i) {
    new_oldVariables[i] = oldVariables[i];
    new_newVariables[i] = newVariables[i];
  }
  new_oldVariables[new_variables_size - 1] = variables;
  new_newVariables[new_variables_size - 1] = result;

  for (size_t i = 0; i < result.size; ++i) {
    result.data[i] = a404m_malloc(sizeof(*result.data[i]));
    result.data[i]->name_begin = variables.data[i]->name_begin;
    result.data[i]->name_end = variables.data[i]->name_end;
    result.data[i]->isConst = variables.data[i]->isConst;
    result.data[i]->type =
        copyAstTreeBack(variables.data[i]->type, new_oldVariables,
                        new_newVariables, new_variables_size);
    if (variables.data[i]->value != NULL) {
      result.data[i]->value =
          copyAstTreeBack(variables.data[i]->value, new_oldVariables,
                          new_newVariables, new_variables_size);
    } else {
      result.data[i]->value = NULL;
    }
  }

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
      printError(eol->str_begin, eol->str_end, "Did you forgot semicolon?",
                 PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    ParserNode *node = (ParserNodeSingleChildMetadata *)eol->metadata;
    if (node->token != PARSER_TOKEN_CONSTANT &&
        node->token != PARSER_TOKEN_VARIABLE) {
      printError(node->str_begin, node->str_end, "Unexpected %s",
                 PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    }
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
    variable->name_begin = node_metadata->name->str_begin;
    variable->name_end = node_metadata->name->str_end;
    variable->isConst = node->token == PARSER_TOKEN_CONSTANT;

    if (!pushVariable(NULL, &root->variables, variable)) {
      astTreeVariableDelete(variable);
      goto RETURN_ERROR;
    }
  }

  AstTreeVariables *variables = &root->variables;
  constexpr size_t variables_size = 1;

  AstTreeHelper helper = {
      .variables = &variables,
      .variables_size = variables_size,
      .variable = NULL,
      .globalDeps = a404m_malloc(variables->size * sizeof(*helper.globalDeps)),
  };

  for (size_t i = 0; i < variables->size; ++i) {
    helper.globalDeps[i].size = 0;
    helper.globalDeps[i].data =
        a404m_malloc(helper.globalDeps->size * sizeof(*helper.globalDeps));
  }

  for (size_t i = 0; i < nodes->size; ++i) {
    helper.variable = root->variables.data[i];
    ParserNode *node =
        (ParserNodeSingleChildMetadata *)nodes->data[i]->metadata;
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    if (node->token != PARSER_TOKEN_CONSTANT &&
        node->token != PARSER_TOKEN_VARIABLE) {
      printError(node->str_begin, node->str_end,
                 "Only variables are allowed here");
      goto RETURN_ERROR;
    }

    if (node_metadata->value == NULL) {
      printError(node->str_begin, node->str_end,
                 "Variables must be initialized");
      goto RETURN_ERROR;
    }

    switch (node_metadata->value->token) {
    case PARSER_TOKEN_KEYWORD_NULL:
    case PARSER_TOKEN_KEYWORD_UNDEFINED:
    case PARSER_TOKEN_VALUE_BOOL:
    case PARSER_TOKEN_VALUE_INT:
    case PARSER_TOKEN_VALUE_FLOAT:
    case PARSER_TOKEN_FUNCTION_DEFINITION:
    case PARSER_TOKEN_FUNCTION_CALL:
    case PARSER_TOKEN_IDENTIFIER:
    case PARSER_TOKEN_OPERATOR_ACCESS:
    case PARSER_TOKEN_OPERATOR_ASSIGN:
    case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
    case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
    case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
    case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
    case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
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
    case PARSER_TOKEN_SYMBOL_PARENTHESIS:
    case PARSER_TOKEN_KEYWORD_IF:
    case PARSER_TOKEN_KEYWORD_WHILE:
    case PARSER_TOKEN_KEYWORD_COMPTIME:
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
    case PARSER_TOKEN_TYPE_F16:
    case PARSER_TOKEN_TYPE_F32:
    case PARSER_TOKEN_TYPE_F64:
    case PARSER_TOKEN_TYPE_F128:
    case PARSER_TOKEN_TYPE_BOOL:
    case PARSER_TOKEN_OPERATOR_POINTER:
    case PARSER_TOKEN_OPERATOR_ADDRESS:
    case PARSER_TOKEN_OPERATOR_DEREFERENCE:
    case PARSER_TOKEN_KEYWORD_STRUCT:
      goto AFTER_SWITCH;
    case PARSER_TOKEN_ROOT:
    case PARSER_TOKEN_KEYWORD_PRINT_U64:
    case PARSER_TOKEN_KEYWORD_RETURN:
    case PARSER_TOKEN_CONSTANT:
    case PARSER_TOKEN_VARIABLE:
    case PARSER_TOKEN_SYMBOL_EOL:
    case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    case PARSER_TOKEN_SYMBOL_COMMA:
      printError(node->str_begin, node->str_end, "Should not be here %s",
                 PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    case PARSER_TOKEN_NONE:
    }
    printError(node_metadata->value->str_begin, node_metadata->value->str_end,
               "Bad token %d", node_metadata->value->token);
    goto RETURN_ERROR;

  AFTER_SWITCH:

    AstTree *value = astTreeParse(node_metadata->value, &helper);
    if (value == NULL) {
      goto RETURN_ERROR;
    }

    AstTree *type;
    if (node_metadata->type != NULL) {
      type = astTreeParse(node_metadata->type, &helper);
      if (type == NULL) {
        goto RETURN_ERROR;
      }
    } else {
      type = NULL;
    }

    root->variables.data[i]->type = type;
    root->variables.data[i]->value = value;
  }

  helper.variable = NULL;
  if (!setAllTypesRoot(root, &helper)) {
    goto RETURN_ERROR;
  }

  for (size_t i = 0; i < variables->size; ++i) {
    free(helper.globalDeps[i].data);
  }
  free(helper.globalDeps);

  return root;

RETURN_ERROR:
  free(root->variables.data);
  free(root);
  return NULL;
}

bool pushVariable(AstTreeHelper *helper, AstTreeVariables *variables,
                  AstTreeVariable *variable) {
  (void)helper;
  for (size_t j = 0; j < variables->size; ++j) {
    char *var_begin = variables->data[j]->name_begin;
    char *var_end = variables->data[j]->name_end;

    if (variable->name_end - variable->name_begin == var_end - var_begin &&
        strncmp(var_begin, variable->name_begin,
                variable->name_end - variable->name_begin) == 0) {
      printError(variable->name_begin, variable->name_end, "Variable exists");
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

AstTreeVariable *getVariable(AstTreeHelper *helper, char *name_begin,
                             char *name_end) {
  for (size_t i = helper->variables_size - 1; i != (size_t)-1ULL; --i) {
    AstTreeVariables vars = *helper->variables[i];
    for (size_t j = 0; j < vars.size; ++j) {
      char *var_begin = vars.data[j]->name_begin;
      char *var_end = vars.data[j]->name_end;

      AstTreeVariable *variable = vars.data[j];

      if (name_end - name_begin == var_end - var_begin &&
          strncmp(var_begin, name_begin, name_end - name_begin) == 0) {

        if (i == 0 && helper->variable != NULL) {
          for (size_t i = 0; i < helper->variables[0]->size; ++i) {
            if (helper->variables[0]->data[i] == helper->variable) {
              size_t globalDeps_size =
                  a404m_malloc_usable_size(helper->globalDeps[i].data) /
                  sizeof(*helper->globalDeps[i].data);
              if (globalDeps_size == helper->globalDeps[i].size) {
                globalDeps_size += globalDeps_size / 2 + 1;
                helper->globalDeps[i].data = a404m_realloc(
                    helper->globalDeps[i].data,
                    globalDeps_size * sizeof(*helper->globalDeps[i].data));
              }
              helper->globalDeps[i].data[helper->globalDeps[i].size] = variable;
              helper->globalDeps[i].size += 1;
            }
          }
        }

        return variable;
      }
    }
  }

  return NULL;
}

AstTree *astTreeParse(ParserNode *parserNode, AstTreeHelper *helper) {
  switch (parserNode->token) {
  case PARSER_TOKEN_FUNCTION_DEFINITION:
    return astTreeParseFunction(parserNode, helper);
  case PARSER_TOKEN_TYPE_TYPE:
    return &AST_TREE_TYPE_TYPE;
  case PARSER_TOKEN_TYPE_FUNCTION:
    return astTreeParseTypeFunction(parserNode, helper);
  case PARSER_TOKEN_TYPE_VOID:
    return &AST_TREE_VOID_TYPE;
  case PARSER_TOKEN_TYPE_I8:
    return &AST_TREE_I8_TYPE;
  case PARSER_TOKEN_TYPE_U8:
    return &AST_TREE_U8_TYPE;
  case PARSER_TOKEN_TYPE_I16:
    return &AST_TREE_I16_TYPE;
  case PARSER_TOKEN_TYPE_U16:
    return &AST_TREE_U16_TYPE;
  case PARSER_TOKEN_TYPE_I32:
    return &AST_TREE_I32_TYPE;
  case PARSER_TOKEN_TYPE_U32:
    return &AST_TREE_U32_TYPE;
  case PARSER_TOKEN_TYPE_I64:
    return &AST_TREE_I64_TYPE;
  case PARSER_TOKEN_TYPE_U64:
    return &AST_TREE_U64_TYPE;
  case PARSER_TOKEN_TYPE_F16:
    return &AST_TREE_F16_TYPE;
  case PARSER_TOKEN_TYPE_F32:
    return &AST_TREE_F32_TYPE;
  case PARSER_TOKEN_TYPE_F64:
    return &AST_TREE_F64_TYPE;
  case PARSER_TOKEN_TYPE_F128:
    return &AST_TREE_F128_TYPE;
  case PARSER_TOKEN_TYPE_BOOL:
    return &AST_TREE_BOOL_TYPE;
  case PARSER_TOKEN_FUNCTION_CALL:
    return astTreeParseFunctionCall(parserNode, helper);
  case PARSER_TOKEN_IDENTIFIER:
    return astTreeParseIdentifier(parserNode, helper);
  case PARSER_TOKEN_VALUE_INT:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_INT,
                             sizeof(AstTreeInt));
  case PARSER_TOKEN_VALUE_FLOAT:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_FLOAT,
                             sizeof(AstTreeFloat));
  case PARSER_TOKEN_VALUE_BOOL:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_BOOL,
                             sizeof(AstTreeBool));
  case PARSER_TOKEN_KEYWORD_NULL:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_VALUE_NULL);
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_VALUE_UNDEFINED);
  case PARSER_TOKEN_KEYWORD_PRINT_U64:
    return astTreeParsePrintU64(parserNode, helper);
  case PARSER_TOKEN_KEYWORD_RETURN:
    return astTreeParseReturn(parserNode, helper);
  case PARSER_TOKEN_OPERATOR_ASSIGN:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_ASSIGN);
  case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode, helper,
                                             AST_TREE_TOKEN_OPERATOR_SUM);
  case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode, helper,
                                             AST_TREE_TOKEN_OPERATOR_SUB);
  case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode, helper,
                                             AST_TREE_TOKEN_OPERATOR_MULTIPLY);
  case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode, helper,
                                             AST_TREE_TOKEN_OPERATOR_DIVIDE);
  case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode, helper,
                                             AST_TREE_TOKEN_OPERATOR_MODULO);
  case PARSER_TOKEN_OPERATOR_SUM:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_SUM);
  case PARSER_TOKEN_OPERATOR_SUB:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_SUB);
  case PARSER_TOKEN_OPERATOR_MULTIPLY:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_MULTIPLY);
  case PARSER_TOKEN_OPERATOR_DIVIDE:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_DIVIDE);
  case PARSER_TOKEN_OPERATOR_MODULO:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_MODULO);
  case PARSER_TOKEN_OPERATOR_EQUAL:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_EQUAL);
  case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_NOT_EQUAL);
  case PARSER_TOKEN_OPERATOR_GREATER:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_GREATER);
  case PARSER_TOKEN_OPERATOR_SMALLER:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_SMALLER);
  case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL);
  case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL);
  case PARSER_TOKEN_OPERATOR_ACCESS:
    return astTreeParseAccessOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_ACCESS);
  case PARSER_TOKEN_OPERATOR_PLUS:
    return astTreeParseUnaryOperator(parserNode, helper,
                                     AST_TREE_TOKEN_OPERATOR_PLUS);
  case PARSER_TOKEN_OPERATOR_MINUS:
    return astTreeParseUnaryOperator(parserNode, helper,
                                     AST_TREE_TOKEN_OPERATOR_MINUS);
  case PARSER_TOKEN_OPERATOR_POINTER:
    return astTreeParseUnaryOperator(parserNode, helper,
                                     AST_TREE_TOKEN_OPERATOR_POINTER);
  case PARSER_TOKEN_OPERATOR_ADDRESS:
    return astTreeParseUnaryOperator(parserNode, helper,
                                     AST_TREE_TOKEN_OPERATOR_ADDRESS);
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
    return astTreeParseUnaryOperator(parserNode, helper,
                                     AST_TREE_TOKEN_OPERATOR_DEREFERENCE);
  case PARSER_TOKEN_VARIABLE:
    return astTreeParseVariable(parserNode, helper);
  case PARSER_TOKEN_KEYWORD_IF:
    return astTreeParseIf(parserNode, helper);
  case PARSER_TOKEN_KEYWORD_WHILE:
    return astTreeParseWhile(parserNode, helper);
  case PARSER_TOKEN_KEYWORD_COMPTIME:
    return astTreeParseComptime(parserNode, helper);
  case PARSER_TOKEN_SYMBOL_EOL:
    return astTreeParse((ParserNodeSingleChildMetadata *)parserNode->metadata,
                        helper);
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    return astTreeParseCurlyBracket(parserNode, helper);
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
    return astTreeParseParenthesis(parserNode, helper);
  case PARSER_TOKEN_KEYWORD_STRUCT:
    return astTreeParseStruct(parserNode, helper);
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_NONE:
  case PARSER_TOKEN_ROOT:
  }
  printError(parserNode->str_begin, parserNode->str_end, "Bad token %d",
             parserNode->token);
  return NULL;
}

AstTree *astTreeParseFunction(ParserNode *parserNode, AstTreeHelper *p_helper) {
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

  size_t variables_size = p_helper->variables_size + 2;
  AstTreeVariables *variables[variables_size];
  for (size_t i = 0; i < p_helper->variables_size; ++i) {
    variables[i] = p_helper->variables[i];
  }
  variables[variables_size - 2] = &function->arguments;
  variables[variables_size - 1] = &scope.variables;

  AstTreeHelper helper = {
      .variables = variables,
      .variables_size = variables_size,
      .variable = p_helper->variable,
      .globalDeps = p_helper->globalDeps,
  };

  for (size_t i = 0; i < node_arguments->size; ++i) {
    ParserNode *arg = node_arguments->data[i];
    if (arg->token == PARSER_TOKEN_SYMBOL_COMMA) {
      arg = (ParserNodeSingleChildMetadata *)arg->metadata;
    }

    ParserNodeVariableMetadata *arg_metadata = arg->metadata;
    if (arg_metadata->value != NULL) {
      printError(arg->str_begin, arg->str_end,
                 "arguments can't have default values (for now)");
      goto RETURN_ERROR;
    }

    AstTree *type = astTreeParse(arg_metadata->type, &helper);
    if (type == NULL) {
      goto RETURN_ERROR;
    }

    AstTreeVariable *argument = a404m_malloc(sizeof(*argument));
    argument->value = NULL;
    argument->type = type;
    argument->name_begin = arg_metadata->name->str_begin;
    argument->name_end = arg_metadata->name->str_end;
    argument->isConst = true; // all arguments are constants

    if (!pushVariable(&helper, &function->arguments, argument)) {
      astTreeVariableDelete(argument);
      goto RETURN_ERROR;
    }
  }

  if ((function->returnType =
           astTreeParse(node_metadata->returnType, &helper)) == NULL) {
    goto RETURN_ERROR;
  }

  for (size_t i = 0; i < body->size; ++i) {
    ParserNode *node = body->data[i];
    switch (node->token) {
    case PARSER_TOKEN_SYMBOL_EOL:
      node = (ParserNodeSingleChildMetadata *)node->metadata;
      goto OK_NODE;
    case PARSER_TOKEN_KEYWORD_IF:
    case PARSER_TOKEN_KEYWORD_WHILE:
      goto OK_NODE;
    case PARSER_TOKEN_ROOT:
    case PARSER_TOKEN_IDENTIFIER:
    case PARSER_TOKEN_VALUE_INT:
    case PARSER_TOKEN_VALUE_FLOAT:
    case PARSER_TOKEN_VALUE_BOOL:
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
    case PARSER_TOKEN_TYPE_F16:
    case PARSER_TOKEN_TYPE_F32:
    case PARSER_TOKEN_TYPE_F64:
    case PARSER_TOKEN_TYPE_F128:
    case PARSER_TOKEN_TYPE_BOOL:
    case PARSER_TOKEN_KEYWORD_PRINT_U64:
    case PARSER_TOKEN_KEYWORD_RETURN:
    case PARSER_TOKEN_KEYWORD_COMPTIME:
    case PARSER_TOKEN_KEYWORD_STRUCT:
    case PARSER_TOKEN_CONSTANT:
    case PARSER_TOKEN_VARIABLE:
    case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    case PARSER_TOKEN_SYMBOL_PARENTHESIS:
    case PARSER_TOKEN_SYMBOL_COMMA:
    case PARSER_TOKEN_OPERATOR_ASSIGN:
    case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
    case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
    case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
    case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
    case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
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
    case PARSER_TOKEN_FUNCTION_CALL:
    case PARSER_TOKEN_KEYWORD_NULL:
    case PARSER_TOKEN_KEYWORD_UNDEFINED:
    case PARSER_TOKEN_OPERATOR_POINTER:
    case PARSER_TOKEN_OPERATOR_ADDRESS:
    case PARSER_TOKEN_OPERATOR_DEREFERENCE:
    case PARSER_TOKEN_OPERATOR_ACCESS:
      printError(node->str_begin, node->str_end, "Unexpected %s",
                 PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    case PARSER_TOKEN_NONE:
    }
    UNREACHABLE;
  OK_NODE:

    if (node->token == PARSER_TOKEN_CONSTANT) {
      if (!astTreeParseConstant(node, &helper)) {
        goto RETURN_ERROR;
      }
    } else {
      AstTree *tree = astTreeParse(node, &helper);

      if (tree == NULL) {
        goto RETURN_ERROR;
      }

      if (expressions_size == scope.expressions_size) {
        expressions_size += expressions_size / 2 + 1;
        scope.expressions = a404m_realloc(
            scope.expressions, expressions_size * sizeof(*scope.expressions));
      }
      scope.expressions[scope.expressions_size] = tree;
      scope.expressions_size += 1;
    }
  }

  scope.expressions = a404m_realloc(
      scope.expressions, scope.expressions_size * sizeof(*scope.expressions));

  function->scope = scope;

  AstTree *result = newAstTree(AST_TREE_TOKEN_FUNCTION, function, NULL,
                               parserNode->str_begin, parserNode->str_end);

  return result;

RETURN_ERROR:
  free(function);
  free(scope.expressions);
  return NULL;
}

AstTree *astTreeParseTypeFunction(ParserNode *parserNode,
                                  AstTreeHelper *helper) {
  ParserNodeTypeFunctionMetadata *metadata = parserNode->metadata;
  ParserNodeArray *node_arguments = metadata->arguments->metadata;

  AstTreeTypeFunction *typeFunction = a404m_malloc(sizeof(*typeFunction));

  size_t arguments_size = 0;
  typeFunction->arguments =
      a404m_malloc(arguments_size * sizeof(*typeFunction->arguments));
  typeFunction->arguments_size = 0;

  for (size_t i = 0; i < node_arguments->size; ++i) {
    ParserNode *node_argument = node_arguments->data[i];

    if (node_argument->token == PARSER_TOKEN_SYMBOL_COMMA) {
      node_argument = (ParserNodeSingleChildMetadata *)node_argument->metadata;
    }

    AstTreeTypeFunctionArgument argument = {
        .str_begin = node_argument->str_begin,
        .str_end = node_argument->str_end,
    };

    if (node_argument->token == PARSER_TOKEN_VARIABLE) {
      ParserNodeVariableMetadata *variable_metadata = node_argument->metadata;
      if (variable_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
        printError(node_argument->str_begin, node_argument->str_end,
                   "Name must be identifier");
        return NULL;
      }
      argument.name_begin = variable_metadata->name->str_begin;
      argument.name_end = variable_metadata->name->str_end;

      argument.type = astTreeParse(variable_metadata->type, helper);
      if (variable_metadata->value != NULL) {
        printError(node_argument->str_begin, node_argument->str_end,
                   "Cannot have value in function type");
        return NULL;
      }
      if (argument.type == NULL) {
        return NULL;
      }
    } else {
      argument.type = astTreeParse(node_argument, helper);
      if (argument.type == NULL) {
        return NULL;
      }
      argument.name_begin = argument.name_end = NULL;
    }

    if (!typeIsEqual(argument.type->type, &AST_TREE_TYPE_TYPE)) {
      printError(argument.str_begin, argument.str_end, "Type is incorrenct");
      return NULL;
    }

    if (typeFunction->arguments_size == arguments_size) {
      arguments_size += arguments_size / 2 + 1;
      typeFunction->arguments =
          a404m_realloc(typeFunction->arguments,
                        arguments_size * sizeof(*typeFunction->arguments));
    }

    typeFunction->arguments[typeFunction->arguments_size] = argument;
    typeFunction->arguments_size += 1;
  }

  if ((typeFunction->returnType = astTreeParse(metadata->returnType, helper)) ==
      NULL) {
    goto RETURN_ERROR;
  }

  return newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, typeFunction,
                    &AST_TREE_TYPE_TYPE, parserNode->str_begin,
                    parserNode->str_end);

RETURN_ERROR:
  return NULL;
}

AstTree *astTreeParseFunctionCall(ParserNode *parserNode,
                                  AstTreeHelper *helper) {
  ParserNodeFunctionCall *node_metadata = parserNode->metadata;
  AstTree *function = astTreeParse(node_metadata->function, helper);
  if (function == NULL) {
    return NULL;
  }
  if (function->token != AST_TREE_TOKEN_VARIABLE) {
    printError(function->str_begin, function->str_end, "Not yet supported");
    return NULL;
  }

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));
  metadata->function = function;

  metadata->parameters =
      a404m_malloc(sizeof(*metadata->parameters) * node_metadata->params->size);
  metadata->parameters_size = node_metadata->params->size;

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    ParserNode *node_param = node_metadata->params->data[i];
    if (node_param->token == PARSER_TOKEN_SYMBOL_COMMA) {
      node_param = (ParserNodeSingleChildMetadata *)node_param->metadata;
    }
    AstTreeFunctionCallParam param;
    if (node_param->token == PARSER_TOKEN_OPERATOR_ASSIGN) {
      ParserNodeInfixMetadata *assign = node_param->metadata;
      if (assign->left->token == PARSER_TOKEN_IDENTIFIER) {
        param.nameBegin = assign->left->str_begin;
        param.nameEnd = assign->left->str_end;
        param.value = astTreeParse(assign->right, helper);
        goto PUSH_PARAM;
      }
    }
    param.nameBegin = param.nameEnd = NULL;
    param.value = astTreeParse(node_param, helper);
  PUSH_PARAM:
    metadata->parameters[i] = param;
  }

  return newAstTree(AST_TREE_TOKEN_FUNCTION_CALL, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseIdentifier(ParserNode *parserNode, AstTreeHelper *helper) {
  AstTreeVariable *var =
      getVariable(helper, parserNode->str_begin, parserNode->str_end);
  if (var == NULL) {
    printError(parserNode->str_begin, parserNode->str_end,
               "Variable not found");
    return NULL;
  }
  return newAstTree(AST_TREE_TOKEN_VARIABLE, var, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseValue(ParserNode *parserNode, AstTreeToken token,
                           size_t metadata_size) {
  void *metadata = a404m_malloc(metadata_size);
  memcpy(metadata, parserNode->metadata, metadata_size);

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseKeyword(ParserNode *parserNode, AstTreeToken token) {
  return newAstTree(token, NULL, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParsePrintU64(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTree *operand = astTreeParse(node_metadata, helper);
  if (operand == NULL) {
    return NULL;
  }

  return newAstTree(AST_TREE_TOKEN_KEYWORD_PRINT_U64,
                    (AstTreeSingleChild *)operand, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseReturn(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNodeReturnMetadata *node_metadata = parserNode->metadata;

  AstTree *value;
  if (node_metadata->value == NULL) {
    value = NULL;
  } else {
    value = astTreeParse(node_metadata->value, helper);
    if (value == NULL) {
      return NULL;
    }
  }

  AstTreeReturn *metadata = a404m_malloc(sizeof(*metadata));
  metadata->value = value;

  return newAstTree(AST_TREE_TOKEN_KEYWORD_RETURN, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseBinaryOperator(ParserNode *parserNode,
                                    AstTreeHelper *helper, AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *left = astTreeParse(node_metadata->left, helper);
  if (left == NULL) {
    return NULL;
  }
  AstTree *right = astTreeParse(node_metadata->right, helper);
  if (right == NULL) {
    return NULL;
  }

  AstTreeInfix *metadata = a404m_malloc(sizeof(*metadata));

  metadata->left = *left;
  metadata->right = *right;

  free(left);
  free(right);

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseUnaryOperator(ParserNode *parserNode,
                                   AstTreeHelper *helper, AstTreeToken token) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeSingleChild *metadata = astTreeParse(node_metadata, helper);
  if (metadata == NULL) {
    return NULL;
  }

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseOperateAssignOperator(ParserNode *parserNode,
                                           AstTreeHelper *helper,
                                           AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *left = astTreeParse(node_metadata->left, helper);
  if (left == NULL) {
    return NULL;
  }
  AstTree *right = astTreeParse(node_metadata->right, helper);
  if (right == NULL) {
    return NULL;
  }

  AstTreeInfix *metadata = a404m_malloc(sizeof(*metadata));

  metadata->left = *left;
  metadata->right = *right;

  free(left);
  free(right);

  AstTreeInfix *assignMetadata = a404m_malloc(sizeof(*assignMetadata));

  AstTree *assignLeft = astTreeParse(node_metadata->left, helper);
  AstTree *assignRight = newAstTree(token, metadata, NULL,
                                    parserNode->str_begin, parserNode->str_end);

  assignMetadata->left = *assignLeft;
  assignMetadata->right = *assignRight;

  free(assignLeft);
  free(assignRight);

  return newAstTree(AST_TREE_TOKEN_OPERATOR_ASSIGN, assignMetadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

bool astTreeParseConstant(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->value == NULL ||
      node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
    printError(parserNode->str_begin, parserNode->str_end, "Not supported");
    return NULL;
  }

  AstTree *value = astTreeParse(node_metadata->value, helper);
  if (value == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *type;
  if (node_metadata->type == NULL) {
    type = NULL;
  } else {
    type = astTreeParse(node_metadata->type, helper);
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

  if (!pushVariable(helper, helper->variables[helper->variables_size - 1],
                    variable)) {
    astTreeVariableDelete(variable);
    goto RETURN_ERROR;
  }

  return true;
RETURN_ERROR:
  return false;
}

AstTree *astTreeParseVariable(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->value == NULL ||
      node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
    printError(parserNode->str_begin, parserNode->str_end, "Not supported");
    return NULL;
  }

  AstTree *value = astTreeParse(node_metadata->value, helper);
  if (value == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *type;
  if (node_metadata->type == NULL) {
    type = NULL;
  } else {
    type = astTreeParse(node_metadata->type, helper);
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

  if (!pushVariable(helper, helper->variables[helper->variables_size - 1],
                    variable)) {
    astTreeVariableDelete(variable);
    goto RETURN_ERROR;
  }

  return newAstTree(AST_TREE_TOKEN_VARIABLE_DEFINE, variable, NULL,
                    parserNode->str_begin, parserNode->str_end);
RETURN_ERROR:
  return NULL;
}

AstTree *astTreeParseIf(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNodeIfMetadata *node_metadata = parserNode->metadata;

  AstTree *condition = astTreeParse(node_metadata->condition, helper);
  if (condition == NULL) {
    return NULL;
  }

  AstTree *ifBody = astTreeParse(node_metadata->ifBody, helper);
  if (ifBody == NULL) {
    return NULL;
  }

  AstTree *elseBody;
  if (node_metadata->elseBody != NULL) {
    elseBody = astTreeParse(node_metadata->elseBody, helper);
    if (elseBody == NULL) {
      return NULL;
    }
  } else {
    elseBody = NULL;
  }

  AstTreeIf *metadata = a404m_malloc(sizeof(*metadata));
  metadata->condition = condition;
  metadata->ifBody = ifBody;
  metadata->elseBody = elseBody;

  return newAstTree(AST_TREE_TOKEN_KEYWORD_IF, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseWhile(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNodeWhileMetadata *node_metadata = parserNode->metadata;

  AstTree *condition = astTreeParse(node_metadata->condition, helper);
  if (condition == NULL) {
    return NULL;
  }

  AstTree *body = astTreeParse(node_metadata->body, helper);
  if (body == NULL) {
    return NULL;
  }

  AstTreeWhile *metadata = a404m_malloc(sizeof(*metadata));
  metadata->condition = condition;
  metadata->body = body;

  return newAstTree(AST_TREE_TOKEN_KEYWORD_WHILE, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseComptime(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeSingleChild *metadata =
      (AstTreeSingleChild *)astTreeParse((ParserNode *)node_metadata, helper);

  return newAstTree(AST_TREE_TOKEN_KEYWORD_COMPTIME, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseCurlyBracket(ParserNode *parserNode,
                                  AstTreeHelper *p_helper) {
  ParserNodeArray *body = parserNode->metadata;

  size_t expressions_size = 0;

  AstTreeScope *scope = a404m_malloc(sizeof(*scope));
  scope->variables.data = a404m_malloc(0);
  scope->variables.size = 0;
  scope->expressions = a404m_malloc(0);
  scope->expressions_size = 0;

  size_t variables_size = p_helper->variables_size + 1;
  AstTreeVariables *variables[variables_size];
  for (size_t i = 0; i < p_helper->variables_size; ++i) {
    variables[i] = p_helper->variables[i];
  }
  variables[variables_size - 1] = &scope->variables;

  AstTreeHelper helper = {
      .variables = variables,
      .variables_size = variables_size,
      .variable = p_helper->variable,
      .globalDeps = p_helper->globalDeps,
  };

  for (size_t i = 0; i < body->size; ++i) {
    ParserNode *node = body->data[i];
    switch (node->token) {
    case PARSER_TOKEN_SYMBOL_EOL:
      node = (ParserNodeSingleChildMetadata *)node->metadata;
      goto OK_NODE;
    case PARSER_TOKEN_KEYWORD_IF:
    case PARSER_TOKEN_KEYWORD_WHILE:
      goto OK_NODE;
    case PARSER_TOKEN_ROOT:
    case PARSER_TOKEN_IDENTIFIER:
    case PARSER_TOKEN_VALUE_INT:
    case PARSER_TOKEN_VALUE_FLOAT:
    case PARSER_TOKEN_VALUE_BOOL:
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
    case PARSER_TOKEN_TYPE_F16:
    case PARSER_TOKEN_TYPE_F32:
    case PARSER_TOKEN_TYPE_F64:
    case PARSER_TOKEN_TYPE_F128:
    case PARSER_TOKEN_TYPE_BOOL:
    case PARSER_TOKEN_KEYWORD_PRINT_U64:
    case PARSER_TOKEN_KEYWORD_RETURN:
    case PARSER_TOKEN_KEYWORD_COMPTIME:
    case PARSER_TOKEN_KEYWORD_STRUCT:
    case PARSER_TOKEN_CONSTANT:
    case PARSER_TOKEN_VARIABLE:
    case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    case PARSER_TOKEN_SYMBOL_PARENTHESIS:
    case PARSER_TOKEN_SYMBOL_COMMA:
    case PARSER_TOKEN_OPERATOR_ASSIGN:
    case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
    case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
    case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
    case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
    case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
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
    case PARSER_TOKEN_FUNCTION_CALL:
    case PARSER_TOKEN_KEYWORD_NULL:
    case PARSER_TOKEN_KEYWORD_UNDEFINED:
    case PARSER_TOKEN_OPERATOR_POINTER:
    case PARSER_TOKEN_OPERATOR_ADDRESS:
    case PARSER_TOKEN_OPERATOR_DEREFERENCE:
    case PARSER_TOKEN_OPERATOR_ACCESS:
      printError(node->str_begin, node->str_end, "Unexpected %s",
                 PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    case PARSER_TOKEN_NONE:
    }
    UNREACHABLE;
  OK_NODE:

    if (node->token == PARSER_TOKEN_CONSTANT) {
      if (!astTreeParseConstant(node, &helper)) {
        goto RETURN_ERROR;
      }
    } else {
      AstTree *tree = astTreeParse(node, &helper);

      if (tree == NULL) {
        goto RETURN_ERROR;
      }

      if (expressions_size == scope->expressions_size) {
        expressions_size += expressions_size / 2 + 1;
        scope->expressions = a404m_realloc(
            scope->expressions, expressions_size * sizeof(*scope->expressions));
      }
      scope->expressions[scope->expressions_size] = tree;
      scope->expressions_size += 1;
    }
  }

  scope->expressions =
      a404m_realloc(scope->expressions,
                    scope->expressions_size * sizeof(*scope->expressions));

  return newAstTree(AST_TREE_TOKEN_SCOPE, scope, NULL, parserNode->str_begin,
                    parserNode->str_end);

RETURN_ERROR:
  for (size_t i = 0; i < scope->expressions_size; ++i) {
    astTreeDelete(scope->expressions[i]);
  }
  free(scope->variables.data);
  free(scope->expressions);
  return NULL;
}

AstTree *astTreeParseParenthesis(ParserNode *parserNode,
                                 AstTreeHelper *helper) {
  ParserNodeArray *metadata = parserNode->metadata;

  if (metadata->size != 1) {
    printError(parserNode->str_begin, parserNode->str_end, "Bad parenthesis");
    return NULL;
  } else {
    return astTreeParse(metadata->data[0], helper);
  }
}

AstTree *astTreeParseStruct(ParserNode *parserNode, AstTreeHelper *helper) {
  ParserNode *body = parserNode->metadata;
  ParserNodeArray *body_metadata = body->metadata;
  AstTreeVariables variables = {
      .data = a404m_malloc(sizeof(*variables.data) * body_metadata->size),
      .size = body_metadata->size,
  };

  for (size_t i = 0; i < body_metadata->size; ++i) {
    ParserNode *node = body_metadata->data[i];
    if (node->token != PARSER_TOKEN_SYMBOL_EOL) {
      printError(node->str_begin, node->str_end,
                 "Semicolon is required, maybe forget a semicolon?");
      return NULL;
    }
    node = (ParserNodeSingleChildMetadata *)node->metadata;

    if (node->token != PARSER_TOKEN_VARIABLE) {
      printError(node->str_begin, node->str_end,
                 "Only variable definitions are allowed here");
      return NULL;
    }
    ParserNodeVariableMetadata *node_variable = node->metadata;

    AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
    variable->name_begin = node_variable->name->str_begin;
    variable->name_end = node_variable->name->str_end;
    if (node_variable->type != NULL) {
      variable->type = astTreeParse(node_variable->type, helper);
    } else {
      variable->type = NULL;
    }
    if (node_variable->value != NULL) {
      printError(node->str_begin, node->str_end, "Can't have default values");
      NOT_IMPLEMENTED;
    }
    variable->value = NULL;
    variable->isConst = false;

    variables.data[i] = variable;
  }

  AstTreeStruct *metadata = a404m_malloc(sizeof(*metadata));
  metadata->variables = variables;
  metadata->id = (size_t)metadata; // TODO: change it

  return newAstTree(AST_TREE_TOKEN_KEYWORD_STRUCT, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseAccessOperator(ParserNode *parserNode,
                                    AstTreeHelper *helper, AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *object = astTreeParse(node_metadata->left, helper);
  if (object == NULL) {
    return NULL;
  }

  ParserNode *right_node = node_metadata->right;
  if (right_node->token != PARSER_TOKEN_IDENTIFIER) {
    printError(right_node->str_begin, right_node->str_end,
               "Should be an identifier but got `%s`",
               PARSER_TOKEN_STRINGS[right_node->token]);
    return NULL;
  }

  AstTreeAccess *metadata = a404m_malloc(sizeof(*metadata));

  metadata->object = object;
  metadata->member.name.begin = right_node->str_begin;
  metadata->member.name.end = right_node->str_end;

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

bool isFunction(AstTree *value) {
  return value->type->token == AST_TREE_TOKEN_TYPE_FUNCTION;
}

bool isConst(AstTree *tree) {
  if (tree->type == NULL) {
    UNREACHABLE;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
    return true;
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = tree->metadata;
    return isConst(metadata->condition) && isConst(metadata->ifBody) &&
           (metadata->elseBody == NULL || isConst(metadata->elseBody));
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->parameters_size; ++i) {
      if (!isConst(metadata->parameters[i].value)) {
        return false;
      }
    }
    return isConst(metadata->function);
  }
  case AST_TREE_TOKEN_FUNCTION: {
    return true;
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
    return false;
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = tree->metadata;
    return metadata->isConst;
  }
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    AstTreeSingleChild *metadata = tree->metadata;
    return isConst(metadata);
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = tree->metadata;
    return isConst(metadata->object);
  }
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

AstTree *makeTypeOf(AstTree *value) {
  switch (value->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
    return &AST_TREE_TYPE_TYPE;
  case AST_TREE_TOKEN_OPERATOR_ADDRESS: {
    AstTreeSingleChild *metadata = value->metadata;
    return newAstTree(AST_TREE_TOKEN_OPERATOR_POINTER, makeTypeOf(metadata),
                      &AST_TREE_TYPE_TYPE, value->str_begin, value->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE: {
    AstTreeSingleChild *metadata = value->metadata;
    AstTree *type = makeTypeOf(metadata);
    if (type->token != AST_TREE_TOKEN_OPERATOR_POINTER) {
      UNREACHABLE;
    }
    AstTree *ret = type->metadata;
    astTreeDelete(type->type);
    astTreeDelete(type->metadata);
    return ret;
  }
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
      AstTreeVariable *arg = function->arguments.data[i];
      type_metadata->arguments[i].name_begin = arg->name_begin;
      type_metadata->arguments[i].name_end = arg->name_end;
      type_metadata->arguments[i].type = copyAstTree(arg->type);
    }

    return newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                      &AST_TREE_TYPE_TYPE, value->str_begin, value->str_end);
  }
  case AST_TREE_TOKEN_VALUE_VOID:
    return &AST_TREE_VOID_TYPE;
  case AST_TREE_TOKEN_VALUE_INT:
    return &AST_TREE_U64_TYPE;
  case AST_TREE_TOKEN_VALUE_FLOAT:
    return &AST_TREE_F128_TYPE;
  case AST_TREE_TOKEN_VALUE_BOOL:
    return &AST_TREE_BOOL_TYPE;
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *variable = value->metadata;
    return copyAstTree(variable->type);
  }
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeSingleChild *metadata = value->metadata;
    return copyAstTree(metadata->type);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO: {
    AstTreeInfix *metadata = value->metadata;
    return copyAstTree(metadata->left.type);
  }
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    return &AST_TREE_BOOL_TYPE;
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = value->metadata;

    AstTreeStruct *struc = metadata->object->type->metadata;
    const size_t size = metadata->member.name.end - metadata->member.name.begin;
    const char *str = metadata->member.name.begin;

    for (size_t i = 0; i < struc->variables.size; ++i) {
      AstTreeVariable *member = struc->variables.data[i];
      const size_t member_size = member->name_end - member->name_begin;
      if (member_size == size && strncmp(member->name_begin, str, size)) {
        return copyAstTree(member->type);
      }
    }
    UNREACHABLE;
  }
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool typeIsEqual(AstTree *type0, AstTree *type1) {
  AstTree *left = getValue(type0);
  AstTree *right = getValue(type1);
  if (left == NULL || right == NULL) {
    printLog("Can't check types");
    UNREACHABLE;
  }

  bool ret = typeIsEqualBack(left, right);

  if (type0 != left) {
    astTreeDelete(left);
  }
  if (type1 != right) {
    astTreeDelete(right);
  }

  return ret;
}

bool typeIsEqualBack(const AstTree *type0, const AstTree *type1) {
  switch (type0->token) {
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
    return false;
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
    return type1->token == type0->token;
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    if (type1->token != AST_TREE_TOKEN_OPERATOR_POINTER) {
      return false;
    }
    AstTreeSingleChild *type0_metadata = type0->metadata;
    AstTreeSingleChild *type1_metadata = type1->metadata;
    return typeIsEqual(type0_metadata, type1_metadata);
  }
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    if (type1->token != AST_TREE_TOKEN_TYPE_FUNCTION) {
      return false;
    }
    AstTreeTypeFunction *type0_metadata = type0->metadata;
    AstTreeTypeFunction *type1_metadata = type1->metadata;
    if (!typeIsEqual(type0_metadata->returnType->type,
                     type1_metadata->returnType->type) ||
        type0_metadata->arguments_size != type1_metadata->arguments_size) {
      return false;
    }
    for (size_t i = 0; i < type0_metadata->arguments_size; ++i) {
      AstTreeTypeFunctionArgument p0 = type0_metadata->arguments[i];
      AstTreeTypeFunctionArgument p1 = type1_metadata->arguments[i];
      if (!typeIsEqual(p0.type, p1.type)) {
        return false;
      }
    }
    return true;
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    if (type1->token != AST_TREE_TOKEN_KEYWORD_STRUCT)
      return false;

    AstTreeStruct *type0_metadata = type0->metadata;
    AstTreeStruct *type1_metadata = type1->metadata;
    return type0_metadata->id == type1_metadata->id;
  }
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

AstTree *getValue(AstTree *tree) {
  if (!isConst(tree)) {
    printError(tree->str_begin, tree->str_end,
               "Can't get value at compile time because it is not const");
    return NULL;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_SCOPE: {
    bool shouldRet = false;
    AstTree *value = runExpression(tree, &shouldRet, false);
    if (value == NULL) {
      printError(tree->str_begin, tree->str_end, "Unknown error");
    }
    return value;
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_FUNCTION: {
    return tree;
  }
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isCircularDependencies(AstTreeHelper *helper, AstTreeVariable *variable) {
  AstTreeVariables checkedVariables = {
      .data = a404m_malloc(0),
      .size = 0,
  };
  bool ret = isCircularDependenciesBack(helper, variable, variable->value,
                                        &checkedVariables);
  free(checkedVariables.data);
  return ret;
}

bool isCircularDependenciesBack(AstTreeHelper *helper,
                                AstTreeVariable *variable, AstTree *tree,
                                AstTreeVariables *checkedVariables) {
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
    return false;
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeSingleChild *metadata = tree->metadata;
    return isCircularDependenciesBack(helper, variable, metadata,
                                      checkedVariables);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeInfix *metadata = tree->metadata;
    return isCircularDependenciesBack(helper, variable, &metadata->left,
                                      checkedVariables) ||
           isCircularDependenciesBack(helper, variable, &metadata->right,
                                      checkedVariables);
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->parameters_size; ++i) {
      if (isCircularDependenciesBack(helper, variable,
                                     metadata->parameters[i].value,
                                     checkedVariables)) {
        return true;
      }
    }
    return isCircularDependenciesBack(helper, variable, metadata->function,
                                      checkedVariables);
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = tree->metadata;
    for (size_t index = 0; index < helper->variables[0]->size; ++index) {
      if (helper->variables[0]->data[index] == metadata) {
        for (size_t i = 0; i < helper->globalDeps[index].size; ++i) {
          AstTreeVariable *currentVariable = helper->globalDeps[index].data[i];
          if (currentVariable == variable ||
              isCircularDependenciesVariable(helper, variable, currentVariable,
                                             checkedVariables)) {
            return true;
          }
        }
        break;
      }
    }
    return false;
  }
  case AST_TREE_TOKEN_VALUE_OBJECT: {
    AstTreeObject *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *variable = metadata->variables.data[i];
      if (variable->value != NULL) {
        if (isCircularDependenciesBack(helper, variable, variable->value,
                                       checkedVariables)) {
          return true;
        }
      }
    }
    return false;
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_FUNCTION: {
    return false;
  }
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isCircularDependenciesVariable(AstTreeHelper *helper,
                                    AstTreeVariable *toBeFound,
                                    AstTreeVariable *currentVariable,
                                    AstTreeVariables *checkedVariables) {
  for (size_t i = 0; i < checkedVariables->size; ++i) {
    if (currentVariable == checkedVariables->data[i]) {
      return false;
    }
  }

  const size_t capacity = a404m_malloc_usable_size(checkedVariables->data) /
                          sizeof(*checkedVariables->data);

  if (capacity == checkedVariables->size) {
    checkedVariables->data = a404m_realloc(checkedVariables->data,
                                           (capacity + capacity / 2 + 1) *
                                               sizeof(*checkedVariables->data));
  }
  checkedVariables->data[checkedVariables->size] = currentVariable;
  checkedVariables->size += 1;

  for (size_t index = 0; index < helper->variables[0]->size; ++index) {
    if (helper->variables[0]->data[index] == currentVariable) {
      for (size_t i = 0; i < helper->globalDeps[index].size; ++i) {
        AstTreeVariable *var = helper->globalDeps[index].data[i];
        if (var == toBeFound || isCircularDependenciesVariable(
                                    helper, toBeFound, var, checkedVariables)) {
          return true;
        }
      }
      break;
    }
  }
  return false;
}

bool setAllTypesRoot(AstTreeRoot *root, AstTreeHelper *helper) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    if (isCircularDependencies(helper, variable)) {
      printError(variable->name_begin, variable->name_end,
                 "Circular dependecies");
      return false;
    }
  }

  AstTreeSetTypesHelper setTypesHelper = {
      .lookingType = NULL,
      .treeHelper = helper,
  };

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    if (!setTypesAstVariable(variable, setTypesHelper)) {
      return false;
    }
  }

  return true;
}

bool setAllTypes(AstTree *tree, AstTreeSetTypesHelper helper,
                 AstTreeFunction *function) {
  if (tree->type != NULL) {
    return true;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F16:
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_VALUE_VOID:
    return true;
  case AST_TREE_TOKEN_VALUE_BOOL:
    return setTypesValueBool(tree, helper);
  case AST_TREE_TOKEN_VALUE_INT:
    return setTypesValueInt(tree, helper);
  case AST_TREE_TOKEN_VALUE_FLOAT:
    return setTypesValueFloat(tree, helper);
  case AST_TREE_TOKEN_VALUE_NULL:
    return setTypesValueNull(tree, helper);
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
    return setTypesValueUndefined(tree, helper);
  case AST_TREE_TOKEN_VALUE_OBJECT:
    return setTypesValueObject(tree, helper);
  case AST_TREE_TOKEN_FUNCTION:
    return setTypesFunction(tree, helper);
  case AST_TREE_TOKEN_KEYWORD_PRINT_U64:
    return setTypesPrintU64(tree, helper);
  case AST_TREE_TOKEN_KEYWORD_RETURN:
    return setTypesReturn(tree, helper, function);
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    return setTypesTypeFunction(tree, helper);
  case AST_TREE_TOKEN_FUNCTION_CALL:
    return setTypesFunctionCall(tree, helper);
  case AST_TREE_TOKEN_VARIABLE:
    return setTypesVariable(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
    return setTypesOperatorAssign(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
    return setTypesOperatorUnary(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
    return setTypesOperatorInfix(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
    return setTypesOperatorInfixWithRet(tree, &AST_TREE_BOOL_TYPE, helper);
  case AST_TREE_TOKEN_OPERATOR_POINTER:
    return setTypesOperatorPointer(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
    return setTypesOperatorAddress(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
    return setTypesOperatorDereference(tree, helper);
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    return setTypesVariableDefine(tree, helper);
  case AST_TREE_TOKEN_KEYWORD_IF:
    return setTypesIf(tree, helper, function);
  case AST_TREE_TOKEN_KEYWORD_WHILE:
    return setTypesWhile(tree, helper, function);
  case AST_TREE_TOKEN_SCOPE:
    return setTypesScope(tree, helper, function);
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
    return setTypesComptime(tree, helper);
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
    return setTypesStruct(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
    return setTypesOperatorAccess(tree, helper);
  case AST_TREE_TOKEN_NONE:
    break;
  }
  printError(tree->str_begin, tree->str_end, "Unknown token %d", tree->token);
  UNREACHABLE;
}

bool setTypesValueBool(AstTree *tree, AstTreeSetTypesHelper helper) {
  (void)helper;
  tree->type = &AST_TREE_BOOL_TYPE;
  return true;
}

bool setTypesValueInt(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.lookingType == NULL ||
      typeIsEqual(helper.lookingType, &AST_TREE_I64_TYPE)) {
    tree->type = &AST_TREE_I64_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U64_TYPE)) {
    tree->type = &AST_TREE_U64_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_I32_TYPE)) {
    AstTreeInt *value = tree->metadata;
    i32 newValue = *value;
    *value = newValue;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_I32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U32_TYPE)) {
    AstTreeInt *value = tree->metadata;
    u32 newValue = *value;
    *value = newValue;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_U32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_I16_TYPE)) {
    AstTreeInt *value = tree->metadata;
    i16 newValue = *value;
    *value = newValue;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_I16_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U16_TYPE)) {
    AstTreeInt *value = tree->metadata;
    u16 newValue = *value;
    *value = newValue;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_U16_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_I8_TYPE)) {
    AstTreeInt *value = tree->metadata;
    i8 newValue = *value;
    *value = newValue;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_I8_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U8_TYPE)) {
    AstTreeInt *value = tree->metadata;
    u8 newValue = *value;
    *value = newValue;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_U8_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F16_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeInt *value = tree->metadata;
    f16 newValue = *value;
    tree->metadata = a404m_malloc(sizeof(f128));
    *(AstTreeFloat *)tree->metadata = *value;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->type = &AST_TREE_F16_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F32_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeInt *value = tree->metadata;
    f32 newValue = *value;
    tree->metadata = a404m_malloc(sizeof(f128));
    *(AstTreeFloat *)tree->metadata = *value;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->type = &AST_TREE_F32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F64_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeInt *value = tree->metadata;
    f64 newValue = *value;
    tree->metadata = a404m_malloc(sizeof(f128));
    *(AstTreeFloat *)tree->metadata = *value;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->type = &AST_TREE_F64_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F128_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeInt *value = tree->metadata;
    f128 newValue = *value;
    tree->metadata = a404m_malloc(sizeof(f128));
    *(AstTreeFloat *)tree->metadata = *value;
    if (*value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->type = &AST_TREE_F128_TYPE;
  } else {
    UNREACHABLE;
  }
  return true;
}

bool setTypesValueFloat(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.lookingType == NULL ||
      typeIsEqual(helper.lookingType, &AST_TREE_F64_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeFloat value = *(AstTreeFloat *)tree->metadata;
    f64 newValue = value;
    *(AstTreeFloat *)tree->metadata = value;
    if (value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_F64_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F16_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeFloat value = *(AstTreeFloat *)tree->metadata;
    f16 newValue = value;
    *(AstTreeFloat *)tree->metadata = value;
    if (value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_F16_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F32_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeFloat value = *(AstTreeFloat *)tree->metadata;
    f32 newValue = value;
    *(AstTreeFloat *)tree->metadata = value;
    if (value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_F32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F128_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeFloat value = *(AstTreeFloat *)tree->metadata;
    f128 newValue = value;
    *(AstTreeFloat *)tree->metadata = value;
    if (value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_F128_TYPE;
  } else {
    UNREACHABLE;
  }

  return true;
}

bool setTypesValueNull(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.lookingType == NULL) {
    printError(tree->str_begin, tree->str_end, "Can't find type of null");
    return false;
  } else if (helper.lookingType->token == AST_TREE_TOKEN_OPERATOR_POINTER) {
    printError(tree->str_begin, tree->str_end,
               "Null must have type of a pointer");
    return false;
  }
  tree->type = copyAstTree(helper.lookingType);
  return true;
}

bool setTypesValueUndefined(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.lookingType == NULL) {
    printError(tree->str_begin, tree->str_end, "Can't find type of undefined");
    return false;
  }
  tree->type = copyAstTree(helper.lookingType);
  return true;
}

bool setTypesValueObject(AstTree *tree, AstTreeSetTypesHelper helper) {
  (void)tree;
  (void)helper;
  NOT_IMPLEMENTED;
}

bool setTypesFunction(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeFunction *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->arguments.size; ++i) {
    AstTreeVariable *variable = metadata->arguments.data[i];
    if (!setTypesAstVariable(variable, helper)) {
      return false;
    }
  }

  if (!setAllTypes(metadata->returnType, helper, NULL)) {
    return false;
  }

  tree->type = makeTypeOf(tree);

  for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
    if (!setAllTypes(metadata->scope.expressions[i], helper, metadata)) {
      return false;
    }
  }

  for (size_t i = 0; i < metadata->scope.variables.size; ++i) {
    if (!setTypesAstVariable(metadata->scope.variables.data[i], helper)) {
      return false;
    }
  }

  return true;
}

bool setTypesPrintU64(AstTree *tree, AstTreeSetTypesHelper _helper) {
  AstTreeSingleChild *metadata = tree->metadata;
  AstTreeSetTypesHelper helper = {
      .lookingType = &AST_TREE_U64_TYPE,
      .treeHelper = _helper.treeHelper,
  };
  if (!setAllTypes(metadata, helper, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->type, &AST_TREE_U64_TYPE)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else {
    tree->type = &AST_TREE_VOID_TYPE;
    return true;
  }
}

bool setTypesReturn(AstTree *tree, AstTreeSetTypesHelper _helper,
                    AstTreeFunction *function) {
  if (function == NULL) {
    printError(tree->str_begin, tree->str_end, "Return can't be here");
  }

  AstTreeReturn *metadata = tree->metadata;
  if (metadata->value != NULL) {
    AstTreeSetTypesHelper helper = {
        .lookingType = getValue(function->returnType),
        .treeHelper = _helper.treeHelper,
    };
    if (helper.lookingType == NULL) {
      return false;
    }
    if (!setAllTypes(metadata->value, helper, NULL)) {
      return false;
    }
    if (helper.lookingType != function->returnType) {
      astTreeDelete(helper.lookingType); // TODO: change plan
    }
    if (!typeIsEqual(metadata->value->type, function->returnType)) {
      printError(tree->str_begin, tree->str_end, "Type mismatch");
      return false;
    }
  }
  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesTypeFunction(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeTypeFunction *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->arguments_size; ++i) {
    AstTreeTypeFunctionArgument arg = metadata->arguments[i];
    if (!setAllTypes(arg.type, helper, NULL)) {
      return false;
    } else if (!typeIsEqual(arg.type, &AST_TREE_TYPE_TYPE)) {
      printError(arg.str_begin, arg.str_end, "Expected a type");
      return false;
    }
  }

  if (!setAllTypes(metadata->returnType, helper, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->returnType->type, &AST_TREE_TYPE_TYPE)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  }

  tree->type = &AST_TREE_TYPE_TYPE;
  return true;
}

bool setTypesFunctionCall(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeFunctionCall *metadata = tree->metadata;

  if (!setAllTypes(metadata->function, helper, NULL)) {
    return false;
  } else if (!isFunction(metadata->function)) {
    printError(metadata->function->str_begin, metadata->function->str_end,
               "Object is not a function");
    return false;
  }

  AstTreeTypeFunction *function = metadata->function->type->metadata;
  if (function == NULL ||
      function->arguments_size != metadata->parameters_size) {
    printError(tree->str_begin, tree->str_end,
               "Arguments doesn't match %ld != %ld", function->arguments_size,
               metadata->parameters_size);
    return NULL;
  }

  AstTreeFunctionCallParam initedArguments[function->arguments_size];
  size_t initedArguments_size = function->arguments_size;

  for (size_t i = 0; i < initedArguments_size; ++i) {
    initedArguments[i].value = NULL;
  }

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    AstTreeFunctionCallParam param = metadata->parameters[i];
    if (param.nameBegin != param.nameEnd) {
      const size_t param_name_size = param.nameEnd - param.nameBegin;
      for (size_t j = 0; j < function->arguments_size; ++j) {
        AstTreeTypeFunctionArgument arg = function->arguments[j];
        if ((size_t)(arg.name_end - arg.name_begin) == param_name_size &&
            strncmp(arg.name_begin, param.nameBegin, param_name_size) == 0) {
          initedArguments[j] = param;
          AstTreeSetTypesHelper newHelper = {
              .lookingType = arg.type,
              .treeHelper = helper.treeHelper,
          };

          if (!setAllTypes(param.value, newHelper, NULL)) {
            return false;
          }
          goto END_OF_NAMED_FOR;
        }
      }
      printError(param.value->str_begin, param.value->str_end,
                 "Argument not found");
      return false;
    }
  END_OF_NAMED_FOR:
  }

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    AstTreeFunctionCallParam param = metadata->parameters[i];
    if (param.nameBegin == param.nameEnd) {
      for (size_t j = 0; j < function->arguments_size; ++j) {
        AstTreeTypeFunctionArgument arg = function->arguments[j];
        if (initedArguments[j].value == NULL) {
          initedArguments[j] = param;
          AstTreeSetTypesHelper newHelper = {
              .lookingType = arg.type,
              .treeHelper = helper.treeHelper,
          };

          if (!setAllTypes(param.value, newHelper, NULL)) {
            return false;
          }
          goto END_OF_UNNAMED_FOR;
        }
      }
      printError(param.value->str_begin, param.value->str_end,
                 "Too many arguments");
      return false;
    }
  END_OF_UNNAMED_FOR:
  }

  for (size_t i = 0; i < function->arguments_size; ++i) {
    AstTreeTypeFunctionArgument arg = function->arguments[i];
    if (initedArguments[i].value == NULL) {
      printError(arg.str_begin, arg.str_end, "Argument is not initialized");
      return false;
    }
  }

  for (size_t i = 0; i < initedArguments_size; ++i) {
    metadata->parameters[i] = initedArguments[i];
  }

  tree->type = copyAstTree(function->returnType);
  return true;
}

bool setTypesVariable(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeVariable *metadata = tree->metadata;
  if (setTypesAstVariable(metadata, helper)) {
    tree->type = copyAstTree(metadata->type);
    return true;
  } else {
    return false;
  }
}

bool setTypesOperatorAssign(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeInfix *infix = tree->metadata;
  // TODO: check left one for being left value
  if (!setTypesAstInfix(infix, helper)) {
    return false;
  } else if (!typeIsEqual(infix->left.type, infix->right.type)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else if (isConst(&infix->left)) {
    printError(tree->str_begin, tree->str_end, "Constants can't be assigned");
    return false;
  } else {
    tree->type = copyAstTree(infix->left.type);
    return true;
  }
}

bool setTypesOperatorInfix(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeInfix *infix = tree->metadata;
  if (!setTypesAstInfix(infix, helper)) {
    return false;
  } else if (!typeIsEqual(infix->left.type, infix->right.type)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else {
    tree->type = copyAstTree(infix->left.type);
    return true;
  }
}

bool setTypesOperatorInfixWithRet(AstTree *tree, AstTree *retType,
                                  AstTreeSetTypesHelper helper) {
  AstTreeInfix *infix = tree->metadata;
  if (!setTypesAstInfix(infix, helper)) {
    return false;
  } else if (!typeIsEqual(infix->left.type, infix->right.type)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else {
    tree->type = retType;
    return true;
  }
}

bool setTypesOperatorUnary(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *operand = tree->metadata;
  if (!setAllTypes(operand, helper, NULL)) {
    return false;
  } else {
    tree->type = copyAstTree(operand->type);
    return true;
  }
}

bool setTypesOperatorPointer(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *metadata = tree->metadata;
  if (!setAllTypes(metadata, helper, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->type, &AST_TREE_TYPE_TYPE)) {
    printError(tree->str_begin, tree->str_end, "Pointer type needs a type");
    return false;
  }

  tree->type = &AST_TREE_TYPE_TYPE;
  return true;
}

bool setTypesOperatorAddress(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *metadata = tree->metadata;
  if (!setAllTypes(metadata, helper, NULL)) {
    return false;
  }

  if (metadata->token != AST_TREE_TOKEN_VARIABLE) {
    printError(tree->str_begin, tree->str_end,
               "Can only get address of a variable (for now)");
    return false;
  }

  tree->type =
      newAstTree(AST_TREE_TOKEN_OPERATOR_POINTER, copyAstTree(metadata->type),
                 &AST_TREE_TYPE_TYPE, tree->str_begin, tree->str_end);
  return true;
}

bool setTypesOperatorDereference(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *metadata = tree->metadata;
  if (!setAllTypes(metadata, helper, NULL)) {
    return false;
  }

  if (metadata->type->token != AST_TREE_TOKEN_OPERATOR_POINTER) {
    printError(tree->str_begin, tree->str_end,
               "Can only dereferenece pointers");
    return false;
  }

  tree->type = copyAstTree(metadata->type->metadata);
  return true;
}

bool setTypesVariableDefine(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeVariable *metadata = tree->metadata;
  tree->type = &AST_TREE_VOID_TYPE;
  return setTypesAstVariable(metadata, helper);
}

bool setTypesAstVariable(AstTreeVariable *variable,
                         AstTreeSetTypesHelper _helper) {
  AstTreeSetTypesHelper helper = {
      .lookingType = &AST_TREE_TYPE_TYPE,
      .treeHelper = _helper.treeHelper,
  };

  if (variable->type != NULL && !setAllTypes(variable->type, helper, NULL)) {
    return false;
  }

  if (variable->type != NULL) {
    AstTree *type = variable->type;
    variable->type = getValue(type);
    if (variable->type == NULL) {
      return false;
    }
    if (type != variable->type) {
      astTreeDelete(type);
    }
  }

  helper.lookingType = variable->type;

  if (variable->value != NULL && !setAllTypes(variable->value, helper, NULL)) {
    return false;
  }

  if (variable->type == NULL) {
    variable->type = copyAstTree(variable->value->type);
  }

  if (variable->type == NULL) {
    return false;
  } else if (variable->value != NULL) {
    if (variable->value != NULL &&
        !typeIsEqual(variable->value->type, variable->type)) {
      printError(variable->name_begin, variable->name_end,
                 "Type mismatch value = %s but type = %s",
                 AST_TREE_TOKEN_STRINGS[variable->value->type->token],
                 AST_TREE_TOKEN_STRINGS[variable->type->token]);
      return false;
    } else if (variable->isConst) {
      if (!isConst(variable->value)) {
        printError(variable->value->str_begin, variable->value->str_end,
                   "Can't initialize constant with non constant value");
        return false;
      }
      AstTree *value = variable->value;
      variable->value = getValue(value);
      if (variable->value == NULL) {
        return false;
      }
      if (variable->value != value) {
        astTreeDelete(value);
      }

      if (variable->value == NULL) {
        return false;
      }
    }
  }

  return true;
}

bool setTypesIf(AstTree *tree, AstTreeSetTypesHelper helper,
                AstTreeFunction *function) {
  AstTreeIf *metadata = tree->metadata;

  if (!setAllTypes(metadata->condition, helper, function) ||
      !setAllTypes(metadata->ifBody, helper, function) ||
      (metadata->elseBody != NULL &&
       !setAllTypes(metadata->elseBody, helper, function))) {
    return false;
  }

  if (!typeIsEqual(metadata->condition->type, &AST_TREE_BOOL_TYPE)) {
    printError(metadata->condition->str_begin, metadata->condition->str_end,
               "If condition must be boolean");
    return false;
  }

  if (metadata->elseBody != NULL &&
      typeIsEqual(metadata->ifBody->type, metadata->elseBody->type)) {
    tree->type = copyAstTree(metadata->ifBody->type);
  } else {
    tree->type = &AST_TREE_VOID_TYPE;
  }

  return true;
}

bool setTypesWhile(AstTree *tree, AstTreeSetTypesHelper helper,
                   AstTreeFunction *function) {
  AstTreeWhile *metadata = tree->metadata;

  if (!setAllTypes(metadata->condition, helper, function) ||
      !setAllTypes(metadata->body, helper, function)) {
    return false;
  }

  if (!typeIsEqual(metadata->condition->type, &AST_TREE_BOOL_TYPE)) {
    printError(metadata->condition->str_begin, metadata->condition->str_end,
               "If condition must be boolean");
    return false;
  }

  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesScope(AstTree *tree, AstTreeSetTypesHelper helper,
                   AstTreeFunction *function) {
  AstTreeScope *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->expressions_size; ++i) {
    if (!setAllTypes(metadata->expressions[i], helper, function)) {
      return false;
    }
  }

  for (size_t i = 0; i < metadata->variables.size; ++i) {
    if (!setTypesAstVariable(metadata->variables.data[i], helper)) {
      return false;
    }
  }

  if (metadata->expressions_size == 0) {
    tree->type = &AST_TREE_VOID_VALUE;
  } else {
    tree->type = copyAstTree(
        metadata->expressions[metadata->expressions_size - 1]->type);
  }

  return true;
}

bool setTypesComptime(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *operand = tree->metadata;

  if (!setAllTypes(operand, helper, NULL)) {
    return false;
  }

  AstTree *newTree = getValue(operand);
  if (newTree == NULL) {
    return false;
  }

  if (operand != newTree) {
    astTreeDelete(operand);
    operand = newTree;
  }
  tree->metadata = operand;
  tree->type = copyAstTree(operand->type);

  return true;
}

bool setTypesStruct(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeStruct *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->variables.size; ++i) {
    if (!setTypesAstVariable(metadata->variables.data[i], helper)) {
      return false;
    }
  }
  tree->type = &AST_TREE_TYPE_TYPE;
  return true;
}

bool setTypesOperatorAccess(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeAccess *metadata = tree->metadata;
  if (!setAllTypes(metadata->object, helper, NULL)) {
    return false;
  } else if (metadata->object->type->token != AST_TREE_TOKEN_KEYWORD_STRUCT) {
    printError(metadata->object->str_begin, metadata->object->str_end,
               "The object is not a struct");
    return false;
  }

  AstTreeStruct *struc = metadata->object->type->metadata;
  const size_t size = metadata->member.name.end - metadata->member.name.begin;
  const char *str = metadata->member.name.begin;

  for (size_t i = 0; i < struc->variables.size; ++i) {
    AstTreeVariable *member = struc->variables.data[i];
    const size_t member_size = member->name_end - member->name_begin;
    if (member_size == size && strncmp(member->name_begin, str, size) == 0) {
      metadata->member.index = i;
      tree->type = copyAstTree(member->type);
      return true;
    }
  }

  printError(metadata->member.name.begin, metadata->member.name.end,
             "Member not found");
  return false;
}

bool setTypesAstInfix(AstTreeInfix *infix, AstTreeSetTypesHelper helper) {
  if (!setAllTypes(&infix->left, helper, NULL)) {
    return false;
  }
  AstTreeSetTypesHelper newHelper = {
      .lookingType = infix->left.type,
      .treeHelper = helper.treeHelper,
  };

  return setAllTypes(&infix->right, newHelper, NULL);
}
