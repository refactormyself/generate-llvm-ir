#include "generator.hpp"
#include "llvm/Support/raw_ostream.h"
#include <map>

using namespace dragon;
using namespace llvm;

// Context is roughly like the namespace
std::unique_ptr<LLVMContext> IRGenerator::TheContext;

// The IR Builder is a helper for building the llvm IR, it helps us call the necessary API and do some dirty works too
std::unique_ptr<IRBuilder<>> IRGenerator::Builder;
// No folding means, don;t do any form of JIT'ing
std::unique_ptr<IRBuilder<NoFolder>> IRGenerator::BuilderNoFold;

// Everything in the IR code must belong to a Module and modules can be nested.
std::unique_ptr<Module> IRGenerator::MainModule;


IRGenerator::IRGenerator() {
    TheContext = std::make_unique<LLVMContext>();
    Builder = std::make_unique<IRBuilder<>>(*TheContext);
    BuilderNoFold = std::make_unique<IRBuilder<NoFolder>>( *TheContext);
    MainModule = std::make_unique<Module>("Main module", *TheContext);

}

Function * IRGenerator::CreateProtoType(const std::string &fnName, const std::vector<std::string> &argList) const {
    // create the parameters in context
    std::vector<Type *> parameters(argList.size(), Type::getInt64Ty(*TheContext));

    // create the function in context
    FunctionType *FT = FunctionType::get(Type::getInt64Ty(*TheContext), parameters, false);

    // create the function inside the module. This incantation is enough to generate the llvm IR code for the prototype
    auto function = Function::Create(FT, Function::ExternalLinkage, fnName, MainModule.get());

    // Set up the names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg: function->args())
        Arg.setName(argList[Idx++]);

    return function;
}

// This will create code for funct(x) = x*x;
Function * IRGenerator::CreateSquareFunction(){
    std::string fnName = "square";
    std::vector<std::string> argList { "x" };
    std::map<std::string, llvm::AllocaInst *> NamedValues;

    // First, we see if the function has already been added to the module.
    auto *function = MainModule->getFunction(fnName);
    if (!function) {  // we have to create its prototype
        function = CreateProtoType(fnName, argList);
    }

    // check again
    if (!function){
        errs() << "FATAL ERROR, while trying to create the function.";
        return nullptr;
    }

    // Create a Basic Block where we will insert the llvm IR instructions.
    BasicBlock *BBlock = BasicBlock::Create(*TheContext, "entry", function);
    Builder->SetInsertPoint(BBlock);

    // allocate memory for all arguments to the function (only 1 in this case)
    for (auto &Arg: function->args()) {
        // Create an alloca for this variable argument using a temporary IRBuilder
        IRBuilder<> tempBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
        AllocaInst *Alloca = tempBuilder.CreateAlloca(Type::getInt64Ty(*TheContext),
                                 0,
                                 Arg.getName());

        // Store the initial value into the alloca.
        Builder->CreateStore(&Arg, Alloca);

        // Add arguments to variable symbol table.
        NamedValues[Arg.getName().str()] = Alloca;
    }

        // There is only 1 parameter, so retrieve it from the NamedValues in the function.
        auto variable = NamedValues[argList[0]];

        // Load the value.
        auto value = Builder->CreateLoad(Type::getInt64Ty(*TheContext), variable, argList[0]);

    // Generate the instructions for x * x
    auto result = Builder->CreateMul(value, value, "square");

    // Finish off the function.
    Builder->CreateRet(result);

    // Validate the generated code, checking for consistency.
    verifyFunction(*function);
    return function;
}

// This will create code for funct(x, y) = x*x + y*y;
Function * IRGenerator::CreateSumOfSquaresFunction(){
    std::string fnName = "sumsquares";
    std::vector<std::string> argList { "x", "y" };
    std::map<std::string, llvm::AllocaInst *> NamedValues;

    // First, we see if the function has already been added to the module.
    auto *function = MainModule->getFunction(fnName);
    if (!function) {  // we have to create its prototype
        function = CreateProtoType(fnName, argList);
    }

    // check again
    if (!function){
        errs() << "FATAL ERROR, while trying to create the function.";
        return nullptr;
    }

    // Create a Basic Block where we will insert the llvm IR instructions.
    BasicBlock *BBlock = BasicBlock::Create(*TheContext, "entry", function);
    Builder->SetInsertPoint(BBlock);

    // allocate memory for all arguments to the function (only 1 in this case)
    for (auto &Arg: function->args()) {
        // Create an alloca for this variable argument using a temporary IRBuilder
        IRBuilder<> tempBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
        AllocaInst *Alloca = tempBuilder.CreateAlloca(Type::getInt64Ty(*TheContext),
                                                      0,
                                                      Arg.getName());

        // Store the initial value into the alloca.
        Builder->CreateStore(&Arg, Alloca);

        // Add arguments to variable symbol table.
        NamedValues[Arg.getName().str()] = Alloca;
    }

    // There are 2 parameters, so retrieve it from the NamedValues in the function.
    auto variableX = NamedValues[argList[0]];
    auto variableY = NamedValues[argList[1]];

    // Load the values.
    auto valueX = Builder->CreateLoad(Type::getInt64Ty(*TheContext), variableX, argList[0]);
    auto valueY = Builder->CreateLoad(Type::getInt64Ty(*TheContext), variableY, argList[1]);


    // Call the square() function
    // Get square() from the module otw bail out.
    auto *squareFun = MainModule->getFunction("square");
    if (!squareFun) {
        errs() << "FATAL ERROR, square() is not available.";
        function->eraseFromParent(); // remove sumsquares() from the Module
        return nullptr;
    }

//    Alternatively you can do the square right here
//    // Generate the instructions for x*x and y*y
//    auto squareX = Builder->CreateMul(valueX, valueX, "square_x");
//    auto squareY = Builder->CreateMul(valueY, valueY, "square_y");

    auto squareX = Builder->CreateCall(squareFun, {valueX}, "callsqr");
    auto squareY = Builder->CreateCall(squareFun, {valueY}, "callsqr");

    // Generate instructions to sum the squares
    auto sumsquares = Builder->CreateAdd(squareX, squareY, "sumsqrs");

    // Finish off the function.
    Builder->CreateRet(sumsquares);

    // Validate the generated code, checking for consistency.
    verifyFunction(*function);
    return function;
}

std::string IRGenerator::GetSquareFuncIR() {
    auto function = CreateSquareFunction();
    if (!function)
        return "";

    std::string functionStr;
    raw_string_ostream ostream(functionStr);
    ostream << *function;
    ostream.flush();

    return functionStr;
}

std::string IRGenerator::GetSumOfSquareFuncIR() {
    Function *function = CreateSumOfSquaresFunction();
    if (!function)
        return "";

    std::string functionStr;
    raw_string_ostream ostream(functionStr);
    ostream << *function;
    ostream.flush();

    return functionStr;
}

std::string IRGenerator::GetMainModuleIR() {
    if (!MainModule)
        return "";

    CreateSquareFunction();
    CreateSumOfSquaresFunction();
    CreateFiveFactorialFunction(true);
    CreateFiveFactorialFunction(false);

    std::string moduleStr;
    raw_string_ostream ostream(moduleStr);
    ostream << *MainModule;
    ostream.flush();

    return moduleStr;
}

// Thing creates a function: 1 * 2 * 3 * 4 * 5
void IRGenerator::CreateFiveFactorialFunction(bool allowConstFold) {
    std::string fnName = allowConstFold ? "fivefactorial" : "fivefactorialnofold";

    // First, we see if the function has already been added to the module.
    auto *function = MainModule->getFunction(fnName);
    if (!function) {  // we have to create its prototype
        function = CreateProtoType(fnName, {});
    }

    // check again
    if (!function){
        errs() << "FATAL ERROR, while trying to create the function.";
        return;
    }

    // Create a Basic Block where we will insert the llvm IR instructions.
    BasicBlock *BBlock = BasicBlock::Create(*TheContext, "entry", function);
    Builder->SetInsertPoint(BBlock);

    // create constants: 1, 2, 3, 4, 5
    auto one = ConstantInt::get(*TheContext, APInt(64, 1, false));
    auto two = ConstantInt::get(*TheContext, APInt(64, 2, false));
    auto three = ConstantInt::get(*TheContext, APInt(64, 3, false));
    auto four = ConstantInt::get(*TheContext, APInt(64, 4, false));
    auto five = ConstantInt::get(*TheContext, APInt(64, 5, false));

    if (allowConstFold) {
        // Generate the instructions for 1 * 2 * 3 * 4 * 5
        auto one_two = Builder->CreateMul(one, two, "mul");
        auto three_four = Builder->CreateMul(three, four, "mul");
        auto five_fact = Builder->CreateMul(five, one_two, "mul");
        five_fact = Builder->CreateMul(five_fact, three_four, "mul");

        // Finish off the function.
        Builder->CreateRet(five_fact);
    } else {
        // TODO: 'BuilderNoFold' will not work here ?!
        IRBuilder<NoFolder> nofoldBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());

        // Generate the instructions for 1 * 2 * 3 * 4 * 5
        auto one_two = nofoldBuilder.CreateMul(one, two, "mul");
        auto three_four = nofoldBuilder.CreateMul(three, four, "mul");
        auto five_onetwo = nofoldBuilder.CreateMul(five, one_two, "mul");
        auto five_fact = nofoldBuilder.CreateMul(five_onetwo, three_four, "mul");

        // Finish off the function.
        nofoldBuilder.CreateRet(five_fact);
    }
    // Validate the generated code, checking for consistency.
    verifyFunction(*function);
}