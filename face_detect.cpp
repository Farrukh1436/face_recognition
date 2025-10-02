#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
    // Load Haar cascade (pre-trained face detector)
    CascadeClassifier face_cascade;
    if (!face_cascade.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")) {
        cout << "Error loading Haar cascade\n";
        return -1;
    }

    // Open webcam
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error: Could not open camera\n";
        return -1;
    }

    Mat frame, gray;
    vector<Rect> faces;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        // Detect faces
        face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));

        for (auto &face : faces) {
            rectangle(frame, face, Scalar(64, 224, 207), 1);
        }

        imshow("Webcam Face Detection", frame);

        if (waitKey(10) == 27) break; // press ESC to exit
    }

    return 0;
}
