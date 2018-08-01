#pragma once
#include "HuffmanTree.h"
#include <string>

//用于存放哈夫曼编码的结构体
struct Charinfo
{
	unsigned char _ch;
	long long _size;
	string _code;

	Charinfo& operator =(const Charinfo& c2)
	{
		if (this != &c2)
		{
			this->_ch = c2._ch;
			this->_size = c2._size;
		}
		return *this;
	}
	Charinfo operator +(const Charinfo& c2)
	{
		Charinfo tmp;
		tmp._size = this->_size + c2._size;
		return tmp;
	}
	bool operator !=(const Charinfo& c2)
	{
		return this->_size != c2._size;
	}
	bool operator >(const Charinfo& c2)
	{
		return this->_size > c2._size;
	}
		 

	/*Charinfo(char ch, size_t size, string code)
		:_ch(ch)
		,_size(size)
		,_code(code)
	{}*/
};

//文件压缩类
class FileCompress
{
	typedef HufmanTreeNode<Charinfo> Node; 

	//内部类，配置文件存放信息的数组
	struct Charinfos
	{
		char _ch;
		long long _size;
	};

public:
	FileCompress()
	{
		//建立一个256的类型为info的结构体数组，用于存放文件的信息，以及后面对应的哈夫曼编码
		for (size_t i = 0; i < 256; ++i)
		{
			_info[i]._ch = i;
			_info[i]._size = 0;
		}
	}
	//压缩
	void Compress(const char* file)
	{
		//将文件打开遍历，把每个字符存放的info数组中
		FILE *fout;
		if ((fout = fopen(file, "rb")) == NULL)
			exit(0);
		char ch = fgetc(fout);
		//记录每个字符出现的次数
		//while (ch != EOF)
		while (!feof(fout))
		{
			//_info[(unsigned char)ch]._ch = ch; //内容已经初始化
			_info[(unsigned char)ch]._size += 1;
			ch = fgetc(fout);
		}
		//构建一个哈夫曼数，结点的权为Charinfo类型
		//利用运算符重载进行比较
		Charinfo invalid;
		invalid._size = 0;
		HufmanTree<Charinfo> tree(_info, 256, invalid);
		string code;
		_GetHufmanCodes(tree.GetRoot(), code);

		// 文件名称修改
		assert(file);
		string Fi = file;
		Fi = Fi + ".huffman1";
		//向压缩文件写入
		FILE* fin = fopen(Fi.c_str(), "wb");
		assert(fin);
		Charinfos infos;
		for (size_t i = 0; i < 256; ++i)
		{
			if (_info[i]._size > 0)
			{
				infos._ch = _info[i]._ch;
				infos._size = _info[i]._size;
				fwrite(&infos, sizeof(Charinfos), 1, fin);
			}
		}
		//做分隔符
		size_t size = 0;
		infos._size = size;
		fwrite(&infos, sizeof(Charinfos), 1, fin);

		//FILE* fout = fopen(file, "r"); //出现bug，写入的字符不足文本字符，故不重新做输入流
		//assert(fout);                   
		fseek(fout, 0, SEEK_SET);   //从起点开始
		char value = 0;             //0字符，相当与8为都为0的编码
		size_t pos = 0;             //用于位操作
		ch = fgetc(fout);           //读出一个字符
		//string code = _info[ch]._code; //把字符的编码另外存放
		size_t count = 0;
		//while ((ch = fgetc(fout)) != EOF)  //俩种方式都一样
		while (!feof(fout))
		{
			string& code = _info[(unsigned char)ch]._code;
			size_t i = 0;
			while (i < code.size())
			{
				if (code[i] == '0')
				{
					//把该位变为0
					value = value & ~( 1 << pos );
				}
				else if (code[i] == '1')
				{
					// 把该位的0变为1
					value = value | ( 1 << pos );
				}
				else
					assert(false);
				++i;
				++pos;
				//当位走到第九位时,即8位都输入了,将这8位按一个字符存入文件
				if (pos == 8)
				{
					fputc(value, fin);
					value = 0;
					pos = 0;
				}
			}
			ch = fgetc(fout);
			count++;
		}
		if (pos != 0)
		{
			fputc(value, fin);
		}
		cout << count << endl;
		fclose(fin);
		fclose(fout);
	}

	//解压缩
	void UnCompress(const char* file)
	{
		assert(file);
		FILE* fout = fopen(file, "rb");
		string Fi = file;
		Fi = Fi + "2"; 
		//读配置信息
		while (1)
		{
			Charinfos infos;
			fread(&infos, sizeof(Charinfos), 1, fout);
			if (infos._size == 0)
				break;
			//_info[(unsigned int)infos._ch]._ch = infos._ch;
			_info[(unsigned char)infos._ch]._size = infos._size;
		}
		//构建哈夫曼树：
		Charinfo invalid;
		invalid._size = 0;
		HufmanTree<Charinfo> tree(_info, 256, invalid);

		Node* root = tree.GetRoot();
		Node* tmp = root;
		//FILE* fout = fopen(file, "r"); 
		//assert(fout);
		FILE* fin = fopen(Fi.c_str(), "wb");
		assert(fin);
		char ch = fgetc(fout);
		long long count = 0; //字符出现个数
		while (1)
		{
			size_t pos = 0;
			while (pos < 8)
			{
				if ((ch & (1 << pos)) == 0)
					tmp = tmp->_left;
				else if ((ch & (1 << pos)) != 0)
					tmp = tmp->_right;
				else
					assert(false);
				++pos;
				if (tmp->_left == NULL && tmp->_right == NULL)
				{
					if (count == root->_weight._size)
					{
						//cout << count << endl;
						break;
					}
					fputc(tmp->_weight._ch, fin);
					++count;
					tmp = root;
				//	break;
				}
			}
			if (count == root->_weight._size)
			{
				cout << count << endl;
				break;
			}
			ch = fgetc(fout);
		}
		fclose(fin);
		fclose(fout);
	}
protected:

	void _GetHufmanCodes(Node* root, string code)
	{
		if (root == NULL)
			return;
		if (root->_left == NULL && root->_right == NULL)
		{
			_info[(unsigned char)root->_weight._ch]._code = code;
			return;
		}
		_GetHufmanCodes(root->_left, code + '0');
		_GetHufmanCodes(root->_right, code + '1');
	}
	Charinfo _info[256];
};

void TestFcm1()
{
	FileCompress fc;
	fc.Compress("Input1.txt");
	fc.UnCompress("Input1.txt.huffman1");
	//fc.Compress("tu.jpg");
	//fc.UnCompress("tu.jpg.huffman1");
}
 