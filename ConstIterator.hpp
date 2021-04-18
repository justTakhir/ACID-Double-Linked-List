#pragma once

#include "Node.hpp"

template<class T>
class ConsistentList;

template<class T>
class Iterator;

template <class T>
class ConstIterator {
    friend class ConsistentList<T>;
    friend class Iterator<T>;
public:
    ConstIterator& operator=(const ConstIterator& other) {
        this->ptr->subRefCount();
        this->ptr = other.ptr;
        this->ptr->addRefCount();
        return *this;
    }

    ConstIterator& operator++() {//pre
      //do smth
        this->ptr = this->ptr->getNext();

        this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount() - 1);
        this->ptr->getPrev()->checkEndRefCount();

        return *this;
    }

    ConstIterator& operator++(int) {//post
        this->ptr = this->ptr->getNext();

        this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount - 1);
        this->ptr->getPrev()->checkEndRefCount();

        return *this;
        //do smth
    }
    // operator--
    ConstIterator& operator--() {//pre
      //do smth
        this->ptr = this->ptr->getPrev();

        this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
        this->ptr->getNext()->checkEndRefCount();

        return *this;
    }

    ConstIterator& operator--(int) {//post
        this->ptr = this->ptr->getPrev();

        this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
        this->ptr->getNext()->checkEndRefCount();

        return *this;
        //do smth
    }
    // operator*
    const T& operator* () const {
        return this->ptr->getValue();
    }

    // operator->
    const T* operator ->() const {
        return this->ptr->getValue();
    }

    bool operator!=(ConstIterator it) const {
        return (this->ptr != it.ptr);
    }

    bool operator==(ConstIterator it) const {
        return (this->ptr == it.ptr);
    }

    ~ConstIterator() {
        this->ptr->subRefCount();
        this->ptr->checkEndRefCount();
    }

    ConstIterator prev() const {
        return { this->ptr->getPrev() };
    }

    ConstIterator next() const {
        return { this->ptr->getNext() };
    }
private:
    ConstIterator(Node<T>* ptr_) : ptr(ptr_) {
        this->ptr->addRefCount();
    }
    Node<T>* ptr;
};
