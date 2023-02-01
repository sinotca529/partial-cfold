#include "PartialCFold.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/Compiler.h>
#include <queue>

using namespace llvm;

char const * const PASS_NAME = "pcfold";

bool isConstantFoldable(Instruction const &I, LLVMContext &C) {
    if (I.getOpcode() != Instruction::BinaryOps::Add) return false;

    auto *A0 = dyn_cast_or_null<ConstantInt>(I.getOperand(0));
    auto *A1 = dyn_cast_or_null<ConstantInt>(I.getOperand(1));
    if (!A0 || !A1) return false;

    auto I32 = IntegerType::getInt32Ty(C);
    if (A0->getType() != I32 || A1->getType() != I32) return false;

    return true;
}

PreservedAnalyses PartialCFoldPass::run(Function &F, FunctionAnalysisManager &FAM) {
    errs() << "Visit function : " << F.getName() << "\n";

    auto &C= F.getContext();

    std::queue<Instruction*> Foldables {};
    std::queue<Instruction*> Candidates {};

    for (auto &BB: F) {
        for (auto &I: BB) {
            if (isConstantFoldable(I, C)) {
                Foldables.push(&I);
            }
        }
    }

    while (!Foldables.empty()) {
        auto * const I = Foldables.front();
        Foldables.pop();

        auto const * const A0 = dyn_cast<ConstantInt>(I->getOperand(0));
        auto const * const A1 = dyn_cast<ConstantInt>(I->getOperand(1));
        auto * const Sum = ConstantInt::get(A0->getType(), A0->getValue() + A1->getValue());

        for (auto *U: I->users()) {
            if (auto *UI = dyn_cast_or_null<Instruction>(U)) {
                Candidates.push(UI);
            }
        }

        I->replaceAllUsesWith(Sum);

        while (!Candidates.empty()) {
            auto * const U = Candidates.front();
            Candidates.pop();
            if (isConstantFoldable(*U, C)) {
                Foldables.push(U);
            }
        }

        // I は delete される．
        // この時点で WorkList に I のエイリアスは無いため，
        // dangling pointer は生じない．
        I->eraseFromParent();
    }

    return PreservedAnalyses::none();
}

extern "C" LLVM_ATTRIBUTE_WEAK
PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "PartialCFold", "v0.1",
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](
                    StringRef Name,
                    FunctionPassManager &FPM,
                    ArrayRef<PassBuilder::PipelineElement>
                ) {
                    if (Name == PASS_NAME) {
                        FPM.addPass(PartialCFoldPass());
                        return true;
                    }
                    return false;
                }
            );
        }
    };
}
