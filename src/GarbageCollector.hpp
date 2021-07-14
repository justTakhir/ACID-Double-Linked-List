#pragma once

#include <vector>
#include <thread>
//#include "TrueNode.hpp"
#include "SpinlockBasedRWLock.hpp"

RWLock static big_gc_lock;

template <class T>
class Node;

template <class T>
class TrueNode;

template <class T>
class Iterator;

template <class T>
class ConstIterator;

template <class T>
struct gc_node{
//public:
  Node<T>* node_;
  bool marked_ = false;
public:
 gc_node(Node<T>* node, bool marked = false) : node_(node), marked_(node) {}
};

template <class T>
class GarbageCollector {
  friend class Iterator<T>;
  friend class ConstIterator<T>;
  friend class Node<T>;

private:
  //std::vector<gc_node<T>> nodes_;
  std::vector <std::pair<Node<T>*, bool>> nodes_;
  std::thread thr_;
  RWLock gc_lock_;
  bool stoped_;
public:
  GarbageCollector() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    this->thr_ = std::thread(&GarbageCollector::init, this);
  }

  ~GarbageCollector() {
    this->nodes_.clear();
    this->thr_.detach();
  }

  std::thread* getThread() {
    return &this->thr_;
  }

  RWLock* getLock() {
    return &this->gc_lock_;
  }

  void putNodeToGC(Node<T>* node){
    //gc_node buf_node(node, false);
    //buf_node.marked_ = false;
    //this->nodes_.push_front(buf_node);
    std::pair<Node<T>*, bool> buf;
    buf.first = node;
    buf.second = false;
    //this->nodes_.insert(this->nodes_.begin(), buf_node);
    this->nodes_.insert(this->nodes_.begin(), buf);
  }

  template <typename U>
  void sendBack(U node_it) {//std::iterator<std::random_access_iterator_tag, gc_node<T>> node_it
    this->nodes_.erase(node_it);
  };

  //void killNode(gc_node *node) {
  template <typename U>
  void killNode(U node_it){
    //gc_node<T>* node = *node_it;
    //TrueNode<T>* victim = node->node_;
    //this->nodes_.erase(node_it);
    //delete victim;
    //delete (&node);
    //std::pair<TrueNode<T>*, bool> buf;
    this->nodes_.erase(node_it);
    delete (*node_it).first;
    //delete(*node_it).second;
  }

  void init() {
    this->stoped_ = false;
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (!this - nodes_.empty()) {
        this->doWorkingCycle();
      }
      if (this->stoped_) {
        return;
      }
    }
  }

  void stop() {
    this->stoped_ = true;
  }

  void doWorkingCycle() {
    if (nodes_.size() > 0) {
      this->gc_lock_.wlock();
      auto purge_start = this->nodes_.begin();
      this->gc_lock_.unlock();

      //for node(from purge_start to null) {
      for (auto node_it = purge_start; node_it != this->nodes_.end(); node_it++) {
        if ((*node_it).first->getRefCount() > 0 && (*node_it).second) {
          this->sendBack(node_it);
        }
        else {
          (*node_it).second = 1;
        }
      }

      this->gc_lock_.wlock();
      //auto new_purge_start = pudge_list.start;
      auto new_purge_start = nodes_.begin();
      this->gc_lock_.unlock();

      for (auto node_it = new_purge_start; node_it != purge_start; node_it++) {
        //for node(from new_purge_start to purge_start) {//ÊÎÃÄÀ ÐÅÔ ÊÀÓÍÒ ÄÎÑÒÈÃ ÍÓËß, ÍÎ ÎÍÈ ÍÅ ÏÎÌÅ×ÅÍÛ ÍÀ ÓÄÀËÅÍÈÅ
        if ((*node_it).second) {
          this->sendBack(node_it);
        }
      }

      //for node(from purge_start to null) {
      for (auto node_it = purge_start; node_it != this->nodes_.end(); node_it++) {
        this->killNode(node_it);
      }
    }
  }
};