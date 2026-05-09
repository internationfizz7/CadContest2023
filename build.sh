#!/bin/bash
# Compile NCKUFplanner inside a Linux Docker container
# Usage: ./build.sh

docker --context desktop-linux run --rm --platform linux/amd64 \
    -v "$(pwd)":/workspace \
    -w /workspace \
    ubuntu:22.04 \
    bash -c "apt-get update -q && apt-get install -y -q g++ make && make clean && make"
