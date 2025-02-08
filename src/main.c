#include "compiler/ast-tree.h"
#include "compiler/code-generator.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "runner/runner.h"
#include "utils/file.h"
#include "utils/log.h"
#include <stdio.h>
#include <stdlib.h>

static int compileRun(const char *filePath, const char *outFilePath,
                      bool print) {
  char *code = readWholeFile(filePath);

  if (code == NULL) {
    return 1;
  }

  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    goto RETURN_ERROR;
  }
  if (print)
    lexerNodeArrayPrint(lexed);

  ParserNode *parsedRoot = parser(lexed);
  lexerNodeArrayDestroy(lexed);
  if (parsedRoot == NULL) {
    goto RETURN_ERROR;
  }
  if (print)
    parserNodePrint(parsedRoot, 0);

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  parserNodeDelete(parsedRoot);
  if (astTree == NULL) {
    goto RETURN_ERROR;
  }
  if (print)
    astTreeRootPrint(astTree);

  CodeGeneratorCodes *codes = codeGenerator(astTree);
  astTreeRootDelete(astTree);
  if (codes == NULL) {
    goto RETURN_ERROR;
  }

  char *fasm = codeGeneratorToFlatASM(codes);
  codeGeneratorDelete(codes);
  free(code);

  if (print)
    puts(fasm);

  if (codeGeneratorFlatASMExec(outFilePath, fasm)) {
    free(fasm);
    return system(outFilePath);
  }

  return 1;

RETURN_ERROR:
  free(code);
  return 1;
}

static int run(const char *filePath, bool print) {
  char *code = readWholeFile(filePath);

  if (code == NULL) {
    return 1;
  }

  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    goto RETURN_ERROR;
  }
  if (print)
    lexerNodeArrayPrint(lexed);

  ParserNode *parsedRoot = parser(lexed);
  lexerNodeArrayDestroy(lexed);
  if (parsedRoot == NULL) {
    goto RETURN_ERROR;
  }
  if (print)
    parserNodePrint(parsedRoot, 0);

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  parserNodeDelete(parsedRoot);
  if (astTree == NULL) {
    goto RETURN_ERROR;
  }
  if (print)
    astTreeRootPrint(astTree);

  if (runAstTree(astTree)) {
    astTreeRootDelete(astTree);
    return 0;
  } else {
    astTreeRootDelete(astTree);
    return 1;
  }

RETURN_ERROR:
  free(code);
  return 1;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    // compileRun("test/main.felan", "build/out", false);
    run("test/main.felan", false);
    printLog("Too few args");
    return 1;
  }

  return run(argv[1], false);
}
