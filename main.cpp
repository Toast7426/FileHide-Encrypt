#include <iostream>
#include <fstream>
#include <vector> 
#include <string>
#include <random>
using namespace std;
int FileHide(bool toHide);
int DirAll();
int FileEncrypt(bool toEncrypt);
int Encrypt(string filePath_Str);
int Decrypt(string filePath_Str);
unsigned short RandomMaker();
void KeyDecode(unsigned short key, char* bitMove);
int main()
{
	bool m_exit = false;
	while (m_exit == false)
	{
		RandomMaker();
		cout << "Choose operation type\n";
		cout << "1.Hide file\t2.Encrypt file\t3.Both of them\n";
		cout << "4.Dir all\t5.Unhide\t6.Decrypt\n";
		cout << "7.Exit\n";
		int number;
		cin >> number;
		switch (number)
		{
		case(1):
		{
			if (FileHide(true) == 0)
			{
				cout << "File is hidden\n";
			}
			else
			{
				cout << "Error!\n";
			}
			break;
		}
		case(2):
		{
			int errorCode = FileEncrypt(true);
			if (errorCode == 0)
			{
				cout << "Done\n";
			}
			else if (errorCode == 1)
			{
				cout << "Error!(Cannot open)\n";
			}
			else if (errorCode == 2)
			{
				cout << "Error!(Cannot read)\n";
			}
			break;
		}
		case(3):
		{
			cout << "To choose 1 and 2\n";
		}
		case(4):
		{
			if (DirAll() == 0)
			{
				cout << "Done\n";
			}
			else
			{
				cout << "Error!\n";
			}
			break;
		}
		case(5):
		{
			if (FileHide(false) == 0)
			{
				cout << "File is unhidden\n";
			}
			else
			{
				cout << "Error!\n";
			}
			break;
		}
		case(6):
		{
			int errorCode = FileEncrypt(false);
			if (errorCode == 0)
			{
				cout << "Done\n";
			}
			else if (errorCode == 3)
			{
				cout << "Error!(Cannot open)\n";
			}
			else if (errorCode == 4)
			{
				cout << "Error!(Cannot read)\n";
			}
			break;
		}
		case(7):
		{
			m_exit = true;
			break;
		}
		}
		cout << "/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";
		cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/\n";
	}
	return 0;
}
int FileHide(bool toHide)
{
	cout << "Choose the file\n";
	string filePath_Str;
	cin >> filePath_Str;
	string cmdParam;
	if (toHide == true)
	{
		cmdParam = "+s +h ";
	}
	else if (toHide == false)
	{
		cmdParam = "-s -h ";
	}
	string sysFileHideCommand = "attrib " + cmdParam + filePath_Str;
	const char* sysFileHideCommand_ConCha = sysFileHideCommand.c_str();
	system(sysFileHideCommand_ConCha);
	return 0;
}
int DirAll()
{
	cout << "Choose the path\n";
	string path_Str;
	cin >> path_Str;
	string dirFileCommand = "dir /a " + path_Str;
	const char* dirFileCommand_ConCha = dirFileCommand.c_str();
	system(dirFileCommand_ConCha);
	return 0;
}
int FileEncrypt(bool toEncrypt)
{
	cout << "Choose the file\n";
	string filePath_Str;
	cin >> filePath_Str;
	if (toEncrypt == true)
	{
	    return Encrypt(filePath_Str);
	}
	else if (toEncrypt == false)
	{
		return Decrypt(filePath_Str);
	}
}
int Encrypt(string filePath_Str)
{
	ifstream ifs;
	ifs.open(filePath_Str, ios::ate | ios::binary);
	if (!ifs.is_open())
	{
		return 1;
	}
	unsigned long long size = ifs.tellg(); // ��ȡ�ļ���С  
	ifs.seekg(0, ifs.beg); // �����ļ���ȡλ�õ��ļ���ͷ
	vector<unsigned char> buffer(size);
	cout << "Did you back it up?Press 'c' to continue";
	char continueInput;
	cin >> continueInput;
	unsigned short randomKey = RandomMaker();//��ȡ�����Կ
	char bitMove[16];
	KeyDecode(randomKey, bitMove);//��ȡ��16�ֽ�Ϊһ��ĸ��ֽ�λ�ƴ�С
	cout << "Key:" << randomKey << "\n";
	cout << "Start to load\n";
	if (ifs.read(reinterpret_cast<char*>(buffer.data()), size))
	{
		// �ɹ���ȡ
		// �����������ʹ��buffer��������������ݻ��߽�����������
	    ifs.close();  
		cout << "Have loaded\n";
		cout << "Start to code\n";
		for (unsigned long long i = 0; i < size; ++i)
		{
			if (i % 4194304 == 0)//����
			{
				int percentNumber = i * 100 / size;
				cout << percentNumber << "%\r";
			}
			buffer[i] = buffer[i] + bitMove[i % 16];
		}
        cout << "Have coded\n";
		cout << "Start to save\n";
		ofstream ofs;
		ofs.open(filePath_Str, ios::out | ios::binary);
		ofs.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
		cout << "Have saved\n";
		return 0;
	}
	else
	{
		return 2;
	}
}
int Decrypt(string filePath_Str)
{
	ifstream ifs;
	ifs.open(filePath_Str, ios::ate | ios::binary);
	if (!ifs.is_open())
	{
		return 3;
	}
	unsigned long long size = ifs.tellg(); // ��ȡ�ļ���С  
	ifs.seekg(0, ifs.beg); // �����ļ���ȡλ�õ��ļ���ͷ
	vector<unsigned char> buffer(size);
	cout << "Did you back it up?Press 'c' to continue";
	char continueInput;
	cin >> continueInput;
	unsigned short Key;//��ȡ��Կ
	cout << "Input your key:";
	cin >> Key;
	char bitMove[16];
	KeyDecode(Key, bitMove);//��ȡ��16�ֽ�Ϊһ��ĸ��ֽ�λ�ƴ�С
	cout << "Start to load\n";
	if (ifs.read(reinterpret_cast<char*>(buffer.data()), size))
	{
		// �ɹ���ȡ
		ifs.close();
		cout << "Have loaded\n";
		cout << "Start to decode\n";
		for (unsigned long long i = 0; i < size; ++i)
		{
			if (i % 4194304 == 0)//����
			{
				int percentNumber = i * 100 / size;
				cout << percentNumber << "%\r";
			}
			buffer[i] = buffer[i] - bitMove[i % 16];
		}
		cout << "Have decoded\n";
		cout << "Start to save\n";
		ofstream ofs;
		ofs.open(filePath_Str, ios::out | ios::binary);
		ofs.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
		cout << "Have saved\n";
		return 0;
	}
	else
	{
		return 4;
	}
}
unsigned short RandomMaker()
{
	random_device rd;  // ��ȡ���������
	mt19937 gen(rd()); // �� rd() ��ʼ�� Mersenne Twister ������
	uniform_int_distribution<> distrib(0, 65535); // ����ֲ���Χ
	// ����һ�������
	int random_number = distrib(gen);
	return random_number;
}
void KeyDecode(unsigned short key, char* bitMove)
{
	*bitMove = { 'a' };//a:�ֽ���λ97
	while (key != 0)
	{//�������ۼ�����ת������
		for (unsigned __int8 j = 0; j <= 16; j++)
		{//��һ
			char* tempBitMove = bitMove;//���������׵�ַ����
			tempBitMove = tempBitMove + j;
			if (*tempBitMove == 'a')
			{
				*tempBitMove = 'b';//b:�ֽ�λ��98
				j = 16;//��ɼ�һ����ѭ��
			}
			else
			{
				*tempBitMove = 'a';//��λ���ٴ�ѭ��
			}
		}
		key--;
	}//�ѽ�һ��usignshort��ת��Ϊ��16�ַ����ַ���
}
