#include <openssl/hmac.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <chrono>

//make sure to set up mingw and g++ and gcc for this project

using namespace std;
#define PORT 5001
#define SERVER_IP "127.0.0.1"

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

void runClient() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return;
    }

    std::cout << "Connected to server" << std::endl;

    // Prepare HMAC payload
    std::string key = "secret_key";
    std::vector<uint8_t> data(1024 * 1024, 'a'); // 1 MB of data
    HMACPayload payload;
    payload.data = data;
    payload.hmac = generateHMAC(data, key);

    auto start_time = std::chrono::high_resolution_clock::now();

    // Send payload
    send(sock, payload.data.data(), payload.data.size(), 0);
    send(sock, payload.hmac.data(), payload.hmac.size(), 0);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    double throughput = payload.data.size() / elapsed.count();

    std::cout << "Sent " << payload.data.size() << " bytes in " << elapsed.count() << " seconds." << std::endl;
    std::cout << "Throughput: " << throughput / 1024 << " KB/s" << std::endl;

    // Shutdown sending side
    shutdown(sock, SHUT_WR);

    // Wait for acknowledgment
    char ack[1024] = {0};
    recv(sock, ack, sizeof(ack), 0);
    std::cout << "Received acknowledgment: " << ack << std::endl;

    close(sock);
}

int main() {
    runClient();
    return 0;
}