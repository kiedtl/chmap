printf '== %s\n' "$0"
trap "printf '\n'" EXIT

begin() {
    trmcols=$(stty size | cut -d' ' -f2)
    padding=$(($trmcols - 5))
    printf "%-${padding}s" "$1"
}

failure() {
    printf 'FAIL\n'
}

success() {
    printf '  OK\n'
}

cmdout() {
    arg=$(/usr/bin/printf '%b' "$2")
    if [ "$(tests/pilot_$1 $arg)" != "$3" ]; then
        failure
    else
        success
    fi
}

cmdend() {
    arg=$(/usr/bin/printf '%b' "$2")
    tests/pilot_$1 $arg 2>/dev/null >&2

    if [ $? -ne "$3" ]; then
        failure
    else
        success
    fi
}

chkend() {
    if [ $? -eq 0 ]; then success; else failure; fi
}
