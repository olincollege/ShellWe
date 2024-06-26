#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../src/server/server.h"
#include "../src/util/util.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

// NOTE: This unit test was taken and modified from Assignment 8
// Check that the listener socket is successfully created.
Test(init_server, listener_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 1111);
  server_t* server = init_server(addr, MAX_BACKLOG);
  cr_expect(ge(int, server->listener, 0));
  close_tcp_socket(server->listener);
  free(server);
}

// NOTE: This unit test was taken and modified from Assignment 8
// Check that the address and port are set correctly.
Test(init_server, addr_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 2222);
  server_t* server = init_server(addr, MAX_BACKLOG);
  cr_expect(eq(u32, server->addr.sin_addr.s_addr, htonl(INADDR_ANY)));
  cr_expect(eq(u16, server->addr.sin_port, htons(2222)));
  close_tcp_socket(server->listener);
  free(server);
}

// NOTE: This unit test was taken and modified from Assignment 8
// Check that the backlog size is set correctly.
Test(init_server, backlog_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 2222);
  server_t* server = init_server(addr, MAX_BACKLOG);
  cr_expect(eq(int, server->max_backlog, MAX_BACKLOG));
  close_tcp_socket(server->listener);
  free(server);
}

// Check that the socket is accepting connections once it starts listening
Test(listen_for_connections, socket_listening) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 4444);
  server_t* server = init_server(addr, MAX_BACKLOG);
  listen_for_connections(server);
  int listening = 0;
  socklen_t option_len = sizeof(int);
  cr_expect(ne(int,
               getsockopt(server->listener, SOL_SOCKET, SO_ACCEPTCONN,
                          &listening, &option_len),
               -1));
  cr_expect(ne(int, listening, 0));
  close_tcp_socket(server->listener);
  free(server);
}

// Check that the server accepts a client connection
Test(accept_client, server_accepts) {
  // Create server
  struct sockaddr_in server_addr = socket_address(INADDR_ANY, 5555);
  server_t* server = init_server(server_addr, MAX_BACKLOG);
  listen_for_connections(server);

  pid_t pid = -1;
  pid = fork();

  if (pid < 0) {
    error_and_exit("Error forking process");
  } else if (pid == 0) {
    // Child, client
    int sock = open_tcp_socket();
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
        0) {
      close_tcp_socket(sock);
      error_and_exit("Failed to connect to server");
    }
    close_tcp_socket(sock);
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(EXIT_SUCCESS);
  } else {
    // Parent, server
    cr_expect(ge(int, accept_client(server), 0));
    if (waitpid(pid, NULL, 0) == -1) {
      error_and_exit("Error awaiting child process");
    }
    close_tcp_socket(server->listener);
    free(server);
  }
}

// Check that the server stores the client descriptor
Test(log_client, client_logged) {
  // Create server
  struct sockaddr_in server_addr = socket_address(INADDR_ANY, 6666);
  server_t* server = init_server(server_addr, MAX_BACKLOG);
  listen_for_connections(server);

  pid_t pid = -1;
  pid = fork();

  if (pid < 0) {
    error_and_exit("Error forking process");
  } else if (pid == 0) {
    // Child, client
    int sock = open_tcp_socket();
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
        0) {
      close_tcp_socket(sock);
      error_and_exit("Failed to connect to server");
    }
    close_tcp_socket(sock);
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(EXIT_SUCCESS);
  } else {
    // Parent, server
    int new_socket = accept_client(server);
    // cr_expect(ge(int, accept_client(server), 0));
    if (new_socket < 0) {
      error_and_exit("Error accepting client");
    }
    if (waitpid(pid, NULL, 0) == -1) {
      error_and_exit("Error awaiting child process");
    }
    // init variables for tracking clients
    int client_sockets[2];
    int* n_clients = malloc(sizeof(int));
    *n_clients = 0;
    pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

    // We are intentionally checking a garbage value to make sure the value is
    // changed.
    // NOLINTNEXTLINE(clang-analyzer-core.uninitialized.Assign)
    int prior = client_sockets[0];
    log_client(new_socket, n_clients, client_sockets, &clients_mutex);
    cr_expect(ne(int, client_sockets[0], prior));
    cr_expect(eq(int, *n_clients, 1));

    close_tcp_socket(server->listener);
    free(server);
  }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
