#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
using namespace cv;
using namespace dnn;
using namespace std;

int main() {
    // Load network
    String modelFile = "models/res10_300x300_ssd_iter_140000.caffemodel";
    String configFile = "models/deploy.prototxt";
    Net net = readNetFromCaffe(configFile, modelFile);

    // Use OpenCV backend (or try CUDA if compiled with it)
    net.setPreferableBackend(DNN_BACKEND_CUDA);
    net.setPreferableTarget(DNN_TARGET_CUDA);

    // Open webcam
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error opening webcam\n";
        return -1;
    }

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        // Prepare blob
        Mat blob = blobFromImage(frame, 1.0, Size(300, 300),
                                 Scalar(104.0, 177.0, 123.0), false, false);

        // Forward pass
        net.setInput(blob);
        Mat detections = net.forward();

        // Loop over detections
        Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());
        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);
            if (confidence > 0.5) { // threshold
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                rectangle(frame, Point(x1, y1), Point(x2, y2), Scalar(64, 224, 207), 2);
                putText(frame, format("Conf: %.2f", confidence), Point(x1, y1 - 5),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
  
            }
        }

        imshow("DNN Face Detection", frame);
        if (waitKey(10) == 27) break; // ESC to quit
    }
    return 0;
}
