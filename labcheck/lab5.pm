package lab5;

use strict;
use warnings;
use File::Basename;

use labcommon qw(print_msg print_ans);

my $preload_lib = "/home/s207210/syssoft-utils/labcheck/lab5_preload.so";
my $stdbuf_bin = "/export/home/studs/s207210/tools/coreutils-8.13/src/stdbuf";
my $timeout_bin = "/export/home/studs/s207210/tools/coreutils-8.13/src/timeout";

# Options 
my $truss   = 1 << 1;
my $preload = 1 << 2;
my $stdbuf = 1 << 3;

sub make_query {
	my ($executable, $options) = @_;
	my $cmd = "";
	if($options & $preload) {
		$cmd .= "LD_PRELOAD=$preload_lib ";
	}

	if($options & $truss) {
		$cmd .= "truss ";
	}

	if($options & $stdbuf) {
		$cmd .= "$stdbuf_bin -o0 "
	}


	$cmd .= "$executable ";
	return $cmd;
}

sub timeout_launch {
	my ($executable, $options, $time) = @_;
	return "$timeout_bin $time " . make_query($executable, $options);
}

sub get_truss_output {
	my ($executable, $options, $timeout) = @_;
	my $query = "" . timeout_launch($executable, $options | $truss, $timeout) . " 2>&1";
	return `$query`;
}

sub truss_check_syscall {
	my ($output, $syscall, $count) = @_;
	labcommon::print_msg("Checking syscall $syscall...");
	labcommon::print_ans($output =~ /($syscall([\d\D])*){$count,}/ ? 0 : 1);
}

sub check_task1 {
	my ($executable) = @_;
	my $truss_output = get_truss_output($executable, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 1);
	truss_check_syscall($truss_output, "getuid", 1);
	truss_check_syscall($truss_output, "getgid", 1);
	truss_check_syscall($truss_output, "time", 1);
	truss_check_syscall($truss_output, "getloadavg", 4);

	truss_check_syscall($truss_output, "shmat", 1);
	truss_check_syscall($truss_output, "shmget", 1);
}

sub check_task9 {
	my ($executable) = @_;
	my $pid = fork();
	if($pid < 0) {
		print "Failed to fork...";
		exit(-1);
	} elsif($pid == 0) {
		# Child
		my $output = qx($executable);
		print "Output: $output\n";
	} else {
		# Parent
		sleep 1;
		my $file = basename($executable);
		print "File $file\n";
		my $whoami = qx(whoami);
		$pid = qx("pgrep -u $whoami $file");
		print "Process with pid $pid started!";
		system("kill -1  $pid");
		system("kill -2  $pid");
		system("kill -15 $pid");
		system("kill -16 $pid");
		system("kill -17 $pid");
		system("kill -9  $pid");

	}

}

sub check {
	my ($varnum, $executable) = @_;
	if($varnum == 1) {
		check_task1($executable);
	} elsif($varnum == 9) {
		check_task9($executable);
	} else {
		print "Checker for variant $varnum not implemented!";
	}	
}

1;

