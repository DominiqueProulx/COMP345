#pragma once
#include <iostream>
#include <string>
//dummy class


class Order {
public:
    Order(const std::string& type) : type(type) {}

    std::string getType() const { return type; }

private:
    std::string type;
};