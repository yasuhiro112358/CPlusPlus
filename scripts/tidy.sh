#!/usr/bin/env bash
# clang-tidy ラッパー。
# macOS では Homebrew の clang-tidy が標準ライブラリ(SDK)を見つけられないため
# -isysroot を補う。Linux(Dev Container)等ではそのまま実行する。
#
# 使い方:
#   ./scripts/tidy.sh                      # 全ソースを解析
#   ./scripts/tidy.sh path/to/file.cpp     # 指定ファイルを解析
#   ./scripts/tidy.sh -fix path/to/file.cpp  # 自動修正
set -euo pipefail
cd "$(dirname "$0")/.."

CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"
if ! command -v "$CLANG_TIDY" >/dev/null 2>&1; then
  if [ -x /opt/homebrew/opt/llvm/bin/clang-tidy ]; then
    CLANG_TIDY=/opt/homebrew/opt/llvm/bin/clang-tidy
  fi
fi

EXTRA=()
if [ "$(uname)" = "Darwin" ]; then
  EXTRA=(--extra-arg=-isysroot --extra-arg="$(xcrun --show-sdk-path)")
fi

if [ "$#" -eq 0 ]; then
  find projects -name '*.cpp' -print0 | xargs -0 "$CLANG_TIDY" -p build "${EXTRA[@]}"
else
  "$CLANG_TIDY" -p build "${EXTRA[@]}" "$@"
fi
