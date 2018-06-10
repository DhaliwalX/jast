#ifndef LIST_H_
#define LIST_H_

#include <type_traits>

namespace jast {

template <typename ListType>
class ListIterator {
public:
  using DataType = ListType;
  using reference = ListType&;

  explicit ListIterator(ListType *element)
    : start_((element)), element_((element))
  { }

  ListIterator(ListType *start, ListType *point)
    : start_(start), element_(point)
  { }

  DataType *operator->() {
    return element_;
  }

  reference operator*() {
    return *element_;
  }

  ListIterator<ListType> &operator++() {
    element_ = element_->Next;
    return *this;
  }

  ListIterator<ListType> operator++(int) {
    ListIterator ret(element_);
    element_ = element_->Next;
    return ret;
  }

  ListIterator<ListType> &operator--() {
    element_ = element_->Previous;
    return *this;
  }

  ListIterator<ListType> operator--(int) {
    ListIterator ret(element_);
    element_ = element_->Previous;
    return ret;
  }

  operator DataType*() {
    return element_;
  }

  bool operator==(const ListIterator<ListType> &rhs) const {
    if (rhs.start_ == nullptr) {
      return element_->Next == start_;
    }

    return rhs.start_ == start_ && rhs.element_ == element_;
  }
  DataType *start_;
  DataType *element_;
};

/**
 * Embeddable doubly linked list
 * List is not the owner of the members.
 */
template <typename T>
class List {
  friend class ListIterator<List<T>>;
public:
  using DataType = T;
  using reference = T&;
  using iterator = ListIterator<List<T>>;

  List() {
    Next = (this);
    Previous = (this);
  }

  /**
   * Do nothing destructor
   */
  virtual ~List() {}

  /**
   * append to the list
   */
  void Append(List<T> *element) {
    element->Previous = Previous;
    element->Next = (this);
    Previous = element;
  }

  /**
   * removes the last element from the list
   */
  T *RemoveLast() {
    T *ret = Previous;
    if (Previous != Next) {
      Previous = Previous->Previous;
      Previous->Previous->Next = (this);
    }

    return ret;
  }

  /**
   * Removes itself and returns pointer to the next element in the list
   */
  T *RemoveSelf() {
    T *ret = Next;
    if (Next == Previous) {
      // single element, don't have to do anything
      return nullptr;
    }

    Previous->Next = Next;
    Next->Previous = Previous;
    Previous = nullptr;
    Next = nullptr;
    return Next;
  }

  /**
   * standard container operations
   */
  iterator begin() {
    return iterator((this));
  }

  iterator end() {
    return iterator(nullptr);
  }

  /**
   * inserts element after `point` and returns iterator to the inserted
   * element.
   */
  iterator Insert(iterator point, List<T> *element) {
    element->Next = point->Next;
    element->Previous = &(*point);
    point->Next = element;
    return ++point;
  }

  /**
   * Removes the element pointed by `point`
   */
  iterator Remove(iterator point) {
    return iterator(this, point->RemoveSelf());
  }

  List<T> *Next;
  List<T> *Previous;
};

}

#endif
