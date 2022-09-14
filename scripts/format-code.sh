#!/usr/bin/env sh

git ls-files| grep -E '.*\.[ch]pp'| xargs clang-format -style=file -i

# return only with EXIT_SUCCESS if there were no changes
# otherwise show the changes and return with error code
STATUS="$(git status -s)"
if [ -n "$STATUS" ]; then
	git --no-pager diff && false
fi
