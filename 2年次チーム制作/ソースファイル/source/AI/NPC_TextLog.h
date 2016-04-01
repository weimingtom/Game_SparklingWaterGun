class TextWriter
{
private:
	std::ofstream	m_file_Buffer;
	std::string		m_file_Path;
	char					m_work_Buffer[256];
public:
	TextWriter();
	~TextWriter();
	void	Close();
	void	Open();
	void	Write(char* str, ...);
	void	Decide_FilePath(const std::string& filepath, const std::string& filename);
};
