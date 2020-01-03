#include "BinaryData.h"

BinaryData::BinaryData()
{
	
}

BinaryData::BinaryData(int maxsize)
{
	data.reserve(maxsize);
}


void BinaryData::append(byte n){
	data.push_back(n);
}

int	 BinaryData::size(){
	return data.size();
}
void BinaryData::append(const std::string n){
	data.insert(data.end(), n.begin(), n.end());
}
const byte* BinaryData::raw_buffer(){
	return data.data();
}
void BinaryData::append(byte *buffer, int size){
	data.insert(data.end(), buffer, buffer + size);
}
std::string BinaryData::to_string(){
	return std::string(data.begin(), data.end());
}
BinaryData::~BinaryData()
{
	
}