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

class IteratorDereferencingException : std::exception {
public:
  explicit IteratorDereferencingException(std::string mes) : message(std::move(mes)) {};
  const char* what() const throw() override {
    return message.c_str();
  }

private:
  std::string message;
};

class EraseException : std::exception {
public:
  explicit EraseException(std::string mes) : message(std::move(mes)) {};
  const char* what() const throw() override {
    return message.c_str();
  }

private:
  std::string message;
};