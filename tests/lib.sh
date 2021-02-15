printf '== %s\n' "$0"
trap "printf '\n'" EXIT

begin() {
	printf '%-50s' "$1"
}

cmdout() {
    if [ "$($1 $2)" != "$3" ]; then
        printf 'FAIL\n'
    else
        printf 'OK\n'
    fi
}

cmdend() {
    $1 $2 2>/dev/null >&2

	if [ $? -ne "$3" ]; then
		printf 'FAIL\n'
	else
		printf 'OK\n'
	fi
}
