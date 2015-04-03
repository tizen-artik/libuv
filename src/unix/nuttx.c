#include "uv.h"
#include "internal.h"
#include "uv-nuttx.h"

#include <assert.h>
#include <poll.h>
  
int uv__platform_loop_init(uv_loop_t* loop) {
  loop->npollfds = 0;
  return 0;
}


void uv__platform_loop_delete(uv_loop_t* loop) {
  loop->npollfds = 0;
}


void uv__add_pollfd(uv_loop_t* loop, struct pollfd* pe) {
  int i;
  bool exist = false;
  for (i = 0; i < loop->npollfds; ++i) {
    struct pollfd* cur = &loop->pollfds[i];
	if (cur->fd == pe->fd) {
	  cur->events = pe->events;
	  exist = true;
	  break;
	}
  }
  if (!exist) {
    struct pollfd* cur = &loop->pollfds[loop->npollfds++];
	cur->fd = pe->fd;
	cur->events = pe->events;
	cur->revents = 0;
	cur->sem = 0;
	cur->priv = 0;
  }
}


void uv__rem_pollfd(uv_loop_t* loop, struct pollfd* pe) {
  int i = 0;
  while (i < loop->npollfds) {
    struct pollfd* cur = &loop->pollfds[i];
	if (cur->fd == pe->fd) {
	  *cur = loop->pollfds[--loop->npollfds];
	} else {
	  ++i;
	}
  }
}


void uv__io_poll(uv_loop_t* loop, int timeout) {
  struct pollfd pfd;
  struct pollfd* pe;
  QUEUE* q;
  uv__io_t* w;
  uint64_t base;
  uint64_t diff;
  int nevents;
  int count;
  int nfd;
  int i;

  if (loop->nfds == 0) {
    assert(QUEUE_EMPTY(&loop->watcher_queue));
	return;
  }

  while (!QUEUE_EMPTY(&loop->watcher_queue)) {
    q = QUEUE_HEAD(&loop->watcher_queue);
	QUEUE_REMOVE(q);
	QUEUE_INIT(q);

	w = QUEUE_DATA(q, uv__io_t, watcher_queue);
	assert(w->pevents != 0);
	assert(w->fd >= 0);
	assert(w->fd < (int)loop->nwatchers);

	pfd.fd = w->fd;
	pfd.events = w->pevents;

	uv__add_pollfd(loop, &pfd);

	w->events = w->pevents;
  }

  assert(timeout >= -1);
  base = loop->time;
  count = 5;

  for (;;) {
    nfd = poll(loop->pollfds, loop->npollfds, timeout);

	SAVE_ERRNO(uv__update_time(loop));
	
	if (nfd == 0) {
	  assert(timeout != -1);
	  return;
	}

	if (nfd == -1) {
	  if (errno != EINTR) {
	    abort();
	  }
	  if (timeout == -1) {
	    continue;
	  }
	  if (timeout == 0) {
	    return;
	  }
	  goto update_timeout;
	}

	nevents = 0;

	for (i = 0; i < loop->npollfds; ++i) {
      pe = &loop->pollfds[i];
	  if (pe->fd >= 0 && pe->revents & POLLIN) {
	    w = loop->watchers[pe->fd];
		if (w == NULL) {
          uv__rem_pollfd(loop, pe);
		} else {
		  w->cb(loop, w, pe->revents);
		  ++nevents;
		}
	  }
	}

	if (nevents != 0) {
	  if (--count != 0) {
        timeout = 0;
		continue;
	  }
	  return;
	}
	if (timeout == 0) {
	  return;
	}
	if (timeout == -1) {
	  continue;
	}
update_timeout:
	assert(timeout > 0);

	diff = loop->time - base;
	if (diff >= (uint64_t)timeout) {
	  return;
	}
	timeout -= diff;
  }
}


int uv_fs_event_start(uv_fs_event_t* handle,
                      uv_fs_event_cb cb,
                      const char* filename,
                      unsigned int flags) {
  return 0;
}


int uv_fs_event_stop(uv_fs_event_t* handle) {
  return 0;
}


void uv__fs_event_close(uv_fs_event_t* handle) {
}


uint64_t uv__hrtime(uv_clocktype_t type) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  uint64_t ret = (((uint64_t) ts.tv_sec) * ((uint64_t) 1e9) + ts.tv_nsec);
  return ret;
}


void uv__platform_invalidate_fd(uv_loop_t* loop, int fd) {
  int i;
  int nfd = loop->npollfds;
  for (i = 0; i < nfd; ++i) {
    struct pollfd* pfd = &loop->pollfds[i];
	pfd->fd = -1;
  }
}



int getpeername(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
  return 0;
}


ssize_t readv(int __fd, const struct iovec* __iovec, int __count) {
  ssize_t result = 0;
  ssize_t total = 0;
  int idx;
  for (idx = 0; idx < __count; ++idx) {
    result = read(__fd, __iovec[idx].iov_base, __iovec[idx].iov_len);
	if (result < 0) {
      return result;
	} else {
		total += result;
	}
  }
  return total;
}


ssize_t writev(int __fd, const struct iovec* __iovec, int __count) {
  ssize_t result = 0;
  ssize_t total = 0;
  int idx;
  for (idx = 0; idx < __count; ++idx) {
    result = write(__fd, __iovec[idx].iov_base, __iovec[idx].iov_len);
	if (result < 0) {
      return result;
	} else {
		total += result;
	}
  }
  return total;
}
