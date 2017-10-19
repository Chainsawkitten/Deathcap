#!/bin/bash -ex

# Default clang-format points to default 3.5 version one
CLANG_FORMAT=clang-format-3.9
$CLANG_FORMAT --version

if [ "$TRAVIS_EVENT_TYPE" = "pull_request" ]; then
    # Get list of every file modified in this pull request
    files_to_lint="$(git diff --name-only --diff-filter=ACMRTUXB $TRAVIS_COMMIT_RANGE | grep '^src/[^.]*[.]\(cpp\|hpp\)$' || true)"
else
    # Check everything for branch pushes
    files_to_lint="$(find src/ -name '*.cpp' -or -name '*.hpp')"
fi

# Turn off tracing for this because it's too verbose
set +x

for f in $files_to_lint; do
    d=$(diff -u "$f" <($CLANG_FORMAT "$f") || true)
    if ! [ -z "$d" ]; then
        echo "!!! $f not compliant to coding style, here is the fix:"
        echo "$d"
        fail=1
    fi
done

set -x

if [ "$fail" = 1 ]; then
    exit 1
fi
