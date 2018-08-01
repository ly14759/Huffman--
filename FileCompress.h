#pragma once
#include "HuffmanTree.h"
#include <string>

//���ڴ�Ź���������Ľṹ��
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

//�ļ�ѹ����
class FileCompress
{
	typedef HufmanTreeNode<Charinfo> Node; 

	//�ڲ��࣬�����ļ������Ϣ������
	struct Charinfos
	{
		char _ch;
		long long _size;
	};

public:
	FileCompress()
	{
		//����һ��256������Ϊinfo�Ľṹ�����飬���ڴ���ļ�����Ϣ���Լ������Ӧ�Ĺ���������
		for (size_t i = 0; i < 256; ++i)
		{
			_info[i]._ch = i;
			_info[i]._size = 0;
		}
	}
	//ѹ��
	void Compress(const char* file)
	{
		//���ļ��򿪱�������ÿ���ַ���ŵ�info������
		FILE *fout;
		if ((fout = fopen(file, "rb")) == NULL)
			exit(0);
		char ch = fgetc(fout);
		//��¼ÿ���ַ����ֵĴ���
		//while (ch != EOF)
		while (!feof(fout))
		{
			//_info[(unsigned char)ch]._ch = ch; //�����Ѿ���ʼ��
			_info[(unsigned char)ch]._size += 1;
			ch = fgetc(fout);
		}
		//����һ����������������ȨΪCharinfo����
		//������������ؽ��бȽ�
		Charinfo invalid;
		invalid._size = 0;
		HufmanTree<Charinfo> tree(_info, 256, invalid);
		string code;
		_GetHufmanCodes(tree.GetRoot(), code);

		// �ļ������޸�
		assert(file);
		string Fi = file;
		Fi = Fi + ".huffman1";
		//��ѹ���ļ�д��
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
		//���ָ���
		size_t size = 0;
		infos._size = size;
		fwrite(&infos, sizeof(Charinfos), 1, fin);

		//FILE* fout = fopen(file, "r"); //����bug��д����ַ������ı��ַ����ʲ�������������
		//assert(fout);                   
		fseek(fout, 0, SEEK_SET);   //����㿪ʼ
		char value = 0;             //0�ַ����൱��8Ϊ��Ϊ0�ı���
		size_t pos = 0;             //����λ����
		ch = fgetc(fout);           //����һ���ַ�
		//string code = _info[ch]._code; //���ַ��ı���������
		size_t count = 0;
		//while ((ch = fgetc(fout)) != EOF)  //���ַ�ʽ��һ��
		while (!feof(fout))
		{
			string& code = _info[(unsigned char)ch]._code;
			size_t i = 0;
			while (i < code.size())
			{
				if (code[i] == '0')
				{
					//�Ѹ�λ��Ϊ0
					value = value & ~( 1 << pos );
				}
				else if (code[i] == '1')
				{
					// �Ѹ�λ��0��Ϊ1
					value = value | ( 1 << pos );
				}
				else
					assert(false);
				++i;
				++pos;
				//��λ�ߵ��ھ�λʱ,��8λ��������,����8λ��һ���ַ������ļ�
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

	//��ѹ��
	void UnCompress(const char* file)
	{
		assert(file);
		FILE* fout = fopen(file, "rb");
		string Fi = file;
		Fi = Fi + "2"; 
		//��������Ϣ
		while (1)
		{
			Charinfos infos;
			fread(&infos, sizeof(Charinfos), 1, fout);
			if (infos._size == 0)
				break;
			//_info[(unsigned int)infos._ch]._ch = infos._ch;
			_info[(unsigned char)infos._ch]._size = infos._size;
		}
		//��������������
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
		long long count = 0; //�ַ����ָ���
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
 