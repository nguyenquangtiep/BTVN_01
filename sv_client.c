#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

typedef struct {
    char mssv[20];
    char ho_ten[50];
    char ngay_sinh[20];
    char diem_tb[10];
} SinhVien;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_address> <server_port>\n", argv[0]);
        exit(1);
    }

    // Tạo socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Thiết lập địa chỉ server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) != 1) {
        printf("Invalid address\n");
        exit(1);
    }

    // Kết nối đến server
    if (connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to connect to server");
        exit(1);
    }

    // Nhập thông tin sinh viên
    SinhVien sv;
    printf("Nhap MSSV: ");
    fgets(sv.mssv, 20, stdin);
    printf("Nhap ho ten: ");
    fgets(sv.ho_ten, 50, stdin);
    printf("Nhap ngay sinh: ");
    fgets(sv.ngay_sinh, 20, stdin);
    printf("Nhap diem trung binh: ");
    fgets(sv.diem_tb, 10, stdin);

    // Gửi thông tin sinh viên đến server
    if (send(sockfd, &sv, sizeof(sv), 0) == -1) {
        perror("Failed to send data to server");
        exit(1);
    }

    // Đóng kết nối
    close(sockfd);

    return 0;
}
