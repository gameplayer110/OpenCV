#include <io.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/*
*  ���ܣ������ļ������ļ����ڵ��ļ�
*  ������
*  ״̬��
*/
void getFiles(string path, vector<string>& files)
{
	//�ļ����
	long  hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.JPG").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮,�������,�����б�
			if ((fileinfo.attrib &_A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

//���ܲ�����
//int main()
//{
//	string path = "G:\\DataSET\\TextDetection\\MSRA-TD500\\test";
//	vector<string> filenames;
//	getFiles(path, filenames);
//	for (vector<string>::iterator it = filenames.begin(); it < filenames.end(); it++)
//	{
//		cout << *it << endl;
//		//mserArea(*it);
//		cin.get();
//	}
//
//	return 0;
//}