#include "opencv2/core/core.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include<conio.h>
#include<Windows.h>

#define BUFFER_SIZE 200


using namespace cv;
using namespace std;
VideoCapture cap;
string fn_haar = "C:\\irfr\\files\\haarcascade_frontalface_alt.xml";
string fn_csv = "C:\\irfr\\files\\csv.csv";
int deviceId = 0;
vector<Mat> images;
vector<int> labels;
int im_width = 92;
int im_height = 112;
CascadeClassifier haar_cascade;
Mat frame;
Mat original;
Mat gray;
Mat face_resized;
int prediction;
double con;
vector< Rect_<int> > faces;
Mat face;
string name;
string phone;
string email;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

int update_window()
{
	cap >> frame;
	if (frame.empty()){
		cout << "Frame is empty";
		return -1;
	}
	original = frame.clone();
	
	cvtColor(original, gray, CV_BGR2GRAY);
	vector< Rect_<int> > faces;
	haar_cascade.detectMultiScale(gray, faces);
	for (int i = 0; i < faces.size(); i++) {
		Rect face_i = faces[i];
		Mat face = gray(face_i);
		cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
		rectangle(original, face_i, CV_RGB(0, 255, 0), 1);
		int pos_x = max(face_i.tl().x - 10, 0);
		int pos_y = max(face_i.tl().y - 10, 0);
	}
	imshow("Image Acquisition", original);
}

int acq()
{	
	try
	{
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}
	haar_cascade.load(fn_haar);
	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		return -1;
	}
	
	for (int i = 0; i < 10; i++){
		cap >> frame;
	}

	for (;;) {
		update_window();
		char key = (char)waitKey(20);
		if (key == 27)
		{
			cap.release();
			break;
		}
		if (key == 's')
		{   HWND xw = (HWND)cvGetWindowHandle("face_detection");
		    HWND yw =GetConsoleWindow();
			TCHAR infoBuf[BUFFER_SIZE];
			ifstream maxid;
			maxid.open("C:\\irfr\\files\\maxid.txt");
			int id;
			string line;
			if (maxid.is_open())
			{
				getline(maxid, line);
				id = stoi(line);
				maxid.close();
			}
			id++;
			ofstream maxidw;
			maxidw.open("C:\\irfr\\files\\maxid.txt");
			maxidw << to_string(id);
			maxidw.close();
			string filename = "pic";
			imwrite("C:\\irfr\\images\\img\\" + filename + to_string(id) + ".jpg", face_resized);
			string x = filename + ".jpg";
			if (!GetCurrentDirectory(BUFFER_SIZE, infoBuf))
				printf("GetCurrentDirectory() failed!\n");
			printf("Your current directory is: %S\n", infoBuf);
			ofstream save;
			save.open("c:\\irfr\\files\\csv.csv", ios::app);
			string wow = to_string(id) + ".jpg;";
			save << "\nC:/irfr/images/img/pic" + wow + to_string(id);
			save.close();
			SwitchToThisWindow(yw,TRUE);
			cout << "Enter Name:\n";
			cin >> name;
			cout << "Enter E-mail id:\n";
			cin >> email;
			cout << "Enter Phone No.\n";
			cin >> phone;
			ofstream details;
			details.open("C:\\irfr\\files\\details\\" + to_string(id) + ".txt");
			details << name + "\n"+email+"\n"+phone+"\n";
			SwitchToThisWindow(xw, TRUE);
			for (int times = 0; times < 5; times++)
			{
				for (;;){
					update_window();
					char key = (char)waitKey(20);
					if (key == 27)
						break;

					if (key == 's')
					{
						imwrite("C:\\irfr\\images\\img\\" + filename + to_string(id) + "_" + to_string(times) + ".jpg", face_resized);
						string x = filename + ".jpg";
						if (!GetCurrentDirectory(BUFFER_SIZE, infoBuf))
							printf("GetCurrentDirectory() failed!\n");
						printf("Your current directory is: %S\n", infoBuf);
						ofstream save;
						save.open("c:\\irfr\\files\\csv.csv", ios::app);
						string wow = to_string(id) + "_" + to_string(times) + ".jpg;";
						save << "\nC:/irfr/images/img/pic" + wow + to_string(id);
						save.close();
						break;
					}
				}				
			}
			cvDestroyAllWindows();
			SwitchToThisWindow(yw,true);
			break;
		}
	}
	cvDestroyAllWindows();
	return 0;
}
int rec() {
	try {
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}
	Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
	model->train(images, labels);
	haar_cascade.load(fn_haar);
	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		return -1;
	}
	for (int i = 0; i < 10; i++){
		cap >> frame;
	}
	for (;;) {
		cap >> frame;
		if (frame.empty()){
			cout << "Frame is empty";
			return -1;
		}
		original = frame.clone();
		cvtColor(original, gray, CV_BGR2GRAY);
		haar_cascade.detectMultiScale(gray, faces);
		for (int i = 0; i < faces.size(); i++) {
			Rect face_i = faces[i];
			Mat face = gray(face_i);
			Mat face_resized;
			cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
			double con = 0.0;
			int prediction = -1;
			model->predict(face_resized, prediction, con);
			rectangle(original, face_i, CV_RGB(0, 255, 0), 1);
			ifstream info;
			info.open("C:\\irfr\\files\\details\\" + to_string(prediction) + ".txt");
			string name1;
			getline(info, name1);
			//cout << name1;
			name1 += " C: " + to_string(con);
			int pos_x = max(face_i.tl().x - 10, 0);
			int pos_y = max(face_i.tl().y - 10, 0);
			putText(original, name1, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0);
		}
		imshow("face_recognizer", original);
		char key = (char)waitKey(20);
		if (key == 27)
		{
			cap.release();
			break;
		}
	}
	cvDestroyAllWindows();
	return 0;
}

int info()
{
	try {
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}	
	Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
	model->train(images, labels);
	haar_cascade.load(fn_haar);
	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		return -1;
	}
	for (int i = 0; i < 10; i++){
		cap >> frame;
	}
	for (;;) {
		cap >> frame;
		if (frame.empty()){
			cout << "Frame is empty";
			return -1;
		}
		original = frame.clone();
		imshow("Image", original);
		char key = (char)waitKey(20);
		if (key == 27)
		{
			cap.release();
			break;
		}
		if (key == 'i')
		{
			cvtColor(original, gray, CV_BGR2GRAY);
			haar_cascade.load(fn_haar);
			haar_cascade.detectMultiScale(gray, faces);
			if (faces.size() > 1)
			{
				cout << "More Than 1 Face Detected.";
				break;
			}
			else if(faces.size()==0){
				cout << "No Face Detected";
				break;
			}
			Rect face_i = faces[0];
			face = gray(face_i);
			cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
			model->predict(face_resized, prediction, con);
			ifstream info;
			info.open("C:\\irfr\\files\\details\\" + to_string(prediction) + ".txt");
			string name1;
			getline(info, name1);
			getline(info, email);
			getline(info,phone);
			cout << "\nWith confidence level " + to_string(con);
			cout << "\nName is: "+name1+"\nE-mail id is: "+email+"\nPhone No. is: "+phone+"\nPress any key to exit...";
			
			cvDestroyAllWindows();
			cap.release();
			getch();
			//break;
			return 0;
		}		
	}
	return 0;
}
int inp()
{
	try {
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}
	string loc;
	cout << "Enter File Location:\n";
	cin >> loc;
	cout << loc;
	Mat file;
	try{
		file = imread(loc);
	}
	catch (cv::Exception& e)
	{
		cerr << "Error opening file...Press any key to continue.";
		getch();
		exit(0);
	}
	cout << "\nPlease Wait.....\n";
	Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
	model->train(images, labels);
	haar_cascade.load(fn_haar);
	cvtColor(file, gray, CV_BGR2GRAY);
	haar_cascade.detectMultiScale(gray, faces);
	if (faces.size() > 1)
	{
		cout << "More Than 1 Face Detected.";
		getch();
		exit(0);
	}
	else if (faces.size() == 0){
		cout << "No Face Detected";
		getch();
		exit(0);
	}
	else{
		Rect face_i = faces[0];
		face = gray(face_i);
	}
	cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
	model->predict(face_resized, prediction, con);
	ifstream info;
	info.open("C:\\irfr\\files\\details\\" + to_string(prediction) + ".txt");
	string name1;
	getline(info, name1);
	getline(info, email);
	getline(info, phone);
	cout << "\nWith confidence level " + to_string(con);
	cout << "\nName is: " + name1 + "\nE-mail id is: " + email + "\nPhone No. is: " + phone + "\nPress any key to exit...";
	cout << "\nPress any key to continue..";
	getch();
	cvDestroyAllWindows();
	return 0;
}

int main(int argc, const char *argv[])
{
	int x;
	while (1)
	{
		cout << "Press 1 to Acquire \n 2 to Recognize \n 3 for Information Retrieval \n 4 to For File Input \n 5 to exit \n ";
		cin >> x;
		switch (x)
		{
		case 1: acq();
			break;
		case 2: rec();
			break;
		case 3: info();
			break;
		case 4: inp();
			break;
		case 5:
			exit(0);
		default:
			cout << "\n Wrong Input\n";
		};
		system("cls");
	}
	return 0;
}