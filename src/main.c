#include "compiler/ast-tree.h"
#include "compiler/code-generator.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "utils/file.h"
#include "utils/log.h"
#include <stdio.h>
#include <stdlib.h>

static int runWithPrint(const char *filePath, const char *outFilePath) {
  char *code = readWholeFile(filePath);

  if (code == NULL) {
    return 1;
  }

  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    goto RETURN_ERROR;
  }
  lexerNodeArrayPrint(lexed);

  ParserNode *parsedRoot = parser(lexed);
  lexerNodeArrayDestroy(lexed);
  if (parsedRoot == NULL) {
    goto RETURN_ERROR;
  }
  parserNodePrint(parsedRoot, 0);

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  parserNodeDelete(parsedRoot);
  if (astTree == NULL) {
    goto RETURN_ERROR;
  }
  astTreeRootPrint(astTree);

  CodeGeneratorCodes *codes = codeGenerator(astTree);
  astTreeRootDelete(astTree);
  if (codes == NULL) {
    goto RETURN_ERROR;
  }

  char *fasm = codeGeneratorToFlatASM(codes);
  codeGeneratorDelete(codes);
  free(code);

  printf("%s", fasm);

  if (codeGeneratorFlatASMExec(outFilePath, fasm)) {
    free(fasm);
    return system(outFilePath);
  }

  return 1;

RETURN_ERROR:
  free(code);
  return 1;
}

static int run(const char *filePath, const char *outFilePath) {
  char *code = readWholeFile(filePath);

  if (code == NULL) {
    return 1;
  }

  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    goto RETURN_ERROR;
  }

  ParserNode *parsedRoot = parser(lexed);
  lexerNodeArrayDestroy(lexed);
  if (parsedRoot == NULL) {
    goto RETURN_ERROR;
  }

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  parserNodeDelete(parsedRoot);
  if (astTree == NULL) {
    goto RETURN_ERROR;
  }

  CodeGeneratorCodes *codes = codeGenerator(astTree);
  astTreeRootDelete(astTree);
  if (codes == NULL) {
    goto RETURN_ERROR;
  }

  char *fasm = codeGeneratorToFlatASM(codes);
  codeGeneratorDelete(codes);
  free(code);

  if (codeGeneratorFlatASMExec(outFilePath, fasm)) {
    free(fasm);
    return system(outFilePath);
  }

  return 1;

RETURN_ERROR:
  free(code);
  return 1;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    run("test/main.felan", "build/out");
    printLog("Too few args");
    return 1;
  }

  return runWithPrint(argv[1], argv[2]);
}
