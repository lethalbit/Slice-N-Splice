#!/bin/bash
if [ $# -lt 2 ]; then
	echo "usage: $0 ASM OBJ <GCC OPTS>"
	exit;
fi

OUT_DIR=${2%/*}
[ ! -d $OUT_DIR ] && mkdir -p $OUT_DIR

TMP_OBJ="$(mktemp -u)"
gcc -c $1 -o $TMP_OBJ
ld -o $TMP_OBJ.im $TMP_OBJ
objcopy -O binary $TMP_OBJ.im $2
rm $TMP_OBJ $TMP_OBJ.im
