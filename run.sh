#!/bin/bash
# Usage: ./run.sh <input.txt> <output.txt>
# Example: ./run.sh Testcase/case02.txt case02_out.txt

INPUT="$1"
OUTPUT="$2"

if [ -z "$INPUT" ] || [ -z "$OUTPUT" ]; then
    echo "Usage: $0 <input.txt> <output.txt>"
    exit 1
fi

docker --context desktop-linux run --rm --platform linux/amd64 \
    -v "$(pwd)":/workspace \
    -w /workspace \
    ncku-floorplanner \
    ./NCKUFplanner "$INPUT" "$OUTPUT"
