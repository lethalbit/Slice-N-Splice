#!/bin/bash
if [ $# -lt 2 ]; then
	echo "usage: $0 ASM OBJ <GCC OPTS>"
	exit;
fi

OUT_DIR=${2%/*}
[ ! -d $OUT_DIR ] && mkdir -p $OUT_DIR

TMP_OBJ="$(mktemp -u)"
gcc -m64 -c $1 -o $TMP_OBJ
ld --oformat binary -o $2 $TMP_OBJ
rm $TMP_OBJ
