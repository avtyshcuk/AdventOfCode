#include <openssl/evp.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <climits>

std::string md5_hex(const std::string& input) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);
    EVP_DigestUpdate(ctx, input.c_str(), input.length());
    EVP_DigestFinal_ex(ctx, digest, &digest_len);
    EVP_MD_CTX_free(ctx);
    
    std::ostringstream ss;
    for (unsigned int i = 0; i < digest_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return ss.str();
}

int main() {
    const std::string secret = "iwrupvqb";
    int part1 = 0, part2 = 0;
    
    for (int i = 1; i <= INT_MAX; i++) {
        std::string input = secret + std::to_string(i);
        std::string hash = md5_hex(input);
        
        // Part 1: Check if hash starts with 5 zeroes
        if (part1 == 0 && hash.substr(0, 5) == "00000") {
            part1 = i;
            std::cout << "Part 1: " << part1 << std::endl;
        }
        
        // Part 2: Check if hash starts with 6 zeroes
        if (hash.substr(0, 6) == "000000") {
            part2 = i;
            std::cout << "Part 2: " << part2 << std::endl;
            break;
        }
        
        // Progress indicator every million iterations
        if (i % 1000000 == 0) {
            std::cout << "Checked " << i << " numbers..." << std::endl;
        }
    }
    
    return 0;
}
