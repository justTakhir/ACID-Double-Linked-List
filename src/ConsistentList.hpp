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
    iterator pos(position.ptr);

    node_type* inserted_node = new TrueNode<T>(x, pos.ptr->getPrev(), pos.ptr);

    pos.ptr->getPrev()->setNext(inserted_node);
    
    pos.ptr->setPrev(inserted_node);
    
    this->list_size++;

    return { inserted_node };
  }

  iterator insert(const_iterator position, size_type n, const T& x) {
    iterator it_to_inserted_node(position.ptr);
    for (size_type i = 0; i < n; i++) {
      it_to_inserted_node = this->insert(it_to_inserted_node, x);
    }
    return it_to_inserted_node;
  }

  template<class InputIterator, std::enable_if_t<std::_Is_iterator_v<InputIterator>, int> = 0>
  iterator insert(const_iterator position, InputIterator first, InputIterator last) {
    /*iterator it_to_inserted_node(position.ptr);
    for(; first != last; first++){//--?
      it_to_inserted_node = this->insert(it_to_inserted_node, *first);
    }
    return it_to_inserted_node;*/
    iterator it_to_inserted_node(position.ptr);
    auto it = std::prev(last);
    for (; it != first; it--) {//--?
      it_to_inserted_node = this->insert(it_to_inserted_node, *it);
    }
    return this->insert(it_to_inserted_node, *it);
  }

  iterator insert(const_iterator position, std::initializer_list<T> init_list) {
    return this->insert(position, init_list.begin(), init_list.end());
  }

  iterator erase(const_iterator position) {
    if (position == this->end()) {
      throw EraseException("Can't erase end()");
    }
    const_iterator new_position = position.next();
    this->delete_node(position.ptr);
    return { new_position.ptr };
  }
  iterator erase(const_iterator position, const_iterator last) {
    iterator new_position(position.ptr);
    for (; position != last; position++) {
      new_position = this->erase(position);
    }
    return new_position;
  }

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
    while (!removed_node->checkSentinel()) {
      node_type* next_node = removed_node->getNext();
      this->delete_node(removed_node);
      removed_node = this->search_node(next_node, value);
    }
  }

  void merge(ConsistentList& x) {
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
      return;
    }

    if (*this->end().prev() <= *x.begin()) {
      for (auto it = x.begin(); it != x.end(); it++) {
        this->push_back(*it);
      }
      x.clear();
      return;
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

  static void swap_nodes(node_type* node1, node_type* node2) { // todo

    auto tmp = node1->getNext() != node2 ? node1->getNext() : node1;
    node1->setNext(node2->getNext());
    node2->setNext(tmp);
    //if swap closer node2->setNext(node1);
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

    /*(*node1)->setNext((*node1)->getNext());
    (*node1)->setPrev((*node1)->getPrev());

    (*node2)->setNext((*buf_ptr)->getNext());
    (*node2)->setPrev((*buf_ptr)->getPrev());*/
  }

  node_type* search_node(const_reference value) {
    return this->search_node(this->sentinel.getNext(), value);
  }

  node_type* search_node(node_type* start_node, const_reference value) {
    const_iterator it(start_node);
    while (it != this->end() && *it != value) {
      it++;
    }

    return it.ptr;
  }

  void delete_node(node_type* deleted_node) {
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
