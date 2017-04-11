#!/bin/sh

TEST_VALGRIND=false
VG_ARGS="--quiet --error-exitcode=128 --leak-check=full --show-leak-kinds=all"
VG_ARGS="$VG_ARGS --track-origins=yes"

t_compile()
{
	local t_name=$1
	$CC $CFLAGS -o ${t_name}.test ${t_name}.c
	test $? -eq 0 || {
		echo "[FAIL] compile ${t_name}"
		return 1
	}
	return 0
}


t_run()
{
	local t_name=$1
	test -x ${t_name}.test || {
		echo "[FAIL] ${t_name} not found"
		return 1
	}
	if $TEST_VALGRIND
	then
		valgrind $VG_ARGS --log-file=./${t_name}.vgout ./${t_name}.test
	else
		./${t_name}.test
	fi
	local t_status=$?
	test $t_status -eq 0 || {
		echo "[FAIL] ${t_name} (${t_status})"
		return 1
	}
	echo "[PASS] ${t_name}"
	rm -f ./${t_name}.test ./${t_name}.vgout
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


test -z "$CC" && {
	echo "[ERROR] CC env var not set"
	exit 1
}
test -z "$CFLAGS" && {
	echo "[ERROR] CFLAGS env var not set"
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
	echo "[INFO] TEST_VALGRIND='$TEST_VALGRIND'"
fi

echo "[INFO] CC='$CC' CFLAGS='$CFLAGS'"

t_main
exit 0
