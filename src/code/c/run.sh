#!/usr/bin/env bash
set -euo pipefail

usage() {
  echo "Usage: $0 [gcc-flags...] <source.c> [-- args...]" >&2
  exit 1
}

if [ $# -lt 1 ]; then
  usage
fi

CFLAGS=()
SRC=""

while [ $# -gt 0 ]; do
  case "$1" in
    *.c)
      SRC="$1"
      shift
      break
      ;;
    *)
      CFLAGS+=("$1")
      shift
      ;;
  esac
done

if [ -z "$SRC" ]; then
  usage
fi

if [ "${1:-}" = "--" ]; then
  shift
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DIST_DIR="$SCRIPT_DIR/dist"
mkdir -p "$DIST_DIR"

BASENAME="$(basename "$SRC" .c)"
OUT="$DIST_DIR/$BASENAME"

gcc "${CFLAGS[@]}" -o "$OUT" "$SRC"
"$OUT" "$@"
