#include <openssl/hmac.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <chrono>


using namespace std;

#define PORT 5001
#define BUFFER_SIZE 4096

struct HMACPayload {
    std::vector<uint8_t> data;
    std::vector<uint8_t> hmac;
};

std::vector<uint8_t> generateHMAC(const std::vector<uint8_t>& data, const std::string& key) {
    std::vector<uint8_t> hmac(EVP_MAX_MD_SIZE);
    unsigned int hmac_len;
    HMAC(EVP_sha256(), key.data(), key.size(), data.data(), data.size(), hmac.data(), &hmac_len);
    hmac.resize(hmac_len);
    return hmac;
}

bool verifyHMAC(const HMACPayload& payload, const std::string& key) {
    auto computed_hmac = generateHMAC(payload.data, key);
    return computed_hmac == payload.hmac;
}

void startServer() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept connection
    if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Client connected" << std::endl;

    HMACPayload payload;
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    ssize_t bytes_read;
    size_t total_received = 0;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Receive data
    while ((bytes_read = recv(client_fd, buffer.data(), buffer.size(), 0)) > 0) {
        payload.data.insert(payload.data.end(), buffer.begin(), buffer.begin() + bytes_read);
        total_received += bytes_read;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    double throughput = total_received / elapsed.count();

    std::cout << "Received " << total_received << " bytes in " << elapsed.count() << " seconds." << std::endl;
    std::cout << "Throughput: " << throughput / 1024 << " KB/s" << std::endl;

    // Verify HMAC
    std::string key = "secret_key";
    if (verifyHMAC(payload, key)) {
        std::cout << "HMAC verification succeeded" << std::endl;
    } else {
        std::cout << "HMAC verification failed" << std::endl;
    }

    // Send acknowledgment
    const char* ack = "ACK";
    send(client_fd, ack, strlen(ack), 0);

    close(client_fd);
    close(server_fd);
}

int main() {
    startServer();
    return 0;
}