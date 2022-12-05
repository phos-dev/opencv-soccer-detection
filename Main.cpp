
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    VideoCapture cap("data.avi");

	if (!cap.isOpened())
	{
		std::cout << "Cannot open the video file. \n";
		return -1;
	}

	double fps = cap.get(CAP_PROP_FPS);

	// namedWindow("Soccer Match", WINDOW_AUTOSIZE);

    while (true)
    {
        Mat frame, src_gray;
        // Mat object is a basic image container. frame is an object of Mat.

        if (!cap.read(frame)) // if not success, break loop
        // read() decodes and captures the next frame.
        {
	        std::cout << "\n Cannot read the video file. \n";
            break;
        }

        cv::Vec3f vec_;

     

        
        imshow("Soccer Match", src_gray);

        if (waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }
    }

	return 0;
}
