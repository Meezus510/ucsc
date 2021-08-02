you can run this program after runing: make
then run: ./httpserver <port>
and the optional flags: -l <log_file.file> -N <integer>
can be run in any order ex: ./httpserver -N 8 -l logfile 8014
note: logfile does not exitst one will be created.

This program is very messy and ugly. If I had another day or so I could orginize it better
Passes most test except some that require logging although it passes the tests on my end.