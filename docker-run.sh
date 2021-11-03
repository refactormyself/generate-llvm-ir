#!/usr/bin/env bash

DCKR_FILE=./x86.Dockerfile
DCKR_IMAGE="generate-lvm-ir"
DCKR_CONTAINER="running-gen-llvm-ir"

docker build -t $DCKR_IMAGE -f $DCKR_FILE .
docker run --rm \
        --name $DCKR_CONTAINER \
        $DCKR_IMAGE

#        -v "${PWD}:/app_src" -w "/app_src" \
