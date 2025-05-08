#include "ast-tree.h"

#include "compiler/parser.h"
#include "runner/runner.h"
#include "utils/file.h"
#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"
#include "utils/time.h"
#include "utils/type.h"
#include <stdlib.h>

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

#ifdef FLOAT_16_SUPPORT
AstTree AST_TREE_F16_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F16,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
};
#endif

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

AstTree AST_TREE_CODE_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_CODE,
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

    "AST_TREE_TOKEN_BUILTIN_CAST",
    "AST_TREE_TOKEN_BUILTIN_TYPE_OF",
    "AST_TREE_TOKEN_BUILTIN_IMPORT",
    "AST_TREE_TOKEN_BUILTIN_IS_COMPTIME",
    "AST_TREE_TOKEN_BUILTIN_STACK_ALLOC",
    "AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC",
    "AST_TREE_TOKEN_BUILTIN_NEG",
    "AST_TREE_TOKEN_BUILTIN_ADD",
    "AST_TREE_TOKEN_BUILTIN_SUB",
    "AST_TREE_TOKEN_BUILTIN_MUL",
    "AST_TREE_TOKEN_BUILTIN_DIV",
    "AST_TREE_TOKEN_BUILTIN_MOD",
    "AST_TREE_TOKEN_BUILTIN_EQUAL",
    "AST_TREE_TOKEN_BUILTIN_NOT_EQUAL",
    "AST_TREE_TOKEN_BUILTIN_GREATER",
    "AST_TREE_TOKEN_BUILTIN_SMALLER",
    "AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL",
    "AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL",

    "AST_TREE_TOKEN_KEYWORD_PUTC",
    "AST_TREE_TOKEN_KEYWORD_RETURN",
    "AST_TREE_TOKEN_KEYWORD_IF",
    "AST_TREE_TOKEN_KEYWORD_WHILE",
    "AST_TREE_TOKEN_KEYWORD_COMPTIME",
    "AST_TREE_TOKEN_KEYWORD_STRUCT",

    "AST_TREE_TOKEN_TYPE_FUNCTION",
    "AST_TREE_TOKEN_TYPE_ARRAY",
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
#ifdef FLOAT_16_SUPPORT
    "AST_TREE_TOKEN_TYPE_F16",
#endif
    "AST_TREE_TOKEN_TYPE_F32",
    "AST_TREE_TOKEN_TYPE_F64",
    "AST_TREE_TOKEN_TYPE_F128",
    "AST_TREE_TOKEN_TYPE_CODE",
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
    "AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT",
    "AST_TREE_TOKEN_OPERATOR_LOGICAL_AND",
    "AST_TREE_TOKEN_OPERATOR_LOGICAL_OR",
    "AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS",

    "AST_TREE_TOKEN_SCOPE",

    "AST_TREE_TOKEN_NONE",
};

AstTreeRoots AST_TREE_ROOTS_ERROR = {
    .data = NULL,
    .size = -1ULL,
};

#ifdef PRINT_COMPILE_TREE
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
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeUnary *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("child=\n");
    astTreePrint(metadata->operand, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("function=%p", metadata->function);
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
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
    printf(",value=%lld", *metadata);
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
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
    astTreePrint(metadata->left, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("right=\n");
    astTreePrint(metadata->right, indent + 1);
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
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    AstTreeBracket *metadata = tree->metadata;
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("operand=\n");
    astTreePrint(metadata->operand, indent + 1);
    printf(",\n");
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("paramters=[\n");
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      AstTree *param = metadata->parameters.data[i];
      astTreePrint(param, indent + 2);
      printf(",\n");
    }
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("]");
  }
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;

RETURN_SUCCESS:
  printf("}");
}

void astTreeVariablePrint(const AstTreeVariable *variable, int indent) {
  for (int i = 0; i < indent; ++i)
    printf(" ");

  printf("{\n");

  for (int i = 0; i < indent; ++i)
    printf(" ");

  printf("name=\"%.*s\",\n", (int)(variable->name_end - variable->name_begin),
         variable->name_begin);

  for (int i = 0; i < indent; ++i)
    printf(" ");

  printf("type=\n");
  astTreePrint(variable->type, indent);
  printf(",\n");

  for (int i = 0; i < indent; ++i)
    printf(" ");

  printf("value=\n");

  if (variable->value == NULL) {
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("null,\n");
  } else {
    astTreePrint(variable->value, indent);
    printf(",\n");
  }

  for (int i = 0; i < indent; ++i)
    printf(" ");

  printf("initValue=\n");

  if (variable->initValue == NULL) {
    for (int i = 0; i < indent; ++i)
      printf(" ");
    printf("null\n");
  } else {
    astTreePrint(variable->initValue, indent);
    printf("\n");
  }

  for (int i = 0; i < indent; ++i)
    printf(" ");

  printf("}");
}

void astTreeRootPrint(const AstTreeRoot *root) {
  for (size_t i = 0; i < root->variables.size; ++i) {
    astTreeVariablePrint(root->variables.data[i], 0);
    printf("\n");
  }
}
#endif

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
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeUnary *metadata = tree.metadata;
    astTreeDelete(metadata->operand);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
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
    astTreeDelete(metadata->left);
    astTreeDelete(metadata->right);
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
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    AstTreeBracket *metadata = tree.metadata;
    astTreeDelete(metadata->operand);
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      astTreeDelete(metadata->parameters.data[i]);
    }
    free(metadata->parameters.data);
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
  if (variable.initValue != NULL) {
    astTreeDelete(variable.initValue);
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
  for (size_t i = 0; i < root->trees.size; ++i) {
    astTreeDelete(root->trees.data[i]);
  }
  for (size_t i = 0; i < root->variables.size; ++i) {
    astTreeVariableDelete(root->variables.data[i]);
  }
  free(root->variables.data);
  free(root->trees.data);
  free(root->filePath);
  free(root->imports);
  free(root);
}

void astTreeRootsDestroy(AstTreeRoots roots) {
  for (size_t i = 0; i < roots.size; ++i) {
    astTreeRootDelete(roots.data[i]);
  }
  free(roots.data);
}

AstTree *newAstTree(AstTreeToken token, void *metadata, AstTree *type,
                    char const *str_begin, char const *str_end) {
  AstTree *result = a404m_malloc(sizeof(*result));
  *result = (AstTree){
      .token = token,
      .metadata = metadata,
      .type = type,
      .str_begin = str_begin,
      .str_end = str_end,
  };
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_VALUE_VOID:
    return tree;
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
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

    return newAstTree(
        tree->token,
        copyAstTreeBackFindVariable(variable, oldVariables, newVariables,
                                    variables_size),
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
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

    new_metadata->left = copyAstTreeBack(metadata->left, oldVariables,
                                         newVariables, variables_size);
    new_metadata->right = copyAstTreeBack(metadata->right, oldVariables,
                                          newVariables, variables_size);
    new_metadata->function = copyAstTreeBackFindVariable(
        metadata->function, oldVariables, newVariables, variables_size);
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeUnary *metadata = tree->metadata;
    AstTreeUnary *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->operand = copyAstTreeBack(metadata->operand, oldVariables,
                                            newVariables, variables_size);
    new_metadata->function = copyAstTreeBackFindVariable(
        metadata->function, oldVariables, newVariables, variables_size);

    return newAstTree(
        tree->token, new_metadata,
        copyAstTreeBack(tree->type, oldVariables, newVariables, variables_size),
        tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
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
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    AstTreeBracket *metadata = tree->metadata;
    AstTreeBracket *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->operand = copyAstTreeBack(metadata->operand, oldVariables,
                                            newVariables, variables_size);

    new_metadata->parameters.size = metadata->parameters.size;
    new_metadata->parameters.data = a404m_malloc(
        sizeof(*new_metadata->parameters.data) * new_metadata->parameters.size);

    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      new_metadata->parameters.data[i] =
          copyAstTreeBack(metadata->parameters.data[i], oldVariables,
                          newVariables, variables_size);
    }

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

AstTreeVariable *copyAstTreeBackFindVariable(AstTreeVariable *variable,
                                             AstTreeVariables oldVariables[],
                                             AstTreeVariables newVariables[],
                                             size_t variables_size) {
  for (size_t i = 0; i < variables_size; ++i) {
    for (size_t j = 0; j < oldVariables[i].size; ++j) {
      if (oldVariables[i].data[j] == variable) {
        return newVariables[i].data[j];
      }
    }
  }
  return variable;
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
    result.data[i]->isLazy = variables.data[i]->isLazy;
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
    if (variables.data[i]->initValue != NULL) {
      result.data[i]->initValue =
          copyAstTreeBack(variables.data[i]->initValue, new_oldVariables,
                          new_newVariables, new_variables_size);
    } else {
      result.data[i]->initValue = NULL;
    }
  }

  return result;
}

AstTreeRoots makeAstTree(const char *filePath
#ifdef PRINT_STATISTICS
                         ,
                         struct timespec *lexingTime,
                         struct timespec *parsingTime
#endif
) {
  AstTreeRoots roots = {
      .data = a404m_malloc(0 * sizeof(*roots.data)),
      .size = 0,
  };

  if (getAstTreeRoot(strClone(filePath), &roots, lexingTime, parsingTime) ==
      NULL) {
    goto RETURN_ERROR;
  }

  for (size_t i = 0; i < roots.size; ++i) {
    AstTreeRoot *root = roots.data[i];
    if (!setAllTypesRoot(root)) {
      goto RETURN_ERROR;
    }
  }

  return roots;

RETURN_ERROR:
  return AST_TREE_ROOTS_ERROR;
}

AstTreeRoot *getAstTreeRoot(char *filePath, AstTreeRoots *roots
#ifdef PRINT_STATISTICS
                            ,
                            struct timespec *lexingTime,
                            struct timespec *parsingTime
#endif
) {
  for (size_t i = 0; i < roots->size; ++i) {
    if (strEquals(roots->data[i]->filePath, filePath)) {
      free(filePath);
      return roots->data[i];
    }
  }

  struct timespec start, end;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

  ParserNode *parserNode = parserFromPath(filePath, lexingTime);
  if (parserNode == NULL) {
    goto RETURN_ERROR;
  }

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
  *parsingTime = time_add(*parsingTime, time_diff(end, start));
  AstTreeRoot *root = makeAstRoot(parserNode, filePath);
  parserNodeDelete(parserNode);
  if (root == NULL) {
    goto RETURN_ERROR;
  }

  const size_t roots_size =
      a404m_malloc_usable_size(roots->data) / sizeof(*roots->data);
  if (roots_size == roots->size) {
    roots->data = a404m_realloc(roots->data, (roots_size + roots_size / 2 + 1) *
                                                 sizeof(*roots->data));
  }
  roots->data[roots->size++] = root;

  for (size_t i = 0; i < root->trees.size; ++i) {
    AstTree *tree = root->trees.data[i];
    if (tree->token == AST_TREE_TOKEN_FUNCTION_CALL) {
      AstTreeFunctionCall *tree_metadata = tree->metadata;
      AstTree *operand = tree_metadata->function;
      if (operand->token == AST_TREE_TOKEN_BUILTIN_IMPORT) {
        AstTreeSetTypesHelper helper = {
            .lookingType = NULL,
            .dependencies.data = NULL,
            .dependencies.size = 0,
            .variables = root->variables,
        };
        if (!setAllTypes(tree, helper, NULL, NULL)) {
          goto RETURN_ERROR;
        }
        AstTree *parameter = tree_metadata->parameters[0].value;
        if (!isConstByValue(parameter)) {
          printError(parameter->str_begin, parameter->str_end,
                     "Is not constant");
          goto RETURN_ERROR;
        }
        parameter = getValue(parameter);
        if (parameter == NULL) {
          goto RETURN_ERROR;
        }

        AstTreeBracket *type_metadata = a404m_malloc(sizeof(*type_metadata));
        type_metadata->operand = &AST_TREE_U8_TYPE;

        type_metadata->parameters.size = 0;
        type_metadata->parameters.data =
            a404m_malloc(0 * sizeof(*type_metadata->parameters.data));

        AstTree *type = newAstTree(AST_TREE_TOKEN_TYPE_ARRAY, type_metadata,
                                   &AST_TREE_TYPE_TYPE, NULL, NULL);

        if (!typeIsEqual(type, parameter->type)) {
          printError(parameter->str_begin, parameter->str_end,
                     "Type mismatch (must be a []u8 aka string)");
          goto RETURN_ERROR;
        }

        char *str = u8ArrayToCString(parameter);
        astTreeDelete(parameter);

        const size_t imports_size =
            a404m_malloc_usable_size(root->imports) / sizeof(*root->imports);
        if (imports_size == root->imports_size) {
          root->imports = a404m_realloc(root->imports,
                                        (imports_size + imports_size / 2 + 1) *
                                            sizeof(*root->imports));
        }

        AstTreeRoot *import = getAstTreeRoot(joinToPathOf(filePath, str), roots,
                                             lexingTime, parsingTime);
        free(str);

        if (import == NULL) {
          goto RETURN_ERROR;
        }

        root->imports[root->imports_size++] = import;

        astTreeDelete(type);
      }
    }
  }

  return root;

RETURN_ERROR:
  return NULL;
}

AstTreeRoot *makeAstRoot(const ParserNode *parsedRoot, char *filePath) {
  if (parsedRoot->token != PARSER_TOKEN_ROOT) {
    return NULL;
  }

  const ParserNodeArray *nodes = parsedRoot->metadata;

  AstTreeRoot *root = a404m_malloc(sizeof(*root));

  root->variables.data =
      a404m_malloc(nodes->size * sizeof(*root->variables.data));
  root->variables.size = 0;
  root->imports = a404m_malloc(0 * sizeof(*root->imports));
  root->imports_size = 0;
  root->filePath = filePath;

  AstTreeVariables *variables = &root->variables;
  static const size_t variables_size = 1;

  AstTreeHelper helper = {
      .variables = &variables,
      .variables_size = variables_size,
  };

  for (size_t i = 0; i < nodes->size; ++i) {
    const ParserNode *eol = nodes->data[i];
    if (eol->token != PARSER_TOKEN_SYMBOL_EOL) {
      printError(eol->str_begin, eol->str_end, "Did you forgot semicolon?",
                 PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    const ParserNode *node = (ParserNodeSingleChildMetadata *)eol->metadata;
    if (node->token == PARSER_TOKEN_KEYWORD_COMPTIME ||
        node->token == PARSER_TOKEN_FUNCTION_CALL) {
      continue;
    } else if (node->token == PARSER_TOKEN_CONSTANT ||
               node->token == PARSER_TOKEN_VARIABLE) {
      ParserNodeVariableMetadata *node_metadata = node->metadata;

      AstTreeVariable *variable = a404m_malloc(sizeof(*variable));
      variable->name_begin = node_metadata->name->str_begin;
      variable->name_end = node_metadata->name->str_end;
      variable->isConst = node->token == PARSER_TOKEN_CONSTANT;
      variable->isLazy = node_metadata->isLazy;

      if (!pushVariable(&helper, &root->variables, variable)) {
        astTreeVariableDelete(variable);
        goto RETURN_ERROR;
      }
    } else {
      printError(node->str_begin, node->str_end, "Unexpected %s",
                 PARSER_TOKEN_STRINGS[node->token]);
      goto RETURN_ERROR;
    }
  }

  root->variables.data =
      a404m_realloc(root->variables.data,
                    root->variables.size * sizeof(*root->variables.data));

  root->trees.data = a404m_malloc((nodes->size - root->variables.size) *
                                  sizeof(*root->trees.data));
  root->trees.size = 0;

  for (size_t i = 0; i < nodes->size; ++i) {
    const ParserNode *eol = nodes->data[i];
    if (eol->token != PARSER_TOKEN_SYMBOL_EOL) {
      printError(eol->str_begin, eol->str_end, "Did you forgot semicolon?",
                 PARSER_TOKEN_STRINGS[eol->token]);
      goto RETURN_ERROR;
    }
    const ParserNode *node = (ParserNodeSingleChildMetadata *)eol->metadata;
    AstTree *tree;
    if (node->token == PARSER_TOKEN_KEYWORD_COMPTIME) {
      tree = astTreeParse(node, &helper);
      if (tree == NULL) {
        goto RETURN_ERROR;
      }
    } else if (node->token == PARSER_TOKEN_FUNCTION_CALL) {
      tree = astTreeParse(node, &helper);
      if (tree == NULL) {
        goto RETURN_ERROR;
      }

      AstTreeFunctionCall *tree_metadata = tree->metadata;
      AstTree *operand = tree_metadata->function;
      if (operand->token == AST_TREE_TOKEN_BUILTIN_IMPORT) {
        if (tree_metadata->parameters_size == 1) {
          goto PUSH;
        }
      }
      printError(tree->str_begin, tree->str_end, "Bad node");
      goto RETURN_ERROR;
    } else {
      continue;
    }
  PUSH:
    root->trees.data[root->trees.size++] = tree;
  }

  for (size_t j = 0, i = 0; j < nodes->size; ++j) {
    const ParserNode *node =
        (ParserNodeSingleChildMetadata *)nodes->data[j]->metadata;
    ParserNodeVariableMetadata *node_metadata = node->metadata;

    if (node->token == PARSER_TOKEN_KEYWORD_COMPTIME ||
        node->token == PARSER_TOKEN_FUNCTION_CALL) {
      continue;
    } else if (node->token == PARSER_TOKEN_CONSTANT ||
               node->token == PARSER_TOKEN_VARIABLE) {
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
      case PARSER_TOKEN_VALUE_CHAR:
      case PARSER_TOKEN_VALUE_STRING:
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
      case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
      case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
      case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
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
#ifdef FLOAT_16_SUPPORT
      case PARSER_TOKEN_TYPE_F16:
#endif
      case PARSER_TOKEN_TYPE_F32:
      case PARSER_TOKEN_TYPE_F64:
      case PARSER_TOKEN_TYPE_F128:
      case PARSER_TOKEN_TYPE_CODE:
      case PARSER_TOKEN_TYPE_BOOL:
      case PARSER_TOKEN_OPERATOR_POINTER:
      case PARSER_TOKEN_OPERATOR_ADDRESS:
      case PARSER_TOKEN_OPERATOR_DEREFERENCE:
      case PARSER_TOKEN_KEYWORD_STRUCT:
      case PARSER_TOKEN_BUILTIN_CAST:
      case PARSER_TOKEN_BUILTIN_TYPE_OF:
      case PARSER_TOKEN_BUILTIN_IMPORT:
      case PARSER_TOKEN_BUILTIN_IS_COMPTIME:
      case PARSER_TOKEN_BUILTIN_STACK_ALLOC:
      case PARSER_TOKEN_BUILTIN_HEAP_ALLOC:
      case PARSER_TOKEN_BUILTIN_NEG:
      case PARSER_TOKEN_BUILTIN_ADD:
      case PARSER_TOKEN_BUILTIN_SUB:
      case PARSER_TOKEN_BUILTIN_MUL:
      case PARSER_TOKEN_BUILTIN_DIV:
      case PARSER_TOKEN_BUILTIN_MOD:
      case PARSER_TOKEN_BUILTIN_EQUAL:
      case PARSER_TOKEN_BUILTIN_NOT_EQUAL:
      case PARSER_TOKEN_BUILTIN_GREATER:
      case PARSER_TOKEN_BUILTIN_SMALLER:
      case PARSER_TOKEN_BUILTIN_GREATER_OR_EQUAL:
      case PARSER_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
      case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
      case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
        goto AFTER_SWITCH;
      case PARSER_TOKEN_ROOT:
      case PARSER_TOKEN_KEYWORD_PUTC:
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
      if (root->variables.data[i]->isConst) {
        root->variables.data[i]->value = value;
        root->variables.data[i]->initValue = NULL;
      } else {
        root->variables.data[i]->value = NULL;
        root->variables.data[i]->initValue = value;
      }
      ++i;
    } else {
      printError(node->str_begin, node->str_end,
                 "Only variables are allowed here");
      goto RETURN_ERROR;
    }
  }

  return root;

RETURN_ERROR:
  for (size_t i = 0; i < root->trees.size; ++i) {
    astTreeDelete(root->trees.data[i]);
  }
  free(root->trees.data);
  free(root->variables.data);
  free(root);
  return NULL;
}

bool pushVariable(AstTreeHelper *helper, AstTreeVariables *variables,
                  AstTreeVariable *variable) {
  if (helper->variables[0] != variables) {
    for (size_t j = 0; j < variables->size; ++j) {
      const char *var_begin = variables->data[j]->name_begin;
      const char *var_end = variables->data[j]->name_end;

      if (variable->name_end - variable->name_begin == var_end - var_begin &&
          strnEquals(var_begin, variable->name_begin,
                     variable->name_end - variable->name_begin)) {
        printError(variable->name_begin, variable->name_end, "Variable exists");
        return false;
      }
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

AstTree *astTreeParse(const ParserNode *parserNode, AstTreeHelper *helper) {
  switch (parserNode->token) {
  case PARSER_TOKEN_FUNCTION_DEFINITION:
    return astTreeParseFunction(parserNode, helper);
  case PARSER_TOKEN_BUILTIN_CAST:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_CAST);
  case PARSER_TOKEN_BUILTIN_TYPE_OF:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_TYPE_OF);
  case PARSER_TOKEN_BUILTIN_IMPORT:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_IMPORT);
  case PARSER_TOKEN_BUILTIN_IS_COMPTIME:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_IS_COMPTIME);
  case PARSER_TOKEN_BUILTIN_STACK_ALLOC:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_STACK_ALLOC);
  case PARSER_TOKEN_BUILTIN_HEAP_ALLOC:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC);
  case PARSER_TOKEN_BUILTIN_NEG:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_NEG);
  case PARSER_TOKEN_BUILTIN_ADD:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_ADD);
  case PARSER_TOKEN_BUILTIN_SUB:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_SUB);
  case PARSER_TOKEN_BUILTIN_MUL:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_MUL);
  case PARSER_TOKEN_BUILTIN_DIV:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_DIV);
  case PARSER_TOKEN_BUILTIN_MOD:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_MOD);
  case PARSER_TOKEN_BUILTIN_EQUAL:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_EQUAL);
  case PARSER_TOKEN_BUILTIN_NOT_EQUAL:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_NOT_EQUAL);
  case PARSER_TOKEN_BUILTIN_GREATER:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_GREATER);
  case PARSER_TOKEN_BUILTIN_SMALLER:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_SMALLER);
  case PARSER_TOKEN_BUILTIN_GREATER_OR_EQUAL:
    return astTreeParseKeyword(parserNode,
                               AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL);
  case PARSER_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
    return astTreeParseKeyword(parserNode,
                               AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL);
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
#ifdef FLOAT_16_SUPPORT
  case PARSER_TOKEN_TYPE_F16:
    return &AST_TREE_F16_TYPE;
#endif
  case PARSER_TOKEN_TYPE_F32:
    return &AST_TREE_F32_TYPE;
  case PARSER_TOKEN_TYPE_F64:
    return &AST_TREE_F64_TYPE;
  case PARSER_TOKEN_TYPE_F128:
    return &AST_TREE_F128_TYPE;
  case PARSER_TOKEN_TYPE_CODE:
    return &AST_TREE_CODE_TYPE;
  case PARSER_TOKEN_TYPE_BOOL:
    return &AST_TREE_BOOL_TYPE;
  case PARSER_TOKEN_FUNCTION_CALL:
    return astTreeParseFunctionCall(parserNode, helper);
  case PARSER_TOKEN_IDENTIFIER:
    return astTreeParseIdentifier(parserNode, helper);
  case PARSER_TOKEN_VALUE_INT:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_INT,
                             sizeof(AstTreeInt), NULL);
  case PARSER_TOKEN_VALUE_FLOAT:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_FLOAT,
                             sizeof(AstTreeFloat), NULL);
  case PARSER_TOKEN_VALUE_BOOL:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_BOOL,
                             sizeof(AstTreeBool), NULL);
  case PARSER_TOKEN_VALUE_CHAR:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_INT,
                             sizeof(AstTreeInt), &AST_TREE_U8_TYPE);
  case PARSER_TOKEN_VALUE_STRING:
    return astTreeParseString(parserNode, helper);
  case PARSER_TOKEN_KEYWORD_NULL:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_VALUE_NULL);
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_VALUE_UNDEFINED);
  case PARSER_TOKEN_KEYWORD_PUTC:
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
  case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_LOGICAL_AND);
  case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
    return astTreeParseBinaryOperator(parserNode, helper,
                                      AST_TREE_TOKEN_OPERATOR_LOGICAL_OR);
  case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
    return astTreeParseUnaryOperator(parserNode, helper,
                                     AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT);
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
    return astTreeParseUnaryOperatorSingleChild(
        parserNode, helper, AST_TREE_TOKEN_OPERATOR_ADDRESS);
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
    return astTreeParseUnaryOperatorSingleChild(
        parserNode, helper, AST_TREE_TOKEN_OPERATOR_DEREFERENCE);
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
  case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
    return astTreeParseBracket(parserNode, helper, AST_TREE_TOKEN_TYPE_ARRAY);
  case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
    return astTreeParseBracket(parserNode, helper,
                               AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS);
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_NONE:
  case PARSER_TOKEN_ROOT:
  }
  printError(parserNode->str_begin, parserNode->str_end, "Bad token %d",
             parserNode->token);
  return NULL;
}

AstTree *astTreeParseFunction(const ParserNode *parserNode,
                              AstTreeHelper *p_helper) {
  ParserNodeFunctionDefnitionMetadata *node_metadata = parserNode->metadata;
  const ParserNodeArray *node_arguments = node_metadata->arguments->metadata;
  const ParserNodeArray *body = node_metadata->body->metadata;

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
  };

  for (size_t i = 0; i < node_arguments->size; ++i) {
    const ParserNode *arg = node_arguments->data[i];
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
    argument->initValue = NULL;
    argument->type = type;
    argument->name_begin = arg_metadata->name->str_begin;
    argument->name_end = arg_metadata->name->str_end;
    argument->isLazy = true; // all arguments are constants

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
    const ParserNode *node = body->data[i];
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
    case PARSER_TOKEN_VALUE_CHAR:
    case PARSER_TOKEN_VALUE_STRING:
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
    case PARSER_TOKEN_KEYWORD_PUTC:
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
    case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
    case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
    case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
    case PARSER_TOKEN_BUILTIN_CAST:
    case PARSER_TOKEN_BUILTIN_TYPE_OF:
    case PARSER_TOKEN_BUILTIN_IMPORT:
    case PARSER_TOKEN_BUILTIN_IS_COMPTIME:
    case PARSER_TOKEN_BUILTIN_STACK_ALLOC:
    case PARSER_TOKEN_BUILTIN_HEAP_ALLOC:
    case PARSER_TOKEN_BUILTIN_NEG:
    case PARSER_TOKEN_BUILTIN_ADD:
    case PARSER_TOKEN_BUILTIN_SUB:
    case PARSER_TOKEN_BUILTIN_MUL:
    case PARSER_TOKEN_BUILTIN_DIV:
    case PARSER_TOKEN_BUILTIN_MOD:
    case PARSER_TOKEN_BUILTIN_EQUAL:
    case PARSER_TOKEN_BUILTIN_NOT_EQUAL:
    case PARSER_TOKEN_BUILTIN_GREATER:
    case PARSER_TOKEN_BUILTIN_SMALLER:
    case PARSER_TOKEN_BUILTIN_GREATER_OR_EQUAL:
    case PARSER_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
    case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
    case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
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

AstTree *astTreeParseTypeFunction(const ParserNode *parserNode,
                                  AstTreeHelper *helper) {
  ParserNodeTypeFunctionMetadata *metadata = parserNode->metadata;
  const ParserNodeArray *node_arguments = metadata->arguments->metadata;

  AstTreeTypeFunction *typeFunction = a404m_malloc(sizeof(*typeFunction));

  size_t arguments_size = 0;
  typeFunction->arguments =
      a404m_malloc(arguments_size * sizeof(*typeFunction->arguments));
  typeFunction->arguments_size = 0;

  for (size_t i = 0; i < node_arguments->size; ++i) {
    const ParserNode *node_argument = node_arguments->data[i];

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

AstTree *astTreeParseFunctionCall(const ParserNode *parserNode,
                                  AstTreeHelper *helper) {
  ParserNodeFunctionCall *node_metadata = parserNode->metadata;
  AstTree *function = astTreeParse(node_metadata->function, helper);
  if (function == NULL) {
    return NULL;
  }

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));
  metadata->function = function;

  metadata->parameters =
      a404m_malloc(sizeof(*metadata->parameters) * node_metadata->params->size);
  metadata->parameters_size = node_metadata->params->size;

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    const ParserNode *node_param = node_metadata->params->data[i];
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

AstTree *astTreeParseIdentifier(const ParserNode *parserNode,
                                AstTreeHelper *helper) {
  (void)helper;
  return newAstTree(AST_TREE_TOKEN_VARIABLE, NULL, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseValue(const ParserNode *parserNode, AstTreeToken token,
                           size_t metadata_size, AstTree *type) {
  u8 *metadata = a404m_malloc(metadata_size);
  for (size_t i = 0; i < metadata_size; ++i) {
    metadata[i] = ((u8 *)parserNode->metadata)[i];
  }

  return newAstTree(token, metadata, type, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseString(const ParserNode *parserNode,
                            AstTreeHelper *helper) {
  (void)helper;
  ParserNodeStringMetadata *node_metadata = parserNode->metadata;

  AstTreeObject *metadata = a404m_malloc(sizeof(*metadata));

  metadata->variables.size = node_metadata->end - node_metadata->begin;
  metadata->variables.data = a404m_malloc(metadata->variables.size *
                                          sizeof(*metadata->variables.data));

  for (size_t i = 0; i < metadata->variables.size; ++i) {
    AstTreeInt *cellMetadata = a404m_malloc(sizeof(*cellMetadata));
    *cellMetadata = node_metadata->begin[i];

    metadata->variables.data[i] =
        a404m_malloc(sizeof(*metadata->variables.data[i]));
    metadata->variables.data[i]->isLazy = true;
    metadata->variables.data[i]->name_begin = NULL;
    metadata->variables.data[i]->name_end = NULL;
    metadata->variables.data[i]->type = copyAstTree(&AST_TREE_U8_TYPE);
    metadata->variables.data[i]->value =
        newAstTree(AST_TREE_TOKEN_VALUE_INT, cellMetadata,
                   copyAstTree(&AST_TREE_U8_TYPE), NULL, NULL);
    metadata->variables.data[i]->initValue = NULL;
  }

  AstTreeBracket *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->operand = &AST_TREE_U8_TYPE;

  AstTreeInt *parameter_metadata = a404m_malloc(sizeof(*parameter_metadata));
  *parameter_metadata = metadata->variables.size;
  AstTree *parameter = newAstTree(AST_TREE_TOKEN_VALUE_INT, parameter_metadata,
                                  &AST_TREE_I64_TYPE, NULL, NULL);

  type_metadata->parameters.size = 1;
  type_metadata->parameters.data = a404m_malloc(
      type_metadata->parameters.size * sizeof(*type_metadata->parameters.data));
  type_metadata->parameters.data[0] = parameter;

  return newAstTree(AST_TREE_TOKEN_VALUE_OBJECT, metadata,
                    newAstTree(AST_TREE_TOKEN_TYPE_ARRAY, type_metadata,
                               &AST_TREE_TYPE_TYPE, NULL, NULL),
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseKeyword(const ParserNode *parserNode, AstTreeToken token) {
  return newAstTree(token, NULL, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParsePrintU64(const ParserNode *parserNode,
                              AstTreeHelper *helper) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTree *operand = astTreeParse(node_metadata, helper);
  if (operand == NULL) {
    return NULL;
  }

  return newAstTree(AST_TREE_TOKEN_KEYWORD_PUTC, (AstTreeSingleChild *)operand,
                    NULL, parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseReturn(const ParserNode *parserNode,
                            AstTreeHelper *helper) {
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

AstTree *astTreeParseBinaryOperator(const ParserNode *parserNode,
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

  metadata->left = left;
  metadata->right = right;
  metadata->function = NULL;

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseUnaryOperator(const ParserNode *parserNode,
                                   AstTreeHelper *helper, AstTreeToken token) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeUnary *metadata = a404m_malloc(sizeof(*metadata));
  metadata->operand = astTreeParse(node_metadata, helper);
  if (metadata->operand == NULL) {
    free(metadata);
    return NULL;
  }
  metadata->function = NULL;

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseUnaryOperatorSingleChild(const ParserNode *parserNode,
                                              AstTreeHelper *helper,
                                              AstTreeToken token) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeSingleChild *metadata = astTreeParse(node_metadata, helper);
  if (metadata == NULL) {
    return NULL;
  }

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseOperateAssignOperator(const ParserNode *parserNode,
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

  metadata->left = left;
  metadata->right = right;
  metadata->function = NULL;

  AstTreeInfix *assignMetadata = a404m_malloc(sizeof(*assignMetadata));

  AstTree *assignLeft = astTreeParse(node_metadata->left, helper);
  AstTree *assignRight = newAstTree(token, metadata, NULL,
                                    parserNode->str_begin, parserNode->str_end);

  assignMetadata->left = assignLeft;
  assignMetadata->right = assignRight;
  assignMetadata->function = NULL;

  return newAstTree(AST_TREE_TOKEN_OPERATOR_ASSIGN, assignMetadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

bool astTreeParseConstant(const ParserNode *parserNode, AstTreeHelper *helper) {
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
  variable->initValue = NULL;
  variable->name_begin = node_metadata->name->str_begin;
  variable->name_end = node_metadata->name->str_end;
  variable->isConst = true;
  variable->isLazy = node_metadata->isLazy;

  if (!pushVariable(helper, helper->variables[helper->variables_size - 1],
                    variable)) {
    astTreeVariableDelete(variable);
    goto RETURN_ERROR;
  }

  return true;
RETURN_ERROR:
  return false;
}

AstTree *astTreeParseVariable(const ParserNode *parserNode,
                              AstTreeHelper *helper) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->value == NULL) {
    printError(parserNode->str_begin, parserNode->str_end,
               "Must have a value (or use undefined)");
    return NULL;
  } else if (node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
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
  variable->value = NULL;
  variable->initValue = value;
  variable->name_begin = node_metadata->name->str_begin;
  variable->name_end = node_metadata->name->str_end;
  variable->isConst = false;
  variable->isLazy = node_metadata->isLazy;

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

AstTree *astTreeParseIf(const ParserNode *parserNode, AstTreeHelper *helper) {
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

AstTree *astTreeParseWhile(const ParserNode *parserNode,
                           AstTreeHelper *helper) {
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

AstTree *astTreeParseComptime(const ParserNode *parserNode,
                              AstTreeHelper *helper) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeSingleChild *metadata = (AstTreeSingleChild *)astTreeParse(
      (const ParserNode *)node_metadata, helper);

  return newAstTree(AST_TREE_TOKEN_KEYWORD_COMPTIME, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseCurlyBracket(const ParserNode *parserNode,
                                  AstTreeHelper *p_helper) {
  const ParserNodeArray *body = parserNode->metadata;

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
  };

  for (size_t i = 0; i < body->size; ++i) {
    const ParserNode *node = body->data[i];
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
    case PARSER_TOKEN_VALUE_CHAR:
    case PARSER_TOKEN_VALUE_STRING:
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
    case PARSER_TOKEN_KEYWORD_PUTC:
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
    case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
    case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
    case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
    case PARSER_TOKEN_BUILTIN_CAST:
    case PARSER_TOKEN_BUILTIN_TYPE_OF:
    case PARSER_TOKEN_BUILTIN_IMPORT:
    case PARSER_TOKEN_BUILTIN_IS_COMPTIME:
    case PARSER_TOKEN_BUILTIN_STACK_ALLOC:
    case PARSER_TOKEN_BUILTIN_HEAP_ALLOC:
    case PARSER_TOKEN_BUILTIN_NEG:
    case PARSER_TOKEN_BUILTIN_ADD:
    case PARSER_TOKEN_BUILTIN_SUB:
    case PARSER_TOKEN_BUILTIN_MUL:
    case PARSER_TOKEN_BUILTIN_DIV:
    case PARSER_TOKEN_BUILTIN_MOD:
    case PARSER_TOKEN_BUILTIN_EQUAL:
    case PARSER_TOKEN_BUILTIN_NOT_EQUAL:
    case PARSER_TOKEN_BUILTIN_GREATER:
    case PARSER_TOKEN_BUILTIN_SMALLER:
    case PARSER_TOKEN_BUILTIN_GREATER_OR_EQUAL:
    case PARSER_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
    case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
    case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
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

AstTree *astTreeParseParenthesis(const ParserNode *parserNode,
                                 AstTreeHelper *helper) {
  const ParserNodeArray *metadata = parserNode->metadata;

  if (metadata->size != 1) {
    printError(parserNode->str_begin, parserNode->str_end, "Bad parenthesis");
    return NULL;
  } else {
    return astTreeParse(metadata->data[0], helper);
  }
}

AstTree *astTreeParseStruct(const ParserNode *parserNode,
                            AstTreeHelper *helper) {
  const ParserNode *body = parserNode->metadata;
  const ParserNodeArray *body_metadata = body->metadata;
  AstTreeVariables variables = {
      .data = a404m_malloc(sizeof(*variables.data) * body_metadata->size),
      .size = body_metadata->size,
  };

  for (size_t i = 0; i < body_metadata->size; ++i) {
    const ParserNode *node = body_metadata->data[i];
    if (node->token != PARSER_TOKEN_SYMBOL_EOL) {
      printError(node->str_begin, node->str_end,
                 "Semicolon is required, maybe forget a semicolon?");
      return NULL;
    }
    node = (ParserNodeSingleChildMetadata *)node->metadata;

    if (node->token != PARSER_TOKEN_VARIABLE &&
        node->token != PARSER_TOKEN_CONSTANT) {
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

    if (node->token == PARSER_TOKEN_CONSTANT) {
      if (node_variable->value == NULL) {
        printError(node->str_begin, node->str_end, "Constants must have value");
        NOT_IMPLEMENTED;
      }
      variable->value = astTreeParse(node_variable->value, helper);
      variable->initValue = NULL;
      variable->isConst = true;
    } else {
      if (node_variable->value != NULL) {
        printError(node->str_begin, node->str_end, "Can't have default values");
        NOT_IMPLEMENTED;
      }
      variable->value = NULL;
      variable->initValue = NULL;
      variable->isConst = false;
    }
    variable->isLazy = node_variable->isLazy;

    variables.data[i] = variable;
  }

  AstTreeStruct *metadata = a404m_malloc(sizeof(*metadata));
  metadata->variables = variables;
  metadata->id = (size_t)metadata; // TODO: change it

  return newAstTree(AST_TREE_TOKEN_KEYWORD_STRUCT, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseAccessOperator(const ParserNode *parserNode,
                                    AstTreeHelper *helper, AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *object = astTreeParse(node_metadata->left, helper);
  if (object == NULL) {
    return NULL;
  }

  const ParserNode *right_node = node_metadata->right;
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

AstTree *astTreeParseBracket(const ParserNode *parserNode,
                             AstTreeHelper *helper, AstTreeToken token) {
  ParserNodeBracketMetadata *node_metadata = parserNode->metadata;

  AstTreeBracket *metadata = a404m_malloc(sizeof(*metadata));

  metadata->operand = astTreeParse(node_metadata->operand, helper);

  metadata->parameters.size = node_metadata->params->size;
  metadata->parameters.data = a404m_malloc(sizeof(*metadata->parameters.data) *
                                           metadata->parameters.size);

  for (size_t i = 0; i < node_metadata->params->size; ++i) {
    const ParserNode *node_param = node_metadata->params->data[i];

    if (node_param->token == PARSER_TOKEN_SYMBOL_COMMA) {
      node_param = (ParserNodeSingleChildMetadata *)node_param->metadata;
    }

    metadata->parameters.data[i] = astTreeParse(node_param, helper);
  }

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
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
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
    return true;
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *member = metadata->variables.data[i];
      if (!isConst(member->type) ||
          (member->value != NULL && !isConst(member->type))) {
        return false;
      }
    }
    return true;
  }
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
  case AST_TREE_TOKEN_KEYWORD_PUTC:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
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
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    AstTreeBracket *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      if (!isConst(metadata->parameters.data[i])) {
        return false;
      }
    }
    return isConst(metadata->operand);
  }
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Unknown token '%d'", tree->token);
  UNREACHABLE;
}

bool isConstByValue(AstTree *tree) {
  if (tree->type == NULL) {
    UNREACHABLE;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
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
    return true;
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      AstTreeVariable *member = metadata->variables.data[i];
      if (!isConstByValue(member->type) ||
          (member->value != NULL && !isConstByValue(member->type))) {
        return false;
      }
    }
    return true;
  }
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = tree->metadata;
    return isConstByValue(metadata->condition) &&
           isConstByValue(metadata->ifBody) &&
           (metadata->elseBody == NULL || isConstByValue(metadata->elseBody));
  }
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->parameters_size; ++i) {
      if (!isConstByValue(metadata->parameters[i].value)) {
        return false;
      }
    }
    return isConstByValue(metadata->function);
  }
  case AST_TREE_TOKEN_FUNCTION: {
    return true;
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
    return false;
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = tree->metadata;
    return metadata->isConst && metadata->value != NULL;
  }
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    AstTreeSingleChild *metadata = tree->metadata;
    return isConstByValue(metadata);
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = tree->metadata;
    return isConstByValue(metadata->object);
  }
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    AstTreeBracket *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      if (!isConstByValue(metadata->parameters.data[i])) {
        return false;
      }
    }
    return isConstByValue(metadata->operand);
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_ARRAY:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
    AstTreeUnary *metadata = value->metadata;
    AstTreeTypeFunction *function = metadata->function->type->metadata;
    return copyAstTree(function->returnType);
  }
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *metadata = value->metadata;
    return copyAstTree(metadata->type);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
    AstTreeInfix *metadata = value->metadata;
    return copyAstTree(metadata->left->type);
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeInfix *metadata = value->metadata;
    AstTreeTypeFunction *function = metadata->function->type->metadata;
    return copyAstTree(function->returnType);
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = value->metadata;

    AstTreeStruct *struc = metadata->object->type->metadata;
    const size_t size = metadata->member.name.end - metadata->member.name.begin;
    const char *str = metadata->member.name.begin;

    for (size_t i = 0; i < struc->variables.size; ++i) {
      AstTreeVariable *member = struc->variables.data[i];
      const size_t member_size = member->name_end - member->name_begin;
      if (member_size == size && strnEquals(member->name_begin, str, size)) {
        return copyAstTree(member->type);
      }
    }
    UNREACHABLE;
  }
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    AstTreeBracket *metadata = value->metadata;

    return copyAstTree(metadata->operand);
  }
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
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
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
    return type1->token == type0->token;
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    if (type1->token != type0->token) {
      return false;
    }
    AstTreeSingleChild *type0_metadata = type0->metadata;
    AstTreeSingleChild *type1_metadata = type1->metadata;
    return typeIsEqual(type0_metadata, type1_metadata);
  }
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    if (type1->token != type0->token) {
      return false;
    }

    AstTreeBracket *type0_metadata = type0->metadata;
    AstTreeBracket *type1_metadata = type1->metadata;

    if (!typeIsEqual(type0_metadata->operand, type1_metadata->operand)) {
      return false;
    } else if (type0_metadata->parameters.size == 0 ||
               type1_metadata->parameters.size == 0) {
      return true;
    } else if (type0_metadata->parameters.size !=
               type1_metadata->parameters.size) {
      return false;
    }

    for (size_t i = 0; i < type0_metadata->parameters.size; ++i) {
      if (!isEqual(type0_metadata->parameters.data[i],
                   type1_metadata->parameters.data[i])) {
        return false;
      }
    }

    return true;
  }
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    if (type1->token != type0->token) {
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
    if (type1->token != type0->token)
      return false;

    AstTreeStruct *type0_metadata = type0->metadata;
    AstTreeStruct *type1_metadata = type1->metadata;

    if (type0_metadata->id != type1_metadata->id ||
        type0_metadata->variables.size != type1_metadata->variables.size) {
      return false;
    }

    for (size_t i = 0; i < type0_metadata->variables.size; ++i) {
      AstTreeVariable *member0 = type0_metadata->variables.data[i];
      AstTreeVariable *member1 = type1_metadata->variables.data[i];
      return isEqualVariable(member0, member1);
    }
    return true;
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
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS: {
    bool shouldRet = false;
    AstTreeScope *scope = a404m_malloc(sizeof(*scope));
    scope->expressions = a404m_malloc(0);
    scope->expressions_size = 0;
    scope->variables.data = a404m_malloc(0);
    scope->variables.size = 0;

    AstTree scopeTree = {
        .token = AST_TREE_TOKEN_SCOPE,
        .metadata = scope,
        .type = &AST_TREE_VOID_TYPE,
        .str_begin = NULL,
        .str_end = NULL,
    };

    AstTree *value = runExpression(tree, scope, &shouldRet, false, true);

    astTreeDestroy(scopeTree);

    if (value == NULL) {
      printError(tree->str_begin, tree->str_end, "Unknown error");
    }
    return value;
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_FUNCTION: {
    return tree;
  }
  case AST_TREE_TOKEN_KEYWORD_PUTC:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isIntType(AstTree *type) {
  switch (type->token) {
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
    return true;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_SCOPE:
    return false;
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isEqual(AstTree *left, AstTree *right) {
  if (left->token != right->token) {
    return false;
  }
  switch (left->token) {
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
    return true;
  case AST_TREE_TOKEN_VALUE_INT: {
    AstTreeInt *left_metadata = left->metadata;
    AstTreeInt *right_metadata = right->metadata;

    return *left_metadata == *right_metadata;
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *left_metadata = left->metadata;
    AstTreeVariable *right_metadata = right->metadata;
    return isEqualVariable(left_metadata, right_metadata);
  }
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_SCOPE:
    printLog("%s", AST_TREE_TOKEN_STRINGS[left->token]);
    NOT_IMPLEMENTED;
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isEqualVariable(AstTreeVariable *left, AstTreeVariable *right) {
  if (!typeIsEqual(left->type, right->type) ||
      left->isConst != right->isConst) {
    return false;
  } else if ((left->value == NULL && right->value != NULL) ||
             (left->value != NULL && right->value == NULL)) {
    return false;
  } else if (left->value != NULL && !isEqual(left->value, right->value)) {
    return false;
  } else if ((left->initValue == NULL && right->initValue != NULL) ||
             (left->initValue != NULL && right->initValue == NULL)) {
    return false;
  } else if (left->initValue != NULL &&
             !isEqual(left->initValue, right->initValue)) {
    return false;
  } else {
    return true;
  }
}

void allOfVariablesWithImport(AstTreeVariables *variables, AstTreeRoot *root,
                              AstTreeRoots *checkedRoots) {
  for (size_t i = 0; i < checkedRoots->size; ++i) {
    if (checkedRoots->data[i] == root) {
      return;
    }
  }

  size_t checkedRoots_size = a404m_malloc_usable_size(checkedRoots->data) /
                             sizeof(*checkedRoots->data);
  if (checkedRoots_size == checkedRoots->size) {
    checkedRoots->data = a404m_realloc(
        checkedRoots->data, (checkedRoots_size + checkedRoots_size / 2 + 1) *
                                sizeof(*checkedRoots->data));
  }
  checkedRoots->data[checkedRoots->size++] = root;

  variables->data =
      a404m_realloc(variables->data, (variables->size + root->variables.size) *
                                         sizeof(*variables->data));
  for (size_t i = 0; i < root->variables.size; ++i) {
    variables->data[variables->size++] = root->variables.data[i];
  }

  for (size_t i = 0; i < root->imports_size; ++i) {
    allOfVariablesWithImport(variables, root->imports[i], checkedRoots);
  }
}

bool setAllTypesRoot(AstTreeRoot *root) {
  AstTreeRoots checkedRoots = {
      .data = a404m_malloc(0),
      .size = 0,
  };
  AstTreeVariables variables = (AstTreeVariables){
      .data = a404m_malloc(0),
      .size = 0,
  };
  allOfVariablesWithImport(&variables, root, &checkedRoots);
  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies =
          {
              .data = NULL,
              .size = 0,
          },
      .variables = variables,
  };

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    if (!setTypesAstVariable(variable, helper)) {
      goto RETURN_ERROR;
    }
  }

  for (size_t i = 0; i < root->trees.size; ++i) {
    AstTree *tree = root->trees.data[i];
    if (!setAllTypes(tree, helper, NULL, NULL)) {
      goto RETURN_ERROR;
    }
  }

  free(checkedRoots.data);
  free(variables.data);
  return true;

RETURN_ERROR:
  free(checkedRoots.data);
  free(variables.data);
  return false;
}

static const char STR_PLUS[] = "__plus__";
static const size_t STR_PLUS_SIZE =
    sizeof(STR_PLUS) / sizeof(*STR_PLUS) - sizeof(*STR_PLUS);
static const char STR_MINUS[] = "__minus__";
static const size_t STR_MINUS_SIZE =
    sizeof(STR_MINUS) / sizeof(*STR_MINUS) - sizeof(*STR_PLUS);
static const char STR_LOGICAL_NOT[] = "__logical_not__";
static const size_t STR_LOGICAL_NOT_SIZE =
    sizeof(STR_LOGICAL_NOT) / sizeof(*STR_LOGICAL_NOT) - sizeof(*STR_PLUS);

static const char STR_SUM[] = "__sum__";
static const size_t STR_SUM_SIZE =
    sizeof(STR_SUM) / sizeof(*STR_SUM) - sizeof(*STR_SUM);
static const char STR_SUB[] = "__sub__";
static const size_t STR_SUB_SIZE =
    sizeof(STR_SUB) / sizeof(*STR_SUB) - sizeof(*STR_SUB);
static const char STR_MUL[] = "__mul__";
static const size_t STR_MUL_SIZE =
    sizeof(STR_MUL) / sizeof(*STR_MUL) - sizeof(*STR_MUL);
static const char STR_DIV[] = "__div__";
static const size_t STR_DIV_SIZE =
    sizeof(STR_DIV) / sizeof(*STR_DIV) - sizeof(*STR_DIV);
static const char STR_MOD[] = "__mod__";
static const size_t STR_MOD_SIZE =
    sizeof(STR_MOD) / sizeof(*STR_MOD) - sizeof(*STR_MOD);
static const char STR_LOGICAL_AND[] = "__logical_and__";
static const size_t STR_LOGICAL_AND_SIZE =
    sizeof(STR_LOGICAL_AND) / sizeof(*STR_LOGICAL_AND) -
    sizeof(*STR_LOGICAL_AND);
static const char STR_LOGICAL_OR[] = "__logical_or__";
static const size_t STR_LOGICAL_OR_SIZE =
    sizeof(STR_LOGICAL_OR) / sizeof(*STR_LOGICAL_OR) - sizeof(*STR_LOGICAL_OR);
static const char STR_EQUAL[] = "__equal__";
static const size_t STR_EQUAL_SIZE =
    sizeof(STR_EQUAL) / sizeof(*STR_EQUAL) - sizeof(*STR_EQUAL);
static const char STR_NOT_EQUAL[] = "__not_equal__";
static const size_t STR_NOT_EQUAL_SIZE =
    sizeof(STR_NOT_EQUAL) / sizeof(*STR_NOT_EQUAL) - sizeof(*STR_NOT_EQUAL);
static const char STR_GREATER[] = "__greater__";
static const size_t STR_GREATER_SIZE =
    sizeof(STR_GREATER) / sizeof(*STR_GREATER) - sizeof(*STR_GREATER);
static const char STR_SMALLER[] = "__smaller__";
static const size_t STR_SMALLER_SIZE =
    sizeof(STR_SMALLER) / sizeof(*STR_SMALLER) - sizeof(*STR_SMALLER);
static const char STR_GREATER_OR_EQUAL[] = "__greater_or_equal__";
static const size_t STR_GREATER_OR_EQUAL_SIZE =
    sizeof(STR_GREATER_OR_EQUAL) / sizeof(*STR_GREATER_OR_EQUAL) -
    sizeof(*STR_GREATER_OR_EQUAL);
static const char STR_SMALLER_OR_EQUAL[] = "__smaller_or_equal__";
static const size_t STR_SMALLER_OR_EQUAL_SIZE =
    sizeof(STR_SMALLER_OR_EQUAL) / sizeof(*STR_SMALLER_OR_EQUAL) -
    sizeof(*STR_SMALLER_OR_EQUAL);

bool setAllTypes(AstTree *tree, AstTreeSetTypesHelper helper,
                 AstTreeFunction *function, AstTreeFunctionCall *functionCall) {
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
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
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
  case AST_TREE_TOKEN_KEYWORD_PUTC:
    return setTypesPrintU64(tree, helper);
  case AST_TREE_TOKEN_KEYWORD_RETURN:
    return setTypesReturn(tree, helper, function);
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    return setTypesTypeFunction(tree, helper);
  case AST_TREE_TOKEN_FUNCTION_CALL:
    return setTypesFunctionCall(tree, helper);
  case AST_TREE_TOKEN_VARIABLE:
    return setTypesVariable(tree, helper, functionCall);
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
    return setTypesOperatorAssign(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_PLUS:
    return setTypesOperatorUnary(tree, helper, STR_PLUS, STR_PLUS_SIZE);
  case AST_TREE_TOKEN_OPERATOR_MINUS:
    return setTypesOperatorUnary(tree, helper, STR_MINUS, STR_MINUS_SIZE);
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
    return setTypesOperatorUnary(tree, helper, STR_LOGICAL_NOT,
                                 STR_LOGICAL_NOT_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SUM:
    return setTypesOperatorInfix(tree, helper, STR_SUM, STR_SUM_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SUB:
    return setTypesOperatorInfix(tree, helper, STR_SUB, STR_SUB_SIZE);
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
    return setTypesOperatorInfix(tree, helper, STR_MUL, STR_MUL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
    return setTypesOperatorInfix(tree, helper, STR_DIV, STR_DIV_SIZE);
  case AST_TREE_TOKEN_OPERATOR_MODULO:
    return setTypesOperatorInfix(tree, helper, STR_MOD, STR_MOD_SIZE);
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
    return setTypesOperatorInfix(tree, helper, STR_EQUAL, STR_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
    return setTypesOperatorInfix(tree, helper, STR_NOT_EQUAL,
                                 STR_NOT_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_GREATER:
    return setTypesOperatorInfix(tree, helper, STR_GREATER, STR_GREATER_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
    return setTypesOperatorInfix(tree, helper, STR_SMALLER, STR_SMALLER_SIZE);
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
    return setTypesOperatorInfix(tree, helper, STR_GREATER_OR_EQUAL,
                                 STR_GREATER_OR_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
    return setTypesOperatorInfix(tree, helper, STR_SMALLER_OR_EQUAL,
                                 STR_SMALLER_OR_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
    return setTypesOperatorInfix(tree, helper, STR_LOGICAL_AND,
                                 STR_LOGICAL_AND_SIZE);
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
    return setTypesOperatorInfix(tree, helper, STR_LOGICAL_OR,
                                 STR_LOGICAL_OR_SIZE);
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
  case AST_TREE_TOKEN_BUILTIN_CAST:
    return setTypesBuiltinCast(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
    return setTypesBuiltinTypeOf(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
    return setTypesBuiltinImport(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
    return setTypesBuiltinIsComptime(tree, helper);
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
    return setTypesBuiltinStackAlloc(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
    return setTypesBuiltinHeapAlloc(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_NEG:
    return setTypesBuiltinUnary(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
    return setTypesBuiltinBinary(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
    return setTypesBuiltinBinaryWithRet(tree, helper, functionCall,
                                        &AST_TREE_BOOL_TYPE);
  case AST_TREE_TOKEN_TYPE_ARRAY:
    return setTypesTypeArray(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
    return setTypesArrayAccess(tree, helper);
  case AST_TREE_TOKEN_NONE:
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
#ifdef FLOAT_16_SUPPORT
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
#endif
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
#ifdef FLOAT_16_SUPPORT
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F16_TYPE)) {
    tree->token = AST_TREE_TOKEN_VALUE_FLOAT;
    AstTreeFloat value = *(AstTreeFloat *)tree->metadata;
    f16 newValue = value;
    *(AstTreeFloat *)tree->metadata = value;
    if (value - newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    tree->type = &AST_TREE_F16_TYPE;
#endif
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
  } else if (helper.lookingType->token != AST_TREE_TOKEN_OPERATOR_POINTER) {
    printError(tree->str_begin, tree->str_end,
               "Null must have type of a pointer but got %s",
               AST_TREE_TOKEN_STRINGS[helper.lookingType->token]);
    return false;
  }
  tree->type = copyAstTree(helper.lookingType);
  return true;
}

bool setTypesValueUndefined(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.lookingType == NULL) {
    printError(tree->str_begin, tree->str_end, "Can't find type of undefined");
    return false;
  } else if (helper.lookingType->token == AST_TREE_TOKEN_TYPE_ARRAY) {
    AstTreeBracket *array_metadata = helper.lookingType->metadata;
    if (array_metadata->parameters.size == 0) {
      printError(tree->str_begin, tree->str_end,
                 "Can't find size of array valued undefined");
      return false;
    }
  }
  tree->type = copyAstTree(helper.lookingType);
  return true;
}

bool setTypesValueObject(AstTree *tree, AstTreeSetTypesHelper helper) {
  (void)tree;
  (void)helper;
  NOT_IMPLEMENTED;
}

bool setTypesFunction(AstTree *tree, AstTreeSetTypesHelper _helper) {
  AstTreeFunction *metadata = tree->metadata;

  AstTreeVariable *variables[_helper.variables.size + metadata->arguments.size +
                             metadata->scope.variables.size];

  for (size_t i = 0; i < _helper.variables.size; ++i) {
    variables[i] = _helper.variables.data[i];
  }

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables.data = variables,
      .variables.size = _helper.variables.size,
  };

  AstTreeVariable *deps[helper.dependencies.size];
  size_t deps_size = 0;

  for (size_t i = 0; i < metadata->arguments.size; ++i) {
    AstTreeVariable *variable = metadata->arguments.data[i];
    if (!setTypesAstVariable(variable, helper)) {
      return false;
    }
    helper.variables.data[helper.variables.size++] = variable;
  }

  if (!setAllTypes(metadata->returnType, helper, NULL, NULL)) {
    return false;
  }

  tree->type = makeTypeOf(tree);

  for (size_t i = 0; i < helper.dependencies.size; ++i) {
    AstTreeVariable *var = helper.dependencies.data[i];
    if (var->value == tree || var->initValue == tree) {
      continue;
    }
    deps[deps_size] = helper.dependencies.data[i];
    deps_size += 1;
  }

  helper.dependencies.data = deps;
  helper.dependencies.size = deps_size;

  for (size_t i = 0; i < metadata->scope.variables.size; ++i) {
    AstTreeVariable *variable = metadata->scope.variables.data[i];
    if (variable->isConst) {
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      helper.variables.data[helper.variables.size++] = variable;
    }
  }

  for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
    AstTree *expr = metadata->scope.expressions[i];
    if (expr->token == AST_TREE_TOKEN_VARIABLE_DEFINE) {
      AstTreeVariable *variable = expr->metadata;
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      helper.variables.data[helper.variables.size++] = variable;
    }
    if (!setAllTypes(expr, helper, metadata, NULL)) {
      return false;
    }
  }

  return true;
}

bool setTypesPrintU64(AstTree *tree, AstTreeSetTypesHelper _helper) {
  AstTreeSingleChild *metadata = tree->metadata;
  AstTreeSetTypesHelper helper = {
      .lookingType = &AST_TREE_U8_TYPE,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
  };
  if (!setAllTypes(metadata, helper, NULL, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->type, &AST_TREE_U8_TYPE)) {
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
        .dependencies = _helper.dependencies,
        .variables = _helper.variables,
    };
    if (helper.lookingType == NULL) {
      return false;
    }
    if (!setAllTypes(metadata->value, helper, NULL, NULL)) {
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
    if (!setAllTypes(arg.type, helper, NULL, NULL)) {
      return false;
    } else if (!typeIsEqual(arg.type, &AST_TREE_TYPE_TYPE)) {
      printError(arg.str_begin, arg.str_end, "Expected a type");
      return false;
    }
  }

  if (!setAllTypes(metadata->returnType, helper, NULL, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->returnType->type, &AST_TREE_TYPE_TYPE)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  }

  tree->type = &AST_TREE_TYPE_TYPE;
  return true;
}

bool setTypesFunctionCall(AstTree *tree, AstTreeSetTypesHelper _helper) {
  AstTreeFunctionCall *metadata = tree->metadata;

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
  };

  for (size_t i = 0; i < metadata->parameters_size; ++i) {
    AstTreeFunctionCallParam param = metadata->parameters[i];
    if (!setAllTypes(param.value, helper, NULL, NULL)) {
      return false;
    }
  }

  if (!setAllTypes(metadata->function, helper, NULL, metadata)) {
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
            strnEquals(arg.name_begin, param.nameBegin, param_name_size)) {
          initedArguments[j] = param;
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
        // AstTreeTypeFunctionArgument arg = function->arguments[j];
        if (initedArguments[j].value == NULL) {
          initedArguments[j] = param;
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

bool setTypesVariable(AstTree *tree, AstTreeSetTypesHelper helper,
                      AstTreeFunctionCall *functionCall) {
  AstTreeVariable *variable = setTypesFindVariable(
      tree->str_begin, tree->str_end, helper, functionCall);
  if (variable == NULL) {
    printError(tree->str_begin, tree->str_end, "No candidate found");
    goto RETURN_ERROR;
  }

  tree->metadata = variable;

  if (!setTypesAstVariable(variable, helper)) {
    return false;
  }
  tree->type = copyAstTree(variable->type);
  return true;

RETURN_ERROR:
  return false;
}

bool setTypesOperatorAssign(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeInfix *infix = tree->metadata;
  // TODO: check left one for being left value
  if (!setTypesAstInfix(infix, helper)) {
    return false;
  } else if (!typeIsEqual(infix->left->type, infix->right->type)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else if (isConst(infix->left)) {
    printError(tree->str_begin, tree->str_end, "Constants can't be assigned");
    return false;
  } else {
    tree->type = copyAstTree(infix->left->type);
    return true;
  }
}

bool setTypesOperatorInfix(AstTree *tree, AstTreeSetTypesHelper _helper,
                           const char *str, size_t str_size) {
  AstTreeInfix *metadata = tree->metadata;

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
  };

  if (!setAllTypes(metadata->left, helper, NULL, NULL) ||
      !setAllTypes(metadata->right, helper, NULL, NULL)) {
    return false;
  }

  AstTreeFunctionCallParam parameters[] = {
      (AstTreeFunctionCallParam){
          .nameBegin = NULL,
          .nameEnd = NULL,
          .value = metadata->left,
      },
      (AstTreeFunctionCallParam){
          .nameBegin = NULL,
          .nameEnd = NULL,
          .value = metadata->right,
      },
  };

  AstTreeFunctionCall functionCall = {
      .function = NULL,
      .parameters = parameters,
      .parameters_size = 2,
  };

  AstTreeVariable *variable =
      setTypesFindVariable(str, str + str_size, helper, &functionCall);
  if (variable == NULL) {
    return false;
  }

  metadata->function = variable;
  AstTreeTypeFunction *function = metadata->function->type->metadata;

  tree->type = copyAstTree(function->returnType);
  return true;
}

bool setTypesOperatorInfixWithRet(AstTree *tree, AstTree *retType,
                                  AstTreeSetTypesHelper helper) {
  AstTreeInfix *infix = tree->metadata;
  if (!setTypesAstInfix(infix, helper)) {
    return false;
  } else if (!typeIsEqual(infix->left->type, infix->right->type)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else {
    tree->type = retType;
    return true;
  }
}

bool setTypesOperatorInfixWithRetAndLooking(AstTree *tree, AstTree *lookingType,
                                            AstTree *retType,
                                            AstTreeSetTypesHelper _helper) {
  AstTreeSetTypesHelper helper = {
      .lookingType = lookingType,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
  };
  return setTypesOperatorInfixWithRet(tree, retType, helper);
}

bool setTypesOperatorUnary(AstTree *tree, AstTreeSetTypesHelper helper,
                           const char *funcStr, size_t funcStr_size) {
  AstTreeUnary *metadata = tree->metadata;
  if (!setAllTypes(metadata->operand, helper, NULL, NULL)) {
    return false;
  }

  AstTreeFunctionCallParam parameters[] = {
      (AstTreeFunctionCallParam){
          .nameBegin = NULL,
          .nameEnd = NULL,
          .value = metadata->operand,
      },
  };

  AstTreeFunctionCall functionCall = {
      .function = NULL,
      .parameters = parameters,
      .parameters_size = 1,
  };

  AstTreeVariable *variable = setTypesFindVariable(
      funcStr, funcStr + funcStr_size, helper, &functionCall);
  if (variable == NULL) {
    return false;
  }

  metadata->function = variable;
  AstTreeTypeFunction *function = metadata->function->type->metadata;

  tree->type = copyAstTree(function->returnType);
  return true;
}

bool setTypesOperatorPointer(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *metadata = tree->metadata;
  if (!setAllTypes(metadata, helper, NULL, NULL)) {
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
  if (!setAllTypes(metadata, helper, NULL, NULL)) {
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
  if (!setAllTypes(metadata, helper, NULL, NULL)) {
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
  (void)helper;
  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesAstVariable(AstTreeVariable *variable,
                         AstTreeSetTypesHelper _helper) {
  AstTreeVariable *deps[_helper.dependencies.size + 1];

  for (size_t i = 0; i < _helper.dependencies.size; ++i) {
    AstTreeVariable *depVar = _helper.dependencies.data[i];
    if (variable == depVar) {
      printError(variable->name_begin, variable->name_end,
                 "Circular dependency found");
      return false;
    }
    deps[i] = depVar;
  }
  deps[_helper.dependencies.size] = variable;

  AstTreeSetTypesHelper helper = {
      .lookingType = &AST_TREE_TYPE_TYPE,
      .dependencies =
          {
              .data = deps,
              .size = _helper.dependencies.size + 1,
          },
      .variables = _helper.variables,
  };

  // previously done
  if ((variable->type != NULL && variable->type->type != NULL) &&
      ((variable->value != NULL && variable->value->type != NULL) ||
       (variable->initValue != NULL && variable->initValue->type != NULL))) {
    return true;
  }

  if (variable->type != NULL) {
    if (!setAllTypes(variable->type, helper, NULL, NULL)) {
      return false;
    }

    if (isConstByValue(variable->type)) {
      AstTree *type = variable->type;
      variable->type = getValue(type);
      if (variable->type == NULL) {
        return false;
      }
      if (type != variable->type) {
        astTreeDelete(type);
      }
    }
  }

  helper.lookingType = variable->type;

  if (variable->value != NULL &&
      !setAllTypes(variable->value, helper, NULL, NULL)) {
    return false;
  }
  if (variable->initValue != NULL &&
      !setAllTypes(variable->initValue, helper, NULL, NULL)) {
    return false;
  }

  AstTree *value;
  if (variable->value == NULL) {
    value = variable->initValue;
  } else {
    value = variable->value;
  }

  if (variable->type == NULL) {
    variable->type = copyAstTree(value->type);
  }

  if (variable->type == NULL) {
    return false;
  } else if (value != NULL) {
    if (value != NULL && !typeIsEqual(value->type, variable->type)) {
      printError(variable->name_begin, variable->name_end,
                 "Type mismatch value = %s but type = %s",
                 AST_TREE_TOKEN_STRINGS[value->type->token],
                 AST_TREE_TOKEN_STRINGS[variable->type->token]);
      return false;
    } else if (variable->isConst) {
      if (!isConst(value)) {
        printError(value->str_begin, value->str_end,
                   "Can't initialize constant with non constant value");
        return false;
      }
    }
  }

  return true;
}

bool setTypesIf(AstTree *tree, AstTreeSetTypesHelper helper,
                AstTreeFunction *function) {
  AstTreeIf *metadata = tree->metadata;

  if (!setAllTypes(metadata->condition, helper, function, NULL) ||
      !setAllTypes(metadata->ifBody, helper, function, NULL) ||
      (metadata->elseBody != NULL &&
       !setAllTypes(metadata->elseBody, helper, function, NULL))) {
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

  if (!setAllTypes(metadata->condition, helper, function, NULL) ||
      !setAllTypes(metadata->body, helper, function, NULL)) {
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

bool setTypesScope(AstTree *tree, AstTreeSetTypesHelper _helper,
                   AstTreeFunction *function) {
  AstTreeScope *metadata = tree->metadata;

  AstTreeVariable *variables[_helper.variables.size + metadata->variables.size];

  for (size_t i = 0; i < _helper.variables.size; ++i) {
    variables[i] = _helper.variables.data[i];
  }

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables.data = variables,
      .variables.size = _helper.variables.size,
  };

  for (size_t i = 0; i < metadata->variables.size; ++i) {
    AstTreeVariable *variable = metadata->variables.data[i];
    if (variable->isConst) {
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      helper.variables.data[helper.variables.size++] = variable;
    }
  }

  for (size_t i = 0; i < metadata->expressions_size; ++i) {
    AstTree *expr = metadata->expressions[i];
    if (expr->token == AST_TREE_TOKEN_VARIABLE_DEFINE) {
      AstTreeVariable *variable = expr->metadata;
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      helper.variables.data[helper.variables.size++] = variable;
    }
    if (!setAllTypes(expr, helper, function, NULL)) {
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

  if (!setAllTypes(operand, helper, NULL, NULL)) {
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
  if (!setAllTypes(metadata->object, helper, NULL, NULL)) {
    return false;
  } else if (metadata->object->type->token == AST_TREE_TOKEN_TYPE_ARRAY) {
    const size_t size = metadata->member.name.end - metadata->member.name.begin;
    const char *str = metadata->member.name.begin;

    static const char LENGTH_STR[] = "length";
    static const size_t LENGTH_STR_SIZE =
        sizeof(LENGTH_STR) / sizeof(*LENGTH_STR) - sizeof(*LENGTH_STR);
    if (LENGTH_STR_SIZE == size && strnEquals(LENGTH_STR, str, size)) {
      metadata->member.index = 0;
      tree->type = copyAstTree(&AST_TREE_U64_TYPE);
      return true;
    }

    printError(metadata->member.name.begin, metadata->member.name.end,
               "Member not found");
    return false;
  } else if (metadata->object->type->token == AST_TREE_TOKEN_KEYWORD_STRUCT) {
    AstTreeStruct *struc = metadata->object->type->metadata;
    const size_t size = metadata->member.name.end - metadata->member.name.begin;
    const char *str = metadata->member.name.begin;

    for (size_t i = 0; i < struc->variables.size; ++i) {
      AstTreeVariable *member = struc->variables.data[i];
      const size_t member_size = member->name_end - member->name_begin;
      if (member_size == size && strnEquals(member->name_begin, str, size)) {
        metadata->member.index = i;
        tree->type = copyAstTree(member->type);
        return true;
      }
    }

    printError(metadata->member.name.begin, metadata->member.name.end,
               "Member not found");
    return false;
  } else if (typeIsEqual(metadata->object->type, &AST_TREE_CODE_TYPE)) {
    printError(metadata->object->str_begin, metadata->object->str_end, "Here");
    UNREACHABLE;
  } else {
    printError(metadata->object->str_begin, metadata->object->str_end,
               "The object is not a struct %s",
               AST_TREE_TOKEN_STRINGS[metadata->object->type->token]);
    return false;
  }
}

bool setTypesBuiltinCast(AstTree *tree, AstTreeSetTypesHelper helper,
                         AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters_size == 2) {
    AstTree *from = NULL;
    AstTree *to = NULL;

    static const char FROM_STR[] = "from";
    static const size_t FROM_STR_SIZE =
        sizeof(FROM_STR) / sizeof(*FROM_STR) - sizeof(*FROM_STR);
    static const char TO_STR[] = "to";
    static const size_t TO_STR_SIZE =
        sizeof(TO_STR) / sizeof(*TO_STR) - sizeof(*TO_STR);

    for (size_t i = 0; i < functionCall->parameters_size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters[i];
      const size_t param_name_size = param.nameEnd - param.nameBegin;

      if (param_name_size == 0) {
        if (from == NULL) {
          from = param.value;
        } else if (to == NULL) {
          to = param.value;
        } else {
          printError(param.value->str_begin, param.value->str_end,
                     "Bad paramter");
          return false;
        }
      } else if (param_name_size == FROM_STR_SIZE &&
                 strnEquals(param.nameBegin, FROM_STR, FROM_STR_SIZE) &&
                 from == NULL) {
        from = param.value;
      } else if (param_name_size == TO_STR_SIZE &&
                 strnEquals(param.nameBegin, TO_STR, TO_STR_SIZE) &&
                 to == NULL) {
        to = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    }

    if (from == NULL || to == NULL) {
      return false;
    }

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = 2;
    type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                            sizeof(*type_metadata->arguments));

    type_metadata->returnType = copyAstTree(to);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(from->type),
        .name_begin = FROM_STR,
        .name_end = FROM_STR + FROM_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
    };

    type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(to->type),
        .name_begin = TO_STR,
        .name_end = TO_STR + TO_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
  return false;
}

bool setTypesBuiltinTypeOf(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters_size == 1) {
    AstTree *variable = NULL;

    static const char VARIABLE_STR[] = "variable";
    static const size_t VARIABLE_STR_SIZE =
        sizeof(VARIABLE_STR) / sizeof(*VARIABLE_STR) - sizeof(*VARIABLE_STR);

    for (size_t i = 0; i < functionCall->parameters_size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters[i];
      const size_t param_name_size = param.nameEnd - param.nameBegin;

      if (param_name_size == 0) {
        if (variable == NULL) {
          variable = param.value;
        } else {
          printError(param.value->str_begin, param.value->str_end,
                     "Bad paramter");
          return false;
        }
      } else if (param_name_size == VARIABLE_STR_SIZE &&
                 strnEquals(param.nameBegin, VARIABLE_STR, VARIABLE_STR_SIZE) &&
                 variable == NULL) {
        variable = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    }

    if (variable == NULL) {
      return false;
    }

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = 1;
    type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                            sizeof(*type_metadata->arguments));

    type_metadata->returnType = copyAstTree(&AST_TREE_TYPE_TYPE);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(variable->type),
        .name_begin = VARIABLE_STR,
        .name_end = VARIABLE_STR + VARIABLE_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
  return false;
}

bool setTypesBuiltinImport(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters_size == 1) {
    AstTree *file = NULL;

    static const char VARIABLE_STR[] = "variable";
    static const size_t VARIABLE_STR_SIZE =
        sizeof(VARIABLE_STR) / sizeof(*VARIABLE_STR) - sizeof(*VARIABLE_STR);

    for (size_t i = 0; i < functionCall->parameters_size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters[i];
      const size_t param_name_size = param.nameEnd - param.nameBegin;

      if (param_name_size == 0) {
        if (file == NULL) {
          file = param.value;
        } else {
          printError(param.value->str_begin, param.value->str_end,
                     "Bad paramter");
          return false;
        }
      } else if (param_name_size == VARIABLE_STR_SIZE &&
                 strnEquals(param.nameBegin, VARIABLE_STR, VARIABLE_STR_SIZE) &&
                 file == NULL) {
        file = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    }

    if (file == NULL) {
      return false;
    }

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = 1;
    type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                            sizeof(*type_metadata->arguments));

    type_metadata->returnType = copyAstTree(&AST_TREE_CODE_TYPE);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(file->type),
        .name_begin = VARIABLE_STR,
        .name_end = VARIABLE_STR + VARIABLE_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
  return false;
}

bool setTypesBuiltinIsComptime(AstTree *tree, AstTreeSetTypesHelper helper) {
  (void)helper;
  tree->type = copyAstTree(&AST_TREE_BOOL_TYPE);
  return true;
}

bool setTypesBuiltinStackAlloc(AstTree *tree, AstTreeSetTypesHelper helper,
                               AstTreeFunctionCall *functionCall) {
  NOT_IMPLEMENTED;
}

bool setTypesBuiltinHeapAlloc(AstTree *tree, AstTreeSetTypesHelper helper,
                              AstTreeFunctionCall *functionCall) {
  NOT_IMPLEMENTED;
}

bool setTypesBuiltinUnary(AstTree *tree, AstTreeSetTypesHelper helper,
                          AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters_size != 1) {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }

  const char *str = functionCall->parameters[0].nameBegin;
  const size_t str_size = functionCall->parameters[0].nameEnd -
                          functionCall->parameters[0].nameBegin;

  static char VALUE_STR[] = "value";
  static size_t VALUE_STR_SIZE =
      sizeof(VALUE_STR) / sizeof(*VALUE_STR) - sizeof(*VALUE_STR);

  if (str_size != 0 && (str_size != VALUE_STR_SIZE ||
                        !strnEquals(str, VALUE_STR, VALUE_STR_SIZE))) {
    printError(functionCall->parameters[0].nameBegin,
               functionCall->parameters[0].nameEnd, "Unknown parameter");
    return false;
  }

  AstTree *type = functionCall->parameters[0].value->type;
  switch (type->token) {
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
    goto AFTER_SWITCH;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
  case AST_TREE_TOKEN_KEYWORD_PUTC:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_NONE:
  }
  printError(functionCall->parameters[0].nameBegin,
             functionCall->parameters[0].nameEnd, "Bad argument");
  return false;

AFTER_SWITCH:
  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 1;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));

  type_metadata->returnType = copyAstTree(type);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(type),
      .name_begin = VALUE_STR,
      .name_end = VALUE_STR + VALUE_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinBinary(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters_size != 2) {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
  AstTree *left = NULL;
  AstTree *right = NULL;

  static char LEFT_STR[] = "left";
  static const size_t LEFT_STR_SIZE =
      sizeof(LEFT_STR) / sizeof(*LEFT_STR) - sizeof(*LEFT_STR);
  static char RIGHT_STR[] = "right";
  static const size_t RIGHT_STR_SIZE =
      sizeof(RIGHT_STR) / sizeof(*RIGHT_STR) - sizeof(*RIGHT_STR);

  for (size_t i = 0; i < functionCall->parameters_size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters[i];
    const size_t param_name_size = param.nameEnd - param.nameBegin;

    if (param_name_size == 0) {
      if (left == NULL) {
        left = param.value;
      } else if (right == NULL) {
        right = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    } else if (param_name_size == LEFT_STR_SIZE &&
               strnEquals(param.nameBegin, LEFT_STR, LEFT_STR_SIZE) &&
               left == NULL) {
      left = param.value;
    } else if (param_name_size == RIGHT_STR_SIZE &&
               strnEquals(param.nameBegin, RIGHT_STR, RIGHT_STR_SIZE) &&
               right == NULL) {
      right = param.value;
    } else {
      printError(param.value->str_begin, param.value->str_end, "Bad paramter");
      return false;
    }
  }

  if (left == NULL || right == NULL) {
    return false;
  }

  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 2;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));

  type_metadata->returnType = copyAstTree(left->type);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = LEFT_STR,
      .name_end = LEFT_STR + LEFT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
  };

  type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = RIGHT_STR,
      .name_end = RIGHT_STR + RIGHT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinBinaryWithRet(AstTree *tree, AstTreeSetTypesHelper helper,
                                  AstTreeFunctionCall *functionCall,
                                  AstTree *retType) {
  (void)helper;
  if (functionCall->parameters_size != 2) {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
  AstTree *left = NULL;
  AstTree *right = NULL;

  static char LEFT_STR[] = "left";
  static const size_t LEFT_STR_SIZE =
      sizeof(LEFT_STR) / sizeof(*LEFT_STR) - sizeof(*LEFT_STR);
  static char RIGHT_STR[] = "right";
  static const size_t RIGHT_STR_SIZE =
      sizeof(RIGHT_STR) / sizeof(*RIGHT_STR) - sizeof(*RIGHT_STR);

  for (size_t i = 0; i < functionCall->parameters_size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters[i];
    const size_t param_name_size = param.nameEnd - param.nameBegin;

    if (param_name_size == 0) {
      if (left == NULL) {
        left = param.value;
      } else if (right == NULL) {
        right = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    } else if (param_name_size == LEFT_STR_SIZE &&
               strnEquals(param.nameBegin, LEFT_STR, LEFT_STR_SIZE) &&
               left == NULL) {
      left = param.value;
    } else if (param_name_size == RIGHT_STR_SIZE &&
               strnEquals(param.nameBegin, RIGHT_STR, RIGHT_STR_SIZE) &&
               right == NULL) {
      right = param.value;
    } else {
      printError(param.value->str_begin, param.value->str_end, "Bad paramter");
      return false;
    }
  }

  if (left == NULL || right == NULL) {
    return false;
  }

  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 2;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));

  type_metadata->returnType = copyAstTree(retType);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = LEFT_STR,
      .name_end = LEFT_STR + LEFT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
  };

  type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = RIGHT_STR,
      .name_end = RIGHT_STR + RIGHT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesTypeArray(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeBracket *metadata = tree->metadata;

  if (!setAllTypes(metadata->operand, helper, NULL, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->operand->type, &AST_TREE_TYPE_TYPE)) {
    printError(metadata->operand->str_begin, metadata->operand->str_end,
               "Expected type");
    return false;
  }

  if (metadata->parameters.size == 0) {
    // left empty
  } else if (metadata->parameters.size == 1) {
    AstTreeSetTypesHelper newHelper = {
        .lookingType = &AST_TREE_U64_TYPE,
        .dependencies = helper.dependencies,
        .variables = helper.variables,
    };

    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      AstTree *param = metadata->parameters.data[i];
      if (!setAllTypes(param, newHelper, NULL, NULL)) {
        return false;
      } else if (!isIntType(param->type)) {
        printError(param->str_begin, param->str_end,
                   "Should only be int (for now)");
        return false;
      }
    }
  } else {
    printError(tree->str_begin, tree->str_end,
               "Multiple param in array is not yet supported");
    return false;
  }

  tree->type = copyAstTree(&AST_TREE_TYPE_TYPE);
  return true;
}

bool setTypesArrayAccess(AstTree *tree, AstTreeSetTypesHelper _helper) {
  AstTreeBracket *metadata = tree->metadata;

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
  };

  if (!setAllTypes(metadata->operand, helper, NULL, NULL)) {
    return false;
  } else if (metadata->operand->type->token != AST_TREE_TOKEN_TYPE_ARRAY) {
    printError(metadata->operand->str_begin, metadata->operand->str_end,
               "Expected array");
    return false;
  }

  if (metadata->parameters.size != 1) {
    printError(tree->str_begin, tree->str_end,
               "Multiple param in array is not yet supported");
    return false;
  }

  for (size_t i = 0; i < metadata->parameters.size; ++i) {
    AstTree *param = metadata->parameters.data[i];
    if (!setAllTypes(param, helper, NULL, NULL)) {
      return false;
    } else if (!isIntType(param->type)) {
      printError(param->str_begin, param->str_end,
                 "Should only be int (for now)");
      return false;
    }
  }

  AstTreeBracket *operand_metadata = metadata->operand->type->metadata;

  tree->type = copyAstTree(operand_metadata->operand);
  return true;
}

bool setTypesAstInfix(AstTreeInfix *infix, AstTreeSetTypesHelper _helper) {
  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
  };

  if (!setAllTypes(infix->left, helper, NULL, NULL)) {
    return false;
  }

  helper.lookingType = infix->left->type;

  return setAllTypes(infix->right, helper, NULL, NULL);
}

AstTreeVariable *setTypesFindVariable(const char *name_begin,
                                      const char *name_end,
                                      AstTreeSetTypesHelper helper,
                                      AstTreeFunctionCall *functionCall) {
  AstTreeVariable *variable = NULL;

  const char *str = name_begin;
  const size_t str_size = name_end - name_begin;

  if (functionCall == NULL) {
    for (size_t i = helper.variables.size - 1; i != -1ULL; --i) {
      AstTreeVariable *var = helper.variables.data[i];

      const char *var_str = var->name_begin;
      const size_t var_str_size = var->name_end - var->name_begin;

      if (var_str_size != str_size || !strnEquals(var_str, str, str_size)) {
        continue;
      }

      if (!setTypesAstVariable(var, helper)) {
        return NULL;
      }

      variable = var;
      break;
    }
  } else {
    for (size_t i = helper.variables.size - 1; i != -1ULL; --i) {
      AstTreeVariable *var = helper.variables.data[i];

      const char *var_str = var->name_begin;
      const size_t var_str_size = var->name_end - var->name_begin;

      if (var_str_size != str_size || !strnEquals(var_str, str, str_size)) {
        continue;
      }

      if (!setTypesAstVariable(var, helper)) {
        return NULL;
      }

      if (var->type->token != AST_TREE_TOKEN_TYPE_FUNCTION) {
        continue;
      }

      AstTreeTypeFunction *function = var->type->metadata;

      AstTreeFunctionCallParam initedArguments[function->arguments_size];
      size_t initedArguments_size = function->arguments_size;

      for (size_t i = 0; i < initedArguments_size; ++i) {
        initedArguments[i].value = NULL;
      }

      for (size_t i = 0; i < functionCall->parameters_size; ++i) {
        AstTreeFunctionCallParam param = functionCall->parameters[i];
        if (param.nameBegin != param.nameEnd) {
          const size_t param_name_size = param.nameEnd - param.nameBegin;
          for (size_t j = 0; j < function->arguments_size; ++j) {
            AstTreeTypeFunctionArgument arg = function->arguments[j];
            if ((size_t)(arg.name_end - arg.name_begin) == param_name_size &&
                strnEquals(arg.name_begin, param.nameBegin, param_name_size)) {
              if (!typeIsEqual(arg.type, param.value->type)) {
                goto CONTINUE_OUTER;
              }
              initedArguments[j] = param;
              goto END_OF_NAMED_FOR;
            }
          }
          goto CONTINUE_OUTER;
        }
      END_OF_NAMED_FOR:
      }

      for (size_t i = 0; i < functionCall->parameters_size; ++i) {
        AstTreeFunctionCallParam param = functionCall->parameters[i];
        if (param.nameBegin == param.nameEnd) {
          for (size_t j = 0; j < function->arguments_size; ++j) {
            AstTreeTypeFunctionArgument arg = function->arguments[j];
            if (initedArguments[j].value == NULL) {
              if (!typeIsEqual(arg.type, param.value->type)) {
                goto CONTINUE_OUTER;
              }
              initedArguments[j] = param;
              goto END_OF_UNNAMED_FOR;
            }
          }
          goto CONTINUE_OUTER;
        }
      END_OF_UNNAMED_FOR:
      }

      for (size_t i = 0; i < function->arguments_size; ++i) {
        if (initedArguments[i].value == NULL) {
          goto CONTINUE_OUTER;
        }
      }
      if (variable != NULL) {
        printError(name_begin, name_end, "Multiple candidates found for %.*s",
                   (int)(name_end - name_begin), name_begin);
        return NULL;
      }
      variable = var;
    CONTINUE_OUTER:
    }
  }
  if (variable == NULL) {
    printError(name_begin, name_end, "No candidates found for %.*s",
               (int)(name_end - name_begin), name_begin);
    return NULL;
  }

  return variable;
}

char *u8ArrayToCString(AstTree *tree) {
  AstTreeObject *object = tree->metadata;
  char *str = a404m_malloc((object->variables.size + 1) * sizeof(*str));
  for (size_t i = 0; i < object->variables.size; ++i) {
    str[i] = *(AstTreeInt *)object->variables.data[i]->value->metadata;
  }
  str[object->variables.size] = '\0';
  return str;
}

AstTree *makeStringType() {
  AstTreeBracket *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->operand = &AST_TREE_U8_TYPE;

  type_metadata->parameters.size = 0;
  type_metadata->parameters.data =
      a404m_malloc(0 * sizeof(*type_metadata->parameters.data));

  return newAstTree(AST_TREE_TOKEN_TYPE_ARRAY, type_metadata,
                    &AST_TREE_TYPE_TYPE, NULL, NULL);
}
