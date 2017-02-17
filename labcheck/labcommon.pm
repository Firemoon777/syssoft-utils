package labcommon;

use strict;
use warnings;

my $splint = "/export/labs/splint/bin/splint +posixlib";
my $flags = "-Wall -Werror -Wextra -std=c99";

sub check_subdirs {
	my $projdir = $_[0];
	print "Detecting directories...";
	opendir(DIR, $projdir) or die $!;
	while (my $file = readdir(DIR)) {
		next unless (-d "$projdir/$file");
		next if ($file =~ m/^\./);
		print "found: $file\n Please read lab requirements carefully!\n";
		exit;
	}
	print "not found, ok.\n";
	closedir(DIR);
}

sub check_makefile {
	my $projdir = $_[0];
	print "Detecting Makefile...";
	
	opendir(DIR, $projdir) or die $!;
	while (my $file = readdir(DIR)) {
		if($file =~ "Makefile") {
			print "ok.\n";
			return;
		}
		
	}
	print "fail.\n";
	closedir(DIR);
	exit;
}

sub check_make {
	my $projdir = $_[0];
	print "Cheсking build with make...";
	my $output = qx(cd $projdir && make 2>&1);
	my $exitcode = $? >> 8;
	if($exitcode == 0) {
		print "ok.\n";
		return;
	}
	print "fail.\n$output\n";
	exit; 
}

sub check_gcc_flags {
	my $projdir = $_[0];
	print "Checking compatibility with $flags...\n";
	my $status = 0;
	opendir(DIR, $projdir) or die $!;
	while (my $file = readdir(DIR)) {
		next unless (-f "$projdir/$file");
		next unless ($file =~ /.*\.c/);
		print "\t$file...";
		my $output = qx(cd $projdir && gcc -c $flags $file 2>&1);
		my $exitcode = $? >> 8;
		if($exitcode == 0) {
			print "ok.\n";
		} else {
			print "fail.\n$output";
			$status = 1;
		}
		
		
	}
	closedir(DIR);
	if($status != 0) {
		exit;
	}
}

sub run_splint {
	my $projdir = $_[0];
	print "Running splint...\n";
	my $status = 0;
	opendir(DIR, $projdir) or die $!;
	while (my $file = readdir(DIR)) {
		next unless (-f "$projdir/$file");
		next unless ($file =~ /.*\.c/);
		print "\t$file...";
		my $output = qx(cd $projdir && $splint $file 2>&1);
		my $exitcode = $? >> 8;
		if($exitcode == 0) {
			print "ok.\n";
		} else {
			print "fail.\n$output";
			$status = 1;
		}
		
		
	}
	closedir(DIR);
	if($status != 0) {
		exit;
	}
}

sub analyze {
	my $projdir = $_[0];
	check_subdirs($projdir);
	check_makefile($projdir);
	check_make($projdir);
	check_gcc_flags($projdir);
	run_splint($projdir);
	print "\n\n";
}

sub print_msg {
	my ($msg) = @_;
	my $s = " "x(60 - length($msg));
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

1;
