/*******************************************************************************
** CPSC 131: Part 1 - Sequence Containers
**           Doubly Linked List Quiz
**
** 1) TO-DO 1:    Implement the insert function for a doubly linked circular list with one dummy node that dynamically allocates nodes.
**                -  Return a pointer to the newly inserted node
**                -  Assume the pointer-to-Node called "position" is valid and points to an existing node in the list (no error checking needed)
**                -  Hint:  Solutions should have no loops or decisions
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
struct DLL
{
  struct Node
  {
    Node() = default;
    Node( T value ) : _data( std::move(value) ) {}

    T      _data = T{};
    Node * _next = this;
    Node * _prev = this;
  };

  Node        _sentinel;
  Node *&     _head = _sentinel._next;
  Node *&     _tail = _sentinel._prev;
  std::size_t _size = 0;



  Node * insert( Node * position, T const & data )
  {
    ///////////////////////// TO-DO (1) //////////////////////////////
  Node* newNode = new(std::nothrow) Node(data);

        if (newNode)
        {
            newNode->_next = position->_next;
            newNode->_prev = position;
            position->_next->_prev = newNode;
            position->_next = newNode;

            ++_size;
            return newNode;
        }
        return nullptr;
    /////////////////////// END-TO-DO (1) ////////////////////////////
  }

  Node * begin() { return _head;      }
  Node * end()   { return &_sentinel; }

  void print();
};






int main()
{
  DLL<unsigned int> theList;
  // Sketch the initial state of the list.
  // Be mindful to how you sketch.  Note the required 6 lines, attribute names, and the arrow syntax.  Be sure to not place your
  // sketch in comments (the autograder's code review pass ignores comments)
  // For example:
  #if 0
   +------------------------------------+
   |                                    |
   +-> _sentinel <-> 73 <-> 94 <-> 13 <-+
   _head -> 73
   _tail -> 13
   _size  =  3

     or

   +---------------+
   |               |
   +-> _sentinel <-+
   _head -> _sentinel
   _tail -> _sentinel
   _size  =  0

  ///////////////////////// TO-DO (2) //////////////////////////////
   +---------------+
   |               |
   +-> _sentinel <-+
   _head -> _sentinel
   _tail -> _sentinel
   _size  =  0
  /////////////////////// END-TO-DO (2) ////////////////////////////
  #endif
  theList.print();


  auto n1 = theList.insert( theList.begin(), 20 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 6 lines, attribute names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (3) //////////////////////////////
   +----------------------+
   |                      |
   +-> _sentinel <-> 20 <-+
   _head -> 20
   _tail -> 20
   _size  =  1
  /////////////////////// END-TO-DO (3) ////////////////////////////
  #endif


  theList.insert( n1, 10 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 6 lines, attribute names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (4) //////////////////////////////
   +-----------------------------+
   |                             |
   +-> _sentinel <-> 20 <-> 10 <-+
   _head -> 20
   _tail -> 10
   _size  =  2
  /////////////////////// END-TO-DO (4) ////////////////////////////
  #endif


  theList.insert( theList.end(), 40 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 6 lines, attribute names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (5) //////////////////////////////
   +------------------------------------+
   |                                    |
   +-> _sentinel <-> 40 <-> 20 <-> 10 <-+
   _head -> 40
   _tail -> 10
   _size  =  3
  /////////////////////// END-TO-DO (5) ////////////////////////////
  #endif


  theList.insert( n1, 30 );
  theList.print();
  // Sketch the state of the list after the above operation has completed.
  // Be mindful to how you sketch.  Note the required 6 lines, attribute names, and the arrow syntax
  #if 0
  ///////////////////////// TO-DO (6) //////////////////////////////
   +-------------------------------------------+
   |                                           |
   +-> _sentinel <-> 40 <-> 20 <-> 30 <-> 10 <-+
   _head -> 40
   _tail -> 10
   _size  =  4
  /////////////////////// END-TO-DO (6) ////////////////////////////
  #endif
}















template<typename T>
void DLL<T>::print()
{
  constexpr unsigned minimumCount = 15;

  std::cout << std::format( "+{0:-<{2}}+\n|{1:{2}}|\n+-> _sentinel", '-', ' ', minimumCount + 41 * _size );

  for( auto p = begin(); p != end(); p = p->_next )
  {
    std::cout << std::format( " <-> {{{:#014x}, {:2}, {:#014x}}}", std::uintptr_t( p->_prev ), p->_data, std::uintptr_t( p->_next ) );
  }

  std::cout << std::format( " <-+\n {:#014x}", std::uintptr_t( end() ) );



  for( auto p = begin(); p != end(); p = p->_next )
  {
    std::cout << std::format( "{:^41}", std::format( "{:#014x}", std::uintptr_t( p ) ) );
  }
  std::cout << '\n';


  std::cout << "_head -> " << ( _size == 0 ? "_sentinel" : std::to_string( _head->_data ) ) << '\n'
            << "_tail -> " << ( _size == 0 ? "_sentinel" : std::to_string( _tail->_data ) ) << '\n'
            << "_size  = " << _size << "\n\n\n";
}
