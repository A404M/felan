#include "compiler/code-generator.h"
#include "runner/runner.h"
#include "utils/file.h"
#include "utils/log.h"
#include "utils/type.h"
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
    return 1;
  }
  if (print)
    lexerNodeArrayPrint(lexed);

  ParserNode *parsedRoot = parser(lexed);
  lexerNodeArrayDestroy(lexed);
  if (parsedRoot == NULL) {
    return 1;
  }
  if (print)
    parserNodePrint(parsedRoot, 0);

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  parserNodeDelete(parsedRoot);
  if (astTree == NULL) {
    return 1;
  }
  if (print)
    astTreeRootPrint(astTree);

  CodeGeneratorCodes *codes = codeGenerator(astTree);
  astTreeRootDelete(astTree);
  if (codes == NULL) {
    return 1;
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
}

int runWithoutRead(char *code, bool shouldPrint) {
  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    return 1;
  }
  if (shouldPrint)
    lexerNodeArrayPrint(lexed);

  ParserNode *parsedRoot = parser(lexed);
  lexerNodeArrayDestroy(lexed);
  if (parsedRoot == NULL) {
    return 1;
  }
  if (shouldPrint)
    parserNodePrint(parsedRoot, 0);

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  parserNodeDelete(parsedRoot);
  if (astTree == NULL) {
    return 1;
  }
  if (shouldPrint)
    astTreeRootPrint(astTree);

  int ret;
  if (runAstTree(astTree)) {
    ret = 0;
  } else {
    ret = 1;
  }
  astTreeRootDelete(astTree);

  return ret;
}

static int run(const char *filePath, bool shouldPrint) {
  char *code = readWholeFile(filePath);

  if (code == NULL) {
    return 1;
  }

  return runWithoutRead(code, shouldPrint);
}

int main(int argc, char *argv[]) {
  checkTypes();
  fileInit();

  if (argc < 2) {
    // compileRun("test/main.felan", "build/out", false);
    // run("test/main.felan", false);
    printLog("Too few args");
    return 1;
  }

  const int ret = run(argv[1], false);
  fileDelete();
  return ret;
}
