#pragma once
#include "llvm/IR/PassManager.h"

using namespace llvm;
struct PartialCFoldPass: public PassInfoMixin<PartialCFoldPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    static bool isRequired() { return true; }
};
