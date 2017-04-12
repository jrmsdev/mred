#!/bin/sh

MRED_BIN="../build/mred.bin"
DIGEST="sha1"

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
	echo "[PASS] $@"
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
	if test "$t_stdout_digest" != "$t_expect_digest"
	then
		t_fail $t_name "digest mistmatch"
		t_info $t_name "expect $t_expect_digest"
		t_info $t_name "stdout $t_stdout_digest"
		t_info $t_name "       $t_stdout"
		return 1
	fi
	return 0
}

t_run()
{
	local t_name=$1
	local t_stdin=$2
	local t_stdout=${t_name}/stdout.$$
	$MRED_BIN <$t_stdin >$t_stdout
	local cmd_ret=$?
	if test 0 -ne $cmd_ret
	then
		t_error $t_name "cmd error: $cmd_ret"
	fi
	t_check $t_name $t_stdout
	local t_ret=$?
	if test 0 -eq $t_ret
	then
		rm -f $t_stdout
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
		if test 0 -eq $t_ret
		then
			t_pass $t_name
		fi
	done
	return 0
}

t_main || exit 2
exit 0
