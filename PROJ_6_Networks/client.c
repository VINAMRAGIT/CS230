#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLINE 2048

void finish(char *flag)
{
    printf("FLAG CAPTURED!!!\n%s", flag);
    return;
}

/**
 * This function provides the main function with a functioning
 * socket file descriptor that can be used for client-server
 * communication. It uses the sockaddr_in and SOCKET API
 * struct to be able to provide the connection.
 *
 * This function consists of steps 1 and 2 of the program.
 *
 * @param port the port number of the host
 * @param add a pointer to the in_addr struct containing the host IP
 * @return integer containing the file descriptor for the socket
 */
int openClientFd(int port, struct in_addr *add)
{
    // STEP 1: Creating a TCP socket() and connect to the server
    int clientfd = socket(AF_INET, SOCK_STREAM, 0); // Creating clientfd so that the connection fd can be returned
    if (clientfd == -1)                             // We create the socket file descriptor with the given protocol and protocol family.
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serveraddr;              // Creating sockaddr_in to store the server info
    memset(&serveraddr, 0, sizeof(serveraddr)); // Setting bytes to 0
    serveraddr.sin_family = AF_INET;            // Setting protocol
    serveraddr.sin_port = htons(port);          // Setting port
    serveraddr.sin_addr = *add;                 // Setting IP
    printf("Socket Created:%i\n", clientfd);

    // STEP 2: Connecting to the server
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0)
    {
        perror("connection with the server failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server!\n");
    return clientfd;
}

/**
 * This function decrypts(encrypts, really) the value
 * specified by the "message" value. This uses the
 * substitution algorithm to replace the values in the
 * message from the corresponding characters in the
 * cypher.
 *
 * @param cypher the 26 character long cypher
 * @param cypherLen the length of the cypher string
 * @param message the message to be encrypted
 * @param messageLen length of the message
 *
 * @return a string pointing to the encrypted string
 */
char *decrypt(char *cypher, int cypherLen, char *message, int messageLen)
{

    char *alpha = malloc(26);
    alpha = "abcdefghijklmnopqrstuvwxyz";

    char *result = malloc(messageLen);
    for (int i = 0; i < messageLen; i++)
    {
        char temp = message[i];
        for (int j = 0; j < cypherLen; j++)
        {
            if (temp == alpha[j])
            {
                result[i] = cypher[j];
                break;
            }
        }
    }
    result[messageLen] = '\0';
    return result;
}

/**
 * The goal in this method would be to implement a client-server relationship,
 * in which we receive a message encrypted in the substitution cypher. We decrypt
 * the message and send it back to the server.
 *
 * TIPS THAT WILL HELP>>>
 *
 * Input format:
 * Identification
 * Port
 * Host IP address
 *
 * Useful functions:
 *  socket(int domain, int type, int protocol);
 *  ssize_t send(int socket, const void * buffer, size_t length, int flags);
 *  ssize_t recv(int socket, void * buffer, size_t length, int flags);
 *  ssize_t connect(int socket, const struct sockaddr *address, socklen_t address_len);
 *  int close(int fildes);
 *
 * Hint: The network requires big-endian format, so you will have to convert between the two.
 * Helpful functions for this are htons and inet_pton. They do have man pages, so take a look.
 *
 * @param argc amount of arguments provided to the function call
 * @param argv a pointer to strings of arguments: identification, port number, and host IP
 *
 * @return integer signalling end of program or error
 */
int main(char argc, char **argv)
{
    // Checking arguments, and nitializing all variables for use
    if (argc != 4)
    {
        fprintf(stderr, "usage: %s \n", argv[0]);
        exit(1);
    }

    char *identity = malloc(strlen("cs230 HELLO ") + strlen(argv[1]) + strlen("\n") + 1);
    strcpy(identity, "cs230 HELLO ");
    strcat(identity, argv[1]);
    strcat(identity, "\n");
    struct in_addr inp = {inet_addr(argv[3])}; // Creating in_addr struct to store IP address
    int port = atoi(argv[2]);                  // port number
    char buf[MAXLINE];                         // String buffer used to store data written to client

    int clientfd = openClientFd(port, &inp); // Obtaining clientfd after completing steps 1 and 2

    // STEP 3: Send the client the identification string
    ssize_t question = send(clientfd, identity, strlen(identity), 0);

    // STEP 4: Receive the encrypted string to decrypt
    int n;
    while (n = recv(clientfd, buf, MAXLINE, 0) != 0)
    {
        if (n == -1)
        {
            perror("Reading failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Generating cypher and message from the status string

            char *str1 = malloc(strlen(buf)); // Duplicating buf as strtok overrides the input string
            strcpy(str1, buf);                // Duplicating buf as strtok overrides the input string
            char *word;                       // Used to store the every word separated by spaces
            char *cypher;                     // Stores the cypher string
            char *flag;                       // Stores the flag string in case of reading end
            int count = 0;                    // Counter to check when we reach the cypher and message
            word = strtok(str1, " ");         // Using strtok to parse the string by spaces

            while (word != NULL)
            {
                count++;
                if (count == 2)
                {
                    flag = strdup(word);
                    if (strcmp(flag, "STATUS") != 0)
                    {
                        finish(flag);
                        close(clientfd);
                        exit(EXIT_SUCCESS);
                    }
                }
                else if (count == 3)
                    cypher = strdup(word);
                else if (count == 4)
                    break;
                word = strtok(NULL, " ");
            }

            // Now that we have the cypher and message string, we can decrypt
            char *result = malloc(strlen("cs230 ") + strlen(word) + 1);
            strcpy(result, "cs230 ");
            strcat(result, decrypt(cypher, strlen(cypher), word, strlen(word)));
            strcat(result, "\n");

            // STEP 5: Writing the decrypted string to the server
            question = send(clientfd, result, strlen(result), 0);
        }
    }
    close(clientfd);
    return 0;
}
