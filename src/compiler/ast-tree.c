#include "ast-tree.h"

#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "runner/runner.h"
#include "utils/file.h"
#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"
#include "utils/time.h"
#include "utils/type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AstTree AST_TREE_TYPE_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_TYPE,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_TYPE_ANY_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_ANY_TYPE,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_VOID_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_VOID,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_BOOL_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_BOOL,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_I8_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I8,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_U8_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U8,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_I16_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I16,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_U16_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U16,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_I32_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I32,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_U32_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U32,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_I64_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_I64,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_U64_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_U64,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

#ifdef FLOAT_16_SUPPORT
AstTree AST_TREE_F16_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F16,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};
#endif

AstTree AST_TREE_F32_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F32,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_F64_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F64,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_F128_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_F128,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_CODE_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_CODE,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_NAMESPACE_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_NAMESPACE,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_SHAPE_SHIFTER_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_C_LIBRARY_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_C_LIBRARY,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_MACRO_TYPE = {
    .token = AST_TREE_TOKEN_TYPE_MACRO,
    .metadata = NULL,
    .type = &AST_TREE_TYPE_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

AstTree AST_TREE_VOID_VALUE = {
    .token = AST_TREE_TOKEN_VALUE_VOID,
    .metadata = NULL,
    .type = &AST_TREE_VOID_TYPE,
    .str_begin = NULL,
    .str_end = NULL,
};

const char *AST_TREE_TOKEN_STRINGS[] = {
    "AST_TREE_TOKEN_FUNCTION",

    "AST_TREE_TOKEN_BUILTIN_CAST",
    "AST_TREE_TOKEN_BUILTIN_TYPE_OF",
    "AST_TREE_TOKEN_BUILTIN_SIZE_OF",
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
    "AST_TREE_TOKEN_BUILTIN_PUTC",
    "AST_TREE_TOKEN_BUILTIN_C_LIBRARY",
    "AST_TREE_TOKEN_BUILTIN_C_FUNCTION",
    "AST_TREE_TOKEN_BUILTIN_BITWISE_NOT",
    "AST_TREE_TOKEN_BUILTIN_BITWISE_AND",
    "AST_TREE_TOKEN_BUILTIN_BITWISE_XOR",
    "AST_TREE_TOKEN_BUILTIN_BITWISE_OR",
    "AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT",
    "AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT",
    "AST_TREE_TOKEN_BUILTIN_INSERT",

    "AST_TREE_TOKEN_KEYWORD_RETURN",
    "AST_TREE_TOKEN_KEYWORD_BREAK",
    "AST_TREE_TOKEN_KEYWORD_CONTINUE",
    "AST_TREE_TOKEN_KEYWORD_IF",
    "AST_TREE_TOKEN_KEYWORD_WHILE",
    "AST_TREE_TOKEN_KEYWORD_COMPTIME",
    "AST_TREE_TOKEN_KEYWORD_STRUCT",

    "AST_TREE_TOKEN_TYPE_FUNCTION",
    "AST_TREE_TOKEN_TYPE_ARRAY",
    "AST_TREE_TOKEN_TYPE_TYPE",
    "AST_TREE_TOKEN_TYPE_ANY_TYPE",
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
    "AST_TREE_TOKEN_TYPE_NAMESPACE",
    "AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER",
    "AST_TREE_TOKEN_TYPE_C_LIBRARY",
    "AST_TREE_TOKEN_TYPE_C_FUNCTION",
    "AST_TREE_TOKEN_TYPE_MACRO",
    "AST_TREE_TOKEN_TYPE_BOOL",
    "AST_TREE_TOKEN_VALUE_VOID",

    "AST_TREE_TOKEN_FUNCTION_CALL",
    "AST_TREE_TOKEN_VARIABLE",
    "AST_TREE_TOKEN_VARIABLE_DEFINE",
    "AST_TREE_TOKEN_VALUE_NULL",
    "AST_TREE_TOKEN_VALUE_UNDEFINED",
    "AST_TREE_TOKEN_VALUE_NAMESPACE",
    "AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER",
    "AST_TREE_TOKEN_VALUE_C_LIBRARY",
    "AST_TREE_TOKEN_VALUE_C_FUNCTION",
    "AST_TREE_TOKEN_VALUE_MACRO",
    "AST_TREE_TOKEN_VALUE_INT",
    "AST_TREE_TOKEN_VALUE_FLOAT",
    "AST_TREE_TOKEN_VALUE_BOOL",
    "AST_TREE_TOKEN_VALUE_OBJECT",
    "AST_TREE_TOKEN_RAW_VALUE",
    "AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED",

    "AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT",

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
    "AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN",
    "AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS",
    "AST_TREE_TOKEN_OPERATOR_BITWISE_NOT",
    "AST_TREE_TOKEN_OPERATOR_BITWISE_AND",
    "AST_TREE_TOKEN_OPERATOR_BITWISE_XOR",
    "AST_TREE_TOKEN_OPERATOR_BITWISE_OR",
    "AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT",
    "AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT",

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
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
    goto RETURN_SUCCESS;
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE: {
    AstTreeLoopControl *meatadata = tree->metadata;
    printf("count=%d", meatadata->count);
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
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
  case AST_TREE_TOKEN_RAW_VALUE: {
    AstTreeRawValue *metadata = tree->metadata;
    printf(",size=%zu", a404m_malloc_usable_size(metadata));
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
    for (size_t i = 0; i < metadata->items_size; ++i) {
      AstTreeObjectItem item = metadata->items[i];
      for (int i = 0; i < indent + 1; ++i)
        printf(" ");
      astTreePrint(item.value, indent + 2);
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
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
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
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS: {
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
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      AstTreeFunctionCallParam param = metadata->parameters.data[i];
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
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreePureInfix *metadata = tree->metadata;
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
    if (metadata->elseBody != NULL) {
      astTreePrint(metadata->elseBody, indent + 1);
    } else {
      printf("null");
    }
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
  case AST_TREE_TOKEN_TYPE_ARRAY: {
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
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_VALUE_MACRO:
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

void astTreeTypeFunctionDestroy(AstTreeTypeFunction functionType) {
  for (size_t i = 0; i < functionType.arguments_size; ++i) {
    AstTreeTypeFunctionArgument arg = functionType.arguments[i];
    astTreeDelete(arg.type);
  }
  astTreeDelete(functionType.returnType);
  free(functionType.arguments);
}

void astTreeScopeDestroy(AstTreeScope scope) {
  for (size_t i = 0; i < scope.expressions_size; ++i) {
    astTreeDelete(scope.expressions[i]);
  }
  for (size_t i = 0; i < scope.variables.size; ++i) {
    astTreeVariableDelete(scope.variables.data[i]);
  }
  for (size_t i = 0; i < scope.stackAllocation_size; ++i) {
    free(scope.stackAllocation[i]);
  }
  free(scope.stackAllocation);
  free(scope.expressions);
  free(scope.variables.data);
}

void astTreeFunctionDestroy(AstTreeFunction function) {
  for (size_t i = 0; i < function.arguments.size; ++i) {
    astTreeVariableDelete(function.arguments.data[i]);
  }
  astTreeScopeDestroy(function.scope);
  astTreeDelete(function.returnType);
  free(function.arguments.data);
}

void astTreeDeleteFunctionCall(AstTreeFunctionCall *functionCall) {
  if (functionCall->function != NULL) {
    astTreeDelete(functionCall->function);
  }
  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    astTreeDelete(functionCall->parameters.data[i].value);
  }
  free(functionCall->parameters.data);
  free(functionCall);
}

void astTreeDestroy(AstTree tree) {
  if (tree.type != NULL) {
    astTreeDelete(tree.type);
  }
  switch (tree.token) {
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *metadata = tree.metadata;
    astTreeFunctionDestroy(*metadata);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
    return;
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE: {
    AstTreeLoopControl *meatadata = tree.metadata;
    free(meatadata);
    return;
  }
  case AST_TREE_TOKEN_VALUE_NAMESPACE: {
    AstTreeNamespace *metadata = tree.metadata;
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER: {
    AstTreeShapeShifter *metadata = tree.metadata;
    astTreeFunctionDestroy(*metadata->function);
    free(metadata->function);
    for (size_t i = 0; i < metadata->generateds.size; ++i) {
      astTreeFunctionDestroy(*metadata->generateds.functions[i]);
      free(metadata->generateds.functions[i]);
      for (size_t j = 0; j < metadata->generateds.calls[i].size; ++j) {
        astTreeDelete(metadata->generateds.calls[i].data[j].value);
      }
      free(metadata->generateds.calls[i].data);
    }
    free(metadata->generateds.functions);
    free(metadata->generateds.calls);
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_VALUE_MACRO: {
    AstTreeMacro *metadata = tree.metadata;
    astTreeFunctionDestroy(*metadata->function);
    free(metadata->function);
    free(metadata);
    return;
  }
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
    for (size_t i = 0; i < metadata->items_size; ++i) {
      AstTreeObjectItem item = metadata->items[i];
      astTreeDelete(item.value);
    }
    free(metadata->items);
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_RAW_VALUE: {
    AstTreeRawValue *metadata = tree.metadata;
    free(metadata);
    return;
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
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
    astTreeTypeFunctionDestroy(*metadata);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
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
    AstTreeFunctionCall *metadata = tree.metadata;
    astTreeDeleteFunctionCall(metadata);
  }
    return;
  case AST_TREE_TOKEN_VARIABLE: {
    // AstTreeIdentifier *metadata = tree.metadata; // not needed
  }
    return;
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreePureInfix *metadata = tree.metadata;
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
    astTreeScopeDestroy(*metadata);
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
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    AstTreeBracket *metadata = tree.metadata;
    astTreeDelete(metadata->operand);
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      astTreeDelete(metadata->parameters.data[i]);
    }
    free(metadata->parameters.data);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT: {
    AstTreeShapeShifterElement *metadata = tree.metadata;
    astTreeDelete(metadata->shapeShifter);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_TYPE_C_FUNCTION: {
    AstTreeCFunctionType *metadata = tree.metadata;
    astTreeDelete(metadata->funcType);
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_VALUE_C_LIBRARY: {
    AstTreeCLibrary *metadata = tree.metadata;
    free(metadata);
  }
    return;
  case AST_TREE_TOKEN_VALUE_C_FUNCTION: {
    AstTreeCFunction *metadata = tree.metadata;
    astTreeDelete(metadata->library);
    astTreeDelete(metadata->name);
    astTreeDelete(metadata->funcType);
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
  return tree != &AST_TREE_TYPE_TYPE && tree != &AST_TREE_TYPE_ANY_TYPE &&
         tree != &AST_TREE_VOID_TYPE && tree != &AST_TREE_BOOL_TYPE &&
         tree != &AST_TREE_I8_TYPE && tree != &AST_TREE_U8_TYPE &&
         tree != &AST_TREE_I16_TYPE && tree != &AST_TREE_U16_TYPE &&
         tree != &AST_TREE_I32_TYPE && tree != &AST_TREE_U32_TYPE &&
         tree != &AST_TREE_I64_TYPE && tree != &AST_TREE_U64_TYPE &&
#ifdef FLOAT_16_SUPPORT
         tree != &AST_TREE_F16_TYPE &&
#endif
         tree != &AST_TREE_F32_TYPE && tree != &AST_TREE_F64_TYPE &&
         tree != &AST_TREE_F128_TYPE && tree != &AST_TREE_CODE_TYPE &&
         tree != &AST_TREE_NAMESPACE_TYPE &&
         tree != &AST_TREE_SHAPE_SHIFTER_TYPE &&
         tree != &AST_TREE_C_LIBRARY_TYPE && tree != &AST_TREE_MACRO_TYPE &&
         tree != &AST_TREE_VOID_VALUE;
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
  return copyAstTreeBack(tree, NULL, NULL, 0, false);
}

AstTree *copyAstTreeWithCheck(AstTree *tree) {
  return copyAstTreeBack(tree, NULL, NULL, 0, true);
}

AstTree *copyAstTreeBack(AstTree *tree, AstTreeVariables oldVariables[],
                         AstTreeVariables newVariables[], size_t variables_size,
                         bool safetyCheck) {
  if (safetyCheck && tree == NULL) {
    return NULL;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_MACRO:
    return tree;
  case AST_TREE_TOKEN_VALUE_VOID:
    if (tree == &AST_TREE_VOID_VALUE) {
      return tree;
    }
    // fall through
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
    return newAstTree(tree->token, NULL,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE: {
    AstTreeLoopControl *metadata = tree->metadata;
    AstTreeLoopControl *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->count = metadata->count;

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_NAMESPACE: {
    AstTreeNamespace *metadata = tree->metadata;
    AstTreeNamespace *newMetadata = a404m_malloc(sizeof(*newMetadata));
    newMetadata->importedIndex = metadata->importedIndex;
    return newAstTree(tree->token, newMetadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER: {
    AstTreeShapeShifter *metadata = tree->metadata;
    AstTreeShapeShifter *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->function = copyAstTreeFunction(
        metadata->function, oldVariables, newVariables, variables_size, true);

    new_metadata->generateds.size = metadata->generateds.size;
    new_metadata->generateds.functions =
        a404m_malloc(new_metadata->generateds.size *
                     sizeof(*metadata->generateds.functions));
    new_metadata->generateds.calls = a404m_malloc(
        new_metadata->generateds.size * sizeof(*metadata->generateds.calls));
    for (size_t i = 0; i < metadata->generateds.size; ++i) {
      new_metadata->generateds.functions[i] =
          copyAstTreeFunction(metadata->generateds.functions[i], oldVariables,
                              newVariables, variables_size, false);

      new_metadata->generateds.calls[i].size =
          metadata->generateds.calls[i].size;

      new_metadata->generateds.calls[i].data =
          a404m_malloc(sizeof(*new_metadata->generateds.calls[i].data) *
                       new_metadata->generateds.calls[i].size);

      for (size_t j = 0; j < new_metadata->generateds.calls[i].size; ++j) {
        new_metadata->generateds.calls[i].data[j].value =
            copyAstTreeBack(metadata->generateds.calls[i].data[j].value,
                            oldVariables, newVariables, variables_size, false);
      }
    }

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_BOOL: {
    AstTreeBool *metadata = tree->metadata;
    AstTreeBool *newMetadata = a404m_malloc(sizeof(*newMetadata));
    *newMetadata = *metadata;
    return newAstTree(tree->token, newMetadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_INT: {
    AstTreeInt *metadata = tree->metadata;
    AstTreeInt *newMetadata = a404m_malloc(sizeof(*newMetadata));
    *newMetadata = *metadata;
    return newAstTree(tree->token, newMetadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_FLOAT: {
    AstTreeFloat *metadata = tree->metadata;
    AstTreeFloat *newMetadata = a404m_malloc(sizeof(*newMetadata));
    *newMetadata = *metadata;
    return newAstTree(tree->token, newMetadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_OBJECT: {
    AstTreeObject *metadata = tree->metadata;
    AstTreeObject *newMetadata = a404m_malloc(sizeof(*newMetadata));

    newMetadata->items_size = metadata->items_size;
    newMetadata->items =
        a404m_malloc(newMetadata->items_size * sizeof(*newMetadata->items));

    for (size_t i = 0; i < newMetadata->items_size; ++i) {
      newMetadata->items[i].value =
          copyAstTreeBack(metadata->items[i].value, oldVariables, newVariables,
                          variables_size, safetyCheck);
    }

    return newAstTree(tree->token, newMetadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_RAW_VALUE: {
    AstTreeRawValue *metadata = tree->metadata;
    const size_t size = a404m_malloc_usable_size(metadata);
    AstTreeRawValue *newMetadata = a404m_malloc(size);

    memcpy(newMetadata, metadata, size);

    return newAstTree(tree->token, newMetadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED: {
    return newAstTree(tree->token, tree->metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE: {
    AstTreeVariable *variable = tree->metadata;

    return newAstTree(tree->token,
                      copyAstTreeBackFindVariable(variable, oldVariables,
                                                  newVariables, variables_size),
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_TYPE_FUNCTION: {
    AstTreeTypeFunction *metadata = tree->metadata;
    AstTreeTypeFunction *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->arguments_size = metadata->arguments_size;
    new_metadata->arguments = a404m_malloc(sizeof(*new_metadata->arguments) *
                                           new_metadata->arguments_size);
    new_metadata->isMacro = metadata->isMacro;
    for (size_t i = 0; i < metadata->arguments_size; ++i) {
      AstTreeTypeFunctionArgument arg = metadata->arguments[i];
      new_metadata->arguments[i].str_begin = arg.str_begin;
      new_metadata->arguments[i].str_end = arg.str_end;
      new_metadata->arguments[i].name_begin = arg.name_begin;
      new_metadata->arguments[i].name_end = arg.name_end;
      new_metadata->arguments[i].type = copyAstTreeBack(
          arg.type, oldVariables, newVariables, variables_size, safetyCheck);
      new_metadata->arguments[i].isComptime = arg.isComptime;
    }
    new_metadata->returnType =
        copyAstTreeBack(metadata->returnType, oldVariables, newVariables,
                        variables_size, safetyCheck);
    return newAstTree(tree->token, new_metadata, &AST_TREE_TYPE_TYPE,
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN: {
    AstTreePureInfix *metadata = tree->metadata;
    AstTreePureInfix *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->left =
        copyAstTreeBack(metadata->left, oldVariables, newVariables,
                        variables_size, safetyCheck);
    new_metadata->right =
        copyAstTreeBack(metadata->right, oldVariables, newVariables,
                        variables_size, safetyCheck);

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *metadata = tree->metadata;
    AstTreeFunction *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->arguments =
        copyAstTreeVariables(metadata->arguments, oldVariables, newVariables,
                             variables_size, safetyCheck);
    new_metadata->isMacro = metadata->isMacro;

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
                        new_newVariables, new_variables_size, safetyCheck);

    new_metadata->scope.variables =
        copyAstTreeVariables(metadata->scope.variables, new_oldVariables,
                             new_newVariables, new_variables_size, safetyCheck);
    new_metadata->scope.expressions_size = metadata->scope.expressions_size;
    new_metadata->scope.expressions =
        a404m_malloc(new_metadata->scope.expressions_size *
                     sizeof(*new_metadata->scope.expressions));

    new_oldVariables[new_variables_size - 1] = metadata->scope.variables;
    new_newVariables[new_variables_size - 1] = new_metadata->scope.variables;

    for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
      new_metadata->scope.expressions[i] =
          copyAstTreeBack(metadata->scope.expressions[i], new_oldVariables,
                          new_newVariables, new_variables_size, safetyCheck);
    }
    new_metadata->scope.stackAllocation_size = 0; // TODO: change later
    new_metadata->scope.stackAllocation = a404m_malloc(0);

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, new_oldVariables,
                                      new_newVariables, new_variables_size,
                                      safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *metadata = tree->metadata;
    AstTreeSingleChild *new_metadata = copyAstTreeBack(
        metadata, oldVariables, newVariables, variables_size, safetyCheck);

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_RETURN: {
    AstTreeReturn *metadata = tree->metadata;
    AstTreeReturn *new_metadata = a404m_malloc(sizeof(*new_metadata));
    if (metadata->value != NULL) {
      new_metadata->value =
          copyAstTreeBack(metadata->value, oldVariables, newVariables,
                          variables_size, safetyCheck);
    } else {
      new_metadata->value = NULL;
    }

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
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
    AstTreeFunctionCall *metadata = tree->metadata;
    AstTreeFunctionCall *new_metadata = copyAstTreeFunctionCall(
        metadata, oldVariables, newVariables, variables_size, safetyCheck);
    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_IF: {
    AstTreeIf *metadata = tree->metadata;
    AstTreeIf *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->condition =
        copyAstTreeBack(metadata->condition, oldVariables, newVariables,
                        variables_size, safetyCheck);
    new_metadata->ifBody =
        copyAstTreeBack(metadata->ifBody, oldVariables, newVariables,
                        variables_size, safetyCheck);
    if (metadata->elseBody != NULL) {
      new_metadata->elseBody =
          copyAstTreeBack(metadata->elseBody, oldVariables, newVariables,
                          variables_size, safetyCheck);
    } else {
      new_metadata->elseBody = NULL;
    }
    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE: {
    AstTreeWhile *metadata = tree->metadata;
    AstTreeWhile *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->condition =
        copyAstTreeBack(metadata->condition, oldVariables, newVariables,
                        variables_size, safetyCheck);
    new_metadata->body =
        copyAstTreeBack(metadata->body, oldVariables, newVariables,
                        variables_size, safetyCheck);
    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_SCOPE: {
    AstTreeScope *metadata = tree->metadata;
    AstTreeScope *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->variables =
        copyAstTreeVariables(metadata->variables, oldVariables, newVariables,
                             variables_size, safetyCheck);
    new_metadata->expressions_size = metadata->expressions_size;
    new_metadata->expressions = a404m_malloc(
        new_metadata->expressions_size * sizeof(*new_metadata->expressions));

    new_metadata->stackAllocation_size = 0; // TODO: change later
    new_metadata->stackAllocation = a404m_malloc(0);

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
                          new_newVariables, new_variables_size, safetyCheck);
    }

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, new_oldVariables,
                                      new_newVariables, new_variables_size,
                                      safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = tree->metadata;
    AstTreeStruct *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->variables =
        copyAstTreeVariables(metadata->variables, oldVariables, newVariables,
                             variables_size, safetyCheck);
    new_metadata->id = metadata->id;

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_OPERATOR_ACCESS: {
    AstTreeAccess *metadata = tree->metadata;
    AstTreeAccess *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->object =
        copyAstTreeBack(metadata->object, oldVariables, newVariables,
                        variables_size, safetyCheck);
    new_metadata->member = metadata->member;

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    AstTreeBracket *metadata = tree->metadata;
    AstTreeBracket *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->operand =
        copyAstTreeBack(metadata->operand, oldVariables, newVariables,
                        variables_size, safetyCheck);

    new_metadata->parameters.size = metadata->parameters.size;
    new_metadata->parameters.data = a404m_malloc(
        sizeof(*new_metadata->parameters.data) * new_metadata->parameters.size);

    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      new_metadata->parameters.data[i] =
          copyAstTreeBack(metadata->parameters.data[i], oldVariables,
                          newVariables, variables_size, safetyCheck);
    }

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT: {
    AstTreeShapeShifterElement *metadata = tree->metadata;
    AstTreeShapeShifterElement *new_metadata =
        a404m_malloc(sizeof(*new_metadata));

    new_metadata->shapeShifter =
        copyAstTreeBack(metadata->shapeShifter, oldVariables, newVariables,
                        variables_size, safetyCheck);
    new_metadata->index = metadata->index;
    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_TYPE_C_FUNCTION: {
    AstTreeCFunctionType *metadata = tree->metadata;
    AstTreeCFunctionType *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->funcType =
        copyAstTreeBack(metadata->funcType, oldVariables, newVariables,
                        variables_size, safetyCheck);

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_C_LIBRARY: {
    AstTreeCLibrary *metadata = tree->metadata;
    AstTreeCLibrary *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->dl = metadata->dl;

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_C_FUNCTION: {
    AstTreeCFunction *metadata = tree->metadata;
    AstTreeCFunction *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->library =
        copyAstTreeBack(metadata->library, oldVariables, newVariables,
                        variables_size, safetyCheck);
    new_metadata->name =
        copyAstTreeBack(metadata->name, oldVariables, newVariables,
                        variables_size, safetyCheck);
    new_metadata->funcType =
        copyAstTreeBack(metadata->funcType, oldVariables, newVariables,
                        variables_size, safetyCheck);

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_VALUE_MACRO: {
    AstTreeMacro *metadata = tree->metadata;
    AstTreeMacro *new_metadata = a404m_malloc(sizeof(*new_metadata));

    new_metadata->function = copyAstTreeFunction(
        metadata->function, oldVariables, newVariables, variables_size, true);

    return newAstTree(tree->token, new_metadata,
                      copyAstTreeBack(tree->type, oldVariables, newVariables,
                                      variables_size, safetyCheck),
                      tree->str_begin, tree->str_end);
  }
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token %d", tree->token);
  UNREACHABLE;
  return NULL;
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
                                      size_t variables_size, bool safetyCheck) {
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
                        new_newVariables, new_variables_size, safetyCheck);
    if (variables.data[i]->value != NULL) {
      result.data[i]->value =
          copyAstTreeBack(variables.data[i]->value, new_oldVariables,
                          new_newVariables, new_variables_size, safetyCheck);
    } else {
      result.data[i]->value = NULL;
    }
    if (variables.data[i]->initValue != NULL) {
      result.data[i]->initValue =
          copyAstTreeBack(variables.data[i]->initValue, new_oldVariables,
                          new_newVariables, new_variables_size, safetyCheck);
    } else {
      result.data[i]->initValue = NULL;
    }
  }

  return result;
}

AstTreeFunction *copyAstTreeFunction(AstTreeFunction *metadata,
                                     AstTreeVariables oldVariables[],
                                     AstTreeVariables newVariables[],
                                     size_t variables_size, bool safetyCheck) {
  AstTreeFunction *new_metadata = a404m_malloc(sizeof(*new_metadata));

  new_metadata->arguments =
      copyAstTreeVariables(metadata->arguments, oldVariables, newVariables,
                           variables_size, safetyCheck);
  new_metadata->isMacro = metadata->isMacro;

  const size_t new_variables_size = variables_size + 2;
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
      copyAstTreeBack(metadata->returnType, new_oldVariables, new_newVariables,
                      new_variables_size, safetyCheck);

  new_metadata->scope.variables =
      copyAstTreeVariables(metadata->scope.variables, new_oldVariables,
                           new_newVariables, new_variables_size, safetyCheck);
  new_metadata->scope.expressions_size = metadata->scope.expressions_size;
  new_metadata->scope.expressions =
      a404m_malloc(new_metadata->scope.expressions_size *
                   sizeof(*new_metadata->scope.expressions));

  new_metadata->scope.stackAllocation_size = 0;
  new_metadata->scope.stackAllocation = a404m_malloc(0);

  new_oldVariables[new_variables_size - 1] = metadata->scope.variables;
  new_newVariables[new_variables_size - 1] = new_metadata->scope.variables;

  for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
    new_metadata->scope.expressions[i] =
        copyAstTreeBack(metadata->scope.expressions[i], new_oldVariables,
                        new_newVariables, new_variables_size, safetyCheck);
  }
  return new_metadata;
}

AstTreeFunctionCall *copyAstTreeFunctionCall(AstTreeFunctionCall *metadata,
                                             AstTreeVariables oldVariables[],
                                             AstTreeVariables newVariables[],
                                             size_t variables_size,
                                             bool safetyCheck) {
  AstTreeFunctionCall *new_metadata = a404m_malloc(sizeof(*new_metadata));
  new_metadata->function =
      copyAstTreeBack(metadata->function, oldVariables, newVariables,
                      variables_size, safetyCheck);

  new_metadata->parameters.size = metadata->parameters.size;
  new_metadata->parameters.data = a404m_malloc(
      metadata->parameters.size * sizeof(*new_metadata->parameters.data));
  for (size_t i = 0; i < metadata->parameters.size; ++i) {
    new_metadata->parameters.data[i].nameBegin =
        metadata->parameters.data[i].nameBegin;
    new_metadata->parameters.data[i].nameEnd =
        metadata->parameters.data[i].nameEnd;
    new_metadata->parameters.data[i].value =
        copyAstTreeBack(metadata->parameters.data[i].value, oldVariables,
                        newVariables, variables_size, safetyCheck);
  }

  return new_metadata;
}

AstTreeRoots makeAstTree(const char *filePath
#ifdef PRINT_STATISTICS
                         ,
                         Time *lexingTime, Time *parsingTime
#endif
) {
  AstTreeRoots roots = {
      .data = a404m_malloc(0 * sizeof(*roots.data)),
      .size = 0,
  };

  if (getAstTreeRoot(strClone(filePath), &roots
#ifdef PRINT_STATISTICS
                     ,
                     lexingTime, parsingTime
#endif
                     ) == NULL) {
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
                            Time *lexingTime, Time *parsingTime
#endif
) {
  for (size_t i = 0; i < roots->size; ++i) {
    if (strEquals(roots->data[i]->filePath, filePath)) {
      free(filePath);
      return roots->data[i];
    }
  }

#ifdef PRINT_STATISTICS
  Time start = get_time();
#endif

  ParserNode *parserNode = parserFromPath(filePath
#ifdef PRINT_STATISTICS
                                          ,
                                          lexingTime
#endif
  );
  if (parserNode == NULL) {
    goto RETURN_ERROR;
  }

#ifdef PRINT_STATISTICS
  Time end = get_time();
  *parsingTime = time_add(*parsingTime, time_diff(end, start));
#endif
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
    if (!astTreeDoImport(roots, root, tree, NULL
#ifdef PRINT_STATISTICS
                         ,
                         lexingTime, parsingTime
#endif
                         )) {
      goto RETURN_ERROR;
    }
  }

  for (size_t i = 0; i < root->variables.size; ++i) {
    AstTreeVariable *variable = root->variables.data[i];
    if (!variable->isConst) {
      continue;
    }
    AstTree *tree = variable->value;
    if (!astTreeDoImport(roots, root, tree, variable
#ifdef PRINT_STATISTICS
                         ,
                         lexingTime, parsingTime
#endif
                         )) {
      goto RETURN_ERROR;
    }
  }

  return root;

RETURN_ERROR:
  return NULL;
}

bool astTreeDoImport(AstTreeRoots *roots, AstTreeRoot *root, AstTree *tree,
                     AstTreeVariable *variable
#ifdef PRINT_STATISTICS
                     ,
                     Time *lexingTime, Time *parsingTime
#endif
) {
  if (tree->token == AST_TREE_TOKEN_FUNCTION_CALL) {
    AstTreeFunctionCall *tree_metadata = tree->metadata;
    AstTree *operand = tree_metadata->function;
    if (operand->token == AST_TREE_TOKEN_BUILTIN_IMPORT) {
      AstTreeSetTypesHelper helper = {
          .lookingType = NULL,
          .dependencies.data = NULL,
          .dependencies.size = 0,
          .variables = &root->variables,
          .variables_size = 1,
          .root = root,
          .loops = NULL,
          .loops_size = 0,
          .scope = NULL,
          .isInScope = false,
      };
      if (!setAllTypes(tree, helper, NULL, NULL)) {
        return false;
      }
      AstTree *parameter = tree_metadata->parameters.data[0].value;
      if (!isConst(parameter)) {
        printError(parameter->str_begin, parameter->str_end, "Is not constant");
        return false;
      }
      parameter = getValue(parameter, true, helper.scope);
      if (parameter == NULL) {
        return false;
      }

      char *str = u8ArrayToCString(parameter);
      astTreeDelete(parameter);

      const size_t imports_size =
          a404m_malloc_usable_size(root->imports) / sizeof(*root->imports);
      if (imports_size == root->imports_size) {
        root->imports =
            a404m_realloc(root->imports, (imports_size + imports_size / 2 + 1) *
                                             sizeof(*root->imports));
      }

      AstTreeRoot *import =
          getAstTreeRoot(joinToPathOf(root->filePath, str), roots
#ifdef PRINT_STATISTICS
                         ,
                         lexingTime, parsingTime
#endif
          );
      free(str);

      if (import == NULL) {
        return false;
      }

      root->imports[root->imports_size].root = import;

      if (variable != NULL) {
        root->imports[root->imports_size].visible = false;
        AstTreeNamespace *value_metadata =
            a404m_malloc(sizeof(*value_metadata));
        value_metadata->importedIndex = root->imports_size;

        AstTree *oldValue = variable->value;

        variable->value =
            newAstTree(AST_TREE_TOKEN_VALUE_NAMESPACE, value_metadata,
                       copyAstTree(&AST_TREE_NAMESPACE_TYPE),
                       oldValue->str_begin, oldValue->str_end);
        astTreeDelete(oldValue);
      } else {
        root->imports[root->imports_size].visible = true;
      }
      root->imports_size += 1;
    }
  }
  return true;
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

      if (node_metadata->isComptime && !variable->isConst) {
        printError(node->str_begin, node->str_end, "Bad comptime %s",
                   PARSER_TOKEN_STRINGS[node->token]);
        goto RETURN_ERROR;
      }

      if (!pushVariable(&root->variables, variable)) {
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
      tree = astTreeParse(node);
      if (tree == NULL) {
        goto RETURN_ERROR;
      }
    } else if (node->token == PARSER_TOKEN_FUNCTION_CALL) {
      tree = astTreeParse(node);
      if (tree == NULL) {
        goto RETURN_ERROR;
      }

      AstTreeFunctionCall *tree_metadata = tree->metadata;
      AstTree *operand = tree_metadata->function;
      if (operand->token == AST_TREE_TOKEN_BUILTIN_IMPORT) {
        if (tree_metadata->parameters.size == 1) {
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
      case PARSER_TOKEN_OPERATOR_BITWISE_NOT:
      case PARSER_TOKEN_OPERATOR_BITWISE_AND:
      case PARSER_TOKEN_OPERATOR_BITWISE_XOR:
      case PARSER_TOKEN_OPERATOR_BITWISE_OR:
      case PARSER_TOKEN_OPERATOR_SHIFT_LEFT:
      case PARSER_TOKEN_OPERATOR_SHIFT_RIGHT:
      case PARSER_TOKEN_SYMBOL_PARENTHESIS:
      case PARSER_TOKEN_KEYWORD_IF:
      case PARSER_TOKEN_KEYWORD_WHILE:
      case PARSER_TOKEN_KEYWORD_COMPTIME:
      case PARSER_TOKEN_TYPE_TYPE:
      case PARSER_TOKEN_TYPE_ANY_TYPE:
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
      case PARSER_TOKEN_TYPE_NAMESPACE:
      case PARSER_TOKEN_TYPE_SHAPE_SHIFTER:
      case PARSER_TOKEN_TYPE_C_LIBRARY:
      case PARSER_TOKEN_TYPE_C_FUNCTION:
      case PARSER_TOKEN_TYPE_BOOL:
      case PARSER_TOKEN_OPERATOR_POINTER:
      case PARSER_TOKEN_OPERATOR_ADDRESS:
      case PARSER_TOKEN_OPERATOR_DEREFERENCE:
      case PARSER_TOKEN_KEYWORD_STRUCT:
      case PARSER_TOKEN_BUILTIN_CAST:
      case PARSER_TOKEN_BUILTIN_TYPE_OF:
      case PARSER_TOKEN_BUILTIN_SIZE_OF:
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
      case PARSER_TOKEN_BUILTIN_PUTC:
      case PARSER_TOKEN_BUILTIN_C_LIBRARY:
      case PARSER_TOKEN_BUILTIN_C_FUNCTION:
      case PARSER_TOKEN_BUILTIN_BITWISE_NOT:
      case PARSER_TOKEN_BUILTIN_BITWISE_AND:
      case PARSER_TOKEN_BUILTIN_BITWISE_XOR:
      case PARSER_TOKEN_BUILTIN_BITWISE_OR:
      case PARSER_TOKEN_BUILTIN_SHIFT_LEFT:
      case PARSER_TOKEN_BUILTIN_SHIFT_RIGHT:
      case PARSER_TOKEN_BUILTIN_INSERT:
      case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
      case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
        goto AFTER_SWITCH;
      case PARSER_TOKEN_ROOT:
      case PARSER_TOKEN_KEYWORD_RETURN:
      case PARSER_TOKEN_KEYWORD_BREAK:
      case PARSER_TOKEN_KEYWORD_CONTINUE:
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

      AstTree *value = astTreeParse(node_metadata->value);
      if (value == NULL) {
        goto RETURN_ERROR;
      }

      AstTree *type;
      if (node_metadata->type != NULL) {
        type = astTreeParse(node_metadata->type);
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

bool pushVariable(AstTreeVariables *variables, AstTreeVariable *variable) {
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

AstTree *astTreeParse(const ParserNode *parserNode) {
  switch (parserNode->token) {
  case PARSER_TOKEN_FUNCTION_DEFINITION:
    return astTreeParseFunction(parserNode);
  case PARSER_TOKEN_BUILTIN_CAST:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_CAST);
  case PARSER_TOKEN_BUILTIN_TYPE_OF:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_TYPE_OF);
  case PARSER_TOKEN_BUILTIN_SIZE_OF:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_SIZE_OF);
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
  case PARSER_TOKEN_BUILTIN_PUTC:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_PUTC);
  case PARSER_TOKEN_BUILTIN_C_LIBRARY:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_C_LIBRARY);
  case PARSER_TOKEN_BUILTIN_C_FUNCTION:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_C_FUNCTION);
  case PARSER_TOKEN_BUILTIN_BITWISE_NOT:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_BITWISE_NOT);
  case PARSER_TOKEN_BUILTIN_BITWISE_AND:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_BITWISE_AND);
  case PARSER_TOKEN_BUILTIN_BITWISE_XOR:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_BITWISE_XOR);
  case PARSER_TOKEN_BUILTIN_BITWISE_OR:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_BITWISE_OR);
  case PARSER_TOKEN_BUILTIN_SHIFT_LEFT:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT);
  case PARSER_TOKEN_BUILTIN_SHIFT_RIGHT:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT);
  case PARSER_TOKEN_BUILTIN_INSERT:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_BUILTIN_INSERT);
  case PARSER_TOKEN_TYPE_TYPE:
    return &AST_TREE_TYPE_TYPE;
  case PARSER_TOKEN_TYPE_ANY_TYPE:
    return &AST_TREE_TYPE_ANY_TYPE;
  case PARSER_TOKEN_TYPE_FUNCTION:
    return astTreeParseTypeFunction(parserNode);
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
  case PARSER_TOKEN_TYPE_NAMESPACE:
    return &AST_TREE_NAMESPACE_TYPE;
  case PARSER_TOKEN_TYPE_SHAPE_SHIFTER:
    return &AST_TREE_SHAPE_SHIFTER_TYPE;
  case PARSER_TOKEN_TYPE_C_LIBRARY:
    return &AST_TREE_C_LIBRARY_TYPE;
  case PARSER_TOKEN_TYPE_C_FUNCTION:
    NOT_IMPLEMENTED;
  case PARSER_TOKEN_TYPE_BOOL:
    return &AST_TREE_BOOL_TYPE;
  case PARSER_TOKEN_FUNCTION_CALL:
    return astTreeParseFunctionCall(parserNode);
  case PARSER_TOKEN_IDENTIFIER:
    return astTreeParseIdentifier(parserNode);
  case PARSER_TOKEN_VALUE_INT:
    return astTreeParseIntValue(parserNode);
  case PARSER_TOKEN_VALUE_FLOAT:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_FLOAT,
                             sizeof(AstTreeFloat), NULL);
  case PARSER_TOKEN_VALUE_BOOL:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_VALUE_BOOL,
                             sizeof(AstTreeBool), NULL);
  case PARSER_TOKEN_VALUE_CHAR:
    return astTreeParseValue(parserNode, AST_TREE_TOKEN_RAW_VALUE, sizeof(u8),
                             &AST_TREE_U8_TYPE);
  case PARSER_TOKEN_VALUE_STRING:
    return astTreeParseString(parserNode);
  case PARSER_TOKEN_KEYWORD_NULL:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_VALUE_NULL);
  case PARSER_TOKEN_KEYWORD_UNDEFINED:
    return astTreeParseKeyword(parserNode, AST_TREE_TOKEN_VALUE_UNDEFINED);
  case PARSER_TOKEN_KEYWORD_BREAK:
    return astTreeParseLoopControl(parserNode, AST_TREE_TOKEN_KEYWORD_BREAK);
  case PARSER_TOKEN_KEYWORD_CONTINUE:
    return astTreeParseLoopControl(parserNode, AST_TREE_TOKEN_KEYWORD_CONTINUE);
  case PARSER_TOKEN_KEYWORD_RETURN:
    return astTreeParseReturn(parserNode);
  case PARSER_TOKEN_OPERATOR_ASSIGN:
    return astTreeParseAssignOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_ASSIGN);
  case PARSER_TOKEN_OPERATOR_SUM_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode,
                                             AST_TREE_TOKEN_OPERATOR_SUM);
  case PARSER_TOKEN_OPERATOR_SUB_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode,
                                             AST_TREE_TOKEN_OPERATOR_SUB);
  case PARSER_TOKEN_OPERATOR_MULTIPLY_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode,
                                             AST_TREE_TOKEN_OPERATOR_MULTIPLY);
  case PARSER_TOKEN_OPERATOR_DIVIDE_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode,
                                             AST_TREE_TOKEN_OPERATOR_DIVIDE);
  case PARSER_TOKEN_OPERATOR_MODULO_ASSIGN:
    return astTreeParseOperateAssignOperator(parserNode,
                                             AST_TREE_TOKEN_OPERATOR_MODULO);
  case PARSER_TOKEN_OPERATOR_SUM:
    return astTreeParseBinaryOperator(parserNode, AST_TREE_TOKEN_OPERATOR_SUM);
  case PARSER_TOKEN_OPERATOR_SUB:
    return astTreeParseBinaryOperator(parserNode, AST_TREE_TOKEN_OPERATOR_SUB);
  case PARSER_TOKEN_OPERATOR_MULTIPLY:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_MULTIPLY);
  case PARSER_TOKEN_OPERATOR_DIVIDE:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_DIVIDE);
  case PARSER_TOKEN_OPERATOR_MODULO:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_MODULO);
  case PARSER_TOKEN_OPERATOR_EQUAL:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_EQUAL);
  case PARSER_TOKEN_OPERATOR_NOT_EQUAL:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_NOT_EQUAL);
  case PARSER_TOKEN_OPERATOR_GREATER:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_GREATER);
  case PARSER_TOKEN_OPERATOR_SMALLER:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_SMALLER);
  case PARSER_TOKEN_OPERATOR_GREATER_OR_EQUAL:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL);
  case PARSER_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL);
  case PARSER_TOKEN_OPERATOR_ACCESS:
    return astTreeParseAccessOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_ACCESS);
  case PARSER_TOKEN_OPERATOR_LOGICAL_AND:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_LOGICAL_AND);
  case PARSER_TOKEN_OPERATOR_LOGICAL_OR:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_LOGICAL_OR);
  case PARSER_TOKEN_OPERATOR_LOGICAL_NOT:
    return astTreeParseUnaryOperator(parserNode,
                                     AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT);
  case PARSER_TOKEN_OPERATOR_BITWISE_AND:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_BITWISE_AND);
  case PARSER_TOKEN_OPERATOR_BITWISE_XOR:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_BITWISE_XOR);
  case PARSER_TOKEN_OPERATOR_BITWISE_OR:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_BITWISE_OR);
  case PARSER_TOKEN_OPERATOR_BITWISE_NOT:
    return astTreeParseUnaryOperator(parserNode,
                                     AST_TREE_TOKEN_OPERATOR_BITWISE_NOT);
  case PARSER_TOKEN_OPERATOR_SHIFT_LEFT:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT);
  case PARSER_TOKEN_OPERATOR_SHIFT_RIGHT:
    return astTreeParseBinaryOperator(parserNode,
                                      AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT);
  case PARSER_TOKEN_OPERATOR_PLUS:
    return astTreeParseUnaryOperator(parserNode, AST_TREE_TOKEN_OPERATOR_PLUS);
  case PARSER_TOKEN_OPERATOR_MINUS:
    return astTreeParseUnaryOperator(parserNode, AST_TREE_TOKEN_OPERATOR_MINUS);
  case PARSER_TOKEN_OPERATOR_POINTER:
    return astTreeParseUnaryOperatorSingleChild(
        parserNode, AST_TREE_TOKEN_OPERATOR_POINTER);
  case PARSER_TOKEN_OPERATOR_ADDRESS:
    return astTreeParseAddressOperator(parserNode);
  case PARSER_TOKEN_OPERATOR_DEREFERENCE:
    return astTreeParseUnaryOperatorSingleChild(
        parserNode, AST_TREE_TOKEN_OPERATOR_DEREFERENCE);
  case PARSER_TOKEN_KEYWORD_IF:
    return astTreeParseIf(parserNode);
  case PARSER_TOKEN_KEYWORD_WHILE:
    return astTreeParseWhile(parserNode);
  case PARSER_TOKEN_KEYWORD_COMPTIME:
    return astTreeParseComptime(parserNode);
  case PARSER_TOKEN_SYMBOL_EOL:
    return astTreeParse((ParserNodeSingleChildMetadata *)parserNode->metadata);
  case PARSER_TOKEN_SYMBOL_CURLY_BRACKET:
    return astTreeParseCurlyBracket(parserNode);
  case PARSER_TOKEN_SYMBOL_PARENTHESIS:
    return astTreeParseParenthesis(parserNode);
  case PARSER_TOKEN_KEYWORD_STRUCT:
    return astTreeParseStruct(parserNode);
  case PARSER_TOKEN_SYMBOL_BRACKET_LEFT:
    return astTreeParseBracket(parserNode, AST_TREE_TOKEN_TYPE_ARRAY);
  case PARSER_TOKEN_SYMBOL_BRACKET_RIGHT:
    return astTreeParseArrayAccessOperator(parserNode);
  case PARSER_TOKEN_VARIABLE:
  case PARSER_TOKEN_CONSTANT:
  case PARSER_TOKEN_SYMBOL_COMMA:
  case PARSER_TOKEN_NONE:
  case PARSER_TOKEN_ROOT:
  }
  printError(parserNode->str_begin, parserNode->str_end, "Bad token %d",
             parserNode->token);
  return NULL;
}

AstTree *astTreeParseFunction(const ParserNode *parserNode) {
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
      .stackAllocation = a404m_malloc(0),
      .stackAllocation_size = 0,
  };

  AstTreeFunction *function = a404m_malloc(sizeof(*function));

  function->arguments.data = a404m_malloc(0);
  function->arguments.size = 0;
  function->isMacro = node_metadata->isMacro;

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

    AstTree *type = astTreeParse(arg_metadata->type);
    if (type == NULL) {
      goto RETURN_ERROR;
    }

    AstTreeVariable *argument = a404m_malloc(sizeof(*argument));
    argument->value = NULL;
    argument->initValue = NULL;
    argument->type = type;
    argument->name_begin = arg_metadata->name->str_begin;
    argument->name_end = arg_metadata->name->str_end;
    argument->isConst = arg_metadata->isComptime;

    if (!pushVariable(&function->arguments, argument)) {
      astTreeVariableDelete(argument);
      goto RETURN_ERROR;
    }
  }

  if ((function->returnType = astTreeParse(node_metadata->returnType)) ==
      NULL) {
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
    case PARSER_TOKEN_TYPE_ANY_TYPE:
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
    case PARSER_TOKEN_TYPE_NAMESPACE:
    case PARSER_TOKEN_TYPE_SHAPE_SHIFTER:
    case PARSER_TOKEN_TYPE_C_LIBRARY:
    case PARSER_TOKEN_TYPE_C_FUNCTION:
    case PARSER_TOKEN_TYPE_BOOL:
    case PARSER_TOKEN_KEYWORD_RETURN:
    case PARSER_TOKEN_KEYWORD_BREAK:
    case PARSER_TOKEN_KEYWORD_CONTINUE:
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
    case PARSER_TOKEN_OPERATOR_BITWISE_NOT:
    case PARSER_TOKEN_OPERATOR_BITWISE_AND:
    case PARSER_TOKEN_OPERATOR_BITWISE_XOR:
    case PARSER_TOKEN_OPERATOR_BITWISE_OR:
    case PARSER_TOKEN_OPERATOR_SHIFT_LEFT:
    case PARSER_TOKEN_OPERATOR_SHIFT_RIGHT:
    case PARSER_TOKEN_BUILTIN_CAST:
    case PARSER_TOKEN_BUILTIN_TYPE_OF:
    case PARSER_TOKEN_BUILTIN_SIZE_OF:
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
    case PARSER_TOKEN_BUILTIN_PUTC:
    case PARSER_TOKEN_BUILTIN_C_LIBRARY:
    case PARSER_TOKEN_BUILTIN_C_FUNCTION:
    case PARSER_TOKEN_BUILTIN_BITWISE_NOT:
    case PARSER_TOKEN_BUILTIN_BITWISE_AND:
    case PARSER_TOKEN_BUILTIN_BITWISE_XOR:
    case PARSER_TOKEN_BUILTIN_BITWISE_OR:
    case PARSER_TOKEN_BUILTIN_SHIFT_LEFT:
    case PARSER_TOKEN_BUILTIN_SHIFT_RIGHT:
    case PARSER_TOKEN_BUILTIN_INSERT:
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
      if (!astTreeParseConstant(node, &scope.variables)) {
        goto RETURN_ERROR;
      }
    } else {
      AstTree *tree;
      if (node->token == PARSER_TOKEN_VARIABLE) {
        tree = astTreeParseVariable(node, &scope.variables);
      } else {
        tree = astTreeParse(node);
      }

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

AstTree *astTreeParseTypeFunction(const ParserNode *parserNode) {
  ParserNodeTypeFunctionMetadata *metadata = parserNode->metadata;
  const ParserNodeArray *node_arguments = metadata->arguments->metadata;

  AstTreeTypeFunction *typeFunction = a404m_malloc(sizeof(*typeFunction));

  size_t arguments_size = 0;
  typeFunction->arguments =
      a404m_malloc(arguments_size * sizeof(*typeFunction->arguments));
  typeFunction->arguments_size = 0;
  typeFunction->isMacro = metadata->isMacro;

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

      argument.type = astTreeParse(variable_metadata->type);
      if (variable_metadata->value != NULL) {
        printError(node_argument->str_begin, node_argument->str_end,
                   "Cannot have value in function type");
        return NULL;
      }
      if (argument.type == NULL) {
        return NULL;
      }
      argument.isComptime = variable_metadata->isComptime;
    } else {
      argument.type = astTreeParse(node_argument);
      if (argument.type == NULL) {
        return NULL;
      }
      argument.name_begin = argument.name_end = NULL;
      argument.isComptime = false;
    }

    /*
if (!typeIsEqual(argument.type->type, &AST_TREE_TYPE_TYPE)) {
 printError(argument.str_begin, argument.str_end, "Type is incorrenct");
 return NULL;
}
    */

    if (typeFunction->arguments_size == arguments_size) {
      arguments_size += arguments_size / 2 + 1;
      typeFunction->arguments =
          a404m_realloc(typeFunction->arguments,
                        arguments_size * sizeof(*typeFunction->arguments));
    }

    typeFunction->arguments[typeFunction->arguments_size] = argument;
    typeFunction->arguments_size += 1;
  }

  if ((typeFunction->returnType = astTreeParse(metadata->returnType)) == NULL) {
    goto RETURN_ERROR;
  }

  return newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, typeFunction, NULL,
                    parserNode->str_begin, parserNode->str_end);

RETURN_ERROR:
  return NULL;
}

AstTree *astTreeParseFunctionCall(const ParserNode *parserNode) {
  ParserNodeFunctionCall *node_metadata = parserNode->metadata;
  AstTree *function = astTreeParse(node_metadata->function);
  if (function == NULL) {
    return NULL;
  }

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));
  metadata->function = function;

  metadata->parameters.data = a404m_malloc(sizeof(*metadata->parameters.data) *
                                           node_metadata->params->size);
  metadata->parameters.size = node_metadata->params->size;

  for (size_t i = 0; i < metadata->parameters.size; ++i) {
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
        param.value = astTreeParse(assign->right);
        goto PUSH_PARAM;
      }
    }
    param.nameBegin = param.nameEnd = NULL;
    param.value = astTreeParse(node_param);
  PUSH_PARAM:
    metadata->parameters.data[i] = param;
  }

  return newAstTree(AST_TREE_TOKEN_FUNCTION_CALL, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseIdentifier(const ParserNode *parserNode) {
  return newAstTree(AST_TREE_TOKEN_VARIABLE, NULL, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseValue(const ParserNode *parserNode, AstTreeToken token,
                           size_t metadata_size, AstTree *type) {
  void *metadata = a404m_malloc(metadata_size);
  memcpy(metadata, parserNode->metadata, metadata_size);

  return newAstTree(token, metadata, type, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseIntValue(const ParserNode *parserNode) {
  ParserNodeIntMetadata *node_metadata = parserNode->metadata;
  switch (node_metadata->type) {
  case PARSER_NODE_INT_TYPE_UNKNOWN: {
    AstTreeInt *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_VALUE_INT, metadata, NULL,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_I8: {
    i8 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_I8_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_U8: {
    u8 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_U8_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_I16: {
    i16 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_I16_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_U16: {
    u16 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_U16_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_I32: {
    i32 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_I32_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_U32: {
    u32 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_U32_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_I64: {
    i64 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_I64_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  case PARSER_NODE_INT_TYPE_U64: {
    u64 *metadata = a404m_malloc(sizeof(*metadata));
    *metadata = node_metadata->value;
    return newAstTree(AST_TREE_TOKEN_RAW_VALUE, metadata, &AST_TREE_U64_TYPE,
                      parserNode->str_begin, parserNode->str_end);
  }
  }
  UNREACHABLE;
  return NULL;
}

AstTree *astTreeParseString(const ParserNode *parserNode) {
  ParserNodeStringMetadata *node_metadata = parserNode->metadata;

  const size_t size = node_metadata->end - node_metadata->begin;
  AstTreeObject *metadata = a404m_malloc(sizeof(*metadata));
  metadata->items_size = size;
  metadata->items =
      a404m_malloc(metadata->items_size * sizeof(*metadata->items));

  for (size_t i = 0; i < size; ++i) {
    u8 *value = a404m_malloc(sizeof(*value));
    *value = node_metadata->begin[i];
    metadata->items[i].value = newAstTree(AST_TREE_TOKEN_RAW_VALUE, value,
                                          &AST_TREE_U8_TYPE, NULL, NULL);
  }

  AstTreeBracket *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->operand = &AST_TREE_U8_TYPE;

  type_metadata->parameters.size = 1;
  type_metadata->parameters.data = a404m_malloc(
      type_metadata->parameters.size * sizeof(*type_metadata->parameters.data));

  u64 *parameter_metadata = a404m_malloc(sizeof(*parameter_metadata));
  *parameter_metadata = size;

  type_metadata->parameters.data[0] =
      newAstTree(AST_TREE_TOKEN_RAW_VALUE, parameter_metadata,
                 &AST_TREE_U64_TYPE, NULL, NULL);

  return newAstTree(AST_TREE_TOKEN_VALUE_OBJECT, metadata,
                    newAstTree(AST_TREE_TOKEN_TYPE_ARRAY, type_metadata,
                               &AST_TREE_TYPE_TYPE, NULL, NULL),
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseKeyword(const ParserNode *parserNode, AstTreeToken token) {
  return newAstTree(token, NULL, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseLoopControl(const ParserNode *parserNode,
                                 AstTreeToken token) {
  AstTreeLoopControl *meatadata = a404m_malloc(sizeof(*meatadata));
  return newAstTree(token, meatadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseReturn(const ParserNode *parserNode) {
  ParserNodeReturnMetadata *node_metadata = parserNode->metadata;

  AstTree *value;
  if (node_metadata->value == NULL) {
    value = NULL;
  } else {
    value = astTreeParse(node_metadata->value);
    if (value == NULL) {
      return NULL;
    }
  }

  AstTreeReturn *metadata = a404m_malloc(sizeof(*metadata));
  metadata->value = value;

  return newAstTree(AST_TREE_TOKEN_KEYWORD_RETURN, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseAssignOperator(const ParserNode *parserNode,
                                    AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *left = astTreeParse(node_metadata->left);
  if (left == NULL) {
    return NULL;
  }
  AstTree *right = astTreeParse(node_metadata->right);
  if (right == NULL) {
    return NULL;
  }
  if (left->token == AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS) {
    left->token = AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN;
    AstTreeFunctionCall *metadata = left->metadata;

    metadata->parameters.data = a404m_realloc(
        metadata->parameters.data,
        (metadata->parameters.size + 1) * sizeof(*metadata->parameters.data));

    metadata->parameters.data[metadata->parameters.size].value = right;
    metadata->parameters.data[metadata->parameters.size].nameBegin = NULL;
    metadata->parameters.data[metadata->parameters.size].nameEnd = NULL;

    metadata->parameters.size += 1;

    return left;
  } else {
    AstTreePureInfix *metadata = a404m_malloc(sizeof(*metadata));

    metadata->left = left;
    metadata->right = right;

    return newAstTree(token, metadata, NULL, parserNode->str_begin,
                      parserNode->str_end);
  }
}

AstTree *astTreeParseBinaryOperator(const ParserNode *parserNode,
                                    AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *left = astTreeParse(node_metadata->left);
  if (left == NULL) {
    return NULL;
  }
  AstTree *right = astTreeParse(node_metadata->right);
  if (right == NULL) {
    astTreeDelete(left);
    return NULL;
  }

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));

  metadata->parameters.size = 2;
  metadata->parameters.data = a404m_malloc(metadata->parameters.size *
                                           sizeof(*metadata->parameters.data));
  metadata->parameters.data[0] = (AstTreeFunctionCallParam){
      .value = left,
      .nameBegin = NULL,
      .nameEnd = NULL,
  };
  metadata->parameters.data[1] = (AstTreeFunctionCallParam){
      .value = right,
      .nameBegin = NULL,
      .nameEnd = NULL,
  };
  metadata->function = NULL;

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseUnaryOperator(const ParserNode *parserNode,
                                   AstTreeToken token) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));
  metadata->parameters.size = 1;
  metadata->parameters.data = a404m_malloc(metadata->parameters.size *
                                           sizeof(*metadata->parameters.data));
  AstTree *operand = astTreeParse(node_metadata);

  if (operand == NULL) {
    free(metadata->parameters.data);
    free(metadata);
    return NULL;
  }

  metadata->parameters.data[0] = (AstTreeFunctionCallParam){
      .value = operand,
      .nameBegin = NULL,
      .nameEnd = NULL,
  };
  metadata->function = NULL;

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseUnaryOperatorSingleChild(const ParserNode *parserNode,
                                              AstTreeToken token) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeSingleChild *metadata = astTreeParse(node_metadata);
  if (metadata == NULL) {
    return NULL;
  }

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseAddressOperator(const ParserNode *parserNode) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeSingleChild *metadata = astTreeParse(node_metadata);
  if (metadata == NULL) {
    return NULL;
  }

  if (metadata->token == AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS) {
    metadata->token = AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS;
    return metadata;
  }

  return newAstTree(AST_TREE_TOKEN_OPERATOR_ADDRESS, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseOperateAssignOperator(const ParserNode *parserNode,
                                           AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *left = astTreeParse(node_metadata->left);
  if (left == NULL) {
    return NULL;
  }
  AstTree *right = astTreeParse(node_metadata->right);
  if (right == NULL) {
    astTreeDelete(left);
    return NULL;
  }

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));

  metadata->parameters.size = 2;
  metadata->parameters.data = a404m_malloc(metadata->parameters.size *
                                           sizeof(*metadata->parameters.data));
  metadata->parameters.data[0] = (AstTreeFunctionCallParam){
      .value = left,
      .nameBegin = NULL,
      .nameEnd = NULL,
  };
  metadata->parameters.data[1] = (AstTreeFunctionCallParam){
      .value = right,
      .nameBegin = NULL,
      .nameEnd = NULL,
  };
  metadata->function = NULL;

  AstTreePureInfix *assignMetadata = a404m_malloc(sizeof(*assignMetadata));

  AstTree *assignLeft = astTreeParse(node_metadata->left);
  AstTree *assignRight = newAstTree(token, metadata, NULL,
                                    parserNode->str_begin, parserNode->str_end);

  assignMetadata->left = assignLeft;
  assignMetadata->right = assignRight;

  return newAstTree(AST_TREE_TOKEN_OPERATOR_ASSIGN, assignMetadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

bool astTreeParseConstant(const ParserNode *parserNode,
                          AstTreeVariables *variables) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->value == NULL ||
      node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
    printError(parserNode->str_begin, parserNode->str_end, "Not supported");
    return NULL;
  }

  AstTree *value = astTreeParse(node_metadata->value);
  if (value == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *type;
  if (node_metadata->type == NULL) {
    type = NULL;
  } else {
    type = astTreeParse(node_metadata->type);
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

  if (!pushVariable(variables, variable)) {
    astTreeVariableDelete(variable);
    goto RETURN_ERROR;
  }

  return true;
RETURN_ERROR:
  return false;
}

AstTree *astTreeParseVariable(const ParserNode *parserNode,
                              AstTreeVariables *variables) {
  ParserNodeVariableMetadata *node_metadata = parserNode->metadata;

  if (node_metadata->isComptime) {
    printError(parserNode->str_begin, parserNode->str_end, "Bad comptime");
    return NULL;
  }

  if (node_metadata->value == NULL) {
    printError(parserNode->str_begin, parserNode->str_end,
               "Must have a value (or use undefined)");
    return NULL;
  } else if (node_metadata->name->token != PARSER_TOKEN_IDENTIFIER) {
    printError(parserNode->str_begin, parserNode->str_end, "Not supported");
    return NULL;
  }

  AstTree *value = astTreeParse(node_metadata->value);
  if (value == NULL) {
    goto RETURN_ERROR;
  }

  AstTree *type;
  if (node_metadata->type == NULL) {
    type = NULL;
  } else {
    type = astTreeParse(node_metadata->type);
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

  if (!pushVariable(variables, variable)) {
    astTreeVariableDelete(variable);
    goto RETURN_ERROR;
  }

  return newAstTree(AST_TREE_TOKEN_VARIABLE_DEFINE, variable, NULL,
                    parserNode->str_begin, parserNode->str_end);
RETURN_ERROR:
  return NULL;
}

AstTree *astTreeParseIf(const ParserNode *parserNode) {
  ParserNodeIfMetadata *node_metadata = parserNode->metadata;

  AstTree *condition = astTreeParse(node_metadata->condition);
  if (condition == NULL) {
    return NULL;
  }

  AstTree *ifBody = astTreeParse(node_metadata->ifBody);
  if (ifBody == NULL) {
    return NULL;
  }

  AstTree *elseBody;
  if (node_metadata->elseBody != NULL) {
    elseBody = astTreeParse(node_metadata->elseBody);
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

AstTree *astTreeParseWhile(const ParserNode *parserNode) {
  ParserNodeWhileMetadata *node_metadata = parserNode->metadata;

  AstTree *condition = astTreeParse(node_metadata->condition);
  if (condition == NULL) {
    return NULL;
  }

  AstTree *body = astTreeParse(node_metadata->body);
  if (body == NULL) {
    return NULL;
  }

  AstTreeWhile *metadata = a404m_malloc(sizeof(*metadata));
  metadata->condition = condition;
  metadata->body = body;

  return newAstTree(AST_TREE_TOKEN_KEYWORD_WHILE, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseComptime(const ParserNode *parserNode) {
  ParserNodeSingleChildMetadata *node_metadata = parserNode->metadata;

  AstTreeSingleChild *metadata =
      (AstTreeSingleChild *)astTreeParse((const ParserNode *)node_metadata);

  return newAstTree(AST_TREE_TOKEN_KEYWORD_COMPTIME, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseCurlyBracket(const ParserNode *parserNode) {
  const ParserNodeArray *body = parserNode->metadata;

  size_t expressions_size = 0;

  AstTreeScope *scope = a404m_malloc(sizeof(*scope));
  scope->variables.data = a404m_malloc(0);
  scope->variables.size = 0;
  scope->expressions = a404m_malloc(0);
  scope->expressions_size = 0;
  scope->stackAllocation = a404m_malloc(0);
  scope->stackAllocation_size = 0;

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
    case PARSER_TOKEN_TYPE_ANY_TYPE:
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
    case PARSER_TOKEN_TYPE_NAMESPACE:
    case PARSER_TOKEN_TYPE_SHAPE_SHIFTER:
    case PARSER_TOKEN_TYPE_C_LIBRARY:
    case PARSER_TOKEN_TYPE_C_FUNCTION:
    case PARSER_TOKEN_TYPE_BOOL:
    case PARSER_TOKEN_KEYWORD_RETURN:
    case PARSER_TOKEN_KEYWORD_BREAK:
    case PARSER_TOKEN_KEYWORD_CONTINUE:
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
    case PARSER_TOKEN_OPERATOR_BITWISE_NOT:
    case PARSER_TOKEN_OPERATOR_BITWISE_AND:
    case PARSER_TOKEN_OPERATOR_BITWISE_XOR:
    case PARSER_TOKEN_OPERATOR_BITWISE_OR:
    case PARSER_TOKEN_OPERATOR_SHIFT_LEFT:
    case PARSER_TOKEN_OPERATOR_SHIFT_RIGHT:
    case PARSER_TOKEN_BUILTIN_CAST:
    case PARSER_TOKEN_BUILTIN_TYPE_OF:
    case PARSER_TOKEN_BUILTIN_SIZE_OF:
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
    case PARSER_TOKEN_BUILTIN_PUTC:
    case PARSER_TOKEN_BUILTIN_C_LIBRARY:
    case PARSER_TOKEN_BUILTIN_C_FUNCTION:
    case PARSER_TOKEN_BUILTIN_BITWISE_NOT:
    case PARSER_TOKEN_BUILTIN_BITWISE_AND:
    case PARSER_TOKEN_BUILTIN_BITWISE_XOR:
    case PARSER_TOKEN_BUILTIN_BITWISE_OR:
    case PARSER_TOKEN_BUILTIN_SHIFT_LEFT:
    case PARSER_TOKEN_BUILTIN_SHIFT_RIGHT:
    case PARSER_TOKEN_BUILTIN_INSERT:
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
      if (!astTreeParseConstant(node, &scope->variables)) {
        goto RETURN_ERROR;
      }
    } else {
      AstTree *tree;
      if (node->token == PARSER_TOKEN_VARIABLE) {
        tree = astTreeParseVariable(node, &scope->variables);
      } else {
        tree = astTreeParse(node);
      }

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

AstTree *astTreeParseParenthesis(const ParserNode *parserNode) {
  const ParserNodeArray *metadata = parserNode->metadata;

  if (metadata->size != 1) {
    printError(parserNode->str_begin, parserNode->str_end, "Bad parenthesis");
    return NULL;
  } else {
    return astTreeParse(metadata->data[0]);
  }
}

AstTree *astTreeParseStruct(const ParserNode *parserNode) {
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
      variable->type = astTreeParse(node_variable->type);
    } else {
      variable->type = NULL;
    }

    if (node->token == PARSER_TOKEN_CONSTANT) {
      if (node_variable->value == NULL) {
        printError(node->str_begin, node->str_end, "Constants must have value");
        NOT_IMPLEMENTED;
      }
      variable->value = astTreeParse(node_variable->value);
      variable->initValue = NULL;
      variable->isConst = true;
    } else {
      if (node_variable->value != NULL) {
        printError(node->str_begin, node->str_end, "Can't have default values");
        NOT_IMPLEMENTED;
      } else if (node_variable->isComptime) {
        printError(node->str_begin, node->str_end, "Bad comptime");
        return NULL;
      }

      variable->value = NULL;
      variable->initValue = NULL;
      variable->isConst = false;
    }

    variables.data[i] = variable;
  }

  AstTreeStruct *metadata = a404m_malloc(sizeof(*metadata));
  metadata->variables = variables;
  metadata->id = (size_t)metadata; // TODO: change it

  return newAstTree(AST_TREE_TOKEN_KEYWORD_STRUCT, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

AstTree *astTreeParseAccessOperator(const ParserNode *parserNode,
                                    AstTreeToken token) {
  ParserNodeInfixMetadata *node_metadata = parserNode->metadata;

  AstTree *object = astTreeParse(node_metadata->left);
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

AstTree *astTreeParseBracket(const ParserNode *parserNode, AstTreeToken token) {
  ParserNodeBracketMetadata *node_metadata = parserNode->metadata;

  AstTreeBracket *metadata = a404m_malloc(sizeof(*metadata));

  metadata->operand = astTreeParse(node_metadata->operand);

  if (metadata->operand == NULL) {
    free(metadata);
    return NULL;
  }

  metadata->parameters.size = node_metadata->params->size;
  metadata->parameters.data = a404m_malloc(sizeof(*metadata->parameters.data) *
                                           metadata->parameters.size);

  for (size_t i = 0; i < node_metadata->params->size; ++i) {
    const ParserNode *node_param = node_metadata->params->data[i];

    if (node_param->token == PARSER_TOKEN_SYMBOL_COMMA) {
      node_param = (ParserNodeSingleChildMetadata *)node_param->metadata;
    }

    metadata->parameters.data[i] = astTreeParse(node_param);

    if (metadata->parameters.data[i] == NULL) {
      free(metadata);
      return NULL;
    }
  }

  return newAstTree(token, metadata, NULL, parserNode->str_begin,
                    parserNode->str_end);
}

AstTree *astTreeParseArrayAccessOperator(const ParserNode *parserNode) {
  ParserNodeBracketMetadata *node_metadata = parserNode->metadata;

  AstTreeFunctionCall *metadata = a404m_malloc(sizeof(*metadata));

  metadata->function = NULL;

  metadata->parameters.size = node_metadata->params->size + 1;
  metadata->parameters.data = a404m_malloc(metadata->parameters.size *
                                           sizeof(*metadata->parameters.data));

  AstTree *value = astTreeParse(node_metadata->operand);
  metadata->parameters.data[0].value =
      newAstTree(AST_TREE_TOKEN_OPERATOR_ADDRESS, value, NULL, value->str_begin,
                 value->str_end);
  metadata->parameters.data[0].nameBegin = NULL;
  metadata->parameters.data[0].nameEnd = NULL;

  if (metadata->parameters.data[0].value == NULL) {
    free(metadata->parameters.data);
    free(metadata);
    return NULL;
  }

  for (size_t i = 0; i < node_metadata->params->size; ++i) {
    const ParserNode *node_param = node_metadata->params->data[i];

    if (node_param->token == PARSER_TOKEN_SYMBOL_COMMA) {
      node_param = (ParserNodeSingleChildMetadata *)node_param->metadata;
    }

    metadata->parameters.data[i + 1].value = astTreeParse(node_param);
    metadata->parameters.data[i + 1].nameBegin = NULL;
    metadata->parameters.data[i + 1].nameEnd = NULL;
    if (metadata->parameters.data[i + 1].value == NULL) {
      return NULL;
    }
  }

  return newAstTree(AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS, metadata, NULL,
                    parserNode->str_begin, parserNode->str_end);
}

bool isFunction(AstTree *value) {
  return value->type->token == AST_TREE_TOKEN_TYPE_FUNCTION ||
         value->type->token == AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER ||
         value->type->token == AST_TREE_TOKEN_TYPE_C_FUNCTION ||
         value->type->token == AST_TREE_TOKEN_TYPE_MACRO;
}

bool isShapeShifter(AstTreeFunction *function) {
  for (size_t i = 0; i < function->arguments.size; ++i) {
    if (function->arguments.data[i]->isConst ||
        hasAnyTypeInside(function->arguments.data[i]->type)) {
      return true;
    }
  }
  return false;
}

bool hasAnyTypeInside(AstTree *type) {
  switch (type->token) {
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
    return true;
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
    return false;
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    AstTreeSingleChild *metadata = type->metadata;
    return hasAnyTypeInside(metadata);
  }
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    AstTreeBracket *metadata = type->metadata;
    return hasAnyTypeInside(metadata->operand);
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = type->metadata;
    for (size_t i = 0; i < metadata->variables.size; ++i) {
      if (hasAnyTypeInside(metadata->variables.data[i]->type)) {
        return true;
      }
    }
    return false;
  }
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_FUNCTION_CALL: {
    AstTreeFunctionCall *metadata = type->metadata;
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      if (hasAnyTypeInside(metadata->parameters.data[i].value)) {
        return true;
      }
    }
    return false;
  }
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
    return false;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
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
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Bad token %s", AST_TREE_TOKEN_STRINGS[type->token]);
  UNREACHABLE;
}

bool isConst(AstTree *tree) {
  if (tree->type == NULL) {
    UNREACHABLE;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
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
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT: {
    AstTreeFunctionCall *metadata = tree->metadata;

    if (metadata->function->token == AST_TREE_TOKEN_BUILTIN_TYPE_OF ||
        metadata->function->token == AST_TREE_TOKEN_BUILTIN_SIZE_OF) {
      return true;
    }

    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      if (!isConst(metadata->parameters.data[i].value)) {
        return false;
      }
    }
    return isConst(metadata->function);
  }
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT: {
    return true;
  }
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
    return false;
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *metadata = tree->metadata;
    return metadata->isConst && metadata->value != NULL;
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
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    AstTreeBracket *metadata = tree->metadata;
    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      if (!isConst(metadata->parameters.data[i])) {
        return false;
      }
    }
    return isConst(metadata->operand) && isConst(metadata->operand);
  }
  case AST_TREE_TOKEN_NONE:
  }
  printLog("Unknown token '%d'", tree->token);
  UNREACHABLE;
  return NULL;
}

bool isLeftValue(AstTree *tree) {
  switch (tree->token) {
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
    return true;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
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
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_SCOPE:
    return false;
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

AstTree *makeTypeOf(AstTree *value) {
  switch (value->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
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
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_OPERATOR_PLUS:
  case AST_TREE_TOKEN_OPERATOR_MINUS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_SUM:
  case AST_TREE_TOKEN_OPERATOR_SUB:
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
  case AST_TREE_TOKEN_OPERATOR_MODULO:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_GREATER:
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL: {
    AstTreeFunctionCall *metadata = value->metadata;
    AstTreeFunction *function = metadata->function->metadata;
    return copyAstTree(function->returnType);
  }
  case AST_TREE_TOKEN_FUNCTION: {
    AstTreeFunction *function = value->metadata;

    return makeTypeOfFunction(function, value->str_begin, value->str_end);
  }
  case AST_TREE_TOKEN_VALUE_VOID:
    return &AST_TREE_VOID_TYPE;
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
    return &AST_TREE_NAMESPACE_TYPE;
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
  case AST_TREE_TOKEN_KEYWORD_COMPTIME: {
    AstTreeSingleChild *metadata = value->metadata;
    return copyAstTree(metadata->type);
  }
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
    AstTreePureInfix *metadata = value->metadata;
    return copyAstTree(metadata->left->type);
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
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT: {
    AstTreeShapeShifterElement *metadata = value->metadata;

    if (metadata->shapeShifter->token != AST_TREE_TOKEN_VARIABLE) {
      UNREACHABLE;
    }
    AstTreeVariable *variable = metadata->shapeShifter->metadata;
    if (variable->value->token != AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER) {
      UNREACHABLE;
    }

    AstTreeShapeShifter *shapeShifter = variable->value->metadata;

    return makeTypeOfFunction(
        shapeShifter->generateds.functions[metadata->index], value->str_begin,
        value->str_end);
  }
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

AstTree *makeTypeOfFunction(AstTreeFunction *function, const char *str_begin,
                            const char *str_end) {
  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = function->arguments.size;
  type_metadata->arguments = a404m_malloc(function->arguments.size *
                                          sizeof(*type_metadata->arguments));
  type_metadata->returnType = copyAstTree(function->returnType);
  type_metadata->isMacro = function->isMacro;

  for (size_t i = 0; i < function->arguments.size; ++i) {
    AstTreeVariable *arg = function->arguments.data[i];
    type_metadata->arguments[i].name_begin = arg->name_begin;
    type_metadata->arguments[i].name_end = arg->name_end;
    type_metadata->arguments[i].str_begin = arg->name_begin;
    type_metadata->arguments[i].str_end = arg->name_end;
    type_metadata->arguments[i].type = copyAstTree(arg->type);
    type_metadata->arguments[i].isComptime = arg->isConst;
  }

  return newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                    &AST_TREE_TYPE_TYPE, str_begin, str_end);
}

bool typeIsEqual(AstTree *type0, AstTree *type1, AstTreeScope *scope) {
  AstTree *left = getValue(type0, true, scope);
  AstTree *right = getValue(type1, true, scope);
  if (left == NULL || right == NULL) {
    printLog("Can't check types");
    UNREACHABLE;
  }

  bool ret = typeIsEqualBack(left, right, scope);

  astTreeDelete(left);
  astTreeDelete(right);

  return ret;
}

bool typeIsEqualBack(const AstTree *type0, const AstTree *type1,
                     AstTreeScope *scope) {
  if (type0->token == AST_TREE_TOKEN_TYPE_ANY_TYPE ||
      type1->token == AST_TREE_TOKEN_TYPE_ANY_TYPE) {
    return true;
  }
  switch (type0->token) {
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
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
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
    return false;
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_MACRO:
    return type1->token == type0->token;
  case AST_TREE_TOKEN_TYPE_C_FUNCTION: {
    if (type1->token != type0->token) {
      return false;
    }
    AstTreeCFunctionType *type0_metadata = type0->metadata;
    AstTreeCFunctionType *type1_metadata = type1->metadata;

    return typeIsEqualBack(type0_metadata->funcType, type1_metadata->funcType,
                           scope);
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER: {
    if (type1->token != type0->token) {
      return false;
    }
    AstTreeSingleChild *type0_metadata = type0->metadata;
    AstTreeSingleChild *type1_metadata = type1->metadata;
    return typeIsEqual(type0_metadata, type1_metadata, scope);
  }
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    if (type1->token != type0->token) {
      return false;
    }

    AstTreeBracket *type0_metadata = type0->metadata;
    AstTreeBracket *type1_metadata = type1->metadata;

    if (!typeIsEqual(type0_metadata->operand, type1_metadata->operand, scope)) {
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
                   type1_metadata->parameters.data[i], scope)) {
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
                     type1_metadata->returnType->type, scope) ||
        type0_metadata->arguments_size != type1_metadata->arguments_size) {
      return false;
    }
    for (size_t i = 0; i < type0_metadata->arguments_size; ++i) {
      AstTreeTypeFunctionArgument p0 = type0_metadata->arguments[i];
      AstTreeTypeFunctionArgument p1 = type1_metadata->arguments[i];
      if (!typeIsEqual(p0.type, p1.type, scope) &&
          p0.isComptime == p1.isComptime) {
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
      return isEqualVariable(member0, member1, scope);
    }
    return true;
  }
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

AstTree *getValue(AstTree *tree, bool copy, AstTreeScope *scope) {
  if (!isConst(tree)) {
    printError(tree->str_begin, tree->str_end,
               "Can't get value at compile time because it is not const");
    return NULL;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
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
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT: {
    bool shouldRet = false;
    u32 breakCount = 0;
    bool shouldContinue = false;

    AstTree *value = runExpression(tree, scope, &shouldRet, false, true,
                                   &breakCount, &shouldContinue);

    if (!copy) {
      astTreeDelete(tree);
    }

    if (value == NULL) {
      printError(tree->str_begin, tree->str_end, "Unknown error");
    }
    return value;
  }
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER: {
    if (copy) {
      return copyAstTree(tree);
    } else {
      return tree;
    }
  }
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
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
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
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
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
    return false;
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isFloatType(AstTree *type) {
  switch (type->token) {
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
  case AST_TREE_TOKEN_TYPE_F32:
  case AST_TREE_TOKEN_TYPE_F64:
  case AST_TREE_TOKEN_TYPE_F128:
    return true;
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
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
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
    return false;
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isEqual(AstTree *left, AstTree *right, AstTreeScope *scope) {
  if (left->token != right->token) {
    return false;
  }
  switch (left->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_TYPE_MACRO:
    return true;
  case AST_TREE_TOKEN_TYPE_C_FUNCTION: {
    NOT_IMPLEMENTED;
  }
  case AST_TREE_TOKEN_VALUE_NAMESPACE: {
    AstTreeNamespace *left_metadata = left->metadata;
    AstTreeNamespace *right_metadata = right->metadata;

    return left_metadata->importedIndex == right_metadata->importedIndex;
  }
  case AST_TREE_TOKEN_VALUE_INT: {
    AstTreeInt *left_metadata = left->metadata;
    AstTreeInt *right_metadata = right->metadata;

    return *left_metadata == *right_metadata;
  }
  case AST_TREE_TOKEN_VARIABLE: {
    AstTreeVariable *left_metadata = left->metadata;
    AstTreeVariable *right_metadata = right->metadata;
    return isEqualVariable(left_metadata, right_metadata, scope);
  }
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED: {
    AstTreeRawValue *left_metadata = left->metadata;
    AstTreeRawValue *right_metadata = right->metadata;

    return memcmp(left_metadata, right_metadata, getSizeOfType(left->type)) ==
           0;
  }
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    AstTreeBracket *left_metadata = left->metadata;
    AstTreeBracket *right_metadata = right->metadata;

    if (left_metadata->parameters.size != right_metadata->parameters.size) {
      return false;
    }

    for (size_t i = 0; i < left_metadata->parameters.size; ++i) {
      if (!isEqual(left_metadata->parameters.data[i],
                   right_metadata->parameters.data[i], scope)) {
        return false;
      }
    }

    return isEqual(left_metadata->operand, right_metadata->operand, scope);
  }
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
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
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
    printLog("%s", AST_TREE_TOKEN_STRINGS[left->token]);
    NOT_IMPLEMENTED;
  case AST_TREE_TOKEN_NONE:
  }
  UNREACHABLE;
}

bool isEqualVariable(AstTreeVariable *left, AstTreeVariable *right,
                     AstTreeScope *scope) {
  if (!typeIsEqual(left->type, right->type, scope) ||
      left->isConst != right->isConst) {
    return false;
  } else if ((left->value == NULL && right->value != NULL) ||
             (left->value != NULL && right->value == NULL)) {
    return false;
  } else if (left->value != NULL &&
             !isEqual(left->value, right->value, scope)) {
    return false;
  } else if ((left->initValue == NULL && right->initValue != NULL) ||
             (left->initValue != NULL && right->initValue == NULL)) {
    return false;
  } else if (left->initValue != NULL &&
             !isEqual(left->initValue, right->initValue, scope)) {
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
    if (root->imports[i].visible) {
      allOfVariablesWithImport(variables, root->imports[i].root, checkedRoots);
    }
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
      .variables = &variables,
      .variables_size = 1,
      .root = root,
      .loops = NULL,
      .loops_size = 0,
      .scope = NULL,
      .isInScope = false,
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
    sizeof(STR_MINUS) / sizeof(*STR_MINUS) - sizeof(*STR_MINUS);
static const char STR_LOGICAL_NOT[] = "__logical_not__";
static const size_t STR_LOGICAL_NOT_SIZE =
    sizeof(STR_LOGICAL_NOT) / sizeof(*STR_LOGICAL_NOT) -
    sizeof(*STR_LOGICAL_NOT);
static const char STR_BITWISE_NOT[] = "__bitwise_not__";
static const size_t STR_BITWISE_NOT_SIZE =
    sizeof(STR_BITWISE_NOT) / sizeof(*STR_BITWISE_NOT) -
    sizeof(*STR_BITWISE_NOT);

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
static const char STR_BITWISE_AND[] = "__bitwise_and__";
static const size_t STR_BITWISE_AND_SIZE =
    sizeof(STR_BITWISE_AND) / sizeof(*STR_BITWISE_AND) -
    sizeof(*STR_BITWISE_AND);
static const char STR_BITWISE_XOR[] = "__bitwise_xor__";
static const size_t STR_BITWISE_XOR_SIZE =
    sizeof(STR_BITWISE_XOR) / sizeof(*STR_BITWISE_XOR) -
    sizeof(*STR_BITWISE_XOR);
static const char STR_BITWISE_OR[] = "__bitwise_or__";
static const size_t STR_BITWISE_OR_SIZE =
    sizeof(STR_BITWISE_OR) / sizeof(*STR_BITWISE_OR) - sizeof(*STR_BITWISE_OR);
static const char STR_SHIFT_LEFT[] = "__shift_left__";
static const size_t STR_SHIFT_LEFT_SIZE =
    sizeof(STR_SHIFT_LEFT) / sizeof(*STR_SHIFT_LEFT) - sizeof(*STR_SHIFT_LEFT);
static const char STR_SHIFT_RIGHT[] = "__shift_right__";
static const size_t STR_SHIFT_RIGHT_SIZE =
    sizeof(STR_SHIFT_RIGHT) / sizeof(*STR_SHIFT_RIGHT) -
    sizeof(*STR_SHIFT_RIGHT);
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
static const char STR_GET_ITEM[] = "__get_item__";
static const size_t STR_GET_ITEM_SIZE =
    sizeof(STR_GET_ITEM) / sizeof(*STR_GET_ITEM) - sizeof(*STR_GET_ITEM);
static const char STR_SET_ITEM[] = "__set_item__";
static const size_t STR_SET_ITEM_SIZE =
    sizeof(STR_SET_ITEM) / sizeof(*STR_SET_ITEM) - sizeof(*STR_SET_ITEM);
static const char STR_GET_ITEM_ADDRESS[] = "__get_item_address__";
static const size_t STR_GET_ITEM_ADDRESS_SIZE =
    sizeof(STR_GET_ITEM_ADDRESS) / sizeof(*STR_GET_ITEM_ADDRESS) -
    sizeof(*STR_GET_ITEM_ADDRESS);

bool setAllTypes(AstTree *tree, AstTreeSetTypesHelper helper,
                 AstTreeFunction *function, AstTreeFunctionCall *functionCall) {
  if (tree->type != NULL) {
    return true;
  }
  switch (tree->token) {
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE: // TODO: fix unwanted stuff later
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
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_VALUE_VOID:
    return true;
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
    NOT_IMPLEMENTED;
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
    NOT_IMPLEMENTED;
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
  case AST_TREE_TOKEN_KEYWORD_RETURN:
    return setTypesReturn(tree, helper, function);
  case AST_TREE_TOKEN_KEYWORD_BREAK:
    return setTypesBreak(tree, helper);
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
    return setTypesContinue(tree, helper);
  case AST_TREE_TOKEN_TYPE_FUNCTION:
    return setTypesTypeFunction(tree, helper);
  case AST_TREE_TOKEN_FUNCTION_CALL:
    return setTypesFunctionCall(tree, helper);
  case AST_TREE_TOKEN_VARIABLE:
    return setTypesVariable(tree, helper, functionCall);
  case AST_TREE_TOKEN_OPERATOR_ASSIGN:
    return setTypesOperatorAssign(tree, helper);
  case AST_TREE_TOKEN_OPERATOR_PLUS:
    return setTypesOperatorGeneral(tree, helper, STR_PLUS, STR_PLUS_SIZE);
  case AST_TREE_TOKEN_OPERATOR_MINUS:
    return setTypesOperatorGeneral(tree, helper, STR_MINUS, STR_MINUS_SIZE);
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
    return setTypesOperatorGeneral(tree, helper, STR_LOGICAL_NOT,
                                   STR_LOGICAL_NOT_SIZE);
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
    return setTypesOperatorGeneral(tree, helper, STR_BITWISE_NOT,
                                   STR_BITWISE_NOT_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SUM:
    return setTypesOperatorGeneral(tree, helper, STR_SUM, STR_SUM_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SUB:
    return setTypesOperatorGeneral(tree, helper, STR_SUB, STR_SUB_SIZE);
  case AST_TREE_TOKEN_OPERATOR_MULTIPLY:
    return setTypesOperatorGeneral(tree, helper, STR_MUL, STR_MUL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_DIVIDE:
    return setTypesOperatorGeneral(tree, helper, STR_DIV, STR_DIV_SIZE);
  case AST_TREE_TOKEN_OPERATOR_MODULO:
    return setTypesOperatorGeneral(tree, helper, STR_MOD, STR_MOD_SIZE);
  case AST_TREE_TOKEN_OPERATOR_EQUAL:
    return setTypesOperatorGeneral(tree, helper, STR_EQUAL, STR_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_NOT_EQUAL:
    return setTypesOperatorGeneral(tree, helper, STR_NOT_EQUAL,
                                   STR_NOT_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_GREATER:
    return setTypesOperatorGeneral(tree, helper, STR_GREATER, STR_GREATER_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SMALLER:
    return setTypesOperatorGeneral(tree, helper, STR_SMALLER, STR_SMALLER_SIZE);
  case AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL:
    return setTypesOperatorGeneral(tree, helper, STR_GREATER_OR_EQUAL,
                                   STR_GREATER_OR_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL:
    return setTypesOperatorGeneral(tree, helper, STR_SMALLER_OR_EQUAL,
                                   STR_SMALLER_OR_EQUAL_SIZE);
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
    return setTypesOperatorGeneral(tree, helper, STR_LOGICAL_AND,
                                   STR_LOGICAL_AND_SIZE);
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
    return setTypesOperatorGeneral(tree, helper, STR_LOGICAL_OR,
                                   STR_LOGICAL_OR_SIZE);
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
    return setTypesOperatorGeneral(tree, helper, STR_BITWISE_AND,
                                   STR_BITWISE_AND_SIZE);
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
    return setTypesOperatorGeneral(tree, helper, STR_BITWISE_XOR,
                                   STR_BITWISE_XOR_SIZE);
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
    return setTypesOperatorGeneral(tree, helper, STR_BITWISE_OR,
                                   STR_BITWISE_OR_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
    return setTypesOperatorGeneral(tree, helper, STR_SHIFT_LEFT,
                                   STR_SHIFT_LEFT_SIZE);
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
    return setTypesOperatorGeneral(tree, helper, STR_SHIFT_RIGHT,
                                   STR_SHIFT_RIGHT_SIZE);
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
    return setTypesOperatorGeneral(tree, helper, STR_GET_ITEM,
                                   STR_GET_ITEM_SIZE);
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
    return setTypesOperatorGeneral(tree, helper, STR_SET_ITEM,
                                   STR_SET_ITEM_SIZE);
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
    return setTypesOperatorGeneral(tree, helper, STR_GET_ITEM_ADDRESS,
                                   STR_GET_ITEM_ADDRESS_SIZE);
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
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
    return setTypesBuiltinSizeOf(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_IMPORT:
    return setTypesBuiltinImport(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_IS_COMPTIME:
    return setTypesBuiltinIsComptime(tree, helper);
  case AST_TREE_TOKEN_BUILTIN_STACK_ALLOC:
    return setTypesBuiltinStackAlloc(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC:
    return setTypesBuiltinHeapAlloc(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_NEG:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
    return setTypesBuiltinUnary(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_ADD:
  case AST_TREE_TOKEN_BUILTIN_SUB:
    return setTypesBuiltinBinaryAlsoPointer(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_MUL:
  case AST_TREE_TOKEN_BUILTIN_DIV:
  case AST_TREE_TOKEN_BUILTIN_MOD:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
    return setTypesBuiltinBinary(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_NOT_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_GREATER:
  case AST_TREE_TOKEN_BUILTIN_SMALLER:
  case AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL:
  case AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL:
    return setTypesBuiltinBinaryWithRet(tree, helper, functionCall,
                                        &AST_TREE_BOOL_TYPE);
  case AST_TREE_TOKEN_BUILTIN_PUTC:
    return setTypesBuiltinPutc(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
    return setTypesBuiltinCLibrary(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
    return setTypesBuiltinCFunction(tree, helper, functionCall);
  case AST_TREE_TOKEN_BUILTIN_INSERT:
    return setTypesBuiltinInsert(tree, helper, functionCall);
  case AST_TREE_TOKEN_TYPE_ARRAY:
    return setTypesTypeArray(tree, helper);
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
  case AST_TREE_TOKEN_NONE:
  }
  printError(tree->str_begin, tree->str_end, "Unknown token %d", tree->token);
  UNREACHABLE;
}

bool setTypesValueBool(AstTree *tree, AstTreeSetTypesHelper helper) {
  (void)helper;
  tree->type = &AST_TREE_BOOL_TYPE;
  tree->token = AST_TREE_TOKEN_RAW_VALUE;
  return true;
}

bool setTypesValueInt(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.lookingType == NULL ||
      typeIsEqual(helper.lookingType, &AST_TREE_I64_TYPE, helper.scope)) {
    tree->type = &AST_TREE_I64_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U64_TYPE,
                         helper.scope)) {
    tree->type = &AST_TREE_U64_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_I32_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    i32 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_I32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U32_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    u32 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_U32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_I16_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    i16 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_I16_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U16_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    u16 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_U16_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_I8_TYPE, helper.scope)) {
    AstTreeInt *value = tree->metadata;
    i8 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_I8_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_U8_TYPE, helper.scope)) {
    AstTreeInt *value = tree->metadata;
    u8 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_U8_TYPE;
#ifdef FLOAT_16_SUPPORT
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F16_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    f16 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_F16_TYPE;
#endif
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F32_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    f32 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_F32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F64_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    f64 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_F64_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F128_TYPE,
                         helper.scope)) {
    AstTreeInt *value = tree->metadata;
    f128 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_F128_TYPE;
  } else {
    UNREACHABLE;
  }

  tree->token = AST_TREE_TOKEN_RAW_VALUE;
  return true;
}

bool setTypesValueFloat(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.lookingType == NULL ||
      typeIsEqual(helper.lookingType, &AST_TREE_F64_TYPE, helper.scope)) {
    AstTreeFloat *value = tree->metadata;
    f64 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_F64_TYPE;
#ifdef FLOAT_16_SUPPORT
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F16_TYPE,
                         helper.scope)) {
    AstTreeFloat *value = tree->metadata;
    f16 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_F16_TYPE;
#endif
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F32_TYPE,
                         helper.scope)) {
    AstTreeFloat *value = tree->metadata;
    f32 *newValue = a404m_malloc(sizeof(*newValue));
    *newValue = *value;
    if (*value - *newValue != 0) {
      printWarning(tree->str_begin, tree->str_end, "Value is overflowing");
    }
    free(value);
    tree->metadata = newValue;
    tree->type = &AST_TREE_F32_TYPE;
  } else if (typeIsEqual(helper.lookingType, &AST_TREE_F128_TYPE,
                         helper.scope)) {
    tree->type = &AST_TREE_F128_TYPE;
  } else {
    UNREACHABLE;
  }

  tree->token = AST_TREE_TOKEN_RAW_VALUE;
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

  const size_t size = getSizeOfType(tree->type);
  tree->metadata = a404m_malloc(size);
  memset(tree->metadata, 0, size);

  tree->token = AST_TREE_TOKEN_RAW_VALUE;
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

  if (metadata->isMacro) {
    AstTreeMacro *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->function = metadata;
    tree->metadata = new_metadata;
    tree->token = AST_TREE_TOKEN_VALUE_MACRO;
    tree->type = copyAstTree(&AST_TREE_MACRO_TYPE);
    return true;
  } else if (isShapeShifter(metadata)) {
    AstTreeShapeShifter *new_metadata = a404m_malloc(sizeof(*new_metadata));
    new_metadata->function = metadata;
    new_metadata->generateds.size = 0;
    new_metadata->generateds.calls = a404m_malloc(0);
    new_metadata->generateds.functions = a404m_malloc(0);
    tree->metadata = new_metadata;
    tree->token = AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER;
    tree->type = copyAstTree(&AST_TREE_SHAPE_SHIFTER_TYPE);
    return true;
  }

  return setTypesAstFunction(metadata, tree, _helper);
}

bool setTypesReturn(AstTree *tree, AstTreeSetTypesHelper _helper,
                    AstTreeFunction *function) {
  if (function == NULL) {
    printError(tree->str_begin, tree->str_end, "Return can't be here");
    return false;
  }

  AstTreeReturn *metadata = tree->metadata;
  if (metadata->value != NULL) {
    AstTreeSetTypesHelper helper = {
        .lookingType = getValue(function->returnType, true, _helper.scope),
        .dependencies = _helper.dependencies,
        .variables = _helper.variables,
        .variables_size = _helper.variables_size,
        .root = _helper.root,
        .loops = _helper.loops,
        .loops_size = _helper.loops_size,
        .scope = _helper.scope,
        .isInScope = false,
    };
    if (helper.lookingType == NULL) {
      return false;
    }
    if (!setAllTypes(metadata->value, helper, NULL, NULL)) {
      astTreeDelete(helper.lookingType);
      return false;
    }
    astTreeDelete(helper.lookingType);
    if (!typeIsEqual(metadata->value->type, function->returnType,
                     helper.scope)) {
      printError(tree->str_begin, tree->str_end, "Type mismatch");
      return false;
    }
  }
  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesBreak(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.loops_size == 0) {
    printError(tree->str_begin, tree->str_end,
               "`break` can't be here, it needs to be inside a loop");
    return false;
  }

  AstTreeLoopControl *meatadata = tree->metadata;
  meatadata->count = 1;

  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesContinue(AstTree *tree, AstTreeSetTypesHelper helper) {
  if (helper.loops_size == 0) {
    printError(tree->str_begin, tree->str_end,
               "`continue` can't be here, it needs to be inside a loop");
    return false;
  }

  AstTreeLoopControl *meatadata = tree->metadata;
  meatadata->count = 1;

  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesTypeFunction(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeTypeFunction *metadata = tree->metadata;

  for (size_t i = 0; i < metadata->arguments_size; ++i) {
    AstTreeTypeFunctionArgument *arg = &metadata->arguments[i];
    if (!setAllTypes(arg->type, helper, NULL, NULL)) {
      return false;
    } else if (!typeIsEqual(arg->type->type, &AST_TREE_TYPE_TYPE,
                            helper.scope)) {
      printError(arg->str_begin, arg->str_end, "Expected a type");
      return false;
    }
    arg->type = getValue(arg->type, false, helper.scope);
  }

  if (!setAllTypes(metadata->returnType, helper, NULL, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->returnType->type, &AST_TREE_TYPE_TYPE,
                          helper.scope)) {
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
      .variables_size = _helper.variables_size,
      .root = _helper.root,
      .loops = _helper.loops,
      .loops_size = _helper.loops_size,
      .scope = _helper.scope,
      .isInScope = false,
  };

  for (size_t i = 0; i < metadata->parameters.size; ++i) {
    AstTreeFunctionCallParam param = metadata->parameters.data[i];
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

  if (metadata->function->type->token == AST_TREE_TOKEN_TYPE_FUNCTION) {
    AstTreeTypeFunction *function = metadata->function->type->metadata;
    if (function == NULL ||
        function->arguments_size != metadata->parameters.size) {
      printError(tree->str_begin, tree->str_end,
                 "Arguments doesn't match %ld != %ld", function->arguments_size,
                 metadata->parameters.size);
      return NULL;
    }

    if (!doesFunctionMatch(function, metadata, helper)) {
      printError(tree->str_begin, tree->str_end, "Function call doesn't match");
      return NULL;
    }

    tree->type = copyAstTree(function->returnType);
  } else if (metadata->function->type->token ==
             AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER) {
    AstTree *function = getShapeShifterElement(metadata, helper);
    if (function == NULL) {
      return false;
    }
    metadata->function = function;
    AstTreeTypeFunction *functionType = function->type->metadata;
    tree->type = copyAstTree(functionType->returnType);
  } else if (metadata->function->type->token ==
             AST_TREE_TOKEN_TYPE_C_FUNCTION) {
    AstTreeCFunctionType *cFunction = metadata->function->type->metadata;
    AstTreeTypeFunction *function = cFunction->funcType->metadata;
    if (function == NULL) {
      printError(tree->str_begin, tree->str_end, "Bad function");
      return NULL;
    }

    if (!doesFunctionMatch(function, metadata, helper)) {
      printError(tree->str_begin, tree->str_end, "Function call doesn't match");
      return NULL;
    }

    tree->type = copyAstTree(function->returnType);
  } else if (metadata->function->type->token == AST_TREE_TOKEN_TYPE_MACRO) {
    metadata->function = getValue(metadata->function, false, helper.scope);
    if (metadata->function->token != AST_TREE_TOKEN_VALUE_MACRO) {
      UNREACHABLE;
    }
    AstTreeMacro *macro = metadata->function->metadata;
    AstTreeFunction *function =
        copyAstTreeFunction(macro->function, NULL, NULL, 0, true);

    if (!setTypesAstFunction(function, NULL, helper)) {
      astTreeFunctionDestroy(*function);
      free(function);
      return false;
    }

    AstTree *functionType = makeTypeOfFunction(function, NULL, NULL);

    if (!doesFunctionMatch(functionType->metadata, metadata, helper)) {
      printError(tree->str_begin, tree->str_end, "Function call doesn't match");
      return NULL;
    }

    const char *str_begin = metadata->function->str_begin;
    const char *str_end = metadata->function->str_end;
    astTreeDelete(metadata->function);

    metadata->function = newAstTree(AST_TREE_TOKEN_FUNCTION, function,
                                    functionType, str_begin, str_end);

    tree->type = copyAstTree(function->returnType);
  } else {
    UNREACHABLE;
  }

  if (metadata->function->token == AST_TREE_TOKEN_BUILTIN_INSERT) {
    char *code = u8ArrayToCString(metadata->parameters.data[0].value);
    filePush("", code);
    LexerNodeArray lexerArray = lexer(code);
    if (lexerNodeArrayIsError(lexerArray)) {
      UNREACHABLE;
    }
    ParserNode *rootParser = parser(lexerArray);
    if (rootParser == NULL) {
      UNREACHABLE;
    }

    ParserNodeArray *nodeArray = rootParser->metadata;
    if (!_helper.isInScope) {
      if (nodeArray->size != 1) {
        UNREACHABLE;
      }
      AstTree *ast = astTreeParse(nodeArray->data[0]);
      if (ast == NULL || !setAllTypes(ast, _helper, NULL, NULL)) {
        return false;
      }
      astTreeDestroy(*tree);
      *tree = *ast;
      free(ast);
    } else {
      size_t parentIndex = _helper.scope->expressions_size;
      for (size_t i = 0; i < _helper.scope->expressions_size; ++i) {
        if (_helper.scope->expressions[i] == tree) {
          parentIndex = i;
          break;
        }
      }
      if (parentIndex == _helper.scope->expressions_size) {
        UNREACHABLE;
      }
      if (nodeArray->size == 0) {
        astTreeDestroy(*tree);
        *tree = AST_TREE_VOID_VALUE;
      } else {
        AstTree *astNodes[nodeArray->size];
        for (size_t i = 0; i < nodeArray->size; ++i) {
          ParserNode *node = nodeArray->data[i];
          while (node->token == PARSER_TOKEN_SYMBOL_EOL) {
            node = (ParserNodeSingleChildMetadata *)node->metadata;
          }
          AstTree *tree = astTreeParse(node);
          if (tree == NULL) {
            return false;
          }
          astNodes[i] = tree;
        }
        astTreeDestroy(*tree);
        *tree = *astNodes[0];
        free(astNodes[0]);
        if (!setAllTypes(tree, _helper, NULL, NULL)) {
          return false;
        }
        size_t expression_capacity =
            a404m_malloc_usable_size(_helper.scope->expressions) /
            sizeof(*_helper.scope->expressions);

        const size_t newSize =
            _helper.scope->expressions_size + nodeArray->size - 1;

        if (expression_capacity < newSize) {
          expression_capacity = newSize;
          _helper.scope->expressions = a404m_realloc(
              _helper.scope->expressions,
              expression_capacity * sizeof(*_helper.scope->expressions));
        }

        for (size_t i = newSize - 1; i > parentIndex; --i) {
          _helper.scope->expressions[i] =
              _helper.scope->expressions[i - nodeArray->size + 1];
        }

        for (size_t i = 1; i < nodeArray->size; ++i) {
          _helper.scope->expressions[i + parentIndex] = astNodes[i];
        }

        _helper.scope->expressions_size = newSize;
      }
    }
    parserNodeDelete(rootParser);
    lexerNodeArrayDestroy(lexerArray);
  }

  return true;
}

bool setTypesVariable(AstTree *tree, AstTreeSetTypesHelper helper,
                      AstTreeFunctionCall *functionCall) {
  AstTreeVariable *variable = setTypesFindVariable(
      tree->str_begin, tree->str_end, helper, functionCall);
  if (variable == NULL) {
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
  AstTreePureInfix *infix = tree->metadata;
  if (!setTypesAstInfix(infix, helper)) {
    return false;
  } else if (!typeIsEqual(infix->left->type, infix->right->type,
                          helper.scope)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else if (isConst(infix->left)) {
    printError(tree->str_begin, tree->str_end, "Constants can't be assigned");
    return false;
  } else if (!isLeftValue(infix->left)) {
    printError(infix->left->str_begin, infix->left->str_end,
               "Need to be l-value");
    return false;
  } else {
    tree->type = copyAstTree(infix->left->type);
    return true;
  }
}

bool setTypesOperatorGeneral(AstTree *tree, AstTreeSetTypesHelper _helper,
                             const char *str, size_t str_size) {
  AstTreeFunctionCall *metadata = tree->metadata;

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
      .variables_size = _helper.variables_size,
      .root = _helper.root,
      .loops = _helper.loops,
      .loops_size = _helper.loops_size,
      .scope = _helper.scope,
      .isInScope = false,
  };

  for (size_t i = 0; i < metadata->parameters.size; ++i) {
    if (!setAllTypes(metadata->parameters.data[i].value, helper, NULL, NULL)) {
      return false;
    }
  }

  AstTreeVariable *variable =
      setTypesFindVariable(str, str + str_size, helper, metadata);
  if (variable == NULL) {
    printError(tree->str_begin, tree->str_end, "Can't find operator");
    return false;
  } else if (!variable->isConst) {
    printError(tree->str_begin, tree->str_end,
               "Overloaded operator must be constant");
    return false;
  }

  metadata->function = newAstTree(AST_TREE_TOKEN_VARIABLE, variable,
                                  copyAstTree(variable->type), NULL, NULL);
  if (metadata->function->type->token == AST_TREE_TOKEN_TYPE_FUNCTION) {
    AstTreeTypeFunction *function = metadata->function->type->metadata;
    tree->type = copyAstTree(function->returnType);
  } else if (metadata->function->type->token == AST_TREE_TOKEN_TYPE_MACRO) {
    metadata->function = getValue(metadata->function, false, helper.scope);
    if (metadata->function->token != AST_TREE_TOKEN_VALUE_MACRO) {
      UNREACHABLE;
    }
    AstTreeMacro *macro = metadata->function->metadata;
    AstTreeFunction *function =
        copyAstTreeFunction(macro->function, NULL, NULL, 0, true);

    if (!setTypesAstFunction(function, NULL, helper)) {
      astTreeFunctionDestroy(*function);
      free(function);
      return false;
    }

    AstTree *functionType = makeTypeOfFunction(function, NULL, NULL);

    if (!doesFunctionMatch(functionType->metadata, metadata, helper)) {
      printError(tree->str_begin, tree->str_end, "Function call doesn't match");
      return NULL;
    }

    const char *str_begin = metadata->function->str_begin;
    const char *str_end = metadata->function->str_end;
    astTreeDelete(metadata->function);

    metadata->function = newAstTree(AST_TREE_TOKEN_FUNCTION, function,
                                    functionType, str_begin, str_end);

    tree->type = copyAstTree(function->returnType);
  } else if (metadata->function->type->token ==
             AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER) {
    AstTree *function = getShapeShifterElement(metadata, helper);
    if (function == NULL) {
      return NULL;
    }
    metadata->function = function;
    AstTreeTypeFunction *functionType = metadata->function->type->metadata;
    tree->type = copyAstTree(functionType->returnType);
  } else {
    printLog("%s", AST_TREE_TOKEN_STRINGS[metadata->function->type->token]);
    UNREACHABLE;
  }

  return true;
}

bool setTypesOperatorPointer(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *metadata = tree->metadata;
  if (!setAllTypes(metadata, helper, NULL, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->type, &AST_TREE_TYPE_TYPE, helper.scope)) {
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
  for (size_t i = 0; i < _helper.variables[0].size; ++i) { // TODO: change this
    if (variable == _helper.variables[0].data[i]) {
      _helper = (AstTreeSetTypesHelper){
          .root = _helper.root,
          .variables = _helper.variables,
          .variables_size = 1,
          .dependencies = _helper.dependencies,
          .lookingType = _helper.lookingType,
          .loops = _helper.loops,
          .loops_size = _helper.loops_size,
          .scope = _helper.scope,
          .isInScope = false,
      };
      break;
    }
  }

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
      .dependencies.data = deps,
      .dependencies.size = _helper.dependencies.size + 1,
      .variables = _helper.variables,
      .variables_size = _helper.variables_size,
      .root = _helper.root,
      .loops = _helper.loops,
      .loops_size = _helper.loops_size,
      .scope = _helper.scope,
      .isInScope = false,
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

    if (isConst(variable->type)) {
      variable->type = getValue(variable->type, false, helper.scope);
    } else {
      printError(variable->name_begin, variable->name_end,
                 "Type must be comptime");
      return false;
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
    if (value != NULL &&
        !typeIsEqual(value->type, variable->type, helper.scope)) {
      printError(variable->name_begin, variable->name_end,
                 "Type mismatch value = %s but type = %s",
                 AST_TREE_TOKEN_STRINGS[value->type->token],
                 AST_TREE_TOKEN_STRINGS[variable->type->token]);
      return false;
    } else if (variable->isConst) {
      if (isConst(value)) {
        variable->value = getValue(value, false, helper.scope);
        AstTree *v = fromRawValue(variable->value);
        if (v != NULL) {
          astTreeDelete(variable->value);
          variable->value = v;
        }
      } else {
        printError(value->str_begin, value->str_end,
                   "Can't initialize constant with non constant value");
        return false;
      }
    }
  }

  if (!typeIsEqual(variable->type->type, &AST_TREE_TYPE_TYPE, helper.scope)) {
    printError(variable->name_begin, variable->name_end,
               "Type must have type of `type`");
    return false;
  }

  return true;
}

bool setTypesIf(AstTree *tree, AstTreeSetTypesHelper helper,
                AstTreeFunction *function) {
  AstTreeIf *metadata = tree->metadata;

  if (!setAllTypes(metadata->condition, helper, function, NULL)) {
    return false;
  }

  if (!typeIsEqual(metadata->condition->type, &AST_TREE_BOOL_TYPE,
                   helper.scope)) {
    printError(metadata->condition->str_begin, metadata->condition->str_end,
               "If condition must be boolean");
    return false;
  }

  if (metadata->condition->token == AST_TREE_TOKEN_KEYWORD_COMPTIME) {
    AstTree *condition = getValue(metadata->condition, true, helper.scope);
    AstTree *result;
    bool condi = *(AstTreeBool *)condition->metadata;
    astTreeDelete(condition);
    if (condi) {
      if (!setAllTypes(metadata->ifBody, helper, function, NULL)) {
        return false;
      }
      result = copyAstTree(metadata->ifBody);
    } else if (metadata->elseBody != NULL) {
      if (!setAllTypes(metadata->elseBody, helper, function, NULL)) {
        return false;
      }
      result = copyAstTree(metadata->elseBody);
    } else {
      result = copyAstTree(&AST_TREE_VOID_VALUE);
    }
    astTreeDestroy(*tree);
    *tree = *result;
    if (astTreeShouldDelete(result)) {
      free(result);
    }
  } else {
    if (!setAllTypes(metadata->ifBody, helper, function, NULL) ||
        (metadata->elseBody != NULL &&
         !setAllTypes(metadata->elseBody, helper, function, NULL))) {
      return false;
    }

    if (metadata->elseBody != NULL &&
        typeIsEqual(metadata->ifBody->type, metadata->elseBody->type,
                    helper.scope)) {
      tree->type = copyAstTree(metadata->ifBody->type);
    } else {
      tree->type = &AST_TREE_VOID_TYPE;
    }
  }

  return true;
}

bool setTypesWhile(AstTree *tree, AstTreeSetTypesHelper _helper,
                   AstTreeFunction *function) {
  AstTreeWhile *metadata = tree->metadata;

  const size_t loops_size = _helper.loops_size + 1;
  AstTreeWhile *loops[loops_size];

  for (size_t i = 0; i < _helper.loops_size; ++i) {
    loops[i] = _helper.loops[i];
  }
  loops[_helper.loops_size] = metadata;

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
      .variables_size = _helper.variables_size,
      .root = _helper.root,
      .loops = loops,
      .loops_size = loops_size,
      .scope = _helper.scope,
      .isInScope = false,
  };

  if (!setAllTypes(metadata->condition, helper, function, NULL)) {
    return false;
  }

  if (!typeIsEqual(metadata->condition->type, &AST_TREE_BOOL_TYPE,
                   helper.scope)) {
    printError(metadata->condition->str_begin, metadata->condition->str_end,
               "If condition must be boolean");
    return false;
  }

  if (metadata->condition->token == AST_TREE_TOKEN_KEYWORD_COMPTIME) {
    AstTree *condition = getValue(metadata->condition, true, helper.scope);
    bool condi = *(AstTreeBool *)condition->metadata;
    astTreeDelete(condition);
    if (!condi) {
      astTreeDestroy(*tree);
      *tree = AST_TREE_VOID_VALUE;
      return true;
    }
  }
  if (!setAllTypes(metadata->body, helper, function, NULL)) {
    return false;
  }
  tree->type = &AST_TREE_VOID_TYPE;
  return true;
}

bool setTypesScope(AstTree *tree, AstTreeSetTypesHelper _helper,
                   AstTreeFunction *function) {
  AstTreeScope *metadata = tree->metadata;

  AstTreeVariables variables[_helper.variables_size + 1];

  for (size_t i = 0; i < _helper.variables_size; ++i) {
    variables[i] = _helper.variables[i];
  }

  variables[_helper.variables_size].data =
      a404m_malloc(metadata->variables.size *
                   sizeof(*variables[_helper.variables_size].data));
  variables[_helper.variables_size].size = 0;

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = variables,
      .variables_size = _helper.variables_size + 1,
      .root = _helper.root,
      .loops = _helper.loops,
      .loops_size = _helper.loops_size,
      .scope = metadata,
      .isInScope = true,
  };

  for (size_t i = 0; i < metadata->variables.size; ++i) {
    AstTreeVariable *variable = metadata->variables.data[i];
    if (variable->isConst) {
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      helper.variables[helper.variables_size - 1]
          .data[helper.variables[helper.variables_size - 1].size++] = variable;
    }
  }

  for (size_t i = 0; i < metadata->expressions_size; ++i) {
    AstTree *expr = metadata->expressions[i];
    if (expr->token == AST_TREE_TOKEN_VARIABLE_DEFINE) {
      AstTreeVariable *variable = expr->metadata;
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      size_t variables_capacity =
          a404m_malloc_usable_size(
              helper.variables[helper.variables_size - 1].data) /
          sizeof(*helper.variables[helper.variables_size - 1].data);
      if (variables_capacity ==
          helper.variables[helper.variables_size - 1].size) {
        variables_capacity += variables_capacity / 2 + 1;
        helper.variables[helper.variables_size - 1].data = a404m_realloc(
            helper.variables[helper.variables_size - 1].data,
            variables_capacity *
                sizeof(*helper.variables[helper.variables_size - 1].data));
      }
      helper.variables[helper.variables_size - 1]
          .data[helper.variables[helper.variables_size - 1].size++] = variable;
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

  free(helper.variables[helper.variables_size - 1].data);
  return true;
}

bool setTypesComptime(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeSingleChild *operand = tree->metadata;

  if (!setAllTypes(operand, helper, NULL, NULL)) {
    return false;
  }

  operand = getValue(operand, false, helper.scope);
  if (operand == NULL) {
    return false;
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
    static const char PTR_STR[] = "ptr";
    static const size_t PTR_STR_SIZE =
        sizeof(PTR_STR) / sizeof(*PTR_STR) - sizeof(*PTR_STR);

    if (LENGTH_STR_SIZE == size && strnEquals(LENGTH_STR, str, size)) {
      metadata->member.index = 1;
      tree->type = copyAstTree(&AST_TREE_U64_TYPE);
      return true;
    } else if (PTR_STR_SIZE == size && strnEquals(PTR_STR, str, size)) {
      metadata->member.index = 0;
      AstTreeBracket *arrayType = metadata->object->type->metadata;
      tree->type = newAstTree(AST_TREE_TOKEN_OPERATOR_POINTER,
                              copyAstTree(arrayType->operand),
                              &AST_TREE_TYPE_TYPE, NULL, NULL);
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
  } else if (typeIsEqual(metadata->object->type, &AST_TREE_NAMESPACE_TYPE,
                         helper.scope)) {
    AstTree *value = getValue(metadata->object, true, helper.scope);
    AstTreeNamespace *namespace = value->metadata;
    AstTreeSetTypesHelper newHelper = {
        .root = helper.root->imports[namespace->importedIndex].root,
        .variables =
            &helper.root->imports[namespace->importedIndex].root->variables,
        .variables_size = 1,
        .dependencies = helper.dependencies,
        .lookingType = helper.lookingType,
        .loops = helper.loops,
        .loops_size = helper.loops_size,
        .scope = helper.scope,
        .isInScope = false,
    };
    astTreeDelete(value);

    AstTreeVariable *var =
        setTypesFindVariable(metadata->member.name.begin,
                             metadata->member.name.end, newHelper, NULL);
    if (var == NULL) {
      printError(metadata->member.name.begin, metadata->member.name.end,
                 "Is not found");
      return false;
    }
    astTreeDestroy(*tree);
    *tree = (AstTree){
        .token = AST_TREE_TOKEN_VARIABLE,
        .metadata = var,
        .type = copyAstTree(var->type),
        .str_begin = var->name_begin,
        .str_end = var->name_end,
    };
    return true;
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
  if (functionCall->parameters.size == 2) {
    AstTree *from = NULL;
    AstTree *to = NULL;

    static const char FROM_STR[] = "from";
    static const size_t FROM_STR_SIZE =
        sizeof(FROM_STR) / sizeof(*FROM_STR) - sizeof(*FROM_STR);
    static const char TO_STR[] = "to";
    static const size_t TO_STR_SIZE =
        sizeof(TO_STR) / sizeof(*TO_STR) - sizeof(*TO_STR);

    for (size_t i = 0; i < functionCall->parameters.size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters.data[i];
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
    type_metadata->isMacro = false;

    type_metadata->returnType = copyAstTree(to);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(from->type),
        .name_begin = FROM_STR,
        .name_end = FROM_STR + FROM_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(to->type),
        .name_begin = TO_STR,
        .name_end = TO_STR + TO_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
}

bool setTypesBuiltinTypeOf(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters.size == 1) {
    AstTree *variable = NULL;

    static const char VARIABLE_STR[] = "variable";
    static const size_t VARIABLE_STR_SIZE =
        sizeof(VARIABLE_STR) / sizeof(*VARIABLE_STR) - sizeof(*VARIABLE_STR);

    for (size_t i = 0; i < functionCall->parameters.size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters.data[i];
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
    type_metadata->isMacro = false;

    type_metadata->returnType = copyAstTree(&AST_TREE_TYPE_TYPE);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(variable->type),
        .name_begin = VARIABLE_STR,
        .name_end = VARIABLE_STR + VARIABLE_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
}

bool setTypesBuiltinSizeOf(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters.size == 1) {
    AstTree *type = NULL;

    static const char TYPE_STR[] = "type";
    static const size_t TYPE_STR_SIZE =
        sizeof(TYPE_STR) / sizeof(*TYPE_STR) - sizeof(*TYPE_STR);

    for (size_t i = 0; i < functionCall->parameters.size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters.data[i];
      const size_t param_name_size = param.nameEnd - param.nameBegin;

      if (param_name_size == 0) {
        if (type == NULL) {
          type = param.value;
        } else {
          printError(param.value->str_begin, param.value->str_end,
                     "Bad paramter");
          return false;
        }
      } else if (param_name_size == TYPE_STR_SIZE &&
                 strnEquals(param.nameBegin, TYPE_STR, TYPE_STR_SIZE) &&
                 type == NULL) {
        type = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    }

    if (type == NULL) {
      return false;
    } else if (!typeIsEqual(type->type, &AST_TREE_TYPE_TYPE, helper.scope)) {
      printError(type->str_begin, type->str_end,
                 "Type missmatch, the argument should be `type`");
      return false;
    }

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = 1;
    type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                            sizeof(*type_metadata->arguments));
    type_metadata->isMacro = false;

    type_metadata->returnType = copyAstTree(&AST_TREE_U64_TYPE);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(&AST_TREE_TYPE_TYPE),
        .name_begin = TYPE_STR,
        .name_end = TYPE_STR + TYPE_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
}

bool setTypesBuiltinImport(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters.size == 1) {
    AstTree *file = NULL;

    static const char PATH_STR[] = "path";
    static const size_t PATH_STR_SIZE =
        sizeof(PATH_STR) / sizeof(*PATH_STR) - sizeof(*PATH_STR);

    for (size_t i = 0; i < functionCall->parameters.size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters.data[i];
      const size_t param_name_size = param.nameEnd - param.nameBegin;

      if (param_name_size == 0) {
        if (file == NULL) {
          file = param.value;
        } else {
          printError(param.value->str_begin, param.value->str_end,
                     "Bad paramter");
          return false;
        }
      } else if (param_name_size == PATH_STR_SIZE &&
                 strnEquals(param.nameBegin, PATH_STR, PATH_STR_SIZE) &&
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

    AstTree *fileType = makeStringType();

    if (!typeIsEqual(file->type, fileType, helper.scope)) {
      astTreeDelete(fileType);
      printError(file->str_begin, file->str_end, "Bad paramter");
      return false;
    }

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = 1;
    type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                            sizeof(*type_metadata->arguments));
    type_metadata->isMacro = false;

    type_metadata->returnType = copyAstTree(&AST_TREE_NAMESPACE_TYPE);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = fileType,
        .name_begin = PATH_STR,
        .name_end = PATH_STR + PATH_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);

    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
}

bool setTypesBuiltinIsComptime(AstTree *tree, AstTreeSetTypesHelper helper) {
  (void)helper;
  tree->type = copyAstTree(&AST_TREE_BOOL_TYPE);
  return true;
}

bool setTypesBuiltinStackAlloc(AstTree *tree, AstTreeSetTypesHelper helper,
                               AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters.size == 2) {
    AstTree *count = NULL;
    AstTree *type = NULL;

    static const char TYPE_STR[] = "type";
    static const size_t TYPE_STR_SIZE =
        sizeof(TYPE_STR) / sizeof(*TYPE_STR) - sizeof(*TYPE_STR);
    static const char COUNT_STR[] = "count";
    static const size_t COUNT_STR_SIZE =
        sizeof(COUNT_STR) / sizeof(*COUNT_STR) - sizeof(*COUNT_STR);

    for (size_t i = 0; i < functionCall->parameters.size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters.data[i];
      const size_t param_name_size = param.nameEnd - param.nameBegin;

      if (param_name_size == 0) {
        if (count == NULL) {
          count = param.value;
        } else if (type == NULL) {
          type = param.value;
        } else {
          printError(param.value->str_begin, param.value->str_end,
                     "Bad paramter");
          return false;
        }
      } else if (param_name_size == COUNT_STR_SIZE &&
                 strnEquals(param.nameBegin, COUNT_STR, COUNT_STR_SIZE) &&
                 count == NULL) {
        count = param.value;
      } else if (param_name_size == TYPE_STR_SIZE &&
                 strnEquals(param.nameBegin, TYPE_STR, TYPE_STR_SIZE) &&
                 type == NULL) {
        type = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    }

    if (count == NULL || type == NULL) {
      return false;
    } else if (!typeIsEqual(count->type, &AST_TREE_I64_TYPE, helper.scope) &&
               !typeIsEqual(count->type, &AST_TREE_U64_TYPE, helper.scope)) {
      printError(count->str_begin, count->str_end,
                 "Type missmatch, the argument should be `u64` or `i64`");
      return false;
    } else if (!typeIsEqual(type->type, &AST_TREE_TYPE_TYPE, helper.scope)) {
      printError(type->str_begin, type->str_end,
                 "Type missmatch, the argument should be `type`");
      return false;
    }

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = 2;
    type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                            sizeof(*type_metadata->arguments));
    type_metadata->isMacro = false;

    type_metadata->returnType =
        newAstTree(AST_TREE_TOKEN_OPERATOR_POINTER, copyAstTree(type),
                   &AST_TREE_TYPE_TYPE, NULL, NULL);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(count->type),
        .name_begin = TYPE_STR,
        .name_end = TYPE_STR + TYPE_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
        .type = copyAstTree(type->type),
        .name_begin = COUNT_STR,
        .name_end = COUNT_STR + COUNT_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
}

bool setTypesBuiltinHeapAlloc(AstTree *tree, AstTreeSetTypesHelper helper,
                              AstTreeFunctionCall *functionCall) {
  (void)tree;
  (void)helper;
  (void)functionCall;
  NOT_IMPLEMENTED;
}

bool setTypesBuiltinUnary(AstTree *tree, AstTreeSetTypesHelper helper,
                          AstTreeFunctionCall *functionCall) {
  (void)helper;
  if (functionCall->parameters.size != 1) {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }

  const char *str = functionCall->parameters.data[0].nameBegin;
  const size_t str_size = functionCall->parameters.data[0].nameEnd -
                          functionCall->parameters.data[0].nameBegin;

  static char VALUE_STR[] = "value";
  static size_t VALUE_STR_SIZE =
      sizeof(VALUE_STR) / sizeof(*VALUE_STR) - sizeof(*VALUE_STR);

  if (str_size != 0 && (str_size != VALUE_STR_SIZE ||
                        !strnEquals(str, VALUE_STR, VALUE_STR_SIZE))) {
    printError(functionCall->parameters.data[0].nameBegin,
               functionCall->parameters.data[0].nameEnd, "Unknown parameter");
    return false;
  }

  AstTree *type = functionCall->parameters.data[0].value->type;
  switch (type->token) {
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
    goto AFTER_SWITCH;
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_KEYWORD_STRUCT:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_ARRAY:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
  case AST_TREE_TOKEN_TYPE_VOID:
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
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
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
  case AST_TREE_TOKEN_NONE:
  }
  printError(functionCall->parameters.data[0].nameBegin,
             functionCall->parameters.data[0].nameEnd, "Bad argument");
  return false;

AFTER_SWITCH:
  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 1;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));
  type_metadata->isMacro = false;

  type_metadata->returnType = copyAstTree(type);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(type),
      .name_begin = VALUE_STR,
      .name_end = VALUE_STR + VALUE_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinBinaryAlsoPointer(AstTree *tree,
                                      AstTreeSetTypesHelper helper,
                                      AstTreeFunctionCall *functionCall) {
  if (functionCall->parameters.size != 2) {
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

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
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
  } else if (left->type->token != AST_TREE_TOKEN_OPERATOR_POINTER &&
             !typeIsEqual(left->type, right->type, helper.scope)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  } else if (left->type->token == AST_TREE_TOKEN_OPERATOR_POINTER &&
             !typeIsEqual(right->type, &AST_TREE_I64_TYPE, helper.scope) &&
             !typeIsEqual(right->type, &AST_TREE_U64_TYPE, helper.scope)) {
    printError(tree->str_begin, tree->str_end,
               "Pointer can only have right hand as u64 or i64");
    return false;
  }

  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 2;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));
  type_metadata->isMacro = false;

  type_metadata->returnType = copyAstTree(left->type);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = LEFT_STR,
      .name_end = LEFT_STR + LEFT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(right->type),
      .name_begin = RIGHT_STR,
      .name_end = RIGHT_STR + RIGHT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinBinary(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  if (functionCall->parameters.size != 2) {
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

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
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
  } else if (!typeIsEqual(left->type, right->type, helper.scope)) {
    printError(tree->str_begin, tree->str_end, "Type mismatch");
    return false;
  }

  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 2;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));
  type_metadata->isMacro = false;

  type_metadata->returnType = copyAstTree(left->type);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = LEFT_STR,
      .name_end = LEFT_STR + LEFT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(right->type),
      .name_begin = RIGHT_STR,
      .name_end = RIGHT_STR + RIGHT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinBinaryWithRet(AstTree *tree, AstTreeSetTypesHelper helper,
                                  AstTreeFunctionCall *functionCall,
                                  AstTree *retType) {
  (void)helper;
  if (functionCall->parameters.size != 2) {
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

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
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
  type_metadata->isMacro = false;

  type_metadata->returnType = copyAstTree(retType);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = LEFT_STR,
      .name_end = LEFT_STR + LEFT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(left->type),
      .name_begin = RIGHT_STR,
      .name_end = RIGHT_STR + RIGHT_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinPutc(AstTree *tree, AstTreeSetTypesHelper helper,
                         AstTreeFunctionCall *functionCall) {
  if (functionCall->parameters.size != 1) {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }

  const char *str = functionCall->parameters.data[0].nameBegin;
  const size_t str_size = functionCall->parameters.data[0].nameEnd -
                          functionCall->parameters.data[0].nameBegin;

  static char VALUE_STR[] = "value";
  static size_t VALUE_STR_SIZE =
      sizeof(VALUE_STR) / sizeof(*VALUE_STR) - sizeof(*VALUE_STR);

  if (str_size != 0 && (str_size != VALUE_STR_SIZE ||
                        !strnEquals(str, VALUE_STR, VALUE_STR_SIZE))) {
    printError(functionCall->parameters.data[0].nameBegin,
               functionCall->parameters.data[0].nameEnd, "Unknown parameter");
    return false;
  }

  if (!typeIsEqual(functionCall->parameters.data[0].value->type,
                   &AST_TREE_U8_TYPE, helper.scope)) {
    printError(functionCall->parameters.data[0].nameBegin,
               functionCall->parameters.data[0].nameEnd,
               "Bad argument (must have a type of u8)");
    return false;
  }

  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 1;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));
  type_metadata->isMacro = false;

  type_metadata->returnType = copyAstTree(&AST_TREE_VOID_TYPE);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(&AST_TREE_U8_TYPE),
      .name_begin = VALUE_STR,
      .name_end = VALUE_STR + VALUE_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinCLibrary(AstTree *tree, AstTreeSetTypesHelper helper,
                             AstTreeFunctionCall *functionCall) {
  if (functionCall->parameters.size == 1) {
    AstTree *path = NULL;

    static const char PATH_STR[] = "path";
    static const size_t PATH_STR_SIZE =
        sizeof(PATH_STR) / sizeof(*PATH_STR) - sizeof(*PATH_STR);

    for (size_t i = 0; i < functionCall->parameters.size; ++i) {
      AstTreeFunctionCallParam param = functionCall->parameters.data[i];
      const size_t param_name_size = param.nameEnd - param.nameBegin;

      if (param_name_size == 0) {
        if (path == NULL) {
          path = param.value;
        } else {
          printError(param.value->str_begin, param.value->str_end,
                     "Bad paramter");
          return false;
        }
      } else if (param_name_size == PATH_STR_SIZE &&
                 strnEquals(param.nameBegin, PATH_STR, PATH_STR_SIZE) &&
                 path == NULL) {
        path = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    }

    AstTree *path_type = makeStringType();

    if (path == NULL) {
      return false;
    } else if (!typeIsEqual(path->type, path_type, helper.scope)) {
      astTreeDelete(path_type);
      printError(path->str_begin, path->str_end,
                 "Type missmatch, the argument should be `type`");
      return false;
    }

    AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
    type_metadata->arguments_size = 1;
    type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                            sizeof(*type_metadata->arguments));
    type_metadata->isMacro = false;

    type_metadata->returnType = copyAstTree(&AST_TREE_C_LIBRARY_TYPE);

    type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
        .type = path_type,
        .name_begin = PATH_STR,
        .name_end = PATH_STR + PATH_STR_SIZE,
        .str_begin = NULL,
        .str_end = NULL,
        .isComptime = false,
    };

    tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                            &AST_TREE_TYPE_TYPE, NULL, NULL);
    return true;
  } else {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
}

bool setTypesBuiltinCFunction(AstTree *tree, AstTreeSetTypesHelper helper,
                              AstTreeFunctionCall *functionCall) {
  if (functionCall->parameters.size != 3) {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }
  AstTree *library = NULL;
  AstTree *name = NULL;
  AstTree *funcType = NULL;

  static char LIBRARY_STR[] = "library";
  static const size_t LIBRARY_STR_SIZE =
      sizeof(LIBRARY_STR) / sizeof(*LIBRARY_STR) - sizeof(*LIBRARY_STR);
  static char NAME_STR[] = "name";
  static const size_t NAME_STR_SIZE =
      sizeof(NAME_STR) / sizeof(*NAME_STR) - sizeof(*NAME_STR);
  static char FUNC_TYPE_STR[] = "func_type";
  static const size_t FUNC_TYPE_STR_SIZE =
      sizeof(FUNC_TYPE_STR) / sizeof(*FUNC_TYPE_STR) - sizeof(*FUNC_TYPE_STR);

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
    const size_t param_name_size = param.nameEnd - param.nameBegin;

    if (param_name_size == 0) {
      if (library == NULL) {
        library = param.value;
      } else if (name == NULL) {
        name = param.value;
      } else if (funcType == NULL) {
        funcType = param.value;
      } else {
        printError(param.value->str_begin, param.value->str_end,
                   "Bad paramter");
        return false;
      }
    } else if (param_name_size == LIBRARY_STR_SIZE &&
               strnEquals(param.nameBegin, LIBRARY_STR, LIBRARY_STR_SIZE) &&
               library == NULL) {
      library = param.value;
    } else if (param_name_size == NAME_STR_SIZE &&
               strnEquals(param.nameBegin, NAME_STR, NAME_STR_SIZE) &&
               name == NULL) {
      name = param.value;
    } else if (param_name_size == FUNC_TYPE_STR_SIZE &&
               strnEquals(param.nameBegin, FUNC_TYPE_STR, FUNC_TYPE_STR_SIZE) &&
               library == NULL) {
      funcType = param.value;
    } else {
      printError(param.value->str_begin, param.value->str_end, "Bad paramter");
      return false;
    }
  }

  AstTree *str_type = makeStringType();

  if (library == NULL || name == NULL || funcType == NULL) {
    return false;
  } else if (!typeIsEqual(library->type, &AST_TREE_C_LIBRARY_TYPE,
                          helper.scope)) {
    printError(library->str_begin, library->str_end,
               "library must have a type of `c_library`");
    return false;
  } else if (!typeIsEqual(name->type, str_type, helper.scope)) {
    astTreeDelete(str_type);
    printError(name->str_begin, name->str_end,
               "name must have a type of `[]u8`");
    return false;
  } else if (funcType->token != AST_TREE_TOKEN_TYPE_FUNCTION) {
    astTreeDelete(str_type);
    printError(funcType->str_begin, funcType->str_end,
               "func_type must have a type of `function type`");
    return false;
  }
  astTreeDelete(str_type);

  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 3;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));
  type_metadata->isMacro = false;

  AstTreeCFunctionType *retType = a404m_malloc(sizeof(*retType));
  retType->funcType = copyAstTree(funcType);

  type_metadata->returnType = newAstTree(
      AST_TREE_TOKEN_TYPE_C_FUNCTION, retType, &AST_TREE_TYPE_TYPE, NULL, NULL);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(library->type),
      .name_begin = LIBRARY_STR,
      .name_end = LIBRARY_STR + LIBRARY_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  type_metadata->arguments[1] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(name->type),
      .name_begin = NAME_STR,
      .name_end = NAME_STR + NAME_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  type_metadata->arguments[2] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(funcType->type),
      .name_begin = FUNC_TYPE_STR,
      .name_end = FUNC_TYPE_STR + FUNC_TYPE_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesBuiltinInsert(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall) {
  if (functionCall->parameters.size != 1) {
    printError(tree->str_begin, tree->str_end, "Too many or too few arguments");
    return false;
  }

  const char *str = functionCall->parameters.data[0].nameBegin;
  const size_t str_size = functionCall->parameters.data[0].nameEnd -
                          functionCall->parameters.data[0].nameBegin;

  static char CODE_STR[] = "code";
  static size_t CODE_STR_SIZE =
      sizeof(CODE_STR) / sizeof(*CODE_STR) - sizeof(*CODE_STR);

  if (str_size != 0 && (str_size != CODE_STR_SIZE ||
                        !strnEquals(str, CODE_STR, CODE_STR_SIZE))) {
    printError(functionCall->parameters.data[0].nameBegin,
               functionCall->parameters.data[0].nameEnd, "Unknown parameter");
    return false;
  }

  AstTree *type = functionCall->parameters.data[0].value->type;
  AstTree *stringType = makeStringType();
  if (typeIsEqual(type->type, stringType, helper.scope)) {
    astTreeDelete(stringType);
    printError(functionCall->parameters.data[0].nameBegin,
               functionCall->parameters.data[0].nameEnd,
               "Type mismatch it must be `code`");
    return false;
  } else if (!isConst(functionCall->parameters.data[0].value)) {
    printError(functionCall->parameters.data[0].nameBegin,
               functionCall->parameters.data[0].nameEnd, "Must be const");
    return false;
  }
  astTreeDelete(stringType);

  AstTreeTypeFunction *type_metadata = a404m_malloc(sizeof(*type_metadata));
  type_metadata->arguments_size = 1;
  type_metadata->arguments = a404m_malloc(type_metadata->arguments_size *
                                          sizeof(*type_metadata->arguments));
  type_metadata->isMacro = false;

  type_metadata->returnType = copyAstTree(type);

  type_metadata->arguments[0] = (AstTreeTypeFunctionArgument){
      .type = copyAstTree(type),
      .name_begin = CODE_STR,
      .name_end = CODE_STR + CODE_STR_SIZE,
      .str_begin = NULL,
      .str_end = NULL,
      .isComptime = false,
  };

  tree->type = newAstTree(AST_TREE_TOKEN_TYPE_FUNCTION, type_metadata,
                          &AST_TREE_TYPE_TYPE, NULL, NULL);
  return true;
}

bool setTypesTypeArray(AstTree *tree, AstTreeSetTypesHelper helper) {
  AstTreeBracket *metadata = tree->metadata;

  if (!setAllTypes(metadata->operand, helper, NULL, NULL)) {
    return false;
  } else if (!typeIsEqual(metadata->operand->type, &AST_TREE_TYPE_TYPE,
                          helper.scope)) {
    printError(metadata->operand->str_begin, metadata->operand->str_end,
               "Expected type");
    return false;
  } else if (!isConst(metadata->operand)) {
    printError(metadata->operand->str_begin, metadata->operand->str_end,
               "Epxected const");
    return false;
  }

  metadata->operand = getValue(metadata->operand, false, helper.scope);

  if (metadata->parameters.size == 0) {
    // left empty
  } else if (metadata->parameters.size == 1) {
    AstTreeSetTypesHelper newHelper = {
        .lookingType = &AST_TREE_U64_TYPE,
        .dependencies = helper.dependencies,
        .variables = helper.variables,
        .variables_size = helper.variables_size,
        .root = helper.root,
        .loops = helper.loops,
        .loops_size = helper.loops_size,
        .scope = helper.scope,
        .isInScope = false,
    };

    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      AstTree *param = metadata->parameters.data[i];
      if (!setAllTypes(param, newHelper, NULL, NULL)) {
        return false;
      } else if (!typeIsEqual(param->type, &AST_TREE_I64_TYPE,
                              newHelper.scope) &&
                 !typeIsEqual(param->type, &AST_TREE_U64_TYPE,
                              newHelper.scope)) {
        printError(param->str_begin, param->str_end,
                   "Should only be int (for now)");
        return false;
      } else if (!isConst(param)) {
        printError(param->str_begin, param->str_end,
                   "Should only be const (for now)");
        return false;
      }
      metadata->parameters.data[i] = getValue(param, false, newHelper.scope);
    }
  } else {
    printError(tree->str_begin, tree->str_end,
               "Multiple param in array is not yet supported");
    return false;
  }

  tree->type = copyAstTree(&AST_TREE_TYPE_TYPE);
  return true;
}

bool setTypesAstInfix(AstTreePureInfix *infix, AstTreeSetTypesHelper _helper) {
  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = _helper.variables,
      .variables_size = _helper.variables_size,
      .root = _helper.root,
      .loops = _helper.loops,
      .loops_size = _helper.loops_size,
      .scope = _helper.scope,
      .isInScope = false,
  };

  if (!setAllTypes(infix->left, helper, NULL, NULL)) {
    return false;
  }

  helper.lookingType = infix->left->type;

  return setAllTypes(infix->right, helper, NULL, NULL);
}

bool setTypesAstFunction(AstTreeFunction *metadata, AstTree *tree,
                         AstTreeSetTypesHelper _helper) {
  AstTreeVariables variables[_helper.variables_size + 1];

  for (size_t i = 0; i < _helper.variables_size; ++i) {
    variables[i] = _helper.variables[i];
  }

  variables[_helper.variables_size].data =
      a404m_malloc((metadata->arguments.size + metadata->scope.variables.size) *
                   sizeof(*variables[_helper.variables_size].data));
  variables[_helper.variables_size].size = 0;

  AstTreeSetTypesHelper helper = {
      .lookingType = NULL,
      .dependencies = _helper.dependencies,
      .variables = variables,
      .variables_size = _helper.variables_size + 1,
      .root = _helper.root,
      .loops = NULL,
      .loops_size = 0,
      .scope = &metadata->scope,
      .isInScope = true,
  };

  for (size_t i = 0; i < metadata->arguments.size; ++i) {
    AstTreeVariable *variable = metadata->arguments.data[i];
    if (!setTypesAstVariable(variable, helper)) {
      return false;
    }
    helper.variables[helper.variables_size - 1]
        .data[helper.variables[helper.variables_size - 1].size++] = variable;
  }

  if (!setAllTypes(metadata->returnType, helper, NULL, NULL)) {
    return false;
  }
  metadata->returnType = getValue(metadata->returnType, false, helper.scope);

  AstTreeVariable *deps[helper.dependencies.size];
  size_t deps_size = 0;
  if (tree != NULL) {
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
  }

  for (size_t i = 0; i < metadata->scope.variables.size; ++i) {
    AstTreeVariable *variable = metadata->scope.variables.data[i];
    if (variable->isConst) {
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      helper.variables[helper.variables_size - 1]
          .data[helper.variables[helper.variables_size - 1].size++] = variable;
    }
  }

  for (size_t i = 0; i < metadata->scope.expressions_size; ++i) {
    AstTree *expr = metadata->scope.expressions[i];
    if (expr->token == AST_TREE_TOKEN_VARIABLE_DEFINE) {
      AstTreeVariable *variable = expr->metadata;
      if (!setTypesAstVariable(variable, helper)) {
        return false;
      }
      size_t variables_capacity =
          a404m_malloc_usable_size(
              helper.variables[helper.variables_size - 1].data) /
          sizeof(*helper.variables[helper.variables_size - 1].data);
      if (variables_capacity ==
          helper.variables[helper.variables_size - 1].size) {
        variables_capacity += variables_capacity / 2 + 1;
        helper.variables[helper.variables_size - 1].data = a404m_realloc(
            helper.variables[helper.variables_size - 1].data,
            variables_capacity *
                sizeof(*helper.variables[helper.variables_size - 1].data));
      }
      helper.variables[helper.variables_size - 1]
          .data[helper.variables[helper.variables_size - 1].size++] = variable;
    }
    if (!setAllTypes(expr, helper, metadata, NULL)) {
      return false;
    }
  }

  free(helper.variables[helper.variables_size - 1].data);
  return true;
}

AstTreeVariable *setTypesFindVariable(const char *name_begin,
                                      const char *name_end,
                                      AstTreeSetTypesHelper helper,
                                      AstTreeFunctionCall *functionCall) {
  struct {
    AstTreeVariable *var;
    int op;
  } variable = {
      .var = NULL,
      .op = 10,
  };

  const char *str = name_begin;
  const size_t str_size = name_end - name_begin;

  if (functionCall == NULL) {
    for (size_t i = helper.variables_size - 1; i != -1ULL; --i) {
      AstTreeVariables variables = helper.variables[i];
      for (size_t j = variables.size - 1; j != -1ULL; --j) {
        AstTreeVariable *var = variables.data[j];

        const char *var_str = var->name_begin;
        const size_t var_str_size = var->name_end - var->name_begin;

        if (var_str_size != str_size || !strnEquals(var_str, str, str_size)) {
          continue;
        }

        if (!setTypesAstVariable(var, helper)) {
          return NULL;
        }

        variable.var = var;
        variable.op = 0;
        goto END;
        break;
      }
    }
  } else {
    for (size_t i = helper.variables_size - 1; i != -1ULL; --i) {
      AstTreeVariables variables = helper.variables[i];
      for (size_t j = variables.size - 1; j != -1ULL; --j) {
        AstTreeVariable *var = variables.data[j];

        const char *var_str = var->name_begin;
        const size_t var_str_size = var->name_end - var->name_begin;

        if (var_str_size != str_size || !strnEquals(var_str, str, str_size)) {
          continue;
        }

        if (!setTypesAstVariable(var, helper)) {
          return NULL;
        }

        if (var->type->token == AST_TREE_TOKEN_TYPE_FUNCTION) {
          AstTreeTypeFunction *function = var->type->metadata;

          if (!doesFunctionMatch(function, functionCall, helper)) {
            continue;
          }

          if (variable.var != NULL && variable.op == 0) {
            printError(name_begin, name_end,
                       "Multiple candidates found for %.*s",
                       (int)(name_end - name_begin), name_begin);
            return NULL;
          }
          variable.var = var;
          variable.op = 0;
        } else if (var->type->token == AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER) {
          if (variable.op < 1) {
            continue;
          }
          AstTreeShapeShifter *shapeShifter = var->value->metadata;

          if (!doesShapeShifterMatch(shapeShifter, functionCall, helper)) {
            continue;
          }

          if (variable.var != NULL) {
            printError(name_begin, name_end,
                       "Multiple candidates found for %.*s",
                       (int)(name_end - name_begin), name_begin);
            return NULL;
          }
          variable.var = var;
          variable.op = 1;
        } else if (var->type->token == AST_TREE_TOKEN_TYPE_C_FUNCTION) {
          AstTreeCFunctionType *cFunction = var->type->metadata;
          AstTreeTypeFunction *function = cFunction->funcType->metadata;

          if (!doesFunctionMatch(function, functionCall, helper)) {
            continue;
          }

          if (variable.var != NULL && variable.op == 0) {
            printError(name_begin, name_end,
                       "Multiple candidates found for %.*s",
                       (int)(name_end - name_begin), name_begin);
            return NULL;
          }
          variable.var = var;
          variable.op = 0;
        } else if (var->type->token == AST_TREE_TOKEN_TYPE_MACRO) {
          AstTreeMacro *macro = var->value->metadata;
          AstTreeFunction *function =
              copyAstTreeFunction(macro->function, NULL, NULL, 0, true);

          if (!setTypesAstFunction(function, NULL, helper)) {
            astTreeFunctionDestroy(*function);
            free(function);
            return NULL;
          }

          AstTree *functionType = makeTypeOfFunction(function, NULL, NULL);

          bool match =
              doesFunctionMatch(functionType->metadata, functionCall, helper);

          astTreeDelete(functionType);
          astTreeFunctionDestroy(*function);
          free(function);

          if (!match) {
            continue;
          }

          if (variable.var != NULL && variable.op == 0) {
            printError(name_begin, name_end,
                       "Multiple candidates found for %.*s",
                       (int)(name_end - name_begin), name_begin);
            return NULL;
          }
          variable.var = var;
          variable.op = 0;
        }
      }
    }
  }
  if (variable.var == NULL) {
    printError(name_begin, name_end, "No candidates found for %.*s",
               (int)(name_end - name_begin), name_begin);
    return NULL;
  }
END:

  return variable.var;
}

AstTree *getShapeShifterElement(AstTreeFunctionCall *metadata,
                                AstTreeSetTypesHelper helper) {
  if (!isConst(metadata->function)) {
    printError(metadata->function->str_begin, metadata->function->str_end,
               "Shape Shifters must be constant");
    return NULL;
  } else if (metadata->function->token != AST_TREE_TOKEN_VARIABLE) {
    printError(metadata->function->str_begin, metadata->function->str_end,
               "Shape Shifters must be constant variable but got %s",
               AST_TREE_TOKEN_STRINGS[metadata->function->token]);
    return NULL;
  }
  AstTreeVariable *variable = metadata->function->metadata;
  AstTreeShapeShifter *shapeShifter = variable->value->metadata;

  AstTreeFunction *newFunction =
      copyAstTreeFunction(shapeShifter->function, NULL, NULL, 0, true);

  AstTreeFunctionCallParam initedArguments[newFunction->arguments.size];
  size_t initedArguments_size = newFunction->arguments.size;

  for (size_t i = 0; i < initedArguments_size; ++i) {
    initedArguments[i].value = NULL;
  }

  for (size_t i = 0; i < metadata->parameters.size; ++i) {
    AstTreeFunctionCallParam param = metadata->parameters.data[i];
    if (param.nameBegin != param.nameEnd) {
      const size_t param_name_size = param.nameEnd - param.nameBegin;
      for (size_t j = 0; j < newFunction->arguments.size; ++j) {
        AstTreeVariable *arg = newFunction->arguments.data[j];
        if ((size_t)(arg->name_end - arg->name_begin) == param_name_size &&
            strnEquals(arg->name_begin, param.nameBegin, param_name_size)) {
          if (arg->isConst) {
            arg->value = getValue(param.value, true, helper.scope);
          }
          if (hasAnyTypeInside(arg->type)) {
            astTreeDelete(arg->type);
            arg->type = copyAstTree(param.value->type);
          }
          initedArguments[j] = param;
          goto END_OF_NAMED_FOR1;
        }
      }
      printError(param.value->str_begin, param.value->str_end,
                 "Argument not found");
      return NULL;
    }
  END_OF_NAMED_FOR1:
  }

  for (size_t i = 0; i < metadata->parameters.size; ++i) {
    AstTreeFunctionCallParam param = metadata->parameters.data[i];
    if (param.nameBegin == param.nameEnd) {
      for (size_t j = 0; j < newFunction->arguments.size; ++j) {
        AstTreeVariable *arg = newFunction->arguments.data[j];
        if (initedArguments[j].value == NULL) {
          if (arg->isConst) {
            arg->value = getValue(param.value, true, helper.scope);
          }
          if (hasAnyTypeInside(arg->type)) {
            astTreeDelete(arg->type);
            arg->type = copyAstTree(param.value->type);
          }
          initedArguments[j] = param;
          goto END_OF_UNNAMED_FOR1;
        }
      }
      printError(param.value->str_begin, param.value->str_end,
                 "Too many arguments");
      return NULL;
    }
  END_OF_UNNAMED_FOR1:
  }

  for (size_t i = 0; i < newFunction->arguments.size; ++i) {
    AstTreeVariable *arg = newFunction->arguments.data[i];
    if (initedArguments[i].value == NULL) {
      printError(arg->name_begin, arg->name_end, "Argument is not initialized");
      return NULL;
    }
  }

  for (size_t i = 0; i < initedArguments_size; ++i) {
    metadata->parameters.data[i] = initedArguments[i];
  }

  bool found = false;
  size_t element_index;

  for (size_t i = 0; i < shapeShifter->generateds.size; ++i) {
    AstTreeFunctionCallParams call = shapeShifter->generateds.calls[i];
    if (metadata->parameters.size != call.size) {
      continue;
    }

    for (size_t i = 0; i < metadata->parameters.size; ++i) {
      AstTreeFunctionCallParam p0 = metadata->parameters.data[i];
      AstTreeFunctionCallParam p1 = call.data[i];
      if (!typeIsEqual(p0.value->type, p1.value->type, helper.scope)) {
        goto SEARCH_LOOP_CONTINUE;
      }
      if (shapeShifter->function->arguments.data[i]->isConst) {
        AstTree *v0 = getValue(p0.value, true, helper.scope);
        AstTree *v1 = getValue(p1.value, true, helper.scope);

        bool res = isEqual(v0, v1, helper.scope);

        astTreeDelete(v0);
        astTreeDelete(v1);

        if (!res) {
          goto SEARCH_LOOP_CONTINUE;
        }
      }
    }
    element_index = i;
    astTreeFunctionDestroy(*newFunction);
    free(newFunction);
    found = true;
    break;
  SEARCH_LOOP_CONTINUE:
  }

  if (!found) {
    AstTreeVariables variables[helper.variables_size + 1];

    for (size_t i = 0; i < helper.variables_size; ++i) {
      variables[i] = helper.variables[i];
    }

    variables[helper.variables_size].data = newFunction->arguments.data;
    variables[helper.variables_size].size = 0;

    AstTreeSetTypesHelper newHelper = {
        .root = helper.root,
        .variables = variables,
        .variables_size = helper.variables_size + 1,
        .lookingType = NULL,
        .dependencies = helper.dependencies,
        .loops = helper.loops,
        .loops_size = helper.loops_size,
        .scope = helper.scope,
        .isInScope = false,
    };

    for (size_t i = 0; i < newFunction->arguments.size; ++i) {
      AstTreeVariable *var = newFunction->arguments.data[i];
      if (!setTypesAstVariable(var, newHelper)) {
        return NULL;
      }
      newHelper.variables[newHelper.variables_size - 1].size += 1;
    }

    if (!setTypesAstFunction(newFunction, NULL, helper)) {
      return NULL;
    }

    size_t generateds_size =
        a404m_malloc_usable_size(shapeShifter->generateds.calls) /
        sizeof(*shapeShifter->generateds.calls);
    if (generateds_size == shapeShifter->generateds.size) {
      generateds_size += generateds_size / 2 + 1;
      shapeShifter->generateds.functions = a404m_realloc(
          shapeShifter->generateds.functions,
          generateds_size * sizeof(*shapeShifter->generateds.functions));
      shapeShifter->generateds.calls = a404m_realloc(
          shapeShifter->generateds.calls,
          generateds_size * sizeof(*shapeShifter->generateds.calls));
    }
    shapeShifter->generateds.functions[shapeShifter->generateds.size] =
        newFunction;

    shapeShifter->generateds.calls[shapeShifter->generateds.size].size =
        metadata->parameters.size;
    shapeShifter->generateds.calls[shapeShifter->generateds.size].data =
        a404m_malloc(
            sizeof(
                *shapeShifter->generateds.calls[shapeShifter->generateds.size]
                     .data) *
            shapeShifter->generateds.calls[shapeShifter->generateds.size].size);

    for (size_t i = 0;
         i < shapeShifter->generateds.calls[shapeShifter->generateds.size].size;
         ++i) {
      shapeShifter->generateds.calls[shapeShifter->generateds.size]
          .data[i]
          .value = copyAstTree(metadata->parameters.data[i].value);
      shapeShifter->generateds.calls[shapeShifter->generateds.size]
          .data[i]
          .nameBegin = metadata->parameters.data[i].nameBegin;
      shapeShifter->generateds.calls[shapeShifter->generateds.size]
          .data[i]
          .nameEnd = metadata->parameters.data[i].nameEnd;
    }

    element_index = shapeShifter->generateds.size;
    shapeShifter->generateds.size += 1;
  }

  AstTreeShapeShifterElement *element = a404m_malloc(sizeof(*element));
  element->shapeShifter = metadata->function;
  element->index = element_index;
  metadata->function =
      newAstTree(AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT, element, NULL,
                 metadata->function->str_begin, metadata->function->str_end);
  metadata->function->type = makeTypeOf(metadata->function);
  return metadata->function;
}

bool doesFunctionMatch(AstTreeTypeFunction *function,
                       AstTreeFunctionCall *functionCall,
                       AstTreeSetTypesHelper helper) {
  if (function->arguments_size != functionCall->parameters.size) {
    return false;
  }

  AstTreeFunctionCallParam initedArguments[function->arguments_size];
  size_t initedArguments_size = function->arguments_size;

  for (size_t i = 0; i < initedArguments_size; ++i) {
    initedArguments[i].value = NULL;
  }

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
    if (param.nameBegin != param.nameEnd) {
      const size_t param_name_size = param.nameEnd - param.nameBegin;
      for (size_t j = 0; j < function->arguments_size; ++j) {
        AstTreeTypeFunctionArgument arg = function->arguments[j];
        if ((size_t)(arg.name_end - arg.name_begin) == param_name_size &&
            strnEquals(arg.name_begin, param.nameBegin, param_name_size)) {
          if (!typeIsEqual(arg.type, param.value->type, helper.scope) ||
              (arg.isComptime && !isConst(param.value))) {
            return false;
          }
          initedArguments[j] = param;
          goto END_OF_NAMED_FOR;
        }
      }
      return false;
    }
  END_OF_NAMED_FOR:
  }

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
    if (param.nameBegin == param.nameEnd) {
      for (size_t j = 0; j < function->arguments_size; ++j) {
        AstTreeTypeFunctionArgument arg = function->arguments[j];
        if (initedArguments[j].value == NULL) {
          if (!typeIsEqual(arg.type, param.value->type, helper.scope) ||
              (arg.isComptime && !isConst(param.value))) {
            return false;
          }
          initedArguments[j] = param;
          goto END_OF_UNNAMED_FOR;
        }
      }
      return false;
    }
  END_OF_UNNAMED_FOR:
  }

  for (size_t i = 0; i < function->arguments_size; ++i) {
    if (initedArguments[i].value == NULL) {
      return false;
    }
  }

  return true;
}

bool doesShapeShifterMatch(AstTreeShapeShifter *shapeShifter,
                           AstTreeFunctionCall *functionCall,
                           AstTreeSetTypesHelper helper) {
  AstTreeFunction *function = shapeShifter->function;

  if (function->arguments.size != functionCall->parameters.size) {
    return false;
  }

  AstTreeVariables arguments =
      copyAstTreeVariables(function->arguments, NULL, NULL, 0, true);

  AstTreeFunctionCallParam initedArguments[arguments.size];
  size_t initedArguments_size = arguments.size;

  for (size_t i = 0; i < initedArguments_size; ++i) {
    initedArguments[i].value = NULL;
  }

  AstTreeVariables variables[helper.variables_size + 1];

  for (size_t i = 0; i < helper.variables_size; ++i) {
    variables[i] = helper.variables[i];
  }

  variables[helper.variables_size] = arguments;

  AstTreeSetTypesHelper newHelper = {
      .variables = variables,
      .variables_size = helper.variables_size + 1,
      .dependencies = helper.dependencies,
      .lookingType = NULL,
      .root = helper.root,
      .loops = helper.loops,
      .loops_size = helper.loops_size,
      .scope = helper.scope,
      .isInScope = false,
  };

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
    if (param.nameBegin != param.nameEnd) {
      const size_t param_name_size = param.nameEnd - param.nameBegin;
      for (size_t j = 0; j < arguments.size; ++j) {
        AstTreeVariable *arg = arguments.data[j];
        if ((size_t)(arg->name_end - arg->name_begin) == param_name_size &&
            strnEquals(arg->name_begin, param.nameBegin, param_name_size)) {
          if (!setTypesAstVariable(arg, newHelper)) {
            goto RETURN_FALSE;
          }
          if (arg->type != NULL && isConst(arg->type)) {
            if (!typeIsEqual(arg->type, param.value->type, helper.scope) ||
                (arg->isConst && !isConst(param.value))) {
              goto RETURN_FALSE;
            }
            arg->value = copyAstTree(param.value);
            astTreeDelete(arg->type);
            arg->type = copyAstTree(param.value->type);
            initedArguments[j] = param;
            goto END_OF_NAMED_FOR1;
          } else {
            goto RETURN_FALSE;
          }
        } else {
          goto RETURN_FALSE;
        }
      }
      goto RETURN_FALSE;
    }
  END_OF_NAMED_FOR1:
  }

  for (size_t i = 0; i < functionCall->parameters.size; ++i) {
    AstTreeFunctionCallParam param = functionCall->parameters.data[i];
    if (param.nameBegin == param.nameEnd) {
      for (size_t j = 0; j < arguments.size; ++j) {
        AstTreeVariable *arg = arguments.data[j];
        if (!setTypesAstVariable(arg, newHelper)) {
          goto RETURN_FALSE;
        }
        if (arg->type != NULL && isConst(arg->type)) {
          if (initedArguments[j].value == NULL) {
            if (!typeIsEqual(arg->type, param.value->type, helper.scope) ||
                (arg->isConst && !isConst(param.value))) {
              goto RETURN_FALSE;
            }
            arg->value = copyAstTree(param.value);
            astTreeDelete(arg->type);
            arg->type = copyAstTree(param.value->type);
            initedArguments[j] = param;
            goto END_OF_UNNAMED_FOR1;
          }
        } else {
          goto RETURN_FALSE;
        }
      }
      goto RETURN_FALSE;
    }
  END_OF_UNNAMED_FOR1:
  }

  for (size_t i = 0; i < arguments.size; ++i) {
    if (initedArguments[i].value == NULL) {
      goto RETURN_FALSE;
    }
    astTreeVariableDelete(arguments.data[i]);
  }
  free(arguments.data);
  return true;

RETURN_FALSE:
  for (size_t i = 0; i < arguments.size; ++i) {
    astTreeVariableDelete(arguments.data[i]);
  }
  free(arguments.data);
  return false;
}

char *u8ArrayToCString(AstTree *tree) {
  AstTreeScope scope = {
      .expressions = a404m_malloc(0),
      .expressions_size = 0,
      .stackAllocation = a404m_malloc(0),
      .stackAllocation_size = 0,
      .variables.data = a404m_malloc(0),
      .variables.size = 0,
  };

  AstTree *treeValue = getValue(tree, true, &scope);

  if (treeValue->token == AST_TREE_TOKEN_RAW_VALUE ||
      treeValue->token == AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED) {
    AstTreeRawValue *value = treeValue->metadata;
    const size_t size = getSizeOfType(treeValue->type);

    char *newValue = a404m_malloc((size + 1) * sizeof(*newValue));
    memcpy(newValue, value, size);
    newValue[size] = '\0';

    astTreeScopeDestroy(scope);
    astTreeDelete(treeValue);

    return newValue;
  } else if (treeValue->token == AST_TREE_TOKEN_VALUE_OBJECT) {
    AstTreeObject *object = treeValue->metadata;
    char *str = a404m_malloc((object->items_size + 1) * sizeof(*str));
    for (size_t i = 0; i < object->items_size; ++i) {
      str[i] = *(u8 *)object->items[i].value->metadata;
    }

    str[object->items_size] = '\0';

    astTreeScopeDestroy(scope);
    astTreeDelete(treeValue);
    return str;
  }
  printLog("%s", AST_TREE_TOKEN_STRINGS[tree->token]);
  UNREACHABLE;
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

size_t getSizeOfType(AstTree *type) {
  switch (type->token) {
  case AST_TREE_TOKEN_TYPE_VOID:
    return 0;
  case AST_TREE_TOKEN_TYPE_BOOL:
  case AST_TREE_TOKEN_TYPE_I8:
  case AST_TREE_TOKEN_TYPE_U8:
    return 1;
  case AST_TREE_TOKEN_TYPE_I16:
  case AST_TREE_TOKEN_TYPE_U16:
#ifdef FLOAT_16_SUPPORT
  case AST_TREE_TOKEN_TYPE_F16:
#endif
    return 2;
  case AST_TREE_TOKEN_TYPE_I32:
  case AST_TREE_TOKEN_TYPE_U32:
  case AST_TREE_TOKEN_TYPE_F32:
    return 4;
  case AST_TREE_TOKEN_TYPE_CODE:
  case AST_TREE_TOKEN_TYPE_TYPE:
  case AST_TREE_TOKEN_TYPE_ANY_TYPE:
  case AST_TREE_TOKEN_TYPE_FUNCTION:
  case AST_TREE_TOKEN_TYPE_I64:
  case AST_TREE_TOKEN_TYPE_U64:
  case AST_TREE_TOKEN_TYPE_F64:
    return 8;
  case AST_TREE_TOKEN_TYPE_F128:
    return 16;
  case AST_TREE_TOKEN_KEYWORD_STRUCT: {
    AstTreeStruct *metadata = type->metadata;
    size_t size = 0;

    for (size_t i = 0; i < metadata->variables.size; ++i) {
      size += getSizeOfType(metadata->variables.data[i]->type);
    }

    return size;
  }
  case AST_TREE_TOKEN_TYPE_ARRAY: {
    return sizeof(void *) + sizeof(u64);
  }
  case AST_TREE_TOKEN_OPERATOR_POINTER:
    return sizeof(void *);
  case AST_TREE_TOKEN_TYPE_NAMESPACE:
  case AST_TREE_TOKEN_TYPE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_TYPE_C_LIBRARY:
  case AST_TREE_TOKEN_TYPE_C_FUNCTION:
  case AST_TREE_TOKEN_TYPE_MACRO:
  case AST_TREE_TOKEN_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_CAST:
  case AST_TREE_TOKEN_BUILTIN_TYPE_OF:
  case AST_TREE_TOKEN_BUILTIN_SIZE_OF:
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
  case AST_TREE_TOKEN_BUILTIN_PUTC:
  case AST_TREE_TOKEN_BUILTIN_C_LIBRARY:
  case AST_TREE_TOKEN_BUILTIN_C_FUNCTION:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_NOT:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_AND:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_XOR:
  case AST_TREE_TOKEN_BUILTIN_BITWISE_OR:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_LEFT:
  case AST_TREE_TOKEN_BUILTIN_SHIFT_RIGHT:
  case AST_TREE_TOKEN_BUILTIN_INSERT:
  case AST_TREE_TOKEN_KEYWORD_RETURN:
  case AST_TREE_TOKEN_KEYWORD_BREAK:
  case AST_TREE_TOKEN_KEYWORD_CONTINUE:
  case AST_TREE_TOKEN_KEYWORD_IF:
  case AST_TREE_TOKEN_KEYWORD_WHILE:
  case AST_TREE_TOKEN_KEYWORD_COMPTIME:
  case AST_TREE_TOKEN_VALUE_VOID:
  case AST_TREE_TOKEN_FUNCTION_CALL:
  case AST_TREE_TOKEN_VARIABLE:
  case AST_TREE_TOKEN_VARIABLE_DEFINE:
  case AST_TREE_TOKEN_VALUE_NULL:
  case AST_TREE_TOKEN_VALUE_UNDEFINED:
  case AST_TREE_TOKEN_VALUE_NAMESPACE:
  case AST_TREE_TOKEN_VALUE_SHAPE_SHIFTER:
  case AST_TREE_TOKEN_VALUE_C_LIBRARY:
  case AST_TREE_TOKEN_VALUE_C_FUNCTION:
  case AST_TREE_TOKEN_VALUE_MACRO:
  case AST_TREE_TOKEN_VALUE_INT:
  case AST_TREE_TOKEN_VALUE_FLOAT:
  case AST_TREE_TOKEN_VALUE_BOOL:
  case AST_TREE_TOKEN_VALUE_OBJECT:
  case AST_TREE_TOKEN_RAW_VALUE:
  case AST_TREE_TOKEN_RAW_VALUE_NOT_OWNED:
  case AST_TREE_TOKEN_SHAPE_SHIFTER_ELEMENT:
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
  case AST_TREE_TOKEN_OPERATOR_ADDRESS:
  case AST_TREE_TOKEN_OPERATOR_DEREFERENCE:
  case AST_TREE_TOKEN_OPERATOR_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_AND:
  case AST_TREE_TOKEN_OPERATOR_LOGICAL_OR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_NOT:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_AND:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_XOR:
  case AST_TREE_TOKEN_OPERATOR_BITWISE_OR:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_LEFT:
  case AST_TREE_TOKEN_OPERATOR_SHIFT_RIGHT:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ASSIGN:
  case AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS_ADDRESS:
  case AST_TREE_TOKEN_SCOPE:
  case AST_TREE_TOKEN_NONE:
  }
  printLog("%s", AST_TREE_TOKEN_STRINGS[type->token]);
  UNREACHABLE;
}
