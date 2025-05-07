#pragma once

#include "compiler/parser.h"
#include <time.h>

typedef enum AstTreeToken {
  AST_TREE_TOKEN_FUNCTION,

  AST_TREE_TOKEN_BUILTIN_CAST,
  AST_TREE_TOKEN_BUILTIN_TYPE_OF,
  AST_TREE_TOKEN_BUILTIN_IMPORT,
  AST_TREE_TOKEN_BUILTIN_IS_COMPTIME,
  AST_TREE_TOKEN_BUILTIN_STACK_ALLOC,
  AST_TREE_TOKEN_BUILTIN_HEAP_ALLOC,
  AST_TREE_TOKEN_BUILTIN_NEG,
  AST_TREE_TOKEN_BUILTIN_ADD,
  AST_TREE_TOKEN_BUILTIN_SUB,
  AST_TREE_TOKEN_BUILTIN_MUL,
  AST_TREE_TOKEN_BUILTIN_DIV,
  AST_TREE_TOKEN_BUILTIN_MOD,
  AST_TREE_TOKEN_BUILTIN_EQUAL,
  AST_TREE_TOKEN_BUILTIN_NOT_EQUAL,
  AST_TREE_TOKEN_BUILTIN_GREATER,
  AST_TREE_TOKEN_BUILTIN_SMALLER,
  AST_TREE_TOKEN_BUILTIN_GREATER_OR_EQUAL,
  AST_TREE_TOKEN_BUILTIN_SMALLER_OR_EQUAL,

  AST_TREE_TOKEN_KEYWORD_PUTC,
  AST_TREE_TOKEN_KEYWORD_RETURN,
  AST_TREE_TOKEN_KEYWORD_IF,
  AST_TREE_TOKEN_KEYWORD_WHILE,
  AST_TREE_TOKEN_KEYWORD_COMPTIME,
  AST_TREE_TOKEN_KEYWORD_STRUCT,

  AST_TREE_TOKEN_TYPE_FUNCTION,
  AST_TREE_TOKEN_TYPE_ARRAY,
  AST_TREE_TOKEN_TYPE_TYPE,
  AST_TREE_TOKEN_STATIC_VARS_BEGIN = AST_TREE_TOKEN_TYPE_TYPE,
  AST_TREE_TOKEN_TYPE_VOID,
  AST_TREE_TOKEN_TYPE_I8,
  AST_TREE_TOKEN_TYPE_U8,
  AST_TREE_TOKEN_TYPE_I16,
  AST_TREE_TOKEN_TYPE_U16,
  AST_TREE_TOKEN_TYPE_I32,
  AST_TREE_TOKEN_TYPE_U32,
  AST_TREE_TOKEN_TYPE_I64,
  AST_TREE_TOKEN_TYPE_U64,
#ifdef FLOAT_16_SUPPORT
  AST_TREE_TOKEN_TYPE_F16,
#endif
  AST_TREE_TOKEN_TYPE_F32,
  AST_TREE_TOKEN_TYPE_F64,
  AST_TREE_TOKEN_TYPE_F128,
  AST_TREE_TOKEN_TYPE_CODE,
  AST_TREE_TOKEN_TYPE_BOOL,
  AST_TREE_TOKEN_VALUE_VOID,
  AST_TREE_TOKEN_STATIC_VARS_END = AST_TREE_TOKEN_VALUE_VOID,

  AST_TREE_TOKEN_FUNCTION_CALL,
  AST_TREE_TOKEN_VARIABLE,
  AST_TREE_TOKEN_VARIABLE_DEFINE,
  AST_TREE_TOKEN_VALUE_NULL,
  AST_TREE_TOKEN_VALUE_UNDEFINED,
  AST_TREE_TOKEN_VALUE_INT,
  AST_TREE_TOKEN_VALUE_FLOAT,
  AST_TREE_TOKEN_VALUE_BOOL,
  AST_TREE_TOKEN_VALUE_OBJECT,

  AST_TREE_TOKEN_OPERATOR_ASSIGN,
  AST_TREE_TOKEN_OPERATOR_PLUS,
  AST_TREE_TOKEN_OPERATOR_MINUS,
  AST_TREE_TOKEN_OPERATOR_SUM,
  AST_TREE_TOKEN_OPERATOR_SUB,
  AST_TREE_TOKEN_OPERATOR_MULTIPLY,
  AST_TREE_TOKEN_OPERATOR_DIVIDE,
  AST_TREE_TOKEN_OPERATOR_MODULO,
  AST_TREE_TOKEN_OPERATOR_EQUAL,
  AST_TREE_TOKEN_OPERATOR_NOT_EQUAL,
  AST_TREE_TOKEN_OPERATOR_GREATER,
  AST_TREE_TOKEN_OPERATOR_SMALLER,
  AST_TREE_TOKEN_OPERATOR_GREATER_OR_EQUAL,
  AST_TREE_TOKEN_OPERATOR_SMALLER_OR_EQUAL,
  AST_TREE_TOKEN_OPERATOR_POINTER,
  AST_TREE_TOKEN_OPERATOR_ADDRESS,
  AST_TREE_TOKEN_OPERATOR_DEREFERENCE,
  AST_TREE_TOKEN_OPERATOR_ACCESS,
  AST_TREE_TOKEN_OPERATOR_LOGICAL_NOT,
  AST_TREE_TOKEN_OPERATOR_LOGICAL_AND,
  AST_TREE_TOKEN_OPERATOR_LOGICAL_OR,
  AST_TREE_TOKEN_OPERATOR_ARRAY_ACCESS,

  AST_TREE_TOKEN_SCOPE,

  AST_TREE_TOKEN_NONE,
} AstTreeToken;

extern const char *AST_TREE_TOKEN_STRINGS[];

typedef struct AstTree {
  AstTreeToken token;
  void *metadata;
  struct AstTree *type;
  char const *str_begin;
  char const *str_end;
} AstTree;

extern AstTree AST_TREE_TYPE_TYPE;
extern AstTree AST_TREE_VOID_TYPE;
extern AstTree AST_TREE_BOOL_TYPE;
extern AstTree AST_TREE_I8_TYPE;
extern AstTree AST_TREE_U8_TYPE;
extern AstTree AST_TREE_I16_TYPE;
extern AstTree AST_TREE_U16_TYPE;
extern AstTree AST_TREE_I32_TYPE;
extern AstTree AST_TREE_U32_TYPE;
extern AstTree AST_TREE_I64_TYPE;
extern AstTree AST_TREE_U64_TYPE;
#ifdef FLOAT_16_SUPPORT
extern AstTree AST_TREE_F16_TYPE;
#endif
extern AstTree AST_TREE_F32_TYPE;
extern AstTree AST_TREE_F64_TYPE;
extern AstTree AST_TREE_F128_TYPE;
extern AstTree AST_TREE_CODE_TYPE;
extern AstTree AST_TREE_VOID_VALUE;

typedef struct AstTreeVariable {
  char const *name_begin;
  char const *name_end;
  AstTree *type;
  AstTree *value;
  AstTree *initValue;
  bool isConst;
  bool isLazy;
} AstTreeVariable;

typedef struct AstTreeVariables {
  AstTreeVariable **data;
  size_t size;
} AstTreeVariables;

typedef struct AstTrees {
  AstTree **data;
  size_t size;
} AstTrees;

typedef struct AstTreeRoot {
  char *filePath;
  AstTreeVariables variables;
  AstTrees trees;
  struct AstTreeRoot **imports;
  size_t imports_size;
} AstTreeRoot;

typedef struct AstTreeRoots {
  AstTreeRoot **data;
  size_t size;
} AstTreeRoots;

extern AstTreeRoots AST_TREE_ROOTS_ERROR;

typedef struct AstTreeScope {
  AstTreeVariables variables;
  AstTree **expressions;
  size_t expressions_size;
} AstTreeScope;

typedef struct AstTreeFunction {
  AstTreeVariables arguments;
  AstTreeScope scope;
  AstTree *returnType;
} AstTreeFunction;

typedef struct AstTreeTypeFunctionArgument {
  char const *str_begin;
  char const *str_end;
  char const *name_begin;
  char const *name_end;
  AstTree *type;
} AstTreeTypeFunctionArgument;

typedef struct AstTreeTypeFunction {
  AstTreeTypeFunctionArgument *arguments;
  size_t arguments_size;
  AstTree *returnType;
} AstTreeTypeFunction;

typedef struct AstTreeFunctionCallParam {
  char const *nameBegin;
  char const *nameEnd;
  AstTree *value;
} AstTreeFunctionCallParam;

typedef struct AstTreeFunctionCall {
  AstTree *function;
  AstTreeFunctionCallParam *parameters;
  size_t parameters_size;
} AstTreeFunctionCall;

typedef u64 AstTreeInt;

typedef f128 AstTreeFloat;

typedef bool AstTreeBool;

typedef struct AstTreeObject {
  AstTreeVariables variables;
} AstTreeObject;

typedef AstTree AstTreeSingleChild;

typedef struct AstTreeUnary {
  AstTree *operand;
  AstTreeVariable *function;
} AstTreeUnary;

typedef struct AstTreeInfix {
  AstTree *left;
  AstTree *right;
  AstTreeVariable *function;
} AstTreeInfix;

typedef struct AstTreeReturn {
  AstTree *value;
} AstTreeReturn;

typedef struct AstTreeIf {
  AstTree *condition;
  AstTree *ifBody;
  AstTree *elseBody;
} AstTreeIf;

typedef struct AstTreeWhile {
  AstTree *condition;
  AstTree *body;
} AstTreeWhile;

typedef struct AstTreeHelper {
  AstTreeVariables **variables;
  size_t variables_size;
} AstTreeHelper;

typedef struct AstTreeSetTypesHelper {
  AstTree *lookingType;
  AstTreeVariables dependencies;
  AstTreeVariables variables;
} AstTreeSetTypesHelper;

typedef struct AstTreeStruct {
  size_t id;
  AstTreeVariables variables;
} AstTreeStruct;

typedef struct AstTreeName {
  char const *begin;
  char const *end;
} AstTreeName;

typedef struct AstTreeAccess {
  AstTree *object;
  struct {
    AstTreeName name;
    size_t index;
  } member;
} AstTreeAccess;

typedef struct AstTreeBracket {
  AstTree *operand;
  AstTrees parameters;
} AstTreeBracket;

#ifdef PRINT_COMPILE_TREE
void astTreePrint(const AstTree *tree, int indent);
void astTreeVariablePrint(const AstTreeVariable *variable, int indent);
void astTreeRootPrint(const AstTreeRoot *root);
#endif

void astTreeDestroy(AstTree tree);
void astTreeVariableDestroy(AstTreeVariable variable);
void astTreeVariableDelete(AstTreeVariable *variable);
void astTreeDelete(AstTree *tree);
bool astTreeShouldDelete(AstTree *tree);
void astTreeRootDelete(AstTreeRoot *root);
void astTreeRootsDestroy(AstTreeRoots roots);

AstTree *newAstTree(AstTreeToken token, void *metadata, AstTree *type,
                    char const *str_begin, char const *str_end);
AstTree *copyAstTree(AstTree *tree);
AstTree *copyAstTreeBack(AstTree *tree, AstTreeVariables oldVariables[],
                         AstTreeVariables newVariables[],
                         size_t variables_size);
AstTreeVariable *copyAstTreeBackFindVariable(AstTreeVariable *variable,
                                             AstTreeVariables oldVariables[],
                                             AstTreeVariables newVariables[],
                                             size_t variables_size);
AstTreeVariables copyAstTreeVariables(AstTreeVariables variables,
                                      AstTreeVariables oldVariables[],
                                      AstTreeVariables newVariables[],
                                      size_t variables_size);

AstTreeRoots makeAstTree(const char *filePath
#ifdef PRINT_STATISTICS
                         ,
                         struct timespec *lexingTime,
                         struct timespec *parsingTime
#endif
);
AstTreeRoot *getAstTreeRoot(char *filePath, AstTreeRoots *roots
#ifdef PRINT_STATISTICS
                            ,
                            struct timespec *lexingTime,
                            struct timespec *parsingTime
#endif
);
AstTreeRoot *makeAstRoot(const ParserNode *parsedRoot, char *filePath);

bool pushVariable(AstTreeHelper *helper, AstTreeVariables *variables,
                  AstTreeVariable *variable);

AstTree *astTreeParse(const ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseFunction(const ParserNode *parserNode,
                              AstTreeHelper *helper);
AstTree *astTreeParseTypeFunction(const ParserNode *parserNode,
                                  AstTreeHelper *helper);
AstTree *astTreeParseFunctionCall(const ParserNode *parserNode,
                                  AstTreeHelper *helper);
AstTree *astTreeParseIdentifier(const ParserNode *parserNode,
                                AstTreeHelper *helper);
AstTree *astTreeParseValue(const ParserNode *parserNode, AstTreeToken token,
                           size_t metadata_size, AstTree *type);
AstTree *astTreeParseString(const ParserNode *parserNode,
                            AstTreeHelper *helper);
AstTree *astTreeParseKeyword(const ParserNode *parserNode, AstTreeToken token);
AstTree *astTreeParsePrintU64(const ParserNode *parserNode,
                              AstTreeHelper *helper);
AstTree *astTreeParseReturn(const ParserNode *parserNode,
                            AstTreeHelper *helper);
AstTree *astTreeParseBinaryOperator(const ParserNode *parserNode,
                                    AstTreeHelper *helper, AstTreeToken token);
AstTree *astTreeParseUnaryOperator(const ParserNode *parserNode,
                                   AstTreeHelper *helper, AstTreeToken token);
AstTree *astTreeParseUnaryOperatorSingleChild(const ParserNode *parserNode,
                                              AstTreeHelper *helper,
                                              AstTreeToken token);
AstTree *astTreeParseOperateAssignOperator(const ParserNode *parserNode,
                                           AstTreeHelper *helper,
                                           AstTreeToken token);
bool astTreeParseConstant(const ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseVariable(const ParserNode *parserNode,
                              AstTreeHelper *helper);
AstTree *astTreeParseIf(const ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseWhile(const ParserNode *parserNode, AstTreeHelper *helper);
AstTree *astTreeParseComptime(const ParserNode *parserNode,
                              AstTreeHelper *helper);
AstTree *astTreeParseCurlyBracket(const ParserNode *parserNode,
                                  AstTreeHelper *helper);
AstTree *astTreeParseParenthesis(const ParserNode *parserNode,
                                 AstTreeHelper *helper);
AstTree *astTreeParseStruct(const ParserNode *parserNode,
                            AstTreeHelper *helper);
AstTree *astTreeParseAccessOperator(const ParserNode *parserNode,
                                    AstTreeHelper *helper, AstTreeToken token);
AstTree *astTreeParseBracket(const ParserNode *parserNode,
                             AstTreeHelper *helper, AstTreeToken token);

bool isFunction(AstTree *value);
bool isConst(AstTree *tree);
bool isConstByValue(AstTree *tree);
AstTree *makeTypeOf(AstTree *value);
bool typeIsEqual(AstTree *type0, AstTree *type1);
bool typeIsEqualBack(const AstTree *type0, const AstTree *type1);
AstTree *getValue(AstTree *tree);
bool isIntType(AstTree *type);
bool isEqual(AstTree *left, AstTree *right);
bool isEqualVariable(AstTreeVariable *left, AstTreeVariable *right);

void allOfVariablesWithImport(AstTreeVariables *variables, AstTreeRoot *root,
                              AstTreeRoots *checkedRoots);
bool setAllTypesRoot(AstTreeRoot *root);
bool setAllTypes(AstTree *tree, AstTreeSetTypesHelper helper,
                 AstTreeFunction *function, AstTreeFunctionCall *functionCall);
bool setTypesValueBool(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueInt(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueFloat(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueNull(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueUndefined(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesValueObject(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesFunction(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesPrintU64(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesReturn(AstTree *tree, AstTreeSetTypesHelper helper,
                    AstTreeFunction *function);
bool setTypesTypeFunction(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesFunctionCall(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesVariable(AstTree *tree, AstTreeSetTypesHelper helper,
                      AstTreeFunctionCall *functionCall);
bool setTypesOperatorAssign(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorInfix(AstTree *tree, AstTreeSetTypesHelper helper,
                           const char *str, size_t str_size);
bool setTypesOperatorInfixWithRet(AstTree *tree, AstTree *retType,
                                  AstTreeSetTypesHelper helper);
bool setTypesOperatorInfixWithRetAndLooking(AstTree *tree, AstTree *lookingType,
                                            AstTree *retType,
                                            AstTreeSetTypesHelper helper);
bool setTypesOperatorUnary(AstTree *tree, AstTreeSetTypesHelper helper,
                           const char *funcStr, size_t funcStr_size);
bool setTypesOperatorPointer(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorAddress(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorDereference(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesVariableDefine(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesIf(AstTree *tree, AstTreeSetTypesHelper helper,
                AstTreeFunction *function);
bool setTypesWhile(AstTree *tree, AstTreeSetTypesHelper helper,
                   AstTreeFunction *function);
bool setTypesScope(AstTree *tree, AstTreeSetTypesHelper helper,
                   AstTreeFunction *function);
bool setTypesComptime(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesStruct(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesOperatorAccess(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesBuiltinCast(AstTree *tree, AstTreeSetTypesHelper helper,
                         AstTreeFunctionCall *functionCall);
bool setTypesBuiltinTypeOf(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall);
bool setTypesBuiltinImport(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall);
bool setTypesBuiltinIsComptime(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesBuiltinStackAlloc(AstTree *tree, AstTreeSetTypesHelper helper,
                               AstTreeFunctionCall *functionCall);
bool setTypesBuiltinHeapAlloc(AstTree *tree, AstTreeSetTypesHelper helper,
                              AstTreeFunctionCall *functionCall);
bool setTypesBuiltinUnary(AstTree *tree, AstTreeSetTypesHelper helper,
                          AstTreeFunctionCall *functionCall);
bool setTypesBuiltinBinary(AstTree *tree, AstTreeSetTypesHelper helper,
                           AstTreeFunctionCall *functionCall);
bool setTypesBuiltinBinaryWithRet(AstTree *tree, AstTreeSetTypesHelper helper,
                                  AstTreeFunctionCall *functionCall,
                                  AstTree *retType);
bool setTypesTypeArray(AstTree *tree, AstTreeSetTypesHelper helper);
bool setTypesArrayAccess(AstTree *tree, AstTreeSetTypesHelper helper);

bool setTypesAstVariable(AstTreeVariable *variable,
                         AstTreeSetTypesHelper helper);
bool setTypesAstInfix(AstTreeInfix *infix, AstTreeSetTypesHelper helper);

AstTreeVariable *setTypesFindVariable(const char *name_begin,
                                      const char *name_end,
                                      AstTreeSetTypesHelper helper,
                                      AstTreeFunctionCall *functionCall);

char *u8ArrayToCString(AstTree *tree);

AstTree *makeStringType();
