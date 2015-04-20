#include <yarrr/id_generator.hpp>
#include <chrono>

namespace yarrr
{

IdGenerator::IdGenerator()
{
}

IdGenerator::Id
IdGenerator::generate( const Key& key )
{
  return
    std::to_string( std::chrono::system_clock::now().time_since_epoch().count() ) +
    std::to_string( incremented_index( key ) );
}

IdGenerator::Index
IdGenerator::incremented_index( const Key& key )
{
  auto index_iterator( m_index.find( key ) );
  if ( index_iterator == std::end( m_index ) )
  {
    bool was_inserted( false );
    std::tie( index_iterator, was_inserted ) = m_index.emplace(
        std::make_pair( key, 0 ) );
  }
  ++index_iterator->second;

  return index_iterator->second;
}

}

