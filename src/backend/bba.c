#include "bba.h"

BBA makeBBA(AstTreeRoots roots) {
  BBA bba;

  for (size_t i = 0; i < roots.size; ++i) {
    makeBBABack(&bba, roots.data[i]);
  }

  return bba;
}

bool makeBBABack(BBA *bba, AstTreeRoot *root){
  for(size_t i = 0;i < root->variables.size;++i){
    AstTreeVariable *variable = root->variables.data[i];
    
  }
}
