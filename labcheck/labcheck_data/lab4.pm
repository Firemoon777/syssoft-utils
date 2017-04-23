package lab4;

use strict;
use warnings;

use File::Basename;

use labcommon qw(print_msg print_ans);

my $data_dir = dirname($0) . "/labcheck_data";
my $preload_x86 = $data_dir . "/lab4_preload_x86.so";
my $preload_x64 = $data_dir . "/lab4_preload_x64.so";
my $labtests = $data_dir . "/labtests/lab4/";
my $preload_test = "${labtests}1.in";
my $preload_test_cmp = "${labtests}1.in ${labtests}0.in";
my $several_files_test = "${labtests}0.in ${labtests}1.in ${labtests}2.in ${labtests}3.in";

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
my $add_minus        = 1 << 10;
my $check_perm       = 1 << 11;
my $stdin_input      = 1 << 12;


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
		if(`file $executable` =~ m/32-bit/) {
			$cmd .= "LD_PRELOAD=$preload_x86 ";
		} else {
			$cmd .= "LD_PRELOAD=$preload_x64 ";
		}
	}
	$cmd .= "$executable ";
	if($options & $file_input) {
		$cmd .= "$in_file ";
	}
	if($options & $add_minus) {
		$cmd .= "- ";
	}
	if($in_file !~ m/.*\.err/) {
		if($options & $trim_whitespaces) {
			$cmd .= " | sed -e 's/  */ /g; s/^ //g' ";
		}
	}
	if($options & $redirect) {
		$cmd .= "> ";
	}
	$cmd .= "$out_file ";
	$cmd .= " 2>&1";
	
	if($options & $stdin_input) {
		if(-r $in_file) {
			$cmd .= "< $in_file ";
		} else {
			return 666;
		}
	}

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
	my $result = 0;
	my $exitcode;
	
	if($options & $several_output) {
		$out .= " ex.out";
	}
	system("rm -f $out $stdout");
	
	$exitcode = launch($executable, $options, $test_file, "$out > $stdout");
	if($exitcode == 666) {
		return -1;
	}
	if($exitcode != 0) {
		if($test_file =~ m/.*\.err/) {
			return 0;
		} else {
			return 1;
		}
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
	my $exitcode = launch($executable, $options, $test_file, $out_exec);
	
	if($exitcode == $preload_exitcode) {
		return 2;
	}
	if(($test_file =~ m/.*\.err/)) {
		return $exitcode != 0 ? 0 : 1;
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
	my $mode_orig;
	my $mode;
	opendir(DIR, $labtests) or die $!;
	while (my $file = readdir(DIR)) {	
		next if ($file =~ m/^\./);
        push @common_tests, "$file";
    }
	closedir(DIR);
	
	@common_tests = sort @common_tests;

	foreach(@common_tests) {
		labcommon::print_msg("Checking test ${_}...");
		$result = check_test($executable, $original, "$labtests$_", $options);
		labcommon::print_ans($result);
		if($_ =~ m/.*\.err/) {
			next;
		}
		if($options & $check_perm) {
			labcommon::print_msg("Checking permissions of ${_}...");
			$mode_orig = (stat("$labtests$_"))[2] & 07777;
			$mode = (stat("exec.out"))[2] & 07777;
			labcommon::print_ans($mode_orig == $mode ? 0 : 1);
		}
	}
}

sub check_preload {
	my ($executable, $options) = @_;
	my $result;
	my $out_exec = "/dev/null";
	my $test = ($options & $check_cmp_op) ? $preload_test_cmp : $preload_test;
	labcommon::print_msg("Checking for read/write...");
	$result = launch($executable, $options | $redirect, $test, $out_exec);
	labcommon::print_ans(($result > 1) * 2);
}

sub check_several_files {
	my ($executable, $original, $options) = @_;
	my $result;
	
	labcommon::print_msg("Checking several files...(optional)");
	$result = check_test($executable, $original, $several_files_test, $options);
	labcommon::print_ans($result);
}

sub check_stdin_input {
	my ($executable, $original, $options) = @_;
	my $result;
	
	if($options & $add_minus) {
		labcommon::print_msg("Checking stdin input with - (optional)");
	} else {
		labcommon::print_msg("Checking pipe input without arguments...(optional)");
	}
	$result = check_test($executable, $original, "${labtests}3.in" ,$options);
	labcommon::print_ans($result);
}

sub check_flag {
	my ($executable, $original, $options, $flag) = @_;
	my $result;
	labcommon::print_msg("Checking flag $flag (optional)");
	$result = check_test("$executable $flag", "$original $flag", "${labtests}3.in" ,$options);
	labcommon::print_ans($result);
}

sub check_cat {
	my $executable = $_[0];
	my $original = "cat";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect);
	check_stdin_input($executable, $original, $pipe_input | $redirect | $add_minus);
	check_stdin_input($executable, $original, $pipe_input | $redirect);
	check_several_files($executable, $original, $file_input | $redirect);
	check_flag($executable, $original, $file_input | $redirect, "-n");
}

sub check_cp {
	my $executable = $_[0];
	my $original = "cp";

	check_common_tests($executable, $original, $file_input | $check_in_out | $remove_out | $check_perm);
}

sub check_head {
	my $executable = $_[0];
	my $original = "head";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect);
	check_stdin_input($executable, $original, $pipe_input | $redirect);
	check_several_files($executable, $original, $file_input | $redirect);
	check_flag($executable, $original, $file_input | $redirect, "-n 5");
	check_flag($executable, $original, $file_input | $redirect, "-5");
}

sub check_tail {
	my $executable = $_[0];
	my $original = "tail";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect);
	check_stdin_input($executable, $original, $pipe_input | $redirect);
}

sub check_tee {
	my $executable = $_[0];
	my $original = "tee";
	
	check_preload($executable, $stdin_input | $redirect);
	check_common_tests($executable, $original, $stdin_input | $remove_out | $check_tee_op);
	#check_several_files($executable, $original, $stdin_input | $several_output | $check_tee_op);
	check_stdin_input($executable, $original, $stdin_input | $redirect);
}

sub check_wc {
	my $executable = $_[0];
	my $original = "wc";
	
	check_preload($executable, $file_input);
	check_common_tests($executable, $original, $file_input | $redirect | $trim_whitespaces);
	check_several_files($executable, $original, $file_input | $redirect | $trim_whitespaces);
	check_stdin_input($executable, $original, $pipe_input | $redirect);
	check_flag($executable, $original, $file_input | $redirect, "-l");
	check_flag($executable, $original, $file_input | $redirect, "-l -c");
}

sub check_cmp {
	my $executable = $_[0];
	my $original = "cmp";
	
	my $result;
	check_preload($executable, $file_input | $redirect | $check_cmp_op);

	labcommon::print_msg("Checking test 1.in 2.in...");
	$result = check_test($executable, $original, "${labtests}1.in ${labtests}2.in", $file_input | $redirect | $check_cmp_op);
	labcommon::print_ans($result);
	
	labcommon::print_msg("Checking test 2.in 1.in...");
	$result = check_test($executable, $original, "${labtests}2.in ${labtests}1.in", $file_input | $redirect | $check_cmp_op);
	labcommon::print_ans($result);
	
	labcommon::print_msg("Checking test 5.in 5.in...");
	$result = check_test($executable, $original, "${labtests}5.in ${labtests}5.in", $file_input | $redirect | $check_cmp_op);
	labcommon::print_ans($result);

	labcommon::print_msg("Checking test 0.in 0.in...");
	$result = check_test($executable, $original, "${labtests}0.in ${labtests}0.in", $file_input | $redirect | $check_cmp_op);
	labcommon::print_ans($result);

	labcommon::print_msg("Checking test 7.err 7.err...");
	$result = check_test($executable, $original, "${labtests}7.err ${labtests}7.err", $file_input | $redirect | $check_cmp_op);
	labcommon::print_ans($result);

	labcommon::print_msg("Checking exitcode...");
	$result = 0;
	
	$result += launch($executable, $file_input | $redirect, "${labtests}5.in ${labtests}5.in", "/dev/null") == 0 ? 0 : 1;
	$result += launch($executable, $file_input | $redirect, "${labtests}4.in ${labtests}5.in", "/dev/null") == 1 ? 0 : 1;
	
	labcommon::print_ans($result);
}

sub check {
	my ($varnum, $executable) = @_;
	my $arch = `file $executable`;
	if($arch =~ m/32-bit/) {
		$arch = "32-bit";
	} elsif ($arch =~ m/64-bit/) {
		$arch = "64-bit";
	} else {
		$arch = "Unknown";
	}
	print "Lab num: 4\nLab variant: $varnum -- $var[$varnum]\nExecutable: $executable\nArch: $arch\n\n\n";
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

