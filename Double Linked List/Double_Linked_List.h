#pragma once
#include <typeinfo>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <iterator>


class MergeException : std::exception {
public:
  explicit MergeException(std::string mes) : message(std::move(mes)) {};
  const char* what() const throw() override {
    return message.c_str();
  }
private:
  std::string message;
};

//template<typename T>
//void swap_something(T *obj1, T *obj2) {
//  T* buf = obj1;
//  obj1 = obj2;
//  obj2 = buf;
//}

template<typename T>
class TrueNode;
template<typename T>
class SentinelingNode;

template<typename T>
class Node {
private:
  T value_;
  Node<T>* prev_;// this is first if = null
  Node<T>* next_;// this is last if = null
  size_t ref_count_;
public:
  using value_type = T;
  using node = Node<value_type>;
  using node_pointer = node*;
  using node_reference = node&;
  using true_node = TrueNode<value_type>;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  Node() {};

  Node(value_type value, size_type ref_count = 2, node_pointer prev = nullptr, node_pointer next = nullptr)
    : value_(value), ref_count_(ref_count), prev_(prev), next_(next) {};

  node operator =(const node_reference other_node) {
    this->ref_count_ = other_node->ref_count_;
    this->prev_ = other_node->prev_;
    this->next_ = other_node->next_;
    this->value_ = other_node.value_;
  }

  value_type getValue() const {
    return this->value_;
  }

  void setValue(const size_type& new_value) {
    this->value_ = new_value;
  }

  size_type getRefCount() const {
    return this->ref_count_;
  }

  void setRefCount(const size_type& new_value) {
    this->value_ = new_value;
    //this->checkEndRefCount();
  }

  void checkEndRefCount() {
    if (this->ref_count_ == 0) {
      delete* this;
    }
  }

  node* getPrev() const {
    return this->prev_;
  }

  void setPrev(node new_prev) {
    this->prev_ = new_prev;
  }

  node* getNext() const {
    return this->next_;
  }

  void setNext(node new_next) {
    this->next_ = new_next;
  }

  virtual bool checkSentinel() = 0;
};

template<typename T>
class SentinelingNode : public virtual Node<T> {
private:
public:
  SentinelingNode() : Node<T>() {};
  SentinelingNode(T value, size_t ref_count = 2, Node<T>* prev = nullptr, Node<T>* next = nullptr) :
    Node<T>(value, ref_count = 2, prev = nullptr, next = nullptr) {};
  bool checkSentinel() override {
    return true;
  }
};

template<typename T>
class TrueNode : public virtual Node<T> {
private:
public:
  TrueNode() : Node<T>() {};
  TrueNode(T value, size_t ref_count = 2, Node<T>* prev = nullptr, Node<T>* next = nullptr) :
    Node<T>(value, ref_count = 2, prev = nullptr, next = nullptr) {};

  bool checkSentinel() override {
    return false;
  }
  //using Node<T>::setNext;
  //void setNext(TrueNode<T>* new_prev) {};
  //using Node<T>::setPrev;
  //void setPrev(TrueNode<T>* new_prev) {};
};

template<class T>
class ConsistentList;

template <class T>
class Iterator {
  friend class ConsistentList<T>;
public:
  
  Iterator* operator++() {//pre
    //do smth
    this->ptr = this->ptr->getNext();
    
    this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount() - 1);
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
  }

  Iterator* operator++(int) {//post
    this->ptr = this->ptr->getNext();

    this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount - 1);
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator--
  Iterator* operator--() {//pre
    //do smth
    this->ptr = this->ptr->getPrev();
    
    this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
    this->ptr->getNext()->checkEndRefCount();

    return *this;
  }

  Iterator* operator--(int) {//post
    this->ptr = this->ptr->getPrev();

    this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
    this->ptr->getNext()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator*
  T operator* () {
    return this->ptr->getValue();
  }
  
  // operator->
  Node<T>& operator ->() {
    return this->ptr;
  }
  

  bool operator!=(Iterator it) {
    return (this->ptr != it.ptr);
  }

  bool operator==(Iterator it) {
    return (this->ptr == it.ptr);
  }

private:
  Iterator() = default;
  Iterator(Node<T>* ptr_) : ptr(ptr_) {}
  Node<T>* ptr;
};

template <class T>
class ConstIterator {
  friend class List<T>;
public:
  ConstIterator* operator++() {//pre
    //do smth
    this->ptr = this->ptr->getNext();

    this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount() - 1);
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
  }

  ConstIterator* operator++(int) {//post
    this->ptr = this->ptr->getNext();

    this->ptr->getPrev()->setRefCount(this->ptr->getPrev()->getRefCount - 1);
    this->ptr->getPrev()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator--
  ConstIterator* operator--() {//pre
    //do smth
    this->ptr = this->ptr->getPrev();

    this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
    this->ptr->getNext()->checkEndRefCount();

    return *this;
  }

  ConstIterator* operator--(int) {//post
    this->ptr = this->ptr->getPrev();

    this->ptr->getNext()->setRefCount(this->ptr->getNext()->getRefCount() - 1);
    this->ptr->getNext()->checkEndRefCount();

    return *this;
    //do smth
  }
  // operator*
  const T operator* () {
    return this->ptr->getValue();
  }

  // operator->
  const Node<T>& operator ->() {
    return this->ptr;
  }


  bool operator!=(ConstIterator it) {
    return (this->ptr != it.ptr);
  }

  bool operator==(ConstIterator it) {
    return (this->ptr == it.ptr);
  }
private:
  ConstIterator() = default;
  ConstIterator(Node<T> ptr_) : ptr(ptr_) {};
  Node<T>* ptr;
};

template<class T>
class �onsistentList {
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
  �onsistentList() { }
  explicit �onsistentList(size_type n) {
    for (size_type i = 0; i < n; i++) {
      push_front(NULL);
    }
  };
  �onsistentList(size_type n, const T& value) {
    for (size_type i = 0; i < n; i++) {
      push_front(value);
    }
  };
  //?//
  template<class InputIterator>
  �onsistentList(InputIterator first, InputIterator last);
  //+//
  �onsistentList(const �onsistentList& x) {
    for (size_type i = 0; i < x.size(); i++) {
      this->push_back(*(x.begin() + i));
    }
  };
  // �onsistentList(�onsistentList&& x);
  //+//
  �onsistentList(std::initializer_list<T> init_list) {
    for (size_type i = 0; i < init_list.size(); i++) {
      push_front(init_list[i]);
    }
  };
  //+//
  ~�onsistentList() {
    for (size_type i = 0; i < this->size(); i++) {
      this->pop_back();
    }
    delete this->list_size;
    delete this->sentinel;
  };
  //+//
  �onsistentList& operator=(const �onsistentList& x) {
    for (size_type i = 0; i < this->size(); i++) {
      this->pop_back();
    }
    for (size_type i = 0; i < x.size(); i++) {
      this->push_back(*(x.begin() + i));
    }
    return *this;
  };
  // �onsistentList& operator=(�onsistentList&& x)
  //+//
  �onsistentList& operator=(std::initializer_list<T> init_list) {
    for (size_type i = 0; i < this->size(); i++) {
      this->pop_back();
    }
    for (size_type i = 0; i < init_list.size(); i++) {
      this->push_back(init_list[i]);
    }
    return *this;
  };

  template<class InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const_reference t) {
    //delete nodes;
    //create new nodes and fill it
    
  }
  void assign(std::initializer_list<value_type> init_list) {};

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

  //++++//maybe-maybe)))already remake for iterators using
  reference front() {
    return *this->begin();
    //return this->sentinel.getNext()->getValue();
  };
  const_reference front() const {
    return front();
  };
  reference back() {
    return *(this->end() - 1);
    //return sentinel.getPrev()->getValue();
  };
  const_reference back() const {
    return back();
  };

  //+// ��� ����� ��� ��������� ����������, ��� ����� ���������� ������ ��������
  void push_front(const T& x) {
    this->insert(std::next(this->begin()), x);
    /*node_type* tmp = new node_type;
    tmp->setValue(x);

    tmp->setNext(this->sentinel.getNext());
    this->sentinel.getNext()->setPrev(&tmp);
    this->sentinel.setNext(&tmp);
    tmp->setPrev(this->sentinel);
    


    //��� ��� ������ � ����������� ������ �����

    this->list_size++;*/
  };
  // void push_front(T&& x);
  //+/
  void pop_front() {
    this->erase(this->begin());
    //������ ���������� � erase
  };

  //+//��� � push_front
  void push_back(const T& x) {
    this->insert(this->end(), x);
    /*node_type* tmp = new node_type;
    tmp->setValue(x);

    tmp->setPrev(this->sentinel.getPrev());
    this->sentinel.getPrev->setNext(&tmp);
    this->sentinel.setPrev(&tmp);
    tmp->setNext(this->sentinel);

    //��� ���� ������ � ����������� ������

    this->list_size++;*/
  };
  // void push_back(T&& x);
  //+//
  void pop_back() {
    this->erase(this->end() - 1);
    //������ ���������� � erase
  };

  //++++//
  iterator insert(const_iterator position, const T& x) {
    //node_type* inserted_node(*position);
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
    for (auto i = first; i != last; i++) {
      it_to_inserted_node = this->insert(position, *it);
    }
    return it_to_inserted_node;
  };
  
  iterator insert(const_iterator position, std::initializer_list<T> init_list) {
    return this->insert(position, init_list.begin(), init_list.end());
  };

  iterator erase(const_iterator position) {//�� ������� ����� �������
    const_iterator new_position = position + 1;
    this->delete_node(position.ptr);//�� �������� ������, ��� ���� � ����� �� ��������� ����� ����������� �����
    return new_position;
  };
  iterator erase(const_iterator position, const_iterator last);
  //+//
  void swap(�onsistentList& other_list) {
    //������ ����� ����� ������ �������� ��������� �� ����� ��� ���-�� �����, ��� �� ����� ������ �������� ��������))
    //std::swap(this->sentinel, other_list.sentinel);
    this->swap_nodes(&this->sentinel, &other_list.sentinel);
    std::swap(this->list_size, other_list.list_size);
  };

  //+//
  void clear() noexcept {
    //erase ��� ������� �������� ��� ������ ���-�� ����� ����� �� �������
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
  void merge(�onsistentList& x) {
  //������ �� ���� ���������� �� ����������� ������� ���� ������� ���������� �� ����������� ������, ������� ��������
    //if (!this->is_sorted_list()) {
    if (!std::is_sorted(this->begin(), this->end())) {
      //std::cout << "No merged, because one or both lists not ordered..." << std::endl;
      //EXIT_FAILURE;
      throw MergeException("List's must be ordered!!!");
    }
    size_type diff_size = std::max(this->size(), x.size()) - std::min(this->size(), x.size());

    if (*this->begin() > *(x.end() - 1)) {//���� 
      for (size_type i = 0; i < diff_size; i++) {
        this->push_front(*(x.end() - 1 - i));
        x.pop_back();
      }
    }
    
    if(*(this->end() - 1) < *x.begin()) {
      for (size_type i = 0; i < diff_size; i++) {
        this->push_back(*(x.begin()));
        x.pop_front();
      }
    }
    //x.clear(); �� �����, ��� �� ����� ����� � ����� �� ������ ���� ������������ � ���������� ��� �����
  };
  // void merge(list&& x);

  //+//
  void reverse() noexcept {
    //����� ����� ������ �������� ���������, �� �� ������ ������ ��� �� ����� ������, �������
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

  bool is_sorted_list(�onsistentList& x) {
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
      deleted_node->getNext()->setRefCount(deleted_node->getNext()->getRefCount() + 1);
      deleted_node->getPrev()->setRefCount(deleted_node->getPrev()->getRefCount() + 1);
      //������� �������� �� +1;
    }//if last, will do nothing

    deleted_node->checkEndRefCount();

    this->list_size--;
  }
protected:

};
