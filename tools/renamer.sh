#!/bin/bash
# Carl Music Box
# a little tool to rename files in a directory for use with DFPlayer Mini's
# folder mode. This tool renames a list of mp3-files into an ascending order,
# i.e. 0001-<oldname>.mp3, 0002-<oldname>.mp3 etc. files already in this 
# format are not touched. 
# Usage:
#   ./renamer.sh --dry-run /path/to/my/files/*mp3   - for the dry run
#   ./renamer.sh /path/to/my/files/*mp3             - to rename the files
#
set -eou pipefail

if [ "$1" == "--dry-run" ]; then
    DRY_RUN=1
    shift
fi

if [ $# -lt 1  ]; then
    echo "usage: $0 [--dry-run] file [file]*"
    exit 1
fi

# case insensitive regexp
shopt -s nocasematch

COUNT=1
for fn in "$@"; do 

    EXT=${fn#*.}

    # consider only files with mp3 extension
    if [ "${EXT##*.}" != "mp3" ]; then
        echo "warn: skipping '$fn' as not having mp3 extension"
        continue
    fi
    # ignore files already in correct format, e.g. 0003.mp3
    if [[ $fn =~ ^(.*/)?[0-9]{4}.mp3$ ]]; then
        echo "warn: skipping '$fn' as already in correct format."
        continue
    fi

    # make sure filename is not already in use and find next free slot.
    while : ; do
        NEWNAME=$(printf "%04d" $COUNT)
        COUNT=$((COUNT +1))
        if [ ! -f "${NEWNAME}.mp3" ] && [ ! -f "${NEWNAME}.MP3" ]; then
            break
        fi
    done

    if [ $DRY_RUN ]; then
        echo mv "${fn}" "$NEWNAME.mp3"
    else 
        mv "${fn}" "$NEWNAME.mp3"
    fi
done

