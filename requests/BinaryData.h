#include <cstdlib>
#include <string>
#include <vector>
#include <stdexcept>
typedef unsigned char byte;
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
	const byte* raw_buffer();
	int	 size();
	std::string	to_string();
private:
	std::vector<byte>	data;
};