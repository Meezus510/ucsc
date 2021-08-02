#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <unistd.h> // write
#include <string.h> // memset
#include <stdlib.h> // atoi
#include <pthread.h>

#define BUFFER_SIZE 4096
void *thread_server(void*);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t  cond_for  = PTHREAD_COND_INITIALIZER;
pthread_cond_t server_wait = PTHREAD_COND_INITIALIZER;
pthread_cond_t thread_creat = PTHREAD_COND_INITIALIZER;


int log_file;
int log_request;
char* port;
int Synchroniz;
int entries, errors;
int thread_cnt, thread_NUM;
int counter = -1;

int server_sockd;
int substring_index(char *string, char *sub_string){

    int i = 0;
     int d = 0;
     if (strlen(string) >= strlen(sub_string)) {
         for (i = strlen(string) - strlen(sub_string); i >= 0; i--) {
             int found = 1; 
             for (d = 0; d < strlen(sub_string); d++) {
                 if (string[i + d] != sub_string[d]) {
                     found = 0; 
                     break;
                 }
             }
             if (found == 1) {
                 return i;
             }
         }
         return -1;
     } else {
         return -1;
     }
}

char *get_value(char* string, int from, char to){
    
    char* start = &string[from-1];
    int i = from-1;
    int len = 0;
    //char *value;
    while(string[i]!=to){
        i++;
        len++;
    }
    char *end = &start[from+len];
    char *value = (char*)calloc(1,end - start + 1);
    i = from-1;
    int count = 0;
    while(string[i]!=to){
        value[count]=string[i];
        i++;
        count++;
    }
    return value;
}

void *thread_server(void* sock){

    pthread_mutex_lock( &mutex1 );
    counter ++;
    int local_count=counter;
    pthread_cond_signal( &thread_creat );
    if(counter<thread_NUM)pthread_cond_wait( &server_wait, &mutex1 );
    struct sockaddr client_addr;
    socklen_t client_addrlen;
    printf("[+] server is waiting...\n");
    int client_sockd = accept(server_sockd, &client_addr, &client_addrlen);
    uint8_t buff[BUFFER_SIZE + 1];
    memset(buff, 0 , BUFFER_SIZE+1);
    ssize_t bytes = recv(client_sockd, buff, BUFFER_SIZE, 0);
    char char_buff[BUFFER_SIZE+1];
    memset(char_buff, 0, strlen(char_buff));
    char request[4];
    memset(request, 0, strlen(request));
    for(int i =0;i<bytes;i++){
        char_buff[i] = (char)buff[i];
        //if(i<3)request[i]=(char)buff[i];
    } 
    buff[bytes] = 0;
    int offset;
    char *req = get_value(char_buff, 1, ' ');
    if(strcmp(req, "PUT")==0){
        int index = substring_index(char_buff, "Content-Length: ");
        char *val;
        val = get_value(char_buff, index+17, '\n');
        size_t length=0;
        length = atoi(val);

        char *file_name;
        file_name = get_value(char_buff, 6, ' ');

        int err_400 = 0;

        char *http;
        http = get_value(char_buff, 7+strlen(file_name), '\r');
        if(strcmp(http, "HTTP/1.1"))err_400=1;

        if(strlen(file_name)>27 || strlen(file_name)<=0){
            err_400=1;
        }else{
            for(int i = 0; i<strlen(file_name);i++){
                if ((file_name[i] >= 'a' && file_name[i] <= 'z') 
                || (file_name[i] >= 'A' && file_name[i] <= 'Z')){}
                else if(file_name[i] >= '0' && file_name[i] <= '9'){}
                else if(file_name[i] == '-'||file_name[i]== '_'){}
                else {
                    err_400=1;
                    break;
                }
            }
        }
        
        if(err_400==1){
            char* err = "HTTP/1.1 400 Bad Request\r\n ";
            send(client_sockd , err , strlen(err) , 0 );
            close(client_sockd);
            write(1,err,strlen(err));
            
            if(log_request==1){
                char log_err[100] = "FAIL: PUT /";
                
                strcat(log_err, file_name);
                strcat(log_err, " HTTP/1.1 --- response 400\n========\n");
                pwrite(log_file, log_err, strlen(log_err),Synchroniz);
                Synchroniz = Synchroniz +strlen(log_err);
            }
            errors++;
            if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
            else if(local_count==thread_NUM-1){
            pthread_cond_signal( &cond_for );
            pthread_cond_signal( &cond );
            }
            else pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex1 );
            return 0;
        }

        if(strcmp(file_name, "healthcheck")==0){
            char* err = "HTTP/1.1 403 Forbidden\r\n ";
            send(client_sockd , err , strlen(err) , 0 );
            close(client_sockd);
            //close(server_sockd);
            write(1,err,strlen(err));
            errors++;
            if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
            else if(local_count==thread_NUM-1){
            pthread_cond_signal( &cond_for );
            pthread_cond_signal( &cond );
            }
            else pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex1 );
            return 0;
        }
        
        if(log_request==1){
            char* requestFile = malloc(strlen(file_name)+1024);
            memset(requestFile, 0, strlen(requestFile));
            strcat(requestFile, "PUT /");
            strcat(requestFile, file_name);
            strcat(requestFile, " length ");
            strcat(requestFile, val);
            pwrite(log_file, requestFile, strlen(requestFile), Synchroniz);
            offset = strlen(requestFile);
            Synchroniz = Synchroniz +offset;
            free(requestFile);
        }

        int fd;
        //printf("stuck thread put out\n");
        fd = open(file_name, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0777);
        size_t lenn = sizeof(buff);
        ssize_t byte = 0;
        long a=0;
        long total=0;
        int len = 69;
        char hex[len];
        char zeros[9] = "00000000";
        char byte_size[9];
        char temp[10];
        int hex_value;
        memset(hex, 0, len);
        memset(byte_size, 0, strlen(byte_size));
        memset(temp, 0, strlen(temp));
        while(length > 0){
            byte =recv(client_sockd, buff, BUFFER_SIZE, 0);
            total++;
            write(fd, buff, byte);

            if(log_request==1){
                for(int i = 0; i<byte; i++){
                    
                    if(a%20==0){
                        strcat(hex, "\n");
                        
                        pwrite(log_file, hex, strlen(hex), Synchroniz);
                        Synchroniz=Synchroniz+strlen(hex);

                        memset(hex, 0, len);
                        memset(temp, 0, strlen(temp));
                        sprintf(byte_size, "%ld", a);
                        int inc = 0;
                        for(int j = 0; j<8; j++){
                            if(j>=(8-strlen(byte_size))){
                                zeros[j]=byte_size[inc];
                                inc++;
                            } 
                        }
                        strcat(hex, zeros);


                    }
                    sprintf(temp, "%02x", buff[i]);
                    
                    strcat(hex, " ");
                    strcat(hex, temp);
                    a++;
                }
                

            }
            length = length - (size_t)byte;
            buff[bytes] = 0;
        }
        if(log_request==1){
            strcat(hex,"\n========\n");
            pwrite(log_file, hex, strlen(hex), Synchroniz);
            Synchroniz=Synchroniz+strlen(hex);
            memset(hex, 0, len);
        }


        close(fd);
        char* created = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
        send(client_sockd , created , strlen(created) , 0 );
        write(1, created, strlen(created));
    }else if(strcmp(req, "GET")==0){
        int buffer= 0;
        char *file_name;
        file_name = get_value(char_buff, 6, ' ');

        if(strcmp(file_name, "healthcheck")==0){
            if(log_request==1){
                entries++;
                char entries_string[200];
                char errors_string[100];
                char health[1024] = "HTTP/1.1 200 OK\r\nContent-Length: ";

                sprintf(entries_string, "%d", errors);
                strcat(entries_string, "\n");
                sprintf(errors_string, "%d", entries);
                strcat(entries_string, errors_string);
                int health_len = strlen(entries_string);
                char healthl[100];
                sprintf(healthl, "%d", health_len);
                strcat(health, healthl);
                char onses[]="\r\n\r\n";
                strcat(health, onses);
                send(client_sockd , health , strlen(health) , 0 );
                send(client_sockd , entries_string , strlen(entries_string) , 0 );

                close(client_sockd);
                write(1,health,strlen(health));
                pthread_mutex_unlock( &mutex1 );
                return 0;
            }else{
                char* err = "HTTP/1.1 404 Not Found\r\n ";
                send(client_sockd , err , strlen(err) , 0 );
                close(client_sockd);
                write(1,err,strlen(err));
                errors++;
                if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
                else if(local_count==thread_NUM-1){
                pthread_cond_signal( &cond_for );
                pthread_cond_signal( &cond );
                }
                else pthread_cond_signal( &cond );
                pthread_mutex_unlock( &mutex1 );
                return 0;
            }
        }

        int err_400 = 0;
        char *http;
        http = get_value(char_buff, 7+strlen(file_name), '\r');
        if(strcmp(http, "HTTP/1.1"))err_400=1;
        if(strlen(file_name)>27 || strlen(file_name)<=0){
            err_400=1;
        }else{
            for(int i = 0; i<strlen(file_name);i++){
                if ((file_name[i] >= 'a' && file_name[i] <= 'z') 
                || (file_name[i] >= 'A' && file_name[i] <= 'Z')){}
                else if(file_name[i] >= '0' && file_name[i] <= '9'){}
                else if(file_name[i] == '-'||file_name[i]== '_'){}
                else {
                    err_400=1;
                    break;
                }
            }
        }
        
        if(err_400==1){
            char* err = "HTTP/1.1 400 Bad Request\r\n ";
            send(client_sockd , err , strlen(err) , 0 );
            close(client_sockd);
            write(1,err,strlen(err));
            
            if(log_request==1){
                char log_err[100] = "FAIL: GET /";
                
        
                strcat(log_err, file_name);
                strcat(log_err, " HTTP/1.1 --- response 400\n========\n");
                pwrite(log_file, log_err, strlen(log_err),Synchroniz);
                Synchroniz = Synchroniz +strlen(log_err);
            }
            errors++;
            if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
            else if(local_count==thread_NUM-1){
            pthread_cond_signal( &cond_for );
            pthread_cond_signal( &cond );
            }
            else pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex1 );
            return 0;
        }

        int file=open(file_name,O_RDONLY,0);

        int buffer_size = 4096;
        uint8_t data[buffer_size+1];
        memset(data, 0 , buffer_size);
        ssize_t byte=0;

        if(file<0){
            char* err = "HTTP/1.1 404 Not Found\r\n ";
            send(client_sockd , err , strlen(err) , 0 );
            close(client_sockd);
            write(1,err,strlen(err));

            if(log_request==1){
                char log_err[100] = "FAIL: GET /";
                strcat(log_err, file_name);
                strcat(log_err, " HTTP/1.1 --- response 404\n========\n");
                pwrite(log_file, log_err, strlen(log_err),Synchroniz);
                Synchroniz = Synchroniz +strlen(log_err);
            }
            errors++;
            if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
            else if(local_count==thread_NUM-1){
            pthread_cond_signal( &cond_for );
            pthread_cond_signal( &cond );
            }
            else pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex1 );
            return 0;
        }
        off_t begginning = lseek(file, (size_t)0, SEEK_CUR);
        buffer = lseek(file, (size_t)0, SEEK_END);
        lseek(file, begginning, SEEK_SET); 

        char *res = "HTTP/1.1 200 OK\r\nContent-Length: ";
        char len[10];
        sprintf(len, "%d", buffer);

        if(log_request==1){
            char* requestFile = malloc(strlen(file_name)+1024);
            memset(requestFile, 0, strlen(requestFile));
            strcat(requestFile, "GET /");
            strcat(requestFile, file_name);
            strcat(requestFile, " length ");
            strcat(requestFile, len);
            pwrite(log_file, requestFile, strlen(requestFile),Synchroniz);
            Synchroniz = Synchroniz +strlen(requestFile);
            free(requestFile);

        }

        char onse[]="\r\n\r\n";
        char *response = malloc(strlen(res)+strlen(len)+strlen(onse)+1);
        memset(response, 0, strlen(response));
        strcat(response, res);
        strcat(response, len);
        strcat(response, onse);
        send(client_sockd , response , strlen(response) , 0 );
        write(1, response, strlen(response) );

        long a=0;
        int lengt = 69;
        char hex[lengt];
        char zeros[9] = "00000000";
        char byte_size[9];
        char temp[10];
        int hex_value;
        memset(hex, 0, lengt);

        while((byte = read(file, data, buffer_size))>0){
            send(client_sockd , data , byte, 0 );

            if(log_request==1){
                for(int i = 0; i<byte; i++){
                    
                    if(a%20==0){
                        strcat(hex, "\n");
                        pwrite(log_file, hex, strlen(hex), Synchroniz);
                        Synchroniz=Synchroniz+strlen(hex);

                        memset(hex, 0, lengt);
                        memset(temp, 0, strlen(temp));
                        sprintf(byte_size, "%ld", a);
                        int inc = 0;
                        for(int j = 0; j<8; j++){
                            if(j>=(8-strlen(byte_size))){
                                zeros[j]=byte_size[inc];
                                inc++;
                            } 
                        }
                        strcat(hex, zeros);
                    }
                    sprintf(temp, "%02x", data[i]);
                    
                    strcat(hex, " ");
                    strcat(hex, temp);
                    a++;
                }
                

            }
        memset(data, 0, BUFFER_SIZE);
        }
        if(log_request==1){
            strcat(hex,"\n========\n");
            pwrite(log_file, hex, strlen(hex), Synchroniz);
            Synchroniz=Synchroniz+strlen(hex);
            memset(hex, 0, lengt);
        }

        close(file);

    }else if(strcmp(req, "HEAD")==0){
        int size=64000;
        char data[size];
        int buffer;
        char *file_name;
        file_name = get_value(char_buff, 7, ' ');

        if(strcmp(file_name, "healthcheck")==0){
            char* err = "HTTP/1.1 403 Forbidden\r\n ";
            send(client_sockd , err , strlen(err) , 0 );
            close(client_sockd);
            write(1,err,strlen(err));
            if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
            else if(local_count==thread_NUM-1){
            pthread_cond_signal( &cond_for );
            pthread_cond_signal( &cond );
            }
            else pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex1 );
            return 0;

        }

        int err_400 = 0;
        char *http;
        http = get_value(char_buff, 8+strlen(file_name), '\r');
        if(strcmp(http, "HTTP/1.1"))err_400=1;

        if(strlen(file_name)>27 || strlen(file_name)<=0){
            err_400=1;
        }else{
            for(int i = 0; i<strlen(file_name);i++){
                if ((file_name[i] >= 'a' && file_name[i] <= 'z') 
                || (file_name[i] >= 'A' && file_name[i] <= 'Z')){}
                else if(file_name[i] >= '0' && file_name[i] <= '9'){}
                else if(file_name[i] == '-'||file_name[i]== '_'){}
                else {
                    err_400=1;
                    break;
                }
            }
        }
        
        if(err_400==1){
            char* err = "HTTP/1.1 400 Bad Request\r\n ";
            send(client_sockd , err , strlen(err) , 0 );
            close(client_sockd);
            write(1,err,strlen(err));
            
            if(log_request==1){
                char log_err[100] = "FAIL: HEAD /";
                strcat(log_err, file_name);
                strcat(log_err, " HTTP/1.1 --- response 400\n========\n");
                pwrite(log_file, log_err, strlen(log_err), Synchroniz);
                Synchroniz = Synchroniz +strlen(log_err);
            }
            errors++;
            if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
            else if(local_count==thread_NUM-1){
            pthread_cond_signal( &cond_for );
            pthread_cond_signal( &cond );
            }
            else pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex1 );
            return 0;
        }

        int file=open(file_name,O_RDONLY,0);
        int buffer_size = 4096;
        if(file<0){
            char* err = "HTTP/1.1 404 Not Found\r\n";
            send(client_sockd , err , strlen(err) , 0 );
            close(client_sockd);
            write(1,err,strlen(err));

            if(log_request==1){
                char log_err[100] = "FAIL: HEAD /";
                strcat(log_err, file_name);
                strcat(log_err, " HTTP/1.1 --- response 404\n========\n");
                pwrite(log_file, log_err, strlen(log_err),Synchroniz);
                Synchroniz = Synchroniz +strlen(log_err);
            }
            errors++;
            if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
            else if(local_count==thread_NUM-1){
            pthread_cond_signal( &cond_for );
            pthread_cond_signal( &cond );
            }
            else pthread_cond_signal( &cond );
            pthread_mutex_unlock( &mutex1 );
            return 0;
        }
        off_t begginning = lseek(file, (size_t)0, SEEK_CUR);
        buffer = lseek(file, (size_t)0, SEEK_END);
        lseek(file, begginning, SEEK_SET); 

        char *res = "HTTP/1.1 200 OK\r\nContent-Length: ";
        char len[10];
        memset(len, 0, strlen(len));
        sprintf(len, "%d", buffer);

        if(log_request==1){
            char* requestFile = malloc(strlen(file_name)+1024);
            memset(requestFile, 0, strlen(requestFile));
            strcat(requestFile, "HEAD /");
            strcat(requestFile, file_name);
            strcat(requestFile, " length ");
            strcat(requestFile, len);
            
            strcat(requestFile, "\n========\n");
            offset = strlen(requestFile);
            pwrite(log_file, requestFile, strlen(requestFile),Synchroniz);
            Synchroniz = Synchroniz +offset;
            free(requestFile);
        }

        char onse[]="\r\n\r\n";
        char *response = malloc(strlen(res)+strlen(len)+strlen(onse)+1);
        memset(response, 0 , strlen(response));
        strcat(response, res);
        strcat(response, len);
        strcat(response, onse);
        write(1,response,strlen(response));
        send(client_sockd , response , strlen(response) , 0 );
        free(response);
        close(file);

    }else{
        char* err = "HTTP/1.1 400 Bad Request\r\n ";
        send(client_sockd , err , strlen(err) , 0 );
        close(client_sockd);
        write(1,err,strlen(err));
        
        if(log_request==1){
            char* file_name = get_value(char_buff, strlen(req), ' ');
            char log_err[100] = "FAIL: ";
            strcat(log_err, req);
            strcat(log_err, " /");
            strcat(log_err, file_name);
            strcat(log_err, " HTTP/1.1 --- response 400\n========\n");
            pwrite(log_file, log_err, strlen(log_err), Synchroniz);
            Synchroniz = Synchroniz +strlen(log_err);

        }
        errors++;
    }
    entries++;
    close(client_sockd);
    if(local_count<thread_NUM-1)pthread_cond_signal( &cond_for );
    else if(local_count==thread_NUM-1){
        pthread_cond_signal( &cond_for );
    }
    else pthread_cond_signal( &cond );
    pthread_mutex_unlock(&mutex1);
    return 0;
}

int main(int argc, char** argv){

    Synchroniz=0;
    errors=0;
    entries=0;
    log_request = 0;
    int thread_count=4;
    int port_num = 0;
    thread_cnt = 0;
    for(int i = 0; i<argc; i++){
        if(((strcmp(argv[i],"-N"))==0)){
            i++;
            thread_count = atoi(argv[i]);
        }else if(((strcmp(argv[i],"-l"))==0)){
            i++;
            log_request=1;
            char *file_name=argv[i];
            log_file = open(file_name, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0777);
        
        }
        else port = argv[i];
    }
    if((port_num+atoi(port))<1)return 0;
    thread_NUM = thread_count;

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t addrlen = sizeof(server_addr);
    server_sockd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockd < 0) {
        perror("socket");
    }
    int enable = 1;
    int ret = setsockopt(server_sockd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    ret = bind(server_sockd, (struct sockaddr *) &server_addr, addrlen);
    ret = listen(server_sockd, 5); // 5 should be enough, if not use SOMAXCONN
    if (ret < 0) {
        return 0;
    }
    struct sockaddr client_addr;
    socklen_t client_addrlen;

    pthread_t thread_id[3000];

    int i;
    
    pthread_mutex_lock(&mutex3);
    for(i= 0; i<thread_count; i++){
        pthread_create( &thread_id[i], NULL, thread_server, (void*) &i);
        pthread_cond_wait(&thread_creat, &mutex3);
    }
    pthread_mutex_unlock(&mutex3);

    pthread_mutex_lock(&mutex);
    for(int j = 0; j <thread_count; j++){
        pthread_cond_signal( &server_wait );
        pthread_cond_wait( &cond_for, &mutex );
    }
    pthread_mutex_unlock(&mutex);

    pthread_mutex_unlock(&mutex2);
    while(1){
        pthread_create( &thread_id[i], NULL, thread_server, (void*) &i);
        pthread_cond_wait( &cond, &mutex2 );
        i++;
    }
    pthread_mutex_unlock(&mutex2);
  
    return 0;
}