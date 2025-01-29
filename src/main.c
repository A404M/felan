#include "compiler/ast-tree.h"
#include "compiler/code-generator.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "utils/file.h"
#include "utils/log.h"
#include <stdio.h>
#include <stdlib.h>

static int runWithPrint(const char *filePath, const char *outFilePath) {
  int ret = 1;
  char *code = readWholeFile(filePath);
  if (code != NULL) {
    LexerNodeArray lexed = lexer(code);
    if (!lexerNodeArrayIsError(lexed)) {
      lexerNodeArrayPrint(lexed);
      ParserNode *parsedRoot = parser(lexed);
      if (parsedRoot != NULL) {
        parserNodePrint(parsedRoot, 0);
        AstTreeRoot *astTree = makeAstTree(parsedRoot);
        if (astTree != NULL) {
          astTreeRootPrint(astTree);
          CodeGeneratorCodes *codes = codeGenerator(astTree);
          if (codes != NULL) {
            char *fasm = codeGeneratorToFlatASM(codes);
            printf("%s", fasm);
            if (codeGeneratorFlatASMExec(outFilePath, fasm)) {
              ret = system(outFilePath);
            }
            free(fasm);
            codeGeneratorDelete(codes);
          }
          astTreeRootDelete(astTree);
        }
        parserNodeDelete(parsedRoot);
      }
      lexerNodeArrayDestroy(lexed);
    }
    free(code);
  }
  return ret;
}

static int run(const char *filePath, const char *outFilePath) {
  int ret = 1;
  char *code = readWholeFile(filePath);
  if (code != NULL) {
    LexerNodeArray lexed = lexer(code);
    if (!lexerNodeArrayIsError(lexed)) {
      ParserNode *parsedRoot = parser(lexed);
      if (parsedRoot != NULL) {
        AstTreeRoot *astTree = makeAstTree(parsedRoot);
        if (astTree != NULL) {
          CodeGeneratorCodes *codes = codeGenerator(astTree);
          if (codes != NULL) {
            char *fasm = codeGeneratorToFlatASM(codes);
            if (codeGeneratorFlatASMExec(outFilePath, fasm)) {
              ret = system(outFilePath);
            }
            free(fasm);
            codeGeneratorDelete(codes);
          }
          astTreeRootDelete(astTree);
        }
        parserNodeDelete(parsedRoot);
      }
      lexerNodeArrayDestroy(lexed);
    }
    free(code);
  }
  return ret;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    run("test/main.felan", "build/out");
    printLog("Too few args");
    return 1;
  }

  return runWithPrint(argv[1], argv[2]);
}
