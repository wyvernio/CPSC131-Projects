#pragma once

///////////////////////// TO-DO (1) //////////////////////////////
  /// Include necessary header files
  /// Hint:  Include what you use, use what you include
  ///
  /// Do not put anything else in this section, i.e. comments, classes, functions, etc.  Only #include directives
#include <iostream>     
#include <unordered_map>
/////////////////////// END-TO-DO (1) ////////////////////////////

class WordFrequency
{
  public:
    WordFrequency( std::istream & iStream = std::cin);                 // (default) constructor, add words from file to hash table

    std::size_t numberOfWords   (                          ) const;    // return the number of unique words
    std::size_t wordCount       ( const std::string & word ) const;    // return the number of occurrences of the given word
    std::string mostFrequentWord(                          ) const;    // return the most frequent word
    std::size_t maxBucketSize   (                          ) const;    // return the size of the largest bucket in the hash table


  private:
    ///////////////////////// TO-DO (2) //////////////////////////////
      /// The class should have a single instance attribute of type std::unordered_map, which is the C++ Standard Library's
      /// implementation of a hash table, to store the association of words (key) to the number of times a word occurs (value).
    std::unordered_map<std::string, std::size_t> wordHashMap;
    /////////////////////// END-TO-DO (2) ////////////////////////////
};
