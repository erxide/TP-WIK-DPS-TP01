// Compile with : gcc server.c -o server
// Run : ./server

#define PORT 8000
#define AF_INET 2
#define SOCK_STREAM 1
#define IPPROTO_TCP 6
#define INADDR_ANY 0
#define SOMAXCONN 128

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned long size_t;
typedef long ssize_t;

struct sockaddr_in {
    uint16_t sin_family;
    uint16_t sin_port;
    uint32_t sin_addr;
    uint8_t sin_zero[8];
};


extern int socket(int domain, int type, int protocol);
extern int bind(int sockfd, const void *addr, size_t addrlen);
extern int listen(int sockfd, int backlog);
extern int accept(int sockfd, void *addr, size_t *addrlen);
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, const void *buf, size_t count);
extern int close(int fd);

uint16_t htons(uint16_t val) {
    return (val << 8 ) | (val >> 8);
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

int sprintf_noimport(char *out, const char *key, const char *val) {
    char *p = out;
    *p++ = '"';
    while (*key) {
        if (*key == '"') *p++ = '\\';
        *p++ = *key++;
    }
    *p++ = '"'; *p++ = ':'; *p++ = ' '; *p++ = '"';
    while (*val) {
        if (*val == '"') *p++ = '\\';
        *p++ = *val++;
    }
    *p++ = '"'; *p++ = ','; *p++ = ' ';
    *p = '\0';
    return p - out;
}

void strcat_noimport(char *dest, const char *src) {
    while (*dest) dest++;
    while (*src) *dest++ = *src++;
    *dest = '\0';
}

void int_to_str(int num, char *str) {
    char tmp[10];
    int i = 0;
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    while (num > 0) {
        tmp[i++] = (num % 10) + '0';
        num /= 10;
    }
    // Reverse
    for (int j = 0; j < i; j++) {
        str[j] = tmp[i - j - 1];
    }
    str[i] = '\0';
}

void strcpy_noimport(char *dest, const char *src) {
    while (*src) *dest++ = *src++;
    *dest = '\0';
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr = INADDR_ANY;
    for (int i  = 0; i < 8; i++) addr.sin_zero[i] = 0;

    bind(sock, (void *)&addr, sizeof(addr));
    listen(sock, SOMAXCONN);

    while (1) {
        int client;
        size_t len = sizeof(addr);
        client = accept(sock, (void *)&addr, &len);

        char buffer[1024];
        read(client, buffer, sizeof(buffer));
        
        if (buffer[0] == 'G' && buffer[1] == 'E' && buffer[2] == 'T' &&
            buffer[3] == ' ' && buffer[4] == '/' &&
            buffer[5] == 'p' && buffer[6] == 'i' &&
            buffer[7] == 'n' && buffer[8] == 'g' &&
            buffer[9] == ' ') {

            char *line = buffer;
            char *json = "{\"";
            char json_buffer[1024] = "{";

            while (*line != '\n' && *line != '\0') line++;
            if (*line == '\n') line++;

            while (*line != '\0' && !(line[0] == '\r' && line[1] == '\n')) {
                char *key = line;
                char *val = 0;
                char *out = json_buffer + strlen(json_buffer);

                while (*line != ':' && *line != '\0') line++;
                if (*line == ':') {
                    *line = '\0';
                    val = line + 1;

                    while (*val == ' ') val++;

                    char *eol = val;
                    while (*eol != '\r' && *eol != '\n' && *eol != '\0') eol++;
                    *eol = '\0';

                    out += sprintf_noimport(out, key, val);

                    line = eol + 1;
                    if (*line == '\n') line++;
                } else {
                    break;
                }
            }

            size_t len = strlen(json_buffer);
            if (len > 2) json_buffer[len - 2] = '\0';
            strcat_noimport(json_buffer, "}");

            char response[1200];
            int content_length = strlen(json_buffer);
            char content_len_str[10];
            int_to_str(content_length, content_len_str);

            strcpy_noimport(response, "HTTP/1.1 200 OK\r\n");
            strcat_noimport(response, "Content-Type: application/json\r\n");
            strcat_noimport(response, "Content-Length: ");
            strcat_noimport(response, content_len_str);
            strcat_noimport(response, "\r\n\r\n");
            strcat_noimport(response, json_buffer);


            write(client, response, strlen(response));
            close(client);
                        
        } else {
            const char *quatrecentquatre_response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: 14\r\n"
                "\r\n"
                "{\"error\": 404}";

            write(client, quatrecentquatre_response, strlen(quatrecentquatre_response));
            close(client); 
        }
        
    }

    return 0;
}