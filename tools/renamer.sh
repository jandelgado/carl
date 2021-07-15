#!/bin/bash
# (c) copyright 2021 by Jan Delgado
# Carl Music Box SD card file renamer
# a little tool to rename files in a directory for use with DFPlayer Mini's
# folder mode. This tool renames a list of mp3-files into an ascending order,
# i.e. 0001-<oldname>.mp3, 0002-<oldname>.mp3 etc. files already in this 
# format are not touched. 
#
set -eou pipefail
shopt -s nocasematch

function usage {
    echo "usage: $0 [--dry-run] [--large-folders] FILES"
    echo "Examples:"
    echo "  renamer.sh --dry-run /path/to/my/files/*mp3"
    echo "  renamer.sh --large-folders /path/to/my/files/*mp3"
    exit 1
}

NUM_DIGITS=3
while [[ $# -ge 1 ]]; do
    key="$1"
    case $key in
        --large-folders)
            NUM_DIGITS=4
            ;;
        --dry-run)
            DRY_RUN=1
            ;;
        -*)
            usage 
            ;;
        *) break
            ;;
    esac
    shift
done

[[ $# -lt 1 ]] && usage

COUNT=1
for fn in "$@"; do 
    EXT=${fn#*.}

    # consider only files with mp3 extension
    if [ "${EXT##*.}" != "mp3" ]; then
        echo "skipping '$fn' as not having mp3 extension"
        continue
    fi
    # ignore files already in correct format, e.g. 0003.mp3
    if [[ $fn =~ ^(.*/)?[0-9]{$NUM_DIGITS}.mp3$ ]]; then
        echo "skipping '$fn' as already in correct format."
        continue
    fi

    # make sure filename is not already in use and find next free slot.
    while : ; do
        NEWNAME=$(printf "%0${NUM_DIGITS}d" $COUNT)
        COUNT=$((COUNT +1))
        if [ ! -f "${NEWNAME}.mp3" ] && [ ! -f "${NEWNAME}.MP3" ]; then
            break
        fi
    done

    if [ $DRY_RUN ]; then
        echo mv \"${fn}\" \"$NEWNAME.mp3\"
    else 
        mv "${fn}" "$NEWNAME.mp3"
    fi
done
