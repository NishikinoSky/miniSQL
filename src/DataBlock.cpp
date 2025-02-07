#include <exception>
#include <iostream>
#include "DataBlock.h"
#pragma once

DataBlock::DataBlock(anyVec _cata)
{
	//recordNum = 0;
	cata = _cata;

	int _recordSpace=0;
	int i = 0, len = cata.size();
	while (i < len) {
		if (cata[i].has_value()) {
			switch (*(cata[i]._Cast<catalog>())) {
				case catalog::CHAR:
					++i;
					_recordSpace += *(cata[i]._Cast<int>());
					break;
				case catalog::FLOAT:
					_recordSpace += 4;
					break;
				case catalog::INT:
					_recordSpace += 4;
					break;

			}
		}
		++i;
	}
	recordSpace = _recordSpace;
	recordMax = blockSpace / (recordSpace+1);
}

void DataBlock::fromFile(std::ifstream & fs)
{
	
	//fs.read((byte*)&recordNum, 4);//使用了c风格的类型转换，待修改
	for (int k = 0; k < recordMax; k++) {
		byte flag;
		fs.read(&flag,charSpace);//检测标记为这一条记录是否为空
		if (fs.eof()) break;
		if (flag == recordInvalid) {
			records.push_back(std::vector<std::any>());
			fs.seekg(static_cast<int64>(recordSpace) - 1, fs.cur);
		}
		anyVec tmp;
		for (int i = 0; i < cata.size(); ++i) {
			if (cata[i].has_value()) {
				auto tmpCata = *(cata[i]._Cast<catalog>());
				if (tmpCata == catalog::CHAR) {
					++i;
					int strlen = *(cata[i]._Cast<int>());
					byte buf[256];
					fs.read(buf, strlen);//使用了c风格的类型转换，待修改
					buf[strlen] = '\0';
					tmp.push_back(std::string(buf));
				}
				if (tmpCata == catalog::FLOAT) {
					float buf;
					fs.read((byte*)&buf, floatSpace);//使用了c风格的类型转换，待修改
					tmp.push_back(buf);
				}
				if (tmpCata == catalog::INT) {
					int buf;
					fs.read((byte*)&buf, intSpace);//使用了c风格的类型转换，待修改
					tmp.push_back(buf);
				}
			}
		}
		records.push_back(tmp);
	}
}

void DataBlock::toFile(std::ofstream & fs)
{
	//fs.write((char*)&recordNum, intSpace);
	for (auto record : records) {
		if (record.size() != 0) {
			int a = fs.tellp();

			fs.write((byte*)&recordValid, charSpace);//写入非空标记
			for (int i = 0, j = 0; i < record.size(); i++, j++) {
				if (record[i].type() == typeid(int)) {
					fs.write((byte*)(record[i]._Cast<int>()), intSpace);
				}
				else if (record[i].type() == typeid(std::string)) {
					j++;
					fs.write((byte*)((*record[i]._Cast<std::string>()).c_str()), *cata[j]._Cast<int>());
				}
				else {
					int a = fs.tellp();
					bool c = record[i].type() == typeid(float);
					float d = *(record[i]._Cast<float>());
					fs.write((byte*)(record[i]._Cast<float>()), floatSpace);
				}
			}
		}
		else {
			fs.write((byte*)&recordInvalid, charSpace);
			fs.seekp(static_cast<int64>(recordSpace) - 1, fs.cur);
		}
	}
}

anyVec& DataBlock::at(int num)
{
	return records[num];
}

void DataBlock::removeRecord(int num)
{
	if ((num+1) > records.size()) throw std::exception("array index out of bounds");
	records[num].resize(0); 
}

void DataBlock::addRecord(anyVec& recToAdd)
{
	if(records.size()<recordMax) records.push_back(recToAdd);
	/*
	else {
		溢出块满了，需要排序
	}
	*/
}

anyVec DataBlock::getRecord(int64 recordSerial)
{
	return records[recordSerial];
}



bool DataBlock::isAbleToAdd()
{
	if (records.size() == recordMax) return false;
	else return true;
}

int64 DataBlock::getRecordNum()
{
	return int64(records.size());
}

int64 DataBlock::getRecordMax()
{
	return int64(recordMax);
}

