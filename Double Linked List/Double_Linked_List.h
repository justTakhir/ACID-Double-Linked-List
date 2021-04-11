#pragma once
#include <typeinfo>
#include <iostream>
#include <initializer_list>

//template<typename T>
//void swap_something(T *obj1, T *obj2) {
//  T* buf = obj1;
//  obj1 = obj2;
//  obj2 = buf;
//}

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

template<typename T>
class List_Iterator {
  Node<T>* target_;

  List_Iterator<T> operator --() {

  }

  List_Iterator<T> operator ++() {
    this->target_.ref_count_--;
    if (this->target_.ref_count_ == 0) {

    }
    this->target_ = this->target_.next_;
  }
};

template<class T>
class List;

template <class T>
class Iterator {
  friend class ConsistentList<T>;
public:
  // operator++
  Iterator* operator++() {//pre
    //do smth
    this->ptr = this->ptr->getNext();
    return *this;
  }

  Iterator* operator++(int) {//post
    this->ptr = this->ptr->getNext();
    return *this;
    //do smth
  }
  // operator--
  Iterator* operator--() {//pre
    //do smth
    this->ptr = this->ptr->getPrev();
    return *this;
  }

  Iterator* operator--(int) {//post
    this->ptr = this->ptr->getPrev();
    return *this;
    //do smth
  }
  // operator*
  Node<T>& operator* () {
    return this->ptr->getValue();
  }
  
  // operator->
  

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
  // operator++
  // operator--
  // operator*
  // operator->
private:
  ConstIterator() = default;
  const Node<T>* ptr;
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

  �onsistentList() { }
  explicit �onsistentList(size_type n);
  �onsistentList(size_type n, const T& value);

  template<class InputIterator>
  �onsistentList(InputIterator first, InputIterator last);

  �onsistentList(const �onsistentList& x);
  // �onsistentList(�onsistentList&& x);

  �onsistentList(initializer_list<T>);

  ~�onsistentList();

  �onsistentList& operator=(const �onsistentList& x);
  // �onsistentList& operator=(�onsistentList&& x)

  �onsistentList& operator=(std::initializer_list<T>);

  template<class InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(size_type n, const_reference t) {
    //delete nodes;
    //create new nodes and fill it
    
  }
  void assign(std::initializer_list<value_type> init_list);

  //+//
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

  //+//
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

  //+//maybe-maybe)))i can remake this for iterator using
  reference front() {
    return sentinel.getNext()->getValue();
  };
  const_reference front() const {
    return front();
  };
  reference back() {
    return sentinel.getPrev()->getValue();
  };
  const_reference back() const {
    return back();
  };

  void push_front(const T& x) {
    node_type tmp(x);
    this->list_size++;
  };
  // void push_front(T&& x);

  void pop_front() {
    this->erase(this->begin());
    this->list_size--;
  };

  void push_back(const T& x) {
    this->list_size++;
  };
  // void push_back(T&& x);

  void pop_back() {
    this->erase(this->end() - 1);
    this->list_size--;
  };

  iterator insert(const_iterator position, const T& x);
  // iterator insert(const_iterator position, T&& x);
  iterator insert(const_iterator position, size_type n, const T& x);

  template<class InputIterator>
  iterator insert(const_iterator position, InputIterator first, InputIterator last);
  iterator insert(const_iterator position, initializer_list<T> il);

  iterator erase(const_iterator position) {
    const_iterator new_position = position + 1;
    this->delete_node(position->ptr);//�� �������� ������, ��� ���� � ����� �� ��������� ����� ����������� �����
    return new_position;
  };
  iterator erase(const_iterator position, const_iterator last);

  void swap(�onsistentList& other_list) {
    //������ ����� ����� ������ �������� ��������� �� ����� ��� ���-�� �����, ��� �� ����� ������ �������� ��������))
  };

  //+//
  void clear() noexcept {
    //erase ��� ������� �������� ��� ������ ���-�� ����� ����� �� �������
    while(this->list_size != 0){
      this->pop_front();
    }
  };

  void remove(const T& value) {
  //delete all = value
    node_type* removed_node = this->search_node(value);
    while (removed_node != nullptr) {
      this->erase(removed_node);
      removed_node = search_node(value);
    }
  };
  //+//
  void merge(�onsistentList& x) {
  //������ �� ���� ���������� �� ����������� ������� ���� ������� ���������� �� ����������� ������, ������� ��������
    if (!this->is_sorted_list()) {
      std::cout << "No merged, because one or both lists not ordered..." << std::endl;
      EXIT_FAILURE;
    }
    if (*this->begin() > *(x.end() - 1)) {//���� 
      for (size_type i = 0; i < abs(this->list_size - x.list_size); i++) {
        this->push_front(*(x.end() - 1 - i));
        x.pop_back();
      }
    }
    if(*(this->end() - 1) < *x.begin()) {
      for (size_type i = 0; i < abs(this->list_size - x.list_size); i++) {
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
    TrueNode<T>* searched_node = this->sentinel.getNext();
    while (seacrhed_node != this->sentinel && searched_node->getValue != value) {
      searched_node = searched_node->getNext();
    }
    if (searched_node == this->sentinel) {
      return nullptr;
    }
    return searched_node;

  }

  void delete_node(iterator deleted_node) {
    deleted_node->getPrev()->setNext(deleted_node->getNext());
    deleted_node->getNext()->setPrev(deleted_node->getPrev());
    
    if (deleted_node->getRefCount() == 0) {
      delete deleted_node;
    }
    size--;
  }
protected:

};
