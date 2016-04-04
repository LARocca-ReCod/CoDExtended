
init() {

	// todo: partial name matching system-> remove id -requirement- for argument

	// Add commands here
	// <cmd> , <call> , <admin> , <info>
	thread [[ level.chatCallback  ]] ( "!login"			, 	::chatcmd_login							, 0 ,	"Access admin commands"		);
	thread [[ level.chatCallback  ]] ( "!name"			, 	::chatcmd_name							, 0 ,	"Rename yourself"			);

	thread [[ level.chatCallback  ]] ( "!help"			, 	::chatcmd_help							, 1 ,	"List of commands" 			);
	thread [[ level.chatCallback  ]] ( "!status" 		, 	::chatcmd_status  						, 1 ,	"Print players info" 		);
	thread [[ level.chatCallback  ]] ( "!say" 			,	::chatcmd_rconsay 						, 1 ,	"Talk as console" 			);
	
	thread [[ level.chatCallback  ]] ( "!shout" 		,	::chatcmd_say 							, 1 ,	"Shout a message"			);
	thread [[ level.chatCallback  ]] ( "!rename" 		,	::rename 								, 1 ,	"Rename player" 			);
	thread [[ level.chatCallback  ]] ( "!kill" 			,	::kill 									, 1 ,	"Kill player" 				);

}

chatcmd_login( tok ) {
	if ( tok == getCvar ( "adminPassword" ) ) {
		if ( isDefined ( self.pers[ "admin" ] ) ) {
			iprintln ( "^1A^7lready ^2L^7ogged ^1I^7n" );
			return;
		}
		
		self.pers[ "admin" ] = 1;
		self iprintln( " ^1L^7ogin ^2S^7uccessful " );
		return;
	} 
	
	self iprintln( " ^1L^7ogin ^1U^7nsuccessful " );
}

chatcmd_name( tok ) {
	self setClientCvar( "name", tok );
}

chatcmd_status ( tok ) {
	players = getEntArray( "player", "classname" );
	for (i = 0; i < players.size; i++) {
		name = players[ i ].name;
		id = players[ i ] getEntityNumber();
		self iprintln(" ^1" + id + " ^2: ^7" + name );
		wait .05;
	}
}

chatcmd_rconsay ( tok ) {
	sendservercommand("h \"console:^2"+tok+"\"");
}

chatcmd_say ( tok ) {
	iprintlnbold( tok );
}

chatcmd_help ( tok ) {
	
	for ( i = 0; i < level.helpcommand.size; i++ ) {
		if ( isDefined( level.helpcommand[ i ]) )
			self iprintln ( "^2" + level.helpcommand[ i ].cmd + " ^1: ^7" + level.helpcommand[ i ].info );
		wait .05;
	}
}