#!/bin/bash
# Install script for temp

set -e

PREFIX="${PREFIX:-/usr/local}"
BINDIR="${BINDIR:-$PREFIX/bin}"

echo "Building..."
make clean
make

echo "Installing to $BINDIR..."
sudo install -D -m 755 bin/temp "$BINDIR/temp"

echo "Done. Run: temp"
