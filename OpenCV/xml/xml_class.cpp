//Function：XML&YAML文件读写操作
//Source: http://blog.csdn.net/yang_xian521/article/details/6897684
//Status:
//Improve:
//Info:[11/18/2014 jmy]

#include <iostream>
#include <string>
#include <time.h>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

void help(char** av)
{
	cout << endl 
		<< av[0] << " shows the usage of the OpenCV serialization functionality."         << endl
		<< "usage: "                                                                      << endl
		<<  av[0] << " outputfile.yml.gz"                                                 << endl
		<< "The output file may be either XML (xml) or YAML (yml/yaml). You can even compress it by "
		<< "specifying this in its extension like xml.gz yaml.gz etc... "                  << endl
		<< "With FileStorage you can serialize objects in OpenCV by using the << and >> operators" << endl
		<< "For example: - create a class and have it serialized"                         << endl
		<< "             - use it to read and write matrices."                            << endl;
}

class MyData
{
public:
	MyData() : A(0), X(0), id()
	{}
	explicit MyData(int) : A(97), X(CV_PI), id("mydata1234") // explicit to avoid implicit conversion
	{}
	void write(FileStorage& fs) const                        //Write serialization for this class
	{
		fs << "{" << "A" << A << "X" << X << "id" << id << "}";
	}
	void read(const FileNode& node)                          //Read serialization for this class
	{
		A = (int)node["A"];
		X = (double)node["X"];
		id = (string)node["id"];
	}
public:   // Data Members
	int A;
	double X;
	string id;
};

//These write and read functions must be defined for the serialization in FileStorage to work
void write(FileStorage& fs, const std::string&, const MyData& x)
{
	x.write(fs);
}

void read(const FileNode& node, MyData& x, const MyData& default_value = MyData()){
	if(node.empty())
		x = default_value;
	else
		x.read(node);
}

// This function will print our custom class to the console
ostream& operator<<(ostream& out, const MyData& m) 
{ 
	out << "{ id = " << m.id << ", ";
	out << "X = " << m.X << ", ";
	out << "A = " << m.A << "}";
	return out;
}

int main_xml(int ac, char** av)
{
// 	if (ac != 2)
// 	{
// 		help(av);
// 		return 1;
// 	}

	string filename = "outputxml.xml";//av[1];
	{ 
		//write
		Mat R = Mat_<uchar>::eye(3, 3),
			T = Mat_<double>::zeros(3, 1);
		MyData m(1);

		FileStorage fs(filename, FileStorage::WRITE);

		fs << "iterationNr" << 100;
		fs << "strings" << "[";                              // text - string sequence
		fs << "image1.jpg" << "Awesomeness" << "baboon.jpg";
		fs << "]";                                           // close sequence

		fs << "Mapping";                              // text - mapping
		fs << "{" << "One" << 1;
		fs <<        "Two" << 2 << "}";               

		fs << "R" << R;                                      // cv::Mat
		fs << "T" << T;

		fs << "MyData" << m;                                // your own data structures

		fs.release();                                       // explicit close
		cout << "Write Done." << endl;
	}

	{
		//read
		cout << endl << "Reading: " << endl;
		FileStorage fs; 
		fs.open(filename, FileStorage::READ);

		int itNr; 
		//fs["iterationNr"] >> itNr;
		itNr = (int) fs["iterationNr"];
		cout << itNr;
		if (!fs.isOpened())
		{
			cerr << "Failed to open " << filename << endl;
			help(av);
			return 1;
		}

		FileNode n = fs["strings"];                         // Read string sequence - Get node
		if (n.type() != FileNode::SEQ)
		{
			cerr << "strings is not a sequence! FAIL" << endl;
			return 1;
		}

		FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
		for (; it != it_end; ++it)
			cout << (string)*it << endl;


		n = fs["Mapping"];                                // Read mappings from a sequence
		cout << "Two  " << (int)(n["Two"]) << "; "; 
		cout << "One  " << (int)(n["One"]) << endl << endl; 


		MyData m;
		Mat R, T;

		fs["R"] >> R;                                      // Read cv::Mat
		fs["T"] >> T;
		fs["MyData"] >> m;                                 // Read your own structure_

		cout << endl 
			<< "R = " << R << endl;
		cout << "T = " << T << endl << endl;
		cout << "MyData = " << endl << m << endl << endl;

		//Show default behavior for non existing nodes
		cout << "Attempt to read NonExisting (should initialize the data structure with its default).";  
		fs["NonExisting"] >> m;
		cout << endl << "NonExisting = " << endl << m << endl;
	}

	cout << endl 
		<< "Tip: Open up " << filename << " with a text editor to see the serialized data." << endl;

	getchar();
	return 0;
}

/*
 *	功能：
 *  参数：
 *  状态：
 */
int  xmldemo()
{
	FileStorage  fs("test.yml", FileStorage::WRITE);
	fs << "frameCount" << 5;
	time_t  rawtime;
	time(&rawtime);
	fs << "calibrationDate" << asctime(localtime(&rawtime));
	Mat  cameraMatrix = (Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
	Mat  distCoeffs = (Mat_<double>(5, 1) << 0.1, 0.01, -0.001, 0, 0);
	fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;
	fs << "features" << "[";
	for (int i = 0; i < 3; i++)
	{
		int  x = rand() % 640;
		int  y = rand() % 480;
		uchar  lbp = rand() % 256;
		fs << "{:" << "x" << x << "y" << y << "lbp" << "[:";
		for (int j = 0; j < 8; j++)
			fs << ((lbp >> j) & 1);
		fs << "]" << "}";
	}
	fs << "]";
	fs.release();
	return  0;
}