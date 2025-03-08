#!/bin/bash
echo "running update_git_hash.sh"
# Get the current git hash
GIT_HASH=$(git rev-parse HEAD)
# Define the header file in the build directory
HEADER_FILE="$1/GitHash.h"

mkdir -p $(dirname ${HEADER_FILE})
touch ${HEADER_FILE}

# Check if the file exists and contains the current git hash
if [ -f "$HEADER_FILE" ]; then
    CURRENT_HASH=$(grep -oP '(?<=GIT_HASH = ")[^"]+' "$HEADER_FILE")
    if [ "$CURRENT_HASH" == "$GIT_HASH" ]; then
        echo "Git hash is up to date. No changes needed."
        exit 0
    fi
fi

# Write the git hash to the header file
echo "#ifndef GITHASH_H" > $HEADER_FILE
echo "#define GITHASH_H" >> $HEADER_FILE
echo "" >> $HEADER_FILE
echo "const char* const GIT_HASH = \"$GIT_HASH\";" >> $HEADER_FILE
echo "" >> $HEADER_FILE
echo "#endif // GITHASH_H" >> $HEADER_FILE