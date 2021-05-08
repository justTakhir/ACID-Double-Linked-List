#pragma once

#include <exception>
#include <string>

class ConsistentListException : std::exception {
public:
  explicit ConsistentListException(std::string mes) : message(std::move(mes)) {};
  const char* what() const throw() override {
    return message.c_str();
  }

private:
  std::string message;
};

class MergeException : public ConsistentListException {
public:
  explicit MergeException(std::string mes) : ConsistentListException(mes) {};
};


class IteratorDereferencingException : public ConsistentListException {
public:
  explicit IteratorDereferencingException(std::string mes) : ConsistentListException(mes) {};
};

class EraseException : public ConsistentListException {
public:
  explicit EraseException(std::string mes) : ConsistentListException(mes) {};
};