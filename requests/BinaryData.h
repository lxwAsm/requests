#ifndef BD
#define BD
#include <string>
#include <vector>
typedef unsigned char byte;//
namespace requests{
	class BinaryData
	{
	public:
		BinaryData();
		explicit BinaryData(int size);
		//BinaryData(const BinaryData &);
		~BinaryData();
		void append(byte n);
		void append(byte *buffer, int size);
		void append(std::string n);
		int  find(const char *s);
		std::string substr(int start, int end);
		void erase(int start, int end);
		const byte* raw_buffer();
		int	 size();
		std::string	to_string();
	private:
		std::vector<byte>	data;
	};
}
#endif