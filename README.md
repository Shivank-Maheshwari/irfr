# irfr
Information Retrieval using Face Recognition

This project is written in C++ using Microsoft Visual Studio 2013 update 4. It uses OpenCV as library for image processsing and recognition.

How to Run?

To run this project follow these instruction:

1) This project is developed on Windows 8.1 system and tested on Windows 8 and Windows 7, so if you are using these Operating systems then the project must run otherwise I can't guarantee its successful running.

2) Download the source code.

3) De-compress the zip file in case if you downloaded the zip file at C:\ drive.
Note: You have to copy project folder in C: drive otherwise it won't run.
The folder name must be "irfr" (excluding "). So directory structure must look like:
C: 
  C:\irfr
    C:\irfr\files
      C:\irfr\files\details
      .....
      PropertySheetRelease.props
    C:\irfr\IRFR
      C:\irfr\IRFR\IRFR
        C:\irfr\IRFR\IRFR\Release
        .....
        C:\irfr\IRFR\IRFR\Source.cpp
    C:\irfr\IRFR.exe
    C:\irfr\LICENSE
    C:\irfr\README.md

4) Run the exe.

5) First you have to choose Image Acquisition option since the project is not containing any image.

6) Your face must be aligned and there must be proper light.

7) Press "s" when your face is detected (box around your face).

8) Enter details(Name, E-mail id and mobile number)

9) Now again you have to take more image in different different face positions (try right side look and left side look and try looking down). This will train our classifier so it can effectively recognize faces.

10) Now, the final step, choose second option Image recognition.

11) Enjoy.

You you have any query e-mail me at seekshivank@live.in

How to make changes to project?

You have to install Microsoft Visual Studio and OpenCV (http://opencv.org).
Then open the project by clicking on IRFR.sln in the C:\irfr\IRFR.
