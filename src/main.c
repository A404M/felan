#include "runner/runner.h"
#include "utils/file.h"
#include "utils/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #define PRINT_COMPILE_TREE
// #define PRINT_STATISTICS

#ifdef PRINT_STATISTICS
static struct timespec diff(struct timespec end, struct timespec start) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return temp;
}

static struct timespec add(struct timespec left, struct timespec right) {
  struct timespec result;
  result.tv_nsec = left.tv_nsec + right.tv_nsec;
  result.tv_sec = (left.tv_sec + right.tv_sec) + result.tv_nsec / 1000000000;
  result.tv_nsec %= 1000000000;
  return result;
}

static void printTime(struct timespec time) {
  printf("%02ld:%02ld.%09ldns", time.tv_sec / 60, time.tv_sec % 60, time.tv_nsec);
}
#endif

static int runWithoutRead(char *code) {
#ifdef PRINT_STATISTICS
  struct timespec start, end;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
  struct timespec lexTime;
  struct timespec parseTime;
  struct timespec astTime;
  struct timespec runTime;
  struct timespec totalTime = {0};
#endif
  LexerNodeArray lexed = lexer(code);
  if (lexerNodeArrayIsError(lexed)) {
    return 1;
  }
#ifdef PRINT_STATISTICS
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
  lexTime = diff(end, start);
  totalTime = add(totalTime, lexTime);
#endif
#ifdef PRINT_COMPILE_TREE
  lexerNodeArrayPrint(lexed);
#endif
#ifdef PRINT_STATISTICS
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
#endif

  ParserNode *parsedRoot = parser(lexed);
  lexerNodeArrayDestroy(lexed);
  if (parsedRoot == NULL) {
    return 1;
  }
#ifdef PRINT_STATISTICS
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
  parseTime = diff(end, start);
  totalTime = add(totalTime, parseTime);
#endif
#ifdef PRINT_COMPILE_TREE
  parserNodePrint(parsedRoot, 0);
#endif
#ifdef PRINT_STATISTICS
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
#endif

  AstTreeRoot *astTree = makeAstTree(parsedRoot);
  parserNodeDelete(parsedRoot);
  if (astTree == NULL) {
    return 1;
  }
#ifdef PRINT_STATISTICS
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
  astTime = diff(end, start);
  totalTime = add(totalTime, astTime);
#endif
#ifdef PRINT_COMPILE_TREE
  astTreeRootPrint(astTree);
#endif
#ifdef PRINT_STATISTICS
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
#endif

  int ret;
  if (runAstTree(astTree)) {
    ret = 0;
  } else {
    ret = 1;
  }
  astTreeRootDelete(astTree);
#ifdef PRINT_STATISTICS
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
  runTime = diff(end, start);
  totalTime = add(totalTime, runTime);
#endif

#ifdef PRINT_STATISTICS
  printf("\nlexTime:   ");
  printTime(lexTime);
  printf("\nparseTime: ");
  printTime(parseTime);
  printf("\nastTime:   ");
  printTime(astTime);
  printf("\nrunTime:   ");
  printTime(runTime);
  printf("\ntotal:     ");
  printTime(totalTime);
  printf("\n");
#endif

  return ret;
}

static int run(const char *filePath) {
  char *code = readWholeFile(filePath);

  if (code == NULL) {
    return 1;
  }

  return runWithoutRead(code);
}

int main(int argc, char *argv[]) {
  fileInit();

  if (argc < 2) {
    // compileRun("test/main.felan", "build/out", false);
    // run("test/main.felan", false);
    printLog("Too few args");
    return 1;
  }

  const int ret = run(argv[1]);
  fileDelete();
  return ret;
}
