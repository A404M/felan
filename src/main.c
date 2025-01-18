#include "compiler/ast-tree.h"
#include "compiler/code-generator.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "utils/file.h"
#include "utils/log.h"
#include <stdio.h>
#include <stdlib.h>

int runWithPrint(const char *filePath) {
  char *code = readWholeFile(filePath);
  if (code == NULL) {
    return 1;
  }

  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    free(code);
    return 1;
  }
  lexerNodeArrayPrint(lexed);

  ParserNode *parsedRoot = parser(lexed);
  if (parsedRoot == NULL) {
    lexerNodeArrayDestroy(lexed);
    free(code);
    return 1;
  }
  parserNodePrint(parsedRoot, 0);
  printf("\n");

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  if (astTree == NULL) {
    parserNodeDelete(parsedRoot);
    lexerNodeArrayDestroy(lexed);
    free(code);
    return 1;
  }
  astTreeRootPrint(astTree);

  CodeGeneratorCodes *codes = codeGenerator(astTree);
  if (codes == NULL) {
    astTreeRootDelete(astTree);
    parserNodeDelete(parsedRoot);
    lexerNodeArrayDestroy(lexed);
    free(code);
    return 1;
  }

  char *fasm = codeGeneratorToFlatASM(codes);
  printf("%s", fasm);
  if (codeGeneratorFlatASMExec("build/test-file", fasm)) {
    system("./build/test-file");
  }

  free(fasm);
  astTreeRootDelete(astTree);
  parserNodeDelete(parsedRoot);
  lexerNodeArrayDestroy(lexed);
  free(code);
  return 0;
}

int run(const char *filePath) {
  char *code = readWholeFile(filePath);
  if (code == NULL) {
    return 1;
  }

  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    free(code);
    return 1;
  }

  ParserNode *parsedRoot = parser(lexed);
  if (parsedRoot == NULL) {
    lexerNodeArrayDestroy(lexed);
    free(code);
    return 1;
  }

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  if (astTree == NULL) {
    parserNodeDelete(parsedRoot);
    lexerNodeArrayDestroy(lexed);
    free(code);
    return 1;
  }

  CodeGeneratorCodes *codes = codeGenerator(astTree);
  if (codes == NULL) {
    astTreeRootDelete(astTree);
    parserNodeDelete(parsedRoot);
    lexerNodeArrayDestroy(lexed);
    free(code);
    return 1;
  }

  char *fasm = codeGeneratorToFlatASM(codes);
  if (codeGeneratorFlatASMExec("build/test-file", fasm)) {
    system("./build/test-file");
  }

  free(fasm);
  astTreeRootDelete(astTree);
  parserNodeDelete(parsedRoot);
  lexerNodeArrayDestroy(lexed);
  free(code);
  return 0;
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  if (argc < 2) {
    printLog("Too few args");
    return 1;
  }

  return runWithPrint(argv[1]);
}
