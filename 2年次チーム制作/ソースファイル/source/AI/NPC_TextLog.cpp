#include	<string>
#include	<fstream>
#include	"NPC_TextLog.h"
#include <cstdarg>

TextWriter::TextWriter() : m_file_Buffer(), m_work_Buffer()
{

}
TextWriter::~TextWriter()
{
	Close();
}

void	TextWriter::Open()
{
	m_file_Buffer.open(m_file_Path.c_str());
	bool	s = m_file_Buffer.is_open();
	int ss =0;
}

void	TextWriter::Decide_FilePath(const std::string& filepath, const std::string& filename)
{
	m_file_Path = filepath + filename;
}

void	TextWriter::Close()
{
	m_file_Buffer.close();
}

void	TextWriter::Write(char* str, ...)
{
	va_list list;
	va_start(list, str);
	vsprintf_s(m_work_Buffer, str, list);

	m_file_Buffer << m_work_Buffer << std::endl;

}