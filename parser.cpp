#include "parser.h"

Range<IO::Parser::UpdateIterator> IO::Parser::GetUpdate()
{
	return Range<IO::Parser::UpdateIterator>(_update.begin(), _update.end());
}

Range<IO::Parser::ReadIterator> IO::Parser::GetRead()
{
	return Range<IO::Parser::ReadIterator>(_read.begin(), _read.end());
}

Range<IO::Parser::MapBuilderSetupIterator> IO::Parser::GetMapBuilderSetup()
{
	return Range<IO::Parser::MapBuilderSetupIterator>(_builder_setup.begin(), _builder_setup.end());
}

Range<IO::Parser::DrawMapIterator> IO::Parser::GetDrawMap()
{
	return Range<IO::Parser::DrawMapIterator>(_draw_map.begin(), _draw_map.end());
}

IO::Parser::~Parser()
{
}
