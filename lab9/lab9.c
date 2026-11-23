/*
Questions to answer at top of client.c:
(You should not need to change the code in client.c)
1. What is the address of the server it is trying to connect to (IP address and port number).
   127.0.0.1 (the local machine), port 8000. ADDR and PORT are
   assigned to addr.sin_addr and addr.sin_port before connect().

2. Is it UDP or TCP? How do you know?
   TCP. socket(AF_INET, SOCK_STREAM, 0) selects an IPv4 TCP stream,
   and connect() establishes the connection to the server.

3. The client is going to send some data to the server. Where does it get this data from? How can
you tell in the code? Standard input, normally the keyboard, but also possibly a file or pipe.
read(STDIN_FILENO, buf, BUF_SIZE) reads that input, and write(sfd, buf, num_read) sends the bytes to
the server.

4. How does the client program end? How can you tell that in the code?
   The loop continues only while read() returns more than one byte.
   EOF (zero bytes) or a one-byte read ends it, then close(sfd) closes the
   connection and exit(EXIT_SUCCESS) ends the program.
   Ctrl-D on an empty input line supplies EOF. Any one-byte read ends the
   loop, not just a newline, and that byte is not sent. A read error returns
   -1 and invokes handle_error(), which prints an error and exits with
   EXIT_FAILURE. The other checked setup/write failures also use
   handle_error().
*/
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

int main() {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexes to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;
  char buf[BUF_SIZE + 1];

  printf("New client created! ID %d on socket FD %d\n", client->client_id, client->cfd);

  for (;;) {
    ssize_t num_read = read(client->cfd, buf, BUF_SIZE);
    if (num_read == 0) {
      break;
    }
    if (num_read == -1) {
      if (errno == EINTR) {
        continue;
      }
      perror("read");
      break;
    }

    buf[num_read] = '\0';
    // shortcut: count read chunks; add framing if logical messages are needed.
    // Keep the count and its output together so message numbers print in order.
    pthread_mutex_lock(&count_mutex);
    ++total_message_count;
    printf("Msg #%4d; Client ID %d: %s%s", total_message_count, client->client_id, buf,
           buf[num_read - 1] == '\n' ? "" : "\n");
    pthread_mutex_unlock(&count_mutex);
  }

  if (close(client->cfd) == -1) {
    perror("close");
  }
  printf("Ending thread for client %d\n", client->client_id);
  free(client);
  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  // Start threads detached so their resources are reclaimed on return.
  pthread_attr_t attr;
  int err = pthread_attr_init(&attr);
  if (err != 0) {
    errno = err;
    handle_error("pthread_attr_init");
  }
  err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (err != 0) {
    errno = err;
    handle_error("pthread_attr_setdetachstate");
  }

  for (;;) {
    int cfd = accept(sfd, NULL, NULL);
    if (cfd == -1) {
      if (errno != EINTR) {
        perror("accept");
      }
      continue;
    }

    struct client_info *client = malloc(sizeof(*client));
    if (client == NULL) {
      perror("malloc");
      close(cfd);
      continue;
    }
    client->cfd = cfd;
    pthread_mutex_lock(&client_id_mutex);
    client->client_id = client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    pthread_t thread;
    err = pthread_create(&thread, &attr, handle_client, client);
    if (err != 0) {
      // pthread functions return an error number rather than setting errno.
      errno = err;
      perror("pthread_create");
      close(cfd);
      free(client);
    }
    // On success, the worker owns client and cfd; main must not use them.
  }

  pthread_attr_destroy(&attr);
  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
