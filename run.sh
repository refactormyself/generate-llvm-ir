#!/usr/bin/env bash

# hey cmake, use clang
# export CC=clang
# export CXX=clang++

SRC_DIR=$PWD
LLVM_INSTALL=$1

if [ ! -f "$LLVM_INSTALL/bin/llc" ]; then
  echo "Cannot find LLVM installation. Please pass in the directory of LLVM installation."
  echo "Expecting: \$LLVM_INSTALL/bin/llc"
  exit
fi

# build app and generate the llvm IR code
# rm -rf build
mkdir -p build/test
cd build || exit

cmake .. -DLLVM_DIR=$LLVM_INSTALL/lib/cmake/llvm 
cmake --build .

./gen-llvm-ir

if [ ! -f "./test/OutputFile.ll" ]; then
  echo "Cannot find the generated IR. Expecting ./test/OutputFile.ll"
  exit
fi

# generate assembly file from the LLVM IR
echo; $LLVM_INSTALL/bin/llc -o "./test/Output.s" "./test/OutputFile.ll"
file "./test/Output.s"

# generate linkable shared object file from the LLVM IR
echo; $LLVM_INSTALL/bin/clang++ -shared -fPIC  -o "./test/Output.so" "./test/OutputFile.ll"
file "./test/Output.so"
echo

# linked to the object files and use it
$LLVM_INSTALL/bin/clang++ -o tested-so "$SRC_DIR/test/call_compiled_ir.cpp" "./test/Output.so"
./tested-so

$LLVM_INSTALL/bin/clang++ -o tested-s "$SRC_DIR/test/call_compiled_ir.cpp" "./test/Output.s"
./tested-s

cd $SRC_DIR