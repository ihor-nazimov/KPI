#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
/* Отображение содержимого Web-страницы, полученной из
   серверного сокета. */
int socket_fd;

void get_home_page(int socket_fd) {
    char buffer[10000];
    ssize_t number_characters_read;
    /* Отправка HTTP-команды GET с запросом начальной страницы. */
    sprintf(buffer, "GET / HTTP/1.1\r\n");
    write(socket_fd, buffer, strlen(buffer));
    sprintf(buffer, "Host: www.google.com\r\n\r\n");
    write(socket_fd, buffer, strlen(buffer));
    /* Чтение данных из сокета. Функция read() может вернуть
        не все данные сразу, поэтому продолжаем чтение, пока
        не будут получены все данные. */
    while (1) {
        //if ( (number_characters_read = recv(socket_fd, buffer, 10000, MSG)) <= 0 )
        printf("\n\nNext read: \n")
        if ((number_characters_read = read(socket_fd, buffer, 10000)) <=0 )
            return;
        printf("\n\nReaded counter:%d\n\n", number_characters_read);
        
        
        //printf("*********%d*****************\n", number_characters_read);
        /* Запись данных в стандартный выходной поток. */
        fwrite(buffer, sizeof(char), number_characters_read, stdout);
    }
printf("gethome3\n");

}

void closeonexit(void) {
    close(socket_fd);
}

int main(int argc, char* const argv[]) {
//    struct sockaddr_in name;
    struct sockaddr_in name;
    struct hostent* hostinfo;
    char argv_hostname[256] = "www.google.com";
    char argv_path[1024] = "/";
    unsigned int argv_port = 80;
    /* Создание сокета. */
    if (argc != 2) {
        printf("Use: lab4_client.out hostname[:port][/path_to_file]\n");
        exit(1);
    }

    sscanf(argv[1], "$s:", argv_hostname);
    sscanf(argv[1], ":$d", argv_port);
    sscanf(argv[1], "$*s/$s", argv_path);
    printf("argv[1]:%s\n", argv[1]);
    printf("hostname:%s\n", argv_hostname);
    printf("port:%d\n", argv_port);
    printf("path:%s\n", argv_path);
    //return;
printf("Test1\n");


    atexit(closeonexit);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    /* Запись имени сервера в адресную структуру. */
    name.sin_family = AF_INET;
    /* Преобразование адреса из текстового представления во
    внутреннюю форму. */
    hostinfo = gethostbyname(argv_hostname);
    if (hostinfo == NULL) {
        printf("Can't process hostname %s\n", argv_hostname);
        return 1;
    }
    else    
        name.sin_addr = *((struct in_addr*)hostinfo->h_addr);
    /* Web-серверы используют порт 80. */
    name.sin_port = htons(argv_port);
    /* Подключаемся к Web-серверу. */
    if (connect(socket_fd, (struct sockaddr *) &name,
//        sizeof(struct sockaddr_in)) == -1) {
        sizeof(name)) == -1) {
        perror("connect");
        return 1;
    }
printf("Test2\n");
    /* получаем содержимое начальной страницы сервера. */
    get_home_page(socket_fd);
    return 0;
}