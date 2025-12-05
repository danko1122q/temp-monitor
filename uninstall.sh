#!/bin/bash
# Uninstall script for temp

set -e

PREFIX="${PREFIX:-/usr/local}"
BINDIR="${BINDIR:-$PREFIX/bin}"

echo "Removing $BINDIR/temp..."
sudo rm -f "$BINDIR/temp"

echo "Done."
