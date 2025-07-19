#!/bin/bash
set -e

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <input_points_file> <planes_file>"
  exit 1
fi

INPUT_FILE="$1"
PLANES_FILE="$2"
BUILD_DIR="build"
VENV_DIR="myenv"

echo "Building the C++ project..."

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
make
cd ..

echo "Setting up Python virtual environment..."

if [ ! -d "$VENV_DIR" ]; then
  python3 -m venv "$VENV_DIR"
fi

source "$VENV_DIR/bin/activate"
pip install --upgrade pip
pip install -r requirements.txt

echo "Running filter on $INPUT_FILE with $PLANES_FILE..."

"./$BUILD_DIR/filter" "$INPUT_FILE" "$PLANES_FILE"

GOOD_POINTS="${INPUT_FILE%.*}_good.txt"

echo "Running visualization on $GOOD_POINTS..."

python visualize.py "$GOOD_POINTS"
