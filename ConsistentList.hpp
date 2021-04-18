#pragma once

#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <iterator>

#include "ConsistentListException.hpp"
#include "Iterator.hpp"
#include "SentinelNode.hpp"
#include "TrueNode.hpp"

template<class T>
class ConsistentList {
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using node_type = Node<T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using iterator = Iterator<value_type>;
  using const_iterator = ConstIterator<value_type>;
  // using reverse_iterator = std::reverse_iterator<iterator>;
  // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  ConsistentList() = default;

  explicit ConsistentList(size_type n) {
    for (size_type i = 0; i < n; i++) {
      this->push_front(value_type());
    }
  }
  ConsistentList(size_type n, const T& value) {
    for (size_type i = 0; i < n; i++) {
      this->push_front(value);
    }
  }

  template<class InputIterator>
  ConsistentList(InputIterator first, InputIterator last) {
    for (auto it = first; it != last; it++) {
      this->push_back(*it);
    }
  }

  ConsistentList(const ConsistentList& x) {
    for (const_reference node_value : x) {
      this->push_back(node_value);
    }
  }
  // ConsistentList(ConsistentList&& x);

  ConsistentList(std::initializer_list<T> init_list) {
    for (auto it = init_list.begin(); it != init_list.end(); it++) {
      this->push_back(*it);
    }
  }

  ~ConsistentList() {
    this->clear();
  }

  ConsistentList& operator=(const ConsistentList& x) {
    this->clear();
    for (auto it = x.begin(); it != x.end(); it++) {
      this->push_back(*it);
    }

    return *this;
  }
  // ConsistentList& operator=(ConsistentList&& x)

  ConsistentList& operator=(std::initializer_list<T> init_list) {
    this->clear();
    for (size_type i = 0; i < init_list.size(); i++) {
      this->push_back(init_list[i]);
    }

    return *this;
  }

  template<class InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const_reference t);
  void assign(std::initializer_list<value_type> init_list);

  iterator begin() noexcept {
    return { this->sentinel.getNext() };
  }
  const_iterator begin() const noexcept {
    return this->cbegin();
  }
  iterator end() noexcept {
    return { &this->sentinel };
  }
  const_iterator end() const noexcept {
    return this->cend();
  }
  // reverse_iterator rbegin() noexcept;
  // const_reverse_iterator rbegin() const noexcept;
  // reverse_iterator rend() noexcept;
  // const_reverse_iterator rend() const noexcept;
  const_iterator cbegin() const noexcept {
    return { this->sentinel.getNext() };
  }
  const_iterator cend() const noexcept {
    return { &this->sentinel };
  }
  // const_reverse_iterator crbegin() const noexcept;
  // const_reverse_iterator crend() const noexcept;

  bool empty() const noexcept {
    return this->size() == 0;
  }

  size_type size() const noexcept {
    return this->list_size;
  }
  //size_type max_size() const noexcept;

  void resize(size_type sz) {
    resize(sz, T());
  }

  void resize(size_type sz, const T& c) {
    if (sz < list_size) {
      size_type diff = list_size - sz;
      for (size_type i = 0; i < diff; i++) {
        this->pop_back();
      }
    }

    if (sz > list_size) {
      size_type diff = sz - list_size;
      for (size_type i = 0; i < diff; i++) {
        this->push_back(c);
      }
    }

    this->list_size = sz;
  }

  //++++//
  reference front() {
    return *this->begin();
  }
  const_reference front() const {
    return front();
  }
  reference back() {
    return this->end().prev();
  }
  const_reference back() const {
    return back();
  }

  void push_front(const T& x) {
    this->insert(this->begin().next(), x);
  }
  // void push_front(T&& x);

  void pop_front() {
    this->erase(this->begin());
  }

  void push_back(const T& x) {
    this->insert(this->end(), x);
  }
  // void push_back(T&& x);

  void pop_back() {
    this->erase(this->end().prev());
  }

  iterator insert(const_iterator position, const T& x) {
    iterator pos(position.ptr);

    node_type* inserted_node = new TrueNode<T>();
    inserted_node->setValue(x);
    inserted_node->setNext(pos.ptr);
    inserted_node->setPrev(pos.ptr->getPrev());

    pos.ptr->getPrev()->setNext(inserted_node);
    pos.ptr->setPrev(inserted_node);

    this->list_size++;

    return { inserted_node };
  }

  iterator insert(const_iterator position, size_type n, const T& x) {
    iterator it_to_inserted_node;
    for (size_type i = 0; i < n; i++) {
      it_to_inserted_node = this->insert(position, x);
    }
    return it_to_inserted_node;
  }

  template<class InputIterator>
  iterator insert(const_iterator position, InputIterator first, InputIterator last) {
    iterator it_to_inserted_node;
    for (auto it = first; it != last; it++) {
      it_to_inserted_node = this->insert(position, *it);
    }
    return it_to_inserted_node;
  }

  iterator insert(const_iterator position, std::initializer_list<T> init_list) {
    return this->insert(position, init_list.begin(), init_list.end());
  }

  iterator erase(const_iterator position) {
    const_iterator new_position = position.next();
    this->delete_node(position.ptr);
    return { new_position.ptr };
  }
  iterator erase(const_iterator position, const_iterator last);

  void swap(ConsistentList& other_list) {
    this->swap_nodes(&this->sentinel, &other_list.sentinel);
    std::swap(this->list_size, other_list.list_size);
  }

  void clear() noexcept {
    while(!this->empty()){
      this->pop_front();
    }
  }

  void remove(const T& value) {
    node_type* removed_node = this->search_node(value);
    while (removed_node != nullptr) {
      node_type* next_node = removed_node->getNext();
      this->delete_node(removed_node);
      removed_node = this->search_node(next_node, value);
    }
  }

  void merge(ConsistentList& x) {
    if (!std::is_sorted(this->begin(), this->end())) {
      throw MergeException("List's must be ordered!!!");
    }
    size_type diff_size = std::max(this->size(), x.size()) - std::min(this->size(), x.size());

    if (*this->begin() > *x.end().prev()) {
      for (size_type i = 0; i < diff_size; i++) {
        this->push_front(*std::prev(x.end(), 1 + i));
        x.pop_back();
      }
    }

    if(*this->end().prev() < *x.begin()) {
      for (size_type i = 0; i < diff_size; i++) {
        this->push_back(*x.begin());
        x.pop_front();
      }
    }
  }
  // void merge(list&& x);

  void reverse() noexcept {
    iterator it1 = this->begin(), it2 = this->end().prev();
    for (size_type i = 0; i < this->size() / 2; i++) {
      ConsistentList::swap_nodes(it1.ptr, it2.ptr);
      it1 = it1 + 1;
      it2 = it2 - 1;
    }
  }

private:
  SentinelNode<T> sentinel;
  size_type list_size;

  static void swap_nodes(node_type* node1, node_type* node2) { // todo
    node_type* buf_ptr = node1;
    node1 = node2;
    node2 = buf_ptr;

    buf_ptr = node1->getNext();
    node1->setNext(node1->getPrev());
    node1->setPrev(buf_ptr);

    buf_ptr = node2->getNext();
    node2->setNext(node2->getPrev());
    node2->setPrev(buf_ptr);
  }

  node_type* search_node(const_reference value) {
    return this->search_node(this->sentinel.getNext(), value);
  }

  node_type* search_node(node_type* start_node, const_reference value) {
    TrueNode<T>* searched_node = start_node;
    while (searched_node != this->sentinel && searched_node->getValue() != value) {
      searched_node = searched_node->getNext();
    }

    if (searched_node == this->sentinel) {
      return nullptr;
    }

    return searched_node;
  }

  void delete_node(node_type* deleted_node) {
    deleted_node->getPrev()->setNext(deleted_node->getNext());
    deleted_node->getNext()->setPrev(deleted_node->getPrev());

    deleted_node->setRefCount(deleted_node->getRefCount() - 2);
    if (!deleted_node->getPrev()->checkSentinel() && !deleted_node->getNext()->checkSentinel()) {
      deleted_node->getNext()->addRefCount();
      deleted_node->getPrev()->addRefCount();
    }

    deleted_node->checkEndRefCount();

    this->list_size--;
  }

};
