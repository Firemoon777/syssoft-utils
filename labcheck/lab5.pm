package lab5;

use strict;
use warnings;
use File::Basename;

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
	if($varnum == 9) {
		check_task9($executable);
	} else {
		print "Checker for variant $varnum not implemented!";
	}	
}

1;

