#pragma once

#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <type_traits>

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
  ConsistentList(size_type n, const_reference value) {
    for (size_type i = 0; i < n; i++) {
      this->push_front(value);
    }
  }

  template<class InputIterator, std::enable_if_t<std::_Is_iterator_v<InputIterator>, int> = 0>
  ConsistentList(InputIterator first, InputIterator last) {
    for (; first != last; first++) {
      this->push_back(*first);
    }
  }

  ConsistentList(const ConsistentList& x) {
    for (const_reference node_value : x) {
      this->push_back(node_value);
    }
  }
  // ConsistentList(ConsistentList&& x);

  ConsistentList(std::initializer_list<T> init_list)
    : ConsistentList(init_list.begin(), init_list.end()) {}

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
    for (auto it = init_list.begin(); it != init_list.end(); it++) {
      this->push_back(*it);
    }

    return *this;
  }

  bool operator==(const std::list<T>& std_list) {
    if (this->size() != std_list.size()) {
      return false;
    }
    
    auto this_it = this->begin();
    auto std_list_it = std_list.begin();
    
    while (this_it != this->end()) {
      if (*this_it != *std_list_it) {
        return false;
      }
      this_it++;
      std_list_it++;
    }

    return true;
  }

  template<class InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const_reference t);
  void assign(std::initializer_list<value_type> init_list);

  iterator begin() noexcept {
    iterator begin_it(this->sentinel.getNext());
    begin_it.setMutex(&this->list_mutex);
    return begin_it;
  }
  const_iterator begin() const noexcept {
    return this->cbegin();
  }
  iterator end() noexcept {
    iterator end_it(&this->sentinel);
    end_it.setMutex(&this->list_mutex);
    return end_it;
  }
  const_iterator end() const noexcept {
    return this->cend();
  }
  // reverse_iterator rbegin() noexcept;
  // const_reverse_iterator rbegin() const noexcept;
  // reverse_iterator rend() noexcept;
  // const_reverse_iterator rend() const noexcept;
  const_iterator cbegin() const noexcept {
    const_iterator cbegin_it(this->sentinel.getNext());
    cbegin_it.setMutex(&this->list_mutex);
    return cbegin_it;
  }
  const_iterator cend() const noexcept {
    const_iterator cend_it(&this->sentinel);
    cend_it.setMutex(&this->list_mutex);
    return cend_it;
  }
  // const_reverse_iterator crbegin() const noexcept;
  // const_reverse_iterator crend() const noexcept;

  bool empty() const noexcept {
    std::shared_lock<std::shared_mutex> lock(this->list_mutex);
    return this->size() == 0;
  }

  size_type size() const noexcept {
    std::shared_lock<std::shared_mutex> lock(this->list_mutex);
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
    this->insert(this->begin(), x);
  }
  // void push_front(T&& x);

  void pop_front() {
    this->erase(this->begin());
  }
  
  /// <summary>
  /// Insert x to the end of list.
  /// </summary>
  /// <param name="x"> - element for insert</param>
  void push_back(const T& x) {
    this->insert(this->end(), x);
  }
  // void push_back(T&& x);

  void pop_back() {
    this->erase(this->end().prev());
  }

  iterator insert(const_iterator position, const T& x) {

    std::unique_lock<std::shared_mutex> lock(this->list_mutex);

    iterator pos(position.ptr);

    node_type* inserted_node = new TrueNode<T>(x, pos.ptr->getPrev(), pos.ptr);

    pos.ptr->getPrev()->setNext(inserted_node);
    
    pos.ptr->setPrev(inserted_node);
    
    this->list_size++;

    iterator it_to_inserted_node(inserted_node);
    it_to_inserted_node.setMutex(position.it_mutex);
    return it_to_inserted_node;
  }

  iterator insert(const_iterator position, size_type n, const T& x) {
    iterator it_to_inserted_node(position.ptr);
    it_to_inserted_node.setMutex(position.it_mutex);
    for (size_type i = 0; i < n; i++) {
      it_to_inserted_node = this->insert(it_to_inserted_node, x);
    }
    return it_to_inserted_node;
  }

  template<class InputIterator, std::enable_if_t<std::_Is_iterator_v<InputIterator>, int> = 0>
  iterator insert(const_iterator position, InputIterator first, InputIterator last) {

    iterator it_to_inserted_node(position.ptr);
    it_to_inserted_node.setMutex(position.it_mutex);
    auto it = std::prev(last);

    for (; it != first; it--) {//--?
      it_to_inserted_node = this->insert(it_to_inserted_node, *it);
    }
    iterator ready_it_to_inserted_node = this->insert(it_to_inserted_node, *it);
    ready_it_to_inserted_node.setMutex(it_to_inserted_node.it_mutex);
    return ready_it_to_inserted_node;
  }

  iterator insert(const_iterator position, std::initializer_list<T> init_list) {
    iterator ready_it_to_inserted_node = this->insert(position, init_list.begin(), init_list.end());
    ready_it_to_inserted_node.setMutex(position.it_mutex);
    return ready_it_to_inserted_node;
  }

  iterator erase(const_iterator position) {
    std::unique_lock<std::shared_mutex> lock(this->list_mutex);
    
    if (position.ptr->isDeleted()) {
      throw DeleteDeletedException("Try to delete already deleted node!");
    }

    if (position == this->end()) {
      throw EraseException("Can't erase end()");
    }
    
    const_iterator c_new_position = position.next();
    this->delete_node(position.ptr);
    iterator new_position(c_new_position.ptr);
    new_position.setMutex(position.it_mutex);
    return new_position;
  }

  iterator erase(const_iterator position, const_iterator last) {

    //не нужно, потому что вызывается ирэйс для одной ноды, в котором она блочится std::unique_lock lock(position.ptr->getMutex());

    iterator new_position(position.ptr);
    new_position.setMutex(position.it_mutex);
    for (; position != last; position++) {
      new_position = this->erase(position);
    }
    return new_position;
  }

  void swap(ConsistentList& other_list) {//TODO: release
    //this->swap_nodes(&this->sentinel, &other_list.sentinel);
    //std::swap(this->list_size, other_list.list_size);
  }

  void clear() noexcept {
    while(!this->empty()) {
      this->pop_front();
    }


  }

  void remove(const T& value) {
    node_type* removed_node = this->search_node(value);
    while (!removed_node->checkSentinel()) {
      node_type* next_node = removed_node->getNext();
      this->delete_node(removed_node);
      removed_node = this->search_node(next_node, value);
    }
  }

  std::pair<const_iterator, const_iterator> merge(ConsistentList& x) {
    iterator x_begin = x.begin();
    iterator x_end = x.end();
    std::pair<const_iterator, const_iterator> x_iterators(x_begin, x_end);

    if (!std::is_sorted(this->begin(), this->end())) {
      throw MergeException("List's must be ordered!!!");
    }

    if (!std::is_sorted(x.begin(), x.end())) {
      throw MergeException("List's must be ordered!!!");
    }

    if (*this->begin() >= *x.end().prev()) {
      for (auto it = x.begin(); it != x.end(); it++) {
        this->push_front(*it);
      }
      x.clear();
      return x_iterators;
    }
    else if (*this->end().prev() <= *x.begin()) {
      for (auto it = x.begin(); it != x.end(); it++) {
        this->push_back(*it);
      }
      x.clear();
      return x_iterators;
    }
    else {
      auto it1 = this->begin();
      auto it2 = x.begin();
      while (it1 != this->end()) {
        if (*it2 < *it1) {
          this->insert(it1, *it2);
          it2 = x.erase(it2);
        }
        else {
          it1++;
        }
      }
      if (!x.empty()) {
        while (!x.empty()) {
          this->push_back(*it2);
          it2 = x.erase(it2);
        }
      }

      return x_iterators;
    }
    
    

  }
  // void merge(list&& x);

  void reverse() noexcept {
    ConsistentList<T> tmp;
    for (auto it = this->begin(); it != this->end(); it++) {
      tmp.push_front(*it);
    }
    *this = tmp;
  }

private:
  SentinelNode<T> sentinel;
  size_type list_size = size_type();
  mutable std::shared_mutex list_mutex;

  static void swap_nodes(node_type* node1, node_type* node2) { // todo

    auto tmp = node1->getNext() != node2 ? node1->getNext() : node1;
    node1->setNext(node2->getNext());
    node2->setNext(tmp);

    tmp = node1->getPrev() != node2 ? node1->getPrev() : node1;
    node1->setPrev(node2->getPrev());
    node2->setPrev(tmp);

    node1->getNext()->setPrev(node1);
    if (node1->getPrev() != node2) {
      node1->getPrev()->setNext(node1);
  }
    if (node1->getPrev() != node2){
      node2->getNext()->setPrev(node2);
    }
    node2->getPrev()->setNext(node2);
  }

  node_type* search_node(const_reference value) {
    return this->search_node(this->sentinel.getNext(), value);
  }

  node_type* search_node(node_type* start_node, const_reference value) {
    const_iterator it(start_node);
    it.setMutex(&this->list_mutex);
    while (it != this->end() && *it != value) {
      it++;
    }

    return it.ptr;
  }

  void delete_node(node_type* deleted_node) {
    deleted_node->setDeleted();
    deleted_node->getPrev()->setNext(deleted_node->getNext());
    deleted_node->getNext()->setPrev(deleted_node->getPrev());

    if (!deleted_node->getPrev()->checkSentinel()) {
      deleted_node->getPrev()->addRefCount();
    }

    if (!deleted_node->getNext()->checkSentinel()) {
      deleted_node->getNext()->addRefCount();
    }

    deleted_node->setRefCount(deleted_node->getRefCount() - 2);

    this->list_size--;
  }

};
