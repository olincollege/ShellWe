#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS];
int n_clients = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg)
{
  // Detach the current thread.
  pthread_detach(pthread_self());

  int sock = *(int *)arg;
  char buffer[BUFFER_SIZE];
  int read_size;

  // Receive messages from client
  while ((read_size = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0)
  {
    buffer[read_size] = '\0'; // Null terminate the message

    // Lock the clients array
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < n_clients; i++)
    {
      if (client_sockets[i] != sock)
      {
        if (send(client_sockets[i], buffer, strlen(buffer), 0) < 0)
        {
          perror("send failed");
          continue;
        }
      }
    }
    pthread_mutex_unlock(&clients_mutex);
  }

  if (read_size == 0)
  {
    puts("Client disconnected");
    fflush(stdout);
  }
  else if (read_size == -1)
  {
    perror("recv failed");
  }

  // Remove the client
  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < n_clients; i++)
  {
    if (client_sockets[i] == sock)
    {
      while (i < n_clients - 1)
      {
        client_sockets[i] = client_sockets[i + 1];
        i++;
      }
      n_clients--;
      break;
    }
  }
  pthread_mutex_unlock(&clients_mutex);

  close(sock);
  return NULL;
}

int main(int argc, char *argv[])
{
  int socket_desc, new_socket, c, *new_sock;
  struct sockaddr_in server, client;
  pthread_t thread_id;

  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    printf("Could not create socket");
  }
  puts("Socket created");

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror("bind failed. Error");
    return 1;
  }
  puts("bind done");

  listen(socket_desc, 3);

  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  while (1)
  {
    new_socket = accept(
        socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    if (new_socket < 0)
    {
      perror("accept failed");
      break;
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    pthread_mutex_lock(&clients_mutex);
    client_sockets[n_clients++] = new_socket;
    pthread_mutex_unlock(&clients_mutex);

    if (pthread_create(&thread_id, NULL, handle_client, (void *)&new_socket) < 0)
    {
      perror("could not create thread");
      return 1;
    }

    puts("Handler assigned");
  }

  return 0;
}
