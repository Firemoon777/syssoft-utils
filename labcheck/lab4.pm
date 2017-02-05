package lab4;

use strict;
use warnings;

my $preload = "./lab4_preload.so";
my $labtests = "./labtests/lab4/";
my $preload_test = "${labtests}1.in";
my $preload_test_cmp = "${labtests}1.in ${labtests}0.in";

my $preload_exitcode = 42;

my @var = (
    "zsh",
    "cat",
    "cp",
    "head",
    "tail",
    "tee",
    "wc",
    "cmp",
    "more",
    "less",
    "sed",
    "awk",
    "sh",
    "ksh",
    "csh"
);

# options 
my $pipe_input       = 1 << 0;
my $file_input       = 1 << 1;
my $trim_whitespaces = 1 << 2;
my $redirect         = 1 << 3;
my $check_in_out     = 1 << 4;
my $disable_preload  = 1 << 5;
my $remove_out       = 1 << 6;
my $check_tee_op     = 1 << 7;
my $several_output   = 1 << 8;
my $check_cmp_op     = 1 << 9;

sub print_msg {
	my ($msg) = @_;
	my $s = " "x(50 - length($msg));
	print "$msg$s";
}

sub print_ans {
	my ($msg) = @_;
	if($msg == 0) {
		print "[  OK  ]\n";
	} elsif($msg == 1) {
		print "[ FAIL ]\n";
	} else {
		print "[ FAIL ]\nCritical error.\n";
		exit;
	}
}

sub launch { 
	my ($executable, $options, $in_file, $out_file) = @_;
	my $cmd = "";
	if($options & $remove_out) {
		system("rm -f $out_file");
	}
	if($options & $pipe_input) {
		$cmd .= "cat $in_file | ";
	}
	if(($options & $disable_preload) == 0) {
		$cmd .= "LD_PRELOAD=$preload ";
	}
	$cmd .= "$executable ";
	if($options & $file_input) {
		$cmd .= "$in_file ";
	}
	if($options & $trim_whitespaces) {
		$cmd .= " | sed -e 's/  */ /g; s/^ //g' ";
	}
	if($options & $redirect) {
		$cmd .= "> ";
	}
	$cmd .= "$out_file ";
	$cmd .= " 2>&1";
	
	#print "cmd: $cmd\n";
	system($cmd);
	my $e = $? >> 8;
	#print "exitcode: $e\n\n" ;
	return $? >> 8;
}

sub check_test_tee {
	my ($executable, $original, $test_file, $options) = @_;
	
	my $stdout = "std.out";
	my $out = "exec.out";
	my $result;
	
	if($options & $several_output) {
		$out .= " ex.out";
	}
	system("rm -f $out $stdout");
	
	if(launch($executable, $options, $test_file, "$out > $stdout")) {
		return 1;
	}
	
	if($options & $several_output) {
		system("cmp -s $out");
		$result += $? >> 8;
		system("cmp -s $stdout ex.out");
		$result += $? >> 8;
	} else {
		system("cmp -s $test_file $stdout");
		$result = $? >> 8;
		system("cmp -s $stdout $out");
		$result += $? >> 8;
	}
	return $result > 0 ? 1 : 0;
}

sub check_test {
	my ($executable, $original, $test_file, $options) = @_;
	
	if($options & $check_tee_op) {
		return check_test_tee(@_);
	}
	
	my $out_exec = "exec.out";
	my $out_orig = "orig.out";
	my $exit_code;
	
	if(launch($executable, $options, $test_file, $out_exec) == $preload_exitcode) {
		return 2;
	}
	if(($options & $check_in_out) == 0) {
		launch($original, $options | $disable_preload, $test_file, $out_orig);
	}
	
	if($options & $check_in_out) {
		system("cmp -s $out_exec $test_file");
	} else {
		system("cmp -s $out_exec $out_orig");
	}
	return $? >> 8;
}

sub check_common_tests {
	my ($executable, $original, $options) = @_;
	my @common_tests = ();
	my $result;
	
	opendir(DIR, $labtests) or die $!;
	while (my $file = readdir(DIR)) {	
		next if ($file =~ m/^\./);
        push @common_tests, "$file";
    }
	closedir(DIR);
	
	@common_tests = sort @common_tests;

	foreach(@common_tests) {
		print_msg("Checking test ${_}...");
		$result = check_test($executable, $original, "$labtests$_", $options);
		print_ans($result * 2);
	}
}

sub check_preload {
	my ($executable, $options) = @_;
	my $result;
	my $out_exec = "/dev/null";
	my $test = ($options & $check_cmp_op) ? $preload_test_cmp : $preload_test;
	print_msg("Checking for read/write...");
	$result = launch($executable, $options, $test, $out_exec);
	print_ans(($result > 1) * 2);
}

sub check_several_files {
	my ($executable, $original, $options) = @_;
	my @common_tests;
	my $result;
	
	opendir(DIR, $labtests) or die $!;
	while (my $file = readdir(DIR)) {	
		next if ($file =~ m/^\./);
        push @common_tests, "$labtests$file";
    }
	closedir(DIR);
	
	print_msg("Checking several files...(optional)");
	$result = check_test($executable, $original, join(" ", @common_tests), $options);
	print_ans($result);
}

sub check_cat {
	my $executable = $_[0];
	my $original = "cat";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect);
	check_several_files($executable, $original, $file_input | $redirect);
}

sub check_cp {
	my $executable = $_[0];
	my $original = "cp";

	check_common_tests($executable, $original, $file_input | $check_in_out | $remove_out);
}

sub check_wc {
	my $executable = $_[0];
	my $original = "wc";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect | $trim_whitespaces);
	check_several_files($executable, $original, $file_input | $redirect | $trim_whitespaces);
}

sub check_head {
	my $executable = $_[0];
	my $original = "head";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect);
	check_several_files($executable, $original, $file_input | $redirect);
}

sub check_tail {
	my $executable = $_[0];
	my $original = "tail";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect);
}

sub check_tee {
	my $executable = $_[0];
	my $original = "tee";
	
	check_preload($executable, $pipe_input | $redirect);
	check_common_tests($executable, $original, $pipe_input | $remove_out | $check_tee_op);
	check_several_files($executable, $original, $pipe_input | $several_output | $check_tee_op);
}

sub check_cmp {
	my $executable = $_[0];
	my $original = "cmp";
	
	my $result;
	check_preload($executable, $file_input | $redirect | $check_cmp_op);

	print_msg("Checking test 1.in 2.in...");
	$result = check_test($executable, $original, "${labtests}1.in ${labtests}2.in", $file_input | $redirect);
	print_ans($result);
	
	print_msg("Checking test 5.in 5.in...");
	$result = check_test($executable, $original, "${labtests}5.in ${labtests}5.in", $file_input | $redirect);
	print_ans($result);
	
	print_msg("Checking test 0.in 0.in...");
	$result = check_test($executable, $original, "${labtests}0.in ${labtests}0.in", $file_input | $redirect);
	print_ans($result);
}

sub check {
	my ($varnum, $executable) = @_;
	print "Lab num: 4\nLab variant: $varnum -- $var[$varnum]\nExecutable: $executable\n\n\n";
	if($varnum == 1) {
		check_cat($executable);
	} elsif($varnum == 2) {
		check_cp($executable);
	} elsif($varnum == 3) {
		check_head($executable);
	} elsif($varnum == 4) {
		check_tail($executable);
	} elsif($varnum == 5) {
		check_tee($executable);
	} elsif($varnum == 6) {
		check_wc($executable);
	} elsif($varnum == 7) {
		check_cmp($executable);
	} else {
		print "Checker for variant $var[$varnum] not implemented!\n";
	}
}

1
