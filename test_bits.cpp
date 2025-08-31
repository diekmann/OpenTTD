/* Test bit manipulation logic for direction storage */
#include <iostream>
#include <cstdint>

// Simulate the bit manipulation functions
template <typename T>
uint GB(const T x, const uint8_t s, const uint8_t n)
{
    return (x >> s) & (((T)1U << n) - 1);
}

template <typename T, typename U>
T SB(T &x, const uint8_t s, const uint8_t n, const U d)
{
    x &= (T)(~((((T)1U << n) - 1) << s));
    x |= (T)(d << s);
    return x;
}

void test_direction_storage(int direction) {
    uint8_t m5 = 0;  // Start with clear field
    
    std::cout << "Testing direction " << direction << ":" << std::endl;
    
    // Storage logic (from MakeRailTunnel)
    if ((direction & 1) == 0) {  // Orthogonal
        SB(m5, 0, 1, direction & 1);           // Store LSB in bit 0
        SB(m5, 6, 2, (direction >> 1) & 3);   // Store bits 1-2 in bits 6-7
        SB(m5, 5, 1, 1);                      // Set extended format flag
        SB(m5, 2, 2, 0);                      // TRANSPORT_RAIL = 0
        
        std::cout << "  Stored m5 = 0x" << std::hex << (int)m5 << std::endl;
        std::cout << "  Stored bit0=" << (direction & 1) << ", bits6-7=" << ((direction >> 1) & 3) << std::endl;
    }
    
    // Retrieval logic (from GetTunnelBridgeFullDirection)
    if (GB(m5, 5, 1)) {  // Extended format
        uint8_t dir = GB(m5, 0, 1) |         // Get bit 0
                      (GB(m5, 6, 2) << 1);   // Get bits 6-7 and shift to positions 1-2
        std::cout << "  Retrieved direction = " << (int)dir << std::endl;
        std::cout << "  Bit 0 = " << GB(m5, 0, 1) << ", bits 6-7 = " << GB(m5, 6, 2) << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    test_direction_storage(0);  // DIR_N
    test_direction_storage(2);  // DIR_E  
    test_direction_storage(4);  // DIR_S
    test_direction_storage(6);  // DIR_W
    return 0;
}
