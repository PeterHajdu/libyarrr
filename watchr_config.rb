require 'libnotify'

def failure( command )
  Libnotify.show( :body => command, :urgency => :critical, :summary => "failure", :timeout => 2.5 )
  false
end

def success( command )
  Libnotify.show( :body => command, :urgency => :low, :summary => "success", :timeout => 2.5 )
  true
end

def run( command )
  if ( system( command ) )
    success( command )
  else
    failure( command )
  end
end

def deploy()
  run( "compile" ) && run( "unittest" ) && run( "local_deploy" )
end

watch( '.*pp' ) do | filename |
  deploy
end

watch( '.*lua' ) do | filename |
  deploy
end

watch( 'CMakeLists.txt' ) do | filename |
  deploy
end

