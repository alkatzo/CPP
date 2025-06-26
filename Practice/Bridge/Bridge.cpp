// Bridge.cpp : Source file for your target.
//

#include "BridgeClasses.h"

int main()
{
    bridge_device::IOOperator plainOperator(bridge_device::makeFileDevice());

    const int n = 10;
    std::vector<std::byte> buffer(n);
    plainOperator.load(buffer, n);
	return 0;
}
