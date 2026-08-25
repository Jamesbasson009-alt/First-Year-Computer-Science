#include "Address.h"

Address::Address(const std::string& address) : address(address) {

}

const std::string& Address::getAddress() {
    return address;
}
    