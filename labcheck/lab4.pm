package lab4;

use strict;
use warnings;

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

sub check {
	my ($varnum, $executable) = @_;
	print "Lab num: 4\nLab variant: $varnum -- $var[$varnum]\nExecutable: $executable\n";
}

1
