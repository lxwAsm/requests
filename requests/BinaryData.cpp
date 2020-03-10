#include "BinaryData.h"
using namespace requests;
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

int  BinaryData::find(const char *s){
	return std::string(data.begin(), data.end()).find(s);
}

std::string BinaryData::substr(int start, int end){
	return std::string(data.begin() + start, data.begin() + end);
}

void BinaryData::erase(int start, int end){
	data.erase(data.begin() + start, data.begin() + end);
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