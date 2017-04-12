#!/bin/sh

MRED_BIN="../build.test/mred.bin"
DIGEST="sha1"
t_RUN=0
t_FAIL=0
TEST_VALGRIND=false
VG_ARGS="--quiet --error-exitcode=128 --leak-check=full --show-leak-kinds=all"
VG_ARGS="$VG_ARGS --track-origins=yes --errors-for-leak-kinds=all"

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
	echo "[INFO] $@"
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
		t_fail $t_name "digest mistmatch"
		t_info $t_name "expect $t_expect_digest"
		t_info $t_name "stdout $t_stdout_digest"
		t_info $t_name "       $t_stdout"
		check_ret=1
	fi
	if test -s ${t_stdout}.vgout
	then
		t_fail $t_name "valgrind report not empty"
		t_info ${t_stdout}.vgout
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
	if test -e ${t_name}/openfile
	then
		t_cmd="$t_cmd ${t_name}/openfile"
	fi
	if $TEST_VALGRIND
	then
		valgrind $VG_ARGS --log-file=${t_stdout}.vgout \
				$t_cmd <$t_stdin >$t_stdout
	else
		$t_cmd <$t_stdin >$t_stdout
	fi
	local cmd_ret=$?
	if test 0 -ne $cmd_ret
	then
		t_error $t_name "cmd run error: $cmd_ret"
	fi
	t_check $t_name $t_stdout
	local t_ret=$?
	if test 0 -eq $t_ret
	then
		rm -f $t_stdout ${t_stdout}.vgout
	fi
	return $t_ret
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
	return 0
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
t_START=`date '+%s'`

t_main
main_ret=$?

t_END=`date '+%s'`
echo "       $t_RUN test(s) ran"
echo "       $t_FAIL test(s) failed"
echo "       in $(expr $t_END - $t_START) second(s) - $(date)"

exit $main_ret
