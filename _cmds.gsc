
init()
{
	// Add commands here
	thread _chat::add_chat_command("!name", ::chatcmd_name);
}

chatcmd_name(tok)
{
	self setClientCvar("name", tok);
}
