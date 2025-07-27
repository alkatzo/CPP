// Bridge.cpp : Source file for your target.
//

#include "BridgeClasses.h"
#include <iostream>

struct Node {
    int val;
    Node* next = nullptr;

    Node(int val) : val{val} {}
};


int main()
{
    bridge_device::IOOperator plainOperator(bridge_device::makeFileDevice());

    const int n = 10;
    std::vector<std::byte> buffer(n);
    plainOperator.load(buffer, n);


    auto l1 = [](int i) {
        return i++;
    };

    auto l2 = [](int i) {
        return i--;
    };

    auto l3 = [](int i) {
        return i*2;
    };


    int ans = l1(l2(l3(2)));

    std::cout << ans;

    Node* _head = new Node(1);
    _head->next = new Node(2);
    _head->next->next = new Node(3);
    _head->next->next->next = new Node(4);
    _head->next->next->next->next = new Node(5);

    auto cur = _head;
    while(cur) {
        std::cout << cur->val;
        cur = cur->next;
    }
    std::cout << std::endl;


    if(!_head)
        return 0;


    Node* nxt = _head->next;
    Node* head = _head;

    while(nxt) {
        _head->next = nxt->next;
        nxt->next = head;
        head = nxt;
        nxt = _head->next;
    }

    cur = head;
    while(cur) {
        std::cout << cur->val;
        cur = cur->next;
    }
    std::cout << std::endl;

	return 0;
}
