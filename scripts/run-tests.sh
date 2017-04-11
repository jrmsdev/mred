#!/bin/sh

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
	./${t_name}.test
	local t_status=$?
	test $t_status -eq 0 || {
		echo "[FAIL] ${t_name} (${t_status})"
		return 1
	}
	echo "[OK] ${t_name}"
	rm -f ./${t_name}.test
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
	exit 0
}
test -z "$CFLAGS" && {
	echo "[ERROR] CFLAGS env var not set"
	exit 0
}
test `basename $(pwd)` == "tests" || {
	echo "[ERROR] should run inside tests dir"
	exit 0
}

t_main
exit 0
