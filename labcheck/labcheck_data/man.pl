=encoding utf8

=pod

=head1 NAME

B<labcheck> - labwork 4-6 checking system 

=head1 SYNOPSIS

labcheck [-p] [-v] labnum labvar projdir bin

labcheck [-p] [-v] labnum labvar projdir server client

=head1 DESCRIPTION

The labcheck utility verify your labwork in two stages:

First stage: static analyze

First stage in common for all labs and variants. Project direcory must contain Makefile and must NOT contain any subfolders. The utility will check for Makefile, subdirecoties and then try to execute make. In case of success labcheck will try to compile sources with compiler options -Wextra -Wall -Werror -std=c99 and check with static analyzer with -std=c89.

Second stage: blackbox testing

Second stage for lab4 is comparasion between lab and original utility. Functions *printf and *scanf from stdio.h are restricted and will cause program crash on tests. All tests will be passed successfully if output of lab equals to original utility. For wc you can output without indents.

Second stage for lab5 is tracing syscalls. labcheck will detect required syscalls and count them. For example, you shouldn't call getpid() in task1 client, if you would pass testing.  

=head1 OPTIONS

-v		If -v is present, fails in static analyze or c99 check will be printed.

-p		if -p is present, static analyze warnings will treated as errors.

=head1 OPERANDS

labnum		number of labwork (4, 5 or 6)

labvar		variant of labwork

projdir		directory with lab and Makefile

bin			executable for lab4

server		server executable for lab5

client		client executable for lab5

=head1 EXAMPLES

labcheck -v 4 1 . cat

labcheck -p 5 8 ~/spo/lab5/ task8_server task8_client

=head1 SEE ALSO

lcheck(1)

=head1 AUTHORS

(C) 2017

s207210

=cut
