/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef UV_NUTTX_H
#define UV_NUTTX_H

#include <unistd.h>
#include <errno.h>
#include <poll.h>


#ifndef ENOTSUP
#define ENOTSUP EOPNOTSUPP
#endif

#ifndef STDIN_FILENO
#define STDIN_FILNO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILNO 1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#ifndef SIGPROF
#define SIGPROF 27
#endif

#ifndef SIGCHID
#define SIGCHLD 17
#endif

#ifndef SHUT_RD
#define SHUT_RD 0
#endif 

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

#ifndef TCP_NODELAY
#define TCP_NODELAY 1  
#endif

#ifndef _SC_CLK_TCK
#define _SC_CLK_TCK 0x0006
#endif

#ifndef _SC_NPROCESSORS_ONLN
#define _SC_NPROCESSORS_ONLN 0x0061
#endif

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif


struct iovec 
{
	void* iov_base;
	size_t iov_len;
};

ssize_t readv(int __fd, const struct iovec* __iovec, int __count);
ssize_t writev(int __fd, const struct iovec* __iovec, int __count);

struct msghdr;

struct addrinfo {
	int ai_flags;
	int ai_family;
	int ai_socktype;
	int ai_protocol;
};

int getpeername(int sockfd, struct sockaddr* addr, socklen_t* addrlen);


#define UV_PLATFORM_LOOP_FIELDS                                               \
  struct pollfd pollfds[1024];                                                \
  int npollfds;                                                               \


#endif /* UV_NUTTX_H */
