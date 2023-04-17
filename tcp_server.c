#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Sử dụng: %s <cổng> <tệp tin chứa câu chào> <tệp tin lưu nội dung client gửi đến>\n", argv[0]);
        exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Lỗi: Không thể tạo socket");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("Lỗi: Không thể liên kết địa chỉ với socket");
        exit(1);
    }

    if (listen(sockfd, 5) == -1) {
        perror("Lỗi: Không thể lắng nghe kết nối");
        exit(1);
    }

    printf("Đang chờ kết nối...\n");

    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_sockfd == -1) {
        perror("Lỗi: Không thể chấp nhận kết nối");
        exit(1);
    }

    printf("Đã kết nối với client.\n");

    // Gửi xâu chào đến client
    FILE *fp_greeting = fopen(argv[2], "r");
    if (fp_greeting == NULL) {
        perror("Lỗi: Không thể mở tệp chào");
        exit(1);
    }

    char greeting_buffer[BUFFER_SIZE];
    int greeting_bytes_read;
    while ((greeting_bytes_read = fread(greeting_buffer, 1, BUFFER_SIZE, fp_greeting)) > 0) {
        if (send(client_sockfd, greeting_buffer, greeting_bytes_read, 0) == -1) {
            perror("Lỗi: Không thể gửi chào đến client");
            exit(1);
        }
    }
    fclose(fp_greeting);

    // Nhận và ghi toàn bộ nội dung client gửi đến vào tệp tin
    FILE *fp_input = fopen(argv[3], "w");
    if (fp_input == NULL) {
        perror("Lỗi: Không thể mở tệp đầu vào");
        exit(1);
    }

    char input_buffer[BUFFER_SIZE];
    int input_bytes_read;
    while ((input_bytes_read = recv(client_sockfd, input_buffer, BUFFER_SIZE, 0)) > 0) {
        if (fwrite(input_buffer, 1, strlen(input_buffer), fp_input) != input_bytes_read) {
            break;
        }
    }
    if (input_bytes_read == -1) {
        perror("Lỗi: Không thể nhận dữ liệu từ client");
        exit(1);
    }

    fclose(fp_input);

    printf("Đã nhận xong nội dung từ client.\n");

    close(client_sockfd);
    close(sockfd);

    return 0;
}
