# https://hub.docker.com/_/gcc
FROM gcc:latest

# I know where it will be installed !
ENV LLVM_DIR /usr
ENV SRC_DIR /app_src

COPY . $SRC_DIR
WORKDIR $SRC_DIR

RUN apt-get update && apt-get install -y \
    git \
    ninja-build \
    llvm \
    cmake \
    clang


# RUN chmod +x ./build_run.sh
# CMD ["sh", "-c", "./build_run.sh $LLVM_DIR"]

RUN whereis llc
RUN whereis lli
RUN chmod +x ./run.sh
CMD ["sh", "-c", "./run.sh $LLVM_DIR"]