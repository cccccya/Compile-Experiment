 #include <iostream>

#include <string>
#include <vector>

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include <fstream>

using namespace llvm;

std::unique_ptr<LLVMContext> theContext;
std::unique_ptr<Module> theModule;
std::unique_ptr<IRBuilder<>> builder;
std::map<std::string, AllocaInst *> namedValues;
std::unique_ptr<legacy::FunctionPassManager> theFPM;

std::map<std::string, AllocaInst *> curNamedValues;

BasicBlock *continueBasicBlock = nullptr;

void InitializeModuleAndPassManager() {
  // Open a new module.
  theContext = std::make_unique<LLVMContext>();
  theModule = std::make_unique<Module>("test", *theContext);

  // theModule->setDataLayout(dL);

  // Create a new builder for the module.
  builder = std::make_unique<IRBuilder<>>(*theContext);

  // Create a new pass manager attached to it.
  theFPM = std::make_unique<legacy::FunctionPassManager>(theModule.get());

  // Promote allocas to registers.
  //theFPM->add(createPromoteMemoryToRegisterPass());
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  //theFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
  //theFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  //theFPM->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  //theFPM->add(createCFGSimplificationPass());

  theFPM->doInitialization();
}

int main(int argc, char *argv[]) {
  // Init
  InitializeModuleAndPassManager();

  //??????????????????putchar
  std::vector<Type *> putArgs;
  putArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *putType =
      FunctionType::get(builder->getInt32Ty(), putArgs, false);
  Function *putFunc = Function::Create(putType, Function::ExternalLinkage,
                                       "putchar", theModule.get());

  //??????????????????
  Type *retType = Type::getInt32Ty(*theContext);
  std::vector<Type *> argsTypes;     //????????????
  std::vector<std::string> argNames; //?????????
  //??????????????????push??????
  //??????????????????
  FunctionType *ft = FunctionType::get(retType, argsTypes, false);
  //????????????
  Function *f =
      Function::Create(ft, Function::ExternalLinkage, "main", theModule.get());
  //??????????????????????????????
  unsigned idx = 0;
  for (auto &arg : f->args()) {
    arg.setName(argNames[idx++]);
  }
  //???????????????????????? ????????????
  BasicBlock *bb = BasicBlock::Create(*theContext, "entry", f);
  builder->SetInsertPoint(bb);
  // ???????????????????????????
  // ??????
  // ????????????????????? a
  AllocaInst *alloca_a =
      builder->CreateAlloca(Type::getInt32Ty(*theContext), nullptr, "a");
  //????????????H
  Value *const_1 = ConstantInt::get(*theContext, APInt(32, 'H', true));
  //?????????
  builder->CreateStore(const_1, alloca_a);
  Function *calleeF = theModule->getFunction("putchar");
  //????????????
  std::vector<Value *> argsV;
  //??????a
  Value *load_a4 =
      builder->CreateLoad(alloca_a->getAllocatedType(), alloca_a, "a");
  argsV.push_back(load_a4);
  //???????????????????????? ????????????
  Value *callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  // begin
  //????????????U
  Value *const_2 = ConstantInt::get(*theContext, APInt(32, 'U', true));
  argsV.pop_back();
  argsV.push_back(const_2);
  callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  //????????????S
  Value *const_3 = ConstantInt::get(*theContext, APInt(32, 'S', true));
  argsV.pop_back();
  argsV.push_back(const_3);
  callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  //????????????T
  Value *const_4 = ConstantInt::get(*theContext, APInt(32, 'T', true));
  argsV.pop_back();
  argsV.push_back(const_4);
  callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  //????????????C
  Value *const_5 = ConstantInt::get(*theContext, APInt(32, 'C', true));
  argsV.pop_back();
  argsV.push_back(const_5);
  callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  //????????????S
  Value *const_6 = ConstantInt::get(*theContext, APInt(32, 'S', true));
  argsV.pop_back();
  argsV.push_back(const_6);
  callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  //????????????E
  Value *const_7 = ConstantInt::get(*theContext, APInt(32, 'E', true));
  argsV.pop_back();
  argsV.push_back(const_7);
  callputchar = builder->CreateCall(calleeF, argsV, "callputchar");
  // end
  //???????????????
  builder->CreateRet(const_1);
  verifyFunction(*f);
  // Run the optimizer on the function.
  // theFPM->run(*f);
  //??????
  theModule->print(outs(), nullptr);

  return 0;
}