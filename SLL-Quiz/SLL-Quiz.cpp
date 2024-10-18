/*******************************************************************************
** CPSC 131: Part 1 - Sequence Containers
**           Singly Linked List Quiz
**
** 1) TO-DO 1:    Implement the insert_after function for a null-terminated list.  Insert at the beginning of a list by passing a
**                null pointer. Return an pointer to the newly inserted node.
**
** 2) TO-DO 2-6:  Sketch (type) the state of the list after each operation
*******************************************************************************/


#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <new>
#include <string>
#include <utility>





template<typename T>
struct SLL
{
  struct Node
  {
    Node() = default;
    Node( T value ) : _data{ std::move( value ) } {}
    T      _data = T{};
    Node * _next = nullptr;
  };

  Node *      _head = nullptr;
  Node *      _tail = nullptr;
  std::size_t _size = 0;



  Node * insert_after( Node * position, T const & data )
  {
    ///////////////////////// TO-DO (1) //////////////////////////////
    Node *newNode = new(std::nothrow) Node(data);
    if (!newNode){
      return nullptr;
    }
    if (!position){
      newNode->_next = _head;
      _head = newNode;

      if (!_tail){
        _tail = newNode;
      }
    } else {
      newNode->_next = position->_next;
      position->_next = newNode;

      if (position == _tail) {
        _tail = newNode;
      }
    }
    ++_size;

    return newNode;
    /////////////////////// END-TO-DO (1) ////////////////////////////
  }


  void print();
};






int main()
{
  SLL<unsigned int> theList;
  // Sketch the initial state of the list.
  // Be mindful how you sketch.  Note the required 3 lines, attribute and null terminator names, and the arrow syntax.  Be sure to not
  // place your sketch in comments (the autograder's code review pass ignores comments)
  // For example:
  #if 0
       _head -> 73 -> 94 -> 13 -> Ø       (will also accept NULL instead of Ø, for example   _head -> 73 -> 94 -> 13 -> NULL)
       _tail -> 13
       _size = 3
  ///////////////////////// TO-DO (2) //////////////////////////////
  _head -> NULL
  _tail -> NULL
  _size = 0
  /////////////////////// END-TO-DO (2) ////////////////////////////
  #endif
  theList.print();


  auto n1 = theList.insert_after( nullptr, 20 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 3 lines, attribute and null terminator names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (3) //////////////////////////////
  _head -> 20 -> NULL
  _tail -> 20
  _size = 1
  /////////////////////// END-TO-DO (3) ////////////////////////////
  #endif


  auto n2 = theList.insert_after( nullptr, 10 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 3 lines, attribute and null terminator names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (4) //////////////////////////////
  _head -> 10 -> 20 -> NULL
  _tail -> 20
  _size = 2
  /////////////////////// END-TO-DO (4) ////////////////////////////
  #endif


  theList.insert_after( n1, 40 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 3 lines, attribute and null terminator names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (5) //////////////////////////////
  _head -> 10 -> 20 -> 40 -> NULL
  _tail -> 40
  _size = 3
  /////////////////////// END-TO-DO (5) ////////////////////////////
  #endif


  theList.insert_after( n2, 30 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 3 lines, attribute and null terminator names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (6) //////////////////////////////
  _head -> 10 -> 30 -> 20 -> 40 -> NULL
  _tail -> 40
  _size = 4
  /////////////////////// END-TO-DO (6) ////////////////////////////
  #endif
}
























template<typename T>
void SLL<T>::print()
{
  const std::string NIL = "\u{00D8}";    // Ø

  // Print entire list starting at the head
  std::cout << "  _head -> ";
  for( const Node * p = _head; p != nullptr; p = p->_next )    std::cout << std::format( "{{{:2}, {:#014x}}} -> ", p->_data, reinterpret_cast<std::uintptr_t>(p->_next) );  // {:014p} coming in C++26
  std::cout << NIL << "\n";

  // Print the addresses of the nodes in the list
  if( _head != nullptr )
  {
    std::cout << "             ";
    for( const Node * p = _head; p != nullptr; p = p->_next )    std::cout << std::format( "({:#014x})        ", reinterpret_cast<std::uintptr_t>(p) );
    std::cout << '\n';
  }

  // Print the contents of the tail and the address of the tail node
  if( _tail == nullptr ) std::cout << "  _tail -> " << NIL << "\n";
  else
  {
    std::cout << std::format( "  _tail -> {{{:2}, {:#014x}}}\n"
                              "             ({:#014x})\n",
                              _tail->_data,  reinterpret_cast<std::uintptr_t>( _tail->_next ),
                              reinterpret_cast<std::uintptr_t>( _tail ) );
  }

  // Print the size of the list
  std::cout << std::format( "  _size =  {}\n\n\n",  _size );
}
