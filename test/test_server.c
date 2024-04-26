#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../src/server/server.h"
#include "../src/util/util.h"

// Check that the listener socket is successfully created.
Test(init_server, listener_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 1111);
  server_t* server = init_server(addr, MAX_BACKLOG);
  cr_expect(ge(int, server->listener, 0));
  close_tcp_socket(server->listener);
  free(server);
}

// Check that the address and port are set correctly.
Test(init_server, addr_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 2222);
  server_t* server = init_server(addr, MAX_BACKLOG);
  cr_expect(eq(u32, server->addr.sin_addr.s_addr, htonl(INADDR_ANY)));
  cr_expect(eq(u16, server->addr.sin_port, htons(2222)));
  close_tcp_socket(server->listener);
  free(server);
}

// Check that the backlog size is set correctly.
Test(init_server, backlog_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 2222);
  server_t* server = init_server(addr, MAX_BACKLOG);
  cr_expect(eq(int, server->max_backlog, MAX_BACKLOG));
  close_tcp_socket(server->listener);
  free(server);
}
