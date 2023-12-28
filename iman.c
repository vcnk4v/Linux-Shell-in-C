#include "headers.h"

#define MAX_BUFFER_SIZE 1024

char *remove_tags(char *string)
{
    int tag = 0;
    char out[MAX_BUFFER_SIZE];
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '<')
        {
            tag = 1;
        }
        if (!tag)
        {
            strncat(out, &string[i], 1);
        }
        if (string[i] == '>')
        {
            tag = 0;
        }
    }
    string = out;
    return string;
}

void print_html(char *html)
{
    char *body_start = "NAME\n";
    char *body_end = "</body>";

    char *pointer_start = strstr(html, body_start);
    if (pointer_start == NULL)
    {
        pointer_start = html;
    }
    // else
    // {
    //     pointer_start = strstr(pointer_start, ">") + 1;
    // }

    char *pointer_end = strstr(pointer_start, body_end);
    if (pointer_end == NULL)
    {
        pointer_end = html + strlen(html);
    }

    int length = pointer_end - pointer_start;

    char substr[length + 1];
    strncpy(substr, pointer_start, length);
    substr[length] = '\0';

    printf("%s\n", substr);
}

void fetch_man_page(char *command_name)
{
    const char *hostname = "man.he.net";
    char url_format[512];
    sprintf(url_format, "/?topic=%s&section=all", command_name);
    int port = 80; // HTTP port

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // ipv4?? both?
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
    {
        fprintf(stderr, RED_TEXT);
        perror("DNS resolution failed");
        fprintf(stderr, NORMAL_TEXT);
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    /*
    host to network short : 16-bit integer value is converted to the correct network byte order,
    so it can be safely transmitted over a network or stored in a network data structure.
    */

    struct addrinfo *rp;
    for (rp = res; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            server_addr.sin_addr = ipv4->sin_addr;
            break;
        }
    }

    freeaddrinfo(res);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        fprintf(stderr, RED_TEXT);
        perror("Error: Creating socket");
        fprintf(stderr, NORMAL_TEXT);

        return;
    }
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        fprintf(stderr, RED_TEXT);
        perror("Error: Connection to server");
        fprintf(stderr, NORMAL_TEXT);
        close(sockfd);
        return;
    }

    char request[MAX_BUFFER_SIZE];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url_format, hostname);

    if (send(sockfd, request, strlen(request), 0) == -1)
    {
        fprintf(stderr, RED_TEXT);
        perror("Error: Failed to send GET request");
        fprintf(stderr, NORMAL_TEXT);
        close(sockfd);
        return;
    }
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;
    char error[4096];
    snprintf(error, sizeof(error), "No matches for \"%s\"", command_name);

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        if (strstr(buffer, error))
        {
            fprintf(stderr, RED_TEXT "No page exists\n" NORMAL_TEXT);
            return;
        }

        buffer[bytes_received] = '\0';
        print_html(buffer);

        // printf("%s", buffer);
    }

    if (bytes_received < 0)
    {
        fprintf(stderr, RED_TEXT);
        perror("Error: Failed to receive");
        fprintf(stderr, NORMAL_TEXT);
    }
    close(sockfd);
}
