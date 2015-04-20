#pragma once

#include <string>
#include <unordered_map>

namespace yarrr
{
class IdGenerator
{
  public:
    IdGenerator();

    using Id = std::string;
    using Key = std::string;
    Id generate( const Key& key );

  private:
    using Index = uint64_t;
    Index incremented_index( const Key& );

    using IndexTable = std::unordered_map< std::string, Index >;
    IndexTable m_index;
};

}

