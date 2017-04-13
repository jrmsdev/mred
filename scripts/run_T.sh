#!/bin/sh

MRED_BIN="../build.test/mred.bin"
DIGEST=${CHECK_DIGEST_CMD:-"sha256"}
MAKE=${MAKE:-"make"}
t_RUN=0
t_FAIL=0
t_COMPILE=0
TEST_VALGRIND=false
VG_ARGS="--quiet --error-exitcode=128 --leak-check=full --show-leak-kinds=all"
VG_ARGS="$VG_ARGS --track-origins=yes --errors-for-leak-kinds=all"
T_COMPILE=false

if test -n "$TEST_COMPILE"
then
	T_COMPILE=true
fi

t_error()
{
	echo "[ERROR] $@"
	exit 1
}

t_fail()
{
	echo "[FAIL] $@"
}

t_info()
{
	echo "       $@"
}

t_pass()
{
	echo "[ OK ] $@"
}

t_check()
{
	local t_name=$1
	local t_stdout=$2
	local t_expect=${t_name}/expect
	local t_stdout_digest=`$DIGEST $t_stdout 2>/dev/null | cut -d' ' -f4`
	local t_expect_digest=`$DIGEST $t_expect 2>/dev/null | cut -d' ' -f4`
	if test -z "$t_expect_digest"
	then
		t_error $t_name "expect digest is empty"
	fi
	if test -z "$t_stdout_digest"
	then
		t_error $t_name "stdout digest is empty"
	fi
	local check_ret=0
	if test "$t_stdout_digest" != "$t_expect_digest"
	then
		t_fail "$t_stdout digest"
		t_info "expect $t_expect_digest"
		t_info "   got $t_stdout_digest"
		check_ret=1
	fi
	return $check_ret
}

t_run()
{
	local t_name=$1
	local t_stdin=$2
	local t_stdout=${t_name}/stdout.$$
	local t_cmd="$MRED_BIN"
	local cmd_ret=$?

	if test -e ${t_name}/openfile
	then
		t_cmd="$t_cmd ${t_name}/openfile"
	fi

	if $TEST_VALGRIND
	then
		valgrind $VG_ARGS --log-file=${t_stdout}.vgout \
				$t_cmd <$t_stdin >$t_stdout
		cmd_ret=$?
	else
		$t_cmd <$t_stdin >$t_stdout
		cmd_ret=$?
	fi

	t_check $t_name $t_stdout
	local check_ret=$?

	if test 0 -ne $cmd_ret
	then
		t_fail $t_name "cmd run error: $cmd_ret"
		t_info ${t_stdout}.vgout
		check_ret=1
	fi

	if test -s ${t_stdout}.vgout
	then
		if test 0 -eq $cmd_ret
		then
			t_fail "${t_stdout}.vgout not empty"
			check_ret=3
		fi
	fi

	if test 0 -eq $check_ret
	then
		rm -f $t_stdout
	fi
	return $check_ret
}

t_compile()
{
	local t_name=$1
	$MAKE ${t_name}.bin >/dev/null
	mk_stat=$?
	t_COMPILE=`expr 1 + $t_COMPILE`
	test 0 -eq $mk_stat || {
		t_fail "t_compile/$t_name compile (${mk_stat})"
		t_FAIL=`expr 1 + $t_FAIL`
		return 1
	}
	test -x ${t_name}.bin || {
		t_fail "t_compile/${t_name}.bin executable not found"
		t_FAIL=`expr 1 + $t_FAIL`
		return 1
	}
	return 0
}

t_run_bin()
{
	local t_name=$1
	if $TEST_VALGRIND
	then
		valgrind $VG_ARGS --log-file=./${t_name}.vgout ./${t_name}.bin
	else
		./${t_name}.bin
	fi
	local t_status=$?
	test $t_status -eq 0 || {
		t_fail "t_compile/${t_name} (${t_status})"
		t_FAIL=`expr 1 + $t_FAIL`
		return 1
	}
	test -s ./${t_name}.vgout && {
		t_fail "t_compile/${t_name}.vgout not empty (${t_status})"
		t_FAIL=`expr 1 + $t_FAIL`
		return 1
	}
	rm -f ./${t_name}.vgout
	t_pass "t_compile/${t_name}"
	return 0
}

t_main_compile()
{
	cd ./t_compile || {
		t_error "could not chdir to ${PWD}/t_compile"
	}
	local tests_run="$(ls t_*.c)"
	if test "x" != "x${TEST_SUITE}"
	then
		local tests_run=`echo $TEST_SUITE | sed 's/,/ /g'`
	fi
	for t in $tests_run
	do
		local t_name=`basename $t .c`
		t_compile $t_name && {
			t_run_bin $t_name
			t_RUN=`expr 1 + $t_RUN`
		}
	done
	cd - >/dev/null
}

t_main()
{
	for t_stdin in t????_*/stdin
	do
		local t_name=`dirname $t_stdin`
		t_run $t_name $t_stdin
		local t_ret=$?
		t_RUN=`expr 1 + $t_RUN`
		if test 0 -eq $t_ret
		then
			t_pass $t_name
		else
			t_FAIL=`expr 1 + $t_FAIL`
		fi
	done
	$T_COMPILE && t_main_compile
	return $t_FAIL
}

test -x $MRED_BIN || {
	t_error "$MRED_BIN not executable"
}
which $DIGEST >/dev/null 2>/dev/null || {
	t_error "$DIGEST command not found"
}

if test "--valgrind" = "$1"
then
	TEST_VALGRIND=true
	which valgrind >/dev/null 2>/dev/null || {
		t_error "valgrind command not found"
	}
fi

echo "test $($MRED_BIN --version)"
echo ""
t_START=`date '+%s'`

t_main
main_ret=$?

t_END=`date '+%s'`
echo ""
$T_COMPILE && echo "       $t_COMPILE test(s) compiled"
echo "       $t_RUN test(s) ran"
echo "       $t_FAIL test(s) failed"
echo "       in $(expr $t_END - $t_START) second(s) - $(date)"

exit $main_ret
