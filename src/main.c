#include "compiler/ast-tree.h"
#include "runner/runner.h"
#include "utils/file.h"
#include "utils/log.h"
#include <stdio.h>

#ifdef PRINT_STATISTICS
#include "utils/time.h"
#endif

static int run(const char *filePath) {
#ifdef PRINT_STATISTICS
  Time start, end;
  Time lexTime = {0};
  Time parseTime = {0};
  Time astTime;
  Time runTime;
  Time totalTime = {0};
  start = get_time();
#endif
  AstTreeRoots astTrees = makeAstTree(filePath, &lexTime, &parseTime);
  if (astTrees.size == AST_TREE_ROOTS_ERROR.size) {
    return 1;
  }
#ifdef PRINT_STATISTICS
  end = get_time();
  astTime = time_diff(end, start);
  totalTime = time_add(totalTime, astTime);
#endif
#ifdef PRINT_COMPILE_TREE
  for (size_t i = 0; i < astTrees.size; ++i) {
    astTreeRootPrint(astTrees.data[i]);
  }
#endif
#ifdef PRINT_STATISTICS
  start = get_time();
#endif

  int ret;
  if (runAstTree(astTrees)) {
    ret = 0;
  } else {
    ret = 1;
  }
  astTreeRootsDestroy(astTrees);
#ifdef PRINT_STATISTICS
  end = get_time();
  runTime = time_diff(end, start);
  totalTime = time_add(totalTime, runTime);
#endif

#ifdef PRINT_STATISTICS
  astTime = time_diff(astTime, parseTime);
  parseTime = time_diff(parseTime, lexTime);
  printf("\nlexTime:   ");
  time_print(lexTime);
  printf("\nparseTime: ");
  time_print(parseTime);
  printf("\nastTime:   ");
  time_print(astTime);
  printf("\nrunTime:   ");
  time_print(runTime);
  printf("\ntotal:     ");
  time_print(totalTime);
  printf("\n");
#endif

  return ret;
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
