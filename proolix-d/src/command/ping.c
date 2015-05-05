/*
$Id: PING.C,v 1.1.1.1 2003/09/07 14:09:56 Prool Exp $

$Log: PING.C,v $
Revision 1.1.1.1  2003/09/07 14:09:56  Prool
Import

Revision 1.1.1.1  2003/09/07 13:57:57  Prool
Imported sources

Revision 1.1.1.1  2003/09/07 13:53:53  Prool
Proolix-d 7 Sep 2003

Revision 1.1.1.1  2003/09/07 13:45:54  Prool
Proolix-d 7 Sep 2003

Revision 1.1.1.1  2003/09/07 13:42:42  Prool
Proolix-d 7 Sep 2003

Revision 1.1.1.1  2003/09/07 12:15:04  Prool
Proolix 7 Sep 2003

Revision 1.2  1996/04/17 09:47:39  axl
Specially for Proolix - removed TCP/IP support and hostname resolving

Revision 1.1  1996/04/11 14:35:54  axl
Initial revision

 */

#include <stdio.h>

#ifdef Proolix
#include <process.h>
#include <string.h>
#endif

#ifdef Proolix
#define DELAY_TIME 300000L
#else
#define DELAY_TIME 300000
#endif

#define PONG_COUNT 100

version()
{
        printf("Proolix PING version 0.1\n");
        printf("TCP/IP _N0T_ REQURIED !!!\n");
        printf("(c) Copyright Dmitry A.Deineka, axl@infocom.kharkov.ua\n");
        printf("                    http://www.infocom.kharkov.ua/~axl\n");
        printf("======================================================\n");
}

usage()
{
        version();
        printf("Usage: ping <host>    -- Ping specified host\n");
        printf("       ping -h        -- Display this help\n");
        printf("       ping -v        -- Display version\n");

}

do_ping(const char *host_to_ping)
{
        unsigned long seq=0;
        unsigned long dl;
        unsigned long count;

        version();
        printf("PING %s: 56 data bytes, MAX %lu packets will be sent\n", host_to_ping, PONG_COUNT);
        for (count=0; count<=PONG_COUNT; count++)
        {
                printf ("64 bytes from %s: icmp_seq=%lu ttl=255 time=0.718 ms\n",
                        host_to_ping, seq);
                seq++;
                for (dl=0; dl<DELAY_TIME; dl++);
        }
}

/* Glucks and Bugs Software Incorporated :-) */
main(int argc, char *argv[])
{
        if (argc == 1) {usage(); goto l_exit;}

        if (strcmp("-h", argv[1]) == 0) {usage(); goto l_exit;}
        if (strcmp("-v", argv[1]) == 0)
        {
                version(); goto l_exit;
        }

        do_ping(argv[1]);
l_exit:;
}
