#pragma once
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <iterator>
#include "ConsistentListException.h"
#include "Iterator.h"
//#include "ConstIterator.h"
#include "SentinelingNode.h"
#include "TrueNode.h"

template<class T>
class ÑonsistentList {
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

  //?++//
  ÑonsistentList() = default;

  explicit ÑonsistentList(size_type n) {
    for (size_type i = 0; i < n; i++) {
      push_front(NULL);
    }
  };
  ÑonsistentList(size_type n, const T& value) {
    for (size_type i = 0; i < n; i++) {
      push_front(value);
    }
  };
  //?//
  template<class InputIterator>
  ÑonsistentList(InputIterator first, InputIterator last) {};
  //+//
  ÑonsistentList(const ÑonsistentList& x) {
    for (size_type i = 0; i < x.size(); i++) {
      this->push_back(*(x.begin() + i));
    }
  };
  // ÑonsistentList(ÑonsistentList&& x);
  //+//
  ÑonsistentList(std::initializer_list<T> init_list) {
    for (size_type i = 0; i < init_list.size(); i++) {
      push_front(init_list[i]);
    }
  };
  //+//
  ~ÑonsistentList() {
    for (size_type i = 0; i < this->size(); i++) {
      this->pop_back();
    }
    delete this->list_size;
    delete this->sentinel;
  };
  //+//
  ÑonsistentList& operator=(const ÑonsistentList& x) {
    for (size_type i = 0; i < this->size(); i++) {
      this->pop_back();
    }
    for (size_type i = 0; i < x.size(); i++) {
      this->push_back(*(x.begin() + i));
    }
    return *this;
  };
  // ÑonsistentList& operator=(ÑonsistentList&& x)
  //+//
  ÑonsistentList& operator=(std::initializer_list<T> init_list) {
    for (size_type i = 0; i < this->size(); i++) {
      this->pop_back();
    }
    for (size_type i = 0; i < init_list.size(); i++) {
      this->push_back(init_list[i]);
    }
    return *this;
  };

  template<class InputIterator>
  void assign(InputIterator first, InputIterator last) {}
  void assign(size_type n, const_reference t) {
    //delete nodes;
    //create new nodes and fill it

  }
  void assign(std::initializer_list<value_type> init_list) {}

  //++++++//
  iterator begin() noexcept {
    iterator begin_iterator(this->sentinel.getNext());
    return begin_iterator;
  }
  const_iterator begin() const noexcept {
    return this->cbegin();
  }
  iterator end() noexcept {
    iterator end_iterator(&this->sentinel);
    return end_iterator;
  };
  const_iterator end() const noexcept {
    return this->cend();
  }
  // reverse_iterator rbegin() noexcept;
  // const_reverse_iterator rbegin() const noexcept;
  // reverse_iterator rend() noexcept;
  // const_reverse_iterator rend() const noexcept;
  const_iterator cbegin() const noexcept {
    const_iterator cbegin_iterator(this->sentinel.getNext());
    return cbegin_iterator;
  };
  const_iterator cend() const noexcept {
    const_iterator cend_iterator(&this->sentinel);
    return cend_iterator;
  };
  // const_reverse_iterator crbegin() const noexcept;
  // const_reverse_iterator crend() const noexcept;

  //+//
  bool empty() const noexcept {
    return(size == 0);
  };
  //+//
  size_type size() const noexcept {
    return this->list_size;
  };
  //size_type max_size() const noexcept;

  //++//
  void resize(size_type sz) {
    resize(sz, NULL);
  };
  void resize(size_type sz, const T& c) {
    if (sz < list_size) {
      size_type diff = list_size - sz;
      for (size_type i = 0; i < diff; i++) {
        this->pop_back();
      }
    }

    if (sz > list_size) {
      size_type diff = sz - list_size;
      node_type* inserted_node;
      for (size_type i = 0; i < diff; i++) {
        this->push_back(c);
      }
    }
    this->list_size = sz;

    //if sz == list size, then do nothing
  };

  //++++//
  reference front() {
    return *this->begin();
    //return this->sentinel.getNext()->getValue();
  };
  const_reference front() const {
    return front();
  };
  reference back() {
    return *std::prev(this->end());
    //return sentinel.getPrev()->getValue();
  };
  const_reference back() const {
    return back();
  };

  //+// ýòî ÷èñòî ïðî äåôîëòíûé ôóíêöèîíàë, åù¸ íóæíî ïðèêðóòèòü ðàáîòó ññûëêàìè
  void push_front(const T& x) {
    this->insert(std::next(this->begin()), x);
  };
  // void push_front(T&& x);
  //+/
  void pop_front() {
    this->erase(this->begin());
  };

  //+//êàê ñ push_front
  void push_back(const T& x) {
    this->insert(this->end(), x);
  };
  // void push_back(T&& x);
  //+//
  void pop_back() {
    this->erase(this->end() - 1);
  };

  //++++//
  iterator insert(const_iterator position, const T& x) {
    //create node in memory
    iterator pos(position->ptr);
    node_type* inserted_node = new node_type;
    inserted_node->setValue(x);
    //insert //pos point to next after inserted element
    inserted_node->setNext(pos.ptr);
    inserted_node->setPrev(pos.ptr->getPrev());
    pos.ptr->setPrev(inserted_node);
    if (this->empty()) {
      pos.ptr->setNext(inserted_node);
      inserted_node->setRefCount(2);
      pos.ptr->setRefCount(2);
      return pos;
    }
    //work with ref count

    inserted_node->setRefCount(3);//3 because have as min 1 pointing iterator
    return pos;
  };
  // iterator insert(const_iterator position, T&& x);
  iterator insert(const_iterator position, size_type n, const T& x) {
    iterator it_to_inserted_node;
    for (size_type i = 0; i < n; i++) {
      it_to_inserted_node = this->insert(position, x);
    }
    return it_to_inserted_node;
  };

  template<class InputIterator>
  iterator insert(const_iterator position, InputIterator first, InputIterator last) {
    iterator it_to_inserted_node;
    for (auto it = first; it != last; it++) {
      it_to_inserted_node = this->insert(position, *it);
    }
    return it_to_inserted_node;
  };
  
  iterator insert(const_iterator position, std::initializer_list<T> init_list) {
    return this->insert(position, init_list.begin(), init_list.end());
  };

  iterator erase(const_iterator position) {//ìû çàðàíåå çíàåì ïîçèöèþ
    const_iterator new_position = position + 1;
    this->delete_node(position.ptr);//ïî çíà÷åíèþ íåëüçÿ, èáî íîäà ñ òàêèì æå çíà÷åíèåì ìîæåò âñòðåòèòüñÿ ðàíåå
    return new_position;
  };
  iterator erase(const_iterator position, const_iterator last) {};
  //+//
  void swap(ÑonsistentList& other_list) {
    //ñêîðåå âñåãî ìîæíî ïðîñòî ñâàïíóòü óêàçàòåëè íà ëèñòû èëè ÷òî-òî òàêîå, èáî ïî ôàêòó ïðîñòî íàçâàíèÿ ìåíÿþòñÿ))
    this->swap_nodes(&this->sentinel, &other_list.sentinel);
    std::swap(this->list_size, other_list.list_size);
  };

  //+//
  void clear() noexcept {
    while(!this->empty()){
      this->pop_front();
    }
  };
  //+//
  void remove(const T& value) {
  //delete all = value
    node_type* removed_node = this->search_node(value);
    while (removed_node != nullptr) {
      node_type* next_node = removed_node->getNext();
      this->delete_node(removed_node);
      removed_node = this->search_node(next_node, value);
    }
  };
  //+//
  void merge(ÑonsistentList& x) {
  //äåëàåò èç äâóõ ñîðòàíóòûõ ïî âîçðàñòàíèþ ñïèñêîâ îäèí áîëüøîé ñîðòàíóòûé ïî âîçðàñòàíèþ ñïèñîê, âõîäíîé ÷èñòèòñÿ
    //if (!this->is_sorted_list()) {
    if (!std::is_sorted(this->begin(), this->end())) {
      throw MergeException("List's must be ordered!!!");
    }
    size_type diff_size = std::max(this->size(), x.size()) - std::min(this->size(), x.size());

    if (*this->begin() > *(x.end() - 1)) {//åñëè 
      for (size_type i = 0; i < diff_size; i++) {
        this->push_front(*std::prev(x.end(), 1 + i));
        x.pop_back();
      }
    }
    
    if(*(this->end() - 1) < *x.begin()) {
      for (size_type i = 0; i < diff_size; i++) {
        this->push_back(*(x.begin()));
        x.pop_front();
      }
    }
    //x.clear(); íå íóæíî, èáî âî âðåìÿ ïóøåé è ïîïîâ âñ¸ äîëæíî ñàìî ïîä÷èñòèòüñÿ è äîáàâèòüñÿ ãäå íóæíî
  };
  // void merge(list&& x);

  //+//
  void reverse() noexcept {
    //ìîæåò ìîæíî ïðîñòî ñâàïíóòü èòåðàòîðû, íî ìû òàêèìè ïóòÿìè åù¸ íå óìååì õîäèòü, ïîýòîìó
    iterator it1 = this->begin(), it2 = this->end() - 1;
    for (size_type i = 0; i < this->size() / 2; i++) {
      this->swap_nodes(it1.ptr, it2.ptr);
      it1 = it1 + 1;
      it2 = it2 - 1;
    }
  };

private:
  SentinelingNode<T> sentinel;
  size_type list_size;

  bool is_sorted_list(ÑonsistentList& x) {
    for (auto i = x.begin() + 1; i != x.end(); i++) {
      if (*(i - 1) > *i) {
        false;
      }
    }
    return true;
  }

  void swap_nodes(node_type* node1, node_type* node2) {
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
  //+//
  void delete_node(node_type* deleted_node) {
    deleted_node->getPrev()->setNext(deleted_node->getNext());
    deleted_node->getNext()->setPrev(deleted_node->getPrev());
    
    deleted_node->setRefCount(deleted_node->getRefCount() - 2);
    if (!deleted_node->getPrev()->checkSentinel() && !deleted_node->getNext()->checkSentinel()) {//check for not last element in list
      deleted_node->getNext()->addRefCount();
      deleted_node->getPrev()->addRefCount();
      //ñäåëàòü ñîñåäíèì ïî +1;
    }//if last, will do nothing

    deleted_node->checkEndRefCount();

    this->list_size--;
  }
protected:

};
