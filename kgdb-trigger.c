/*
 * kgdb-trigger - tool to trigger remote linux target to enter kgdb
 * over serial port
 *
 * Copyright (C) 2017 Kirill Marinushkin <k.marinushkin@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

static void print_help(void)
{
    printf("Usage: " PROGNAME " /dev/ttyX\n"
	   "       " PROGNAME " [OPTION]\n"
	   "Trigger remote Linux target to enter kgdb\n"
	   "\n"
	   "Options:\n"
	   "/dev/ttyX     serial port device to send signal to\n"
	   "-h, --help    display this help and exit\n"
           "-v, --version display version and exit\n");
}

static void print_version(void)
{
    printf(PROGNAME " version " VERSION "\n");
}

static int send_kgdb_signal(const char *devname)
{
    const int break_dur = 0;
    const char trigger_sym[] = "g";
    int rc = 1;
    int fd = open(devname, O_RDWR);

    if (fd < 0) {
	perror(devname);
	goto out;
    }

    if (tcsendbreak(fd, break_dur)) {
	perror("send break");
	goto out;
    }

    if (0 >= write(fd, trigger_sym, 1)) {
	perror("write");
	goto out;
    }

    rc = 0;

 out:
    if (fd >= 0)
	close(fd);
    
    return rc;
}

int main(int argc, char *argv[])
{
    struct option ops[] = {
	{"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        { NULL, 0, 0, 0 }
    };
    int op_ind = 0;
    int c;

    if (argc != 2) {
	print_help();
	return 1;
    }

    c = getopt_long(argc, argv, "hv", ops, &op_ind);

    switch (c) {
    case 'h':
	print_help();
	break;

    case 'v':
	print_version();
	break;

    default:
	return send_kgdb_signal(argv[1]);
    }

    return 0;
}

