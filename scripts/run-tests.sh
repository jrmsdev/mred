#!/bin/sh

t_FAIL=0
TEST_VALGRIND=false
VG_ARGS="--quiet --error-exitcode=128 --leak-check=full --show-leak-kinds=all"
VG_ARGS="$VG_ARGS --track-origins=yes --errors-for-leak-kinds=all"


t_compile()
{
	local t_name=$1
	$MAKE ${t_name}.bin >/dev/null
	test $? -eq 0 || {
		echo "[FAIL] compile ${t_name}"
		return 1
	}
	return 0
}


t_run()
{
	local t_name=$1
	test -x ${t_name}.bin || {
		echo "[FAIL] ${t_name} not found"
		t_FAIL=`expr 1 + $t_FAIL`
		return 1
	}
	if $TEST_VALGRIND
	then
		valgrind $VG_ARGS --log-file=./${t_name}.vgout ./${t_name}.bin
	else
		./${t_name}.bin
	fi
	local t_status=$?
	test $t_status -eq 0 || {
		echo "[FAIL] ${t_name} (${t_status})"
		t_FAIL=`expr 1 + $t_FAIL`
		return 1
	}
	test -s ./${t_name}.vgout && {
		echo "[FAIL] ${t_name} (${t_status}) vgout not empty"
		t_FAIL=`expr 1 + $t_FAIL`
		return 1
	}
	rm -f ./${t_name}.vgout
	echo "[PASS] ${t_name}"
	return 0
}


t_main()
{
	for t in t_*.c
	do
		local t_name=`basename $t .c`
		t_compile $t_name && t_run $t_name
	done
}


test -z "$MAKE" && {
	echo "[ERROR] MAKE env var not set"
	exit 1
}
which $MAKE >/dev/null 2>/dev/null || {
	echo "[ERROR] $MAKE command not found"
	exit 1
}
test "`basename $(pwd)`" = "tests" || {
	echo "[ERROR] should run inside tests dir"
	exit 1
}

if test "$1" = "--valgrind"
then
	TEST_VALGRIND=true
	which valgrind >/dev/null 2>/dev/null || {
		echo "[ERROR] valgrind command not found"
		exit 1
	}
	echo "[INFO] valgrind ${VG_ARGS}"
fi

t_main

if test 0 -lt $t_FAIL
then
	echo "[ERROR] $t_FAIL test(s) failed!"
	exit 2
fi
exit 0
