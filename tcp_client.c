#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]) {
    // Kiểm tra tham số đầu vào
    if (argc != 3) {
        printf("Sử dụng: %s <IP server> <port>\n", argv[0]);
        return -1;
    }

    // Tạo socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Không thể tạo socket");
        return -1;
    }

    // Thiết lập địa chỉ máy chủ
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    // Kết nối đến máy chủ
    if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Không thể kết nối đến máy chủ");
        return -1;
    }

    // Nhập dữ liệu từ bàn phím và gửi đến máy chủ
    char buffer[1024];
    while (1) {
        printf("Nhập dữ liệu: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (strlen(buffer) == 1) break;
        fflush(stdin);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Đóng socket
    close(client_socket);

    return 0;
}
