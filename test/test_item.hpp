#pragma once

namespace test
{

class TestItem : public yarrr::Item
{
  public:
    add_polymorphic_ctci( "yarrr_test_item" );

    TestItem()
      : Item( "test item name" )
    {
    }

    TestItem( const Id& id )
      : Item( id, "test item name" )
    {
    }

    virtual yarrr::ObjectBehavior::Pointer clone() const override
    {
      return Pointer( new TestItem( id() ) );
    }
};

}


