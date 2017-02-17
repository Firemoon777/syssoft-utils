package lab5;

use strict;
use warnings;
use File::Basename;

use labcommon qw(print_msg print_ans);

my $preload_lib = "/home/s207210/syssoft-utils/labcheck/lab5_preload.so";
my $stdbuf_bin = "/export/home/studs/s207210/tools/coreutils-8.13/src/stdbuf";
my $timeout_bin = "/export/home/studs/s207210/tools/coreutils-8.13/src/timeout";
my $truss_params = "-f -u librt::sem_init,sem_post,sem_wait "; 
$truss_params .= "-u libsocket::listen,bind,accept ";
$truss_params .= "-u libnsl:: ";

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
		$cmd .= "truss $truss_params";
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
	my $output = `$query`;
	# cut truss start 
	$output =~ s/([\d\D]+?sysi86){2}//;
	#print "query: $query\noutput: $output\n";
	return $output;
}

sub truss_check_syscall {
	my ($output, $syscall, $min, $max) = @_;
	my $answer = 0;
	$max++;
	labcommon::print_msg("Checking syscall $syscall...");
	$answer += ($output =~ /($syscall([\d\D])*){$min,}/) ? 0 : 1;
	$answer += ($output =~ /($syscall([\d\D])*){$max,}/) ? 1 : 0;
	labcommon::print_ans($answer > 0 ? 1 : 0);
}

sub check_task1 {
	my ($server, $client) = @_;
	print "Checking server side...\n";
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 1, 1);
	truss_check_syscall($truss_output, "getuid", 1, 1);
	truss_check_syscall($truss_output, "getgid", 1, 1);
	truss_check_syscall($truss_output, "time", 3, 100);
	truss_check_syscall($truss_output, "getloadavg", 3, 100);

	truss_check_syscall($truss_output, "shmat", 1, 1);
	truss_check_syscall($truss_output, "shmget", 1, 1);

	print "\nChecking client side...\n";
	system(timeout_launch($server, 0, 10) . " &");
	sleep 1;
	$truss_output = get_truss_output($client, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 0, 0);
	truss_check_syscall($truss_output, "getuid", 0, 0);
	truss_check_syscall($truss_output, "getgid", 0, 0);
	truss_check_syscall($truss_output, "time", 0, 0);
	truss_check_syscall($truss_output, "getloadavg", 0, 0);

	truss_check_syscall($truss_output, "shmat", 1, 1);
	truss_check_syscall($truss_output, "shmget", 1, 1);
	
}

sub check_task2 {
	my ($server, $client) = @_;
	print "Checking server side...\n";
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 1, 1);
	truss_check_syscall($truss_output, "getuid", 1, 1);
	truss_check_syscall($truss_output, "getgid", 1, 1);
	truss_check_syscall($truss_output, "time", 3, 100);
	truss_check_syscall($truss_output, "getloadavg", 3, 1);

	truss_check_syscall($truss_output, "msgget", 1, 1);
	truss_check_syscall($truss_output, "msgsnd", 3, 100);

	print "\nChecking client side...\n";
	system(timeout_launch($server, 0, 10) . " &");
	sleep 1;
	$truss_output = get_truss_output($client, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 1, 1);
	truss_check_syscall($truss_output, "getuid", 0, 0);
	truss_check_syscall($truss_output, "getgid", 0, 0);
	truss_check_syscall($truss_output, "time", 0, 0);
	truss_check_syscall($truss_output, "getloadavg", 0, 0);

	truss_check_syscall($truss_output, "msgget", 1, 1);
	truss_check_syscall($truss_output, "msgrcv", 1, 1);
	
}

sub check_task3 {
	my ($server, $client) = @_;
	print "Checking server side...\n";
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 1, 1);
	truss_check_syscall($truss_output, "getuid", 1, 1);
	truss_check_syscall($truss_output, "getgid", 1, 1);
	truss_check_syscall($truss_output, "time", 3, 100);
	truss_check_syscall($truss_output, "getloadavg", 4, 100);
	truss_check_syscall($truss_output, "mmap", 1, 1);

	print "\nChecking client side...\n";
	system(timeout_launch($server, 0, 10) . " &");
	sleep 1;
	$truss_output = get_truss_output($client, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 0, 0);
	truss_check_syscall($truss_output, "getuid", 0, 0);
	truss_check_syscall($truss_output, "getgid", 0, 0);
	truss_check_syscall($truss_output, "time", 0, 0);
	truss_check_syscall($truss_output, "getloadavg", 0, 0);
	truss_check_syscall($truss_output, "mmap", 1, 1);
	
}

sub check_task4 {
	my ($server) = @_;
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "sem_init", 1, 100);
	truss_check_syscall($truss_output, "sem_post", 1, 100);
	truss_check_syscall($truss_output, "sem_wait", 1, 100);
}

sub check_task5 {
	my ($server) = @_;
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "semop", 1, 100);
	truss_check_syscall($truss_output, "semget", 1, 100);
	truss_check_syscall($truss_output, "semctl", 1, 100);
}

sub check_task8 {
	my ($server, $client) = @_;
	print "Checking server side...\n";
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 1, 1);
	truss_check_syscall($truss_output, "getuid", 1, 1);
	truss_check_syscall($truss_output, "getgid", 1, 1);
	truss_check_syscall($truss_output, "time", 3, 100);
	truss_check_syscall($truss_output, "getloadavg", 4, 100);
	truss_check_syscall($truss_output, "socket", 1, 100);
	truss_check_syscall($truss_output, "bind", 1, 1);
	truss_check_syscall($truss_output, "libsocket:listen", 2, 2); # truss outputs two listen, first on enter, second on return
	truss_check_syscall($truss_output, "accept", 1, 10);

	print "\nChecking client side...\n";
	system(timeout_launch($server, 0, 10) . " &");
	sleep 1;
	$truss_output = get_truss_output($client, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 0, 0);
	truss_check_syscall($truss_output, "getuid", 0, 0);
	truss_check_syscall($truss_output, "getgid", 0, 0);
	truss_check_syscall($truss_output, "time", 0, 0);
	truss_check_syscall($truss_output, "getloadavg", 0, 0);
	truss_check_syscall($truss_output, "socket", 1, 1);
	truss_check_syscall($truss_output, "connect", 1, 10);
	
}


sub check_task9 {
	my ($server) = @_;
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "getpid", 1, 1);
	truss_check_syscall($truss_output, "getuid", 1, 1);
	truss_check_syscall($truss_output, "getgid", 1, 1);
	truss_check_syscall($truss_output, "time", 3, 100);
	truss_check_syscall($truss_output, "getloadavg", 4, 100);

	truss_check_syscall($truss_output, "fork", 1, 100);
	truss_check_syscall($truss_output, "execl", 1, 100);
	truss_check_syscall($truss_output, "pipe", 1, 100);
	truss_check_syscall($truss_output, "dup2", 1, 100);
}

sub check_task10 {
	my ($server) = @_;
	my $truss_output = get_truss_output($server, $stdbuf, 3);
	truss_check_syscall($truss_output, "fork", 1, 100);
	truss_check_syscall($truss_output, "exec", 1, 100);
	truss_check_syscall($truss_output, "pipe", 1, 100);
	truss_check_syscall($truss_output, "dup2", 1, 100);
}

sub check {
	my ($varnum, $server, $client) = @_;
	if($varnum == 1) {
		check_task1($server, $client);
	} elsif($varnum == 2) {
		check_task2($server, $client);
	} elsif($varnum == 3) {
		check_task3($server, $client);
	} elsif($varnum == 4) {
		check_task4($server);
	} elsif($varnum == 5) {
		check_task5($server);
	} elsif($varnum == 8) {
		check_task8($server, $client);
	} elsif($varnum == 9) {
		check_task9($server);
	} elsif($varnum == 10) {
		check_task10("$server /etc/passwd");
	} else {
		print "Checker for variant $varnum not implemented!\n";
	}	
}

1;

