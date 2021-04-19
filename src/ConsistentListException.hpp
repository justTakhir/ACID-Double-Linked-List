#pragma once

#include <exception>
#include <string>

class MergeException : std::exception {
public:
  explicit MergeException(std::string mes) : message(std::move(mes)) {};
  const char* what() const throw() override {
    return message.c_str();
  }
private:
  std::string message;
};