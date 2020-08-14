#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PERIOD 0x2e
#define	hyphenchar(c) ((c) == 0x2d)
#define bslashchar(c) ((c) == 0x5c)
#define periodchar(c) ((c) == PERIOD)
#define asterchar(c) ((c) == 0x2a)
#define alphachar(c) (((c) >= 0x41 && (c) <= 0x5a) \
		   || ((c) >= 0x61 && (c) <= 0x7a))
#define digitchar(c) ((c) >= 0x30 && (c) <= 0x39)
#define underscorechar(c)  ((c) == 0x5f)

#define borderchar(c) (alphachar(c) || digitchar(c))
#define middlechar(c) (borderchar(c) || hyphenchar(c) || underscorechar(c))
#define	domainchar(c) ((c) > 0x20 && (c) < 0x7f)

int
res_hnok(const char *dn) {
	int pch = PERIOD, ch = *dn++;

	while (ch != '\0') {
		int nch = *dn++;

		if (periodchar(ch)) {
			;
		} else if (periodchar(pch)) {
			if (!borderchar(ch))
				return (0);
		} else if (periodchar(nch) || nch == '\0') {
			if (!borderchar(ch))
				return (0);
		} else {
			if (!middlechar(ch))
				return (0);
		}
		pch = ch, ch = nch;
	}
	return (1);
}

int main(int argc, char *argv[])
{
	const char* testurl = "dns-server-dns-server-dns-server-dns-server-dns-server.zte.com.cn";

    printf("res_hook return %d\n", res_hnok(testurl));

    exit(0);


    if (argc != 2) {
        printf("Usage:%s <domain name>\n", basename(argv[0]));
        return -1;
    }

    struct addrinfo hint, *result;
    memset(&hint, 0, sizeof(hint));

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    int ret = getaddrinfo(argv[1], nullptr, &hint, &result);
    if (ret != 0) {
        printf("getaddrinfo %s failed, %d %s\n", argv[1], ret, gai_strerror(ret));
        return -1;
    }

    for (; result; result = result->ai_next) {
        if (result->ai_family == AF_INET) {
            struct sockaddr_in* addr;
            addr = (struct sockaddr_in*)result->ai_addr;
            printf("ip:%s, cname:%s\n", inet_ntoa(addr->sin_addr), result->ai_canonname);
        }
    }

    freeaddrinfo(result);

    return 0;
}
