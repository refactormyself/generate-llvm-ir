## Generate LLVM IR from C++
The  odf this project is to make a point that the LLVM IR is just another (low level) programming language. We can write be hand or more appropriately generate it using tools provided by the LLVM framework.

- This app generates llvm IR code for two functions: `square(x)` and `sumsquares(x, y)`.
- The IR code the compiled using *llc* tool to produce a shared object
- The shared object is then linked to from an app (a googletest suite)


### TO RUN

- Normal: `sh ./run.sh $PATH_TO_LLVM_INSTALL`
- Docker: `sh .docker-run.sh`


### Concepts donmstrated in this project
- Using the *IRBuilder* for LLVM IR code generation
- Constant Folding
- Compiling an LLVM IR code
- Docker build