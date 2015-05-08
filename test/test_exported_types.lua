Coordinate.new( 1, 2 )
TileCoordinate.new( 1, 2 )
a_tile = Tile.new(
  TileCoordinate.new( 0, 0 ),
  TileCoordinate.new( 1, -1 ) )

a_shape = Shape.new()
a_shape:add_tile( a_tile )

behavior = ShapeBehavior.new( a_shape )
behavior.shape = a_shape

ShapeGraphics.new()

PhysicalBehavior.new()

Inventory.new()
CargoSpace.new()

assert( ship_layer )
Collider.new( ship_layer )
DamageCauser.new()
LootDropper.new()
ObjectIdentity.new( "imperial duck" )

assert( main_thruster )
Thruster.new( main_thruster, Coordinate.new( 0, 0 ), 180 )
assert( port_thruster )
assert( starboard_thruster )

assert( degrees( 10 ) )
assert( metres( 10 ) )

Canon.new( TileCoordinate.new( 0, 0 ), 0 )

mission_info = MissionInfo.new( "name", "description" )

mission = Mission.new( mission_info )

function objective_updater( context )
end

objective = MissionObjective.new( "objective description", objective_updater )

mission:add_objective( objective )

assert( ongoing )
assert( succeeded )
assert( failed )
assert( na )

function a( object )
end

lua_function = LuaFunction.new( a )
LuaAgent.new( lua_function, 1000000 )

