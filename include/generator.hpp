#ifndef GEN_LLVM_IR_GENERATOR_HPP
#define GEN_LLVM_IR_GENERATOR_HPP

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/NoFolder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"

namespace dragon {
    class IRGenerator {
        static std::unique_ptr<llvm::LLVMContext> TheContext;
        static std::unique_ptr<llvm::IRBuilder<>> Builder;
        static std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> BuilderNoFold;
        static std::unique_ptr<llvm::Module> MainModule;

        void CreateFiveFactorialFunction(bool allowConstFold);
        llvm::Function * CreateSquareFunction();
        llvm::Function * CreateSumOfSquaresFunction();
        llvm::Function * CreateProtoType(const std::string &fnName, const std::vector<std::string> &argList) const;
    public:
        IRGenerator();
        std::string GetMainModuleIR();
        std::string GetSquareFuncIR();
        std::string GetSumOfSquareFuncIR();

    };
};


#endif //GEN_LLVM_IR_GENERATOR_HPP
