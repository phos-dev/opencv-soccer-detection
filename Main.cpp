
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

	namedWindow("Soccer Match", WINDOW_AUTOSIZE);

    while (true)
    {

        Mat frame, src_gray, rgb[3];
        // Mat object is a basic image container. frame is an object of Mat.

        if (!cap.read(frame)) // if not success, break loop
        // read() decodes and captures the next frame.
        {
	        std::cout << "\n Cannot read the video file. \n";
            break;
        }

        cv::Vec3f vec_;
        split(frame, rgb);
        Mat image = frame;
        Mat grad_x, grad_y, abs_grad_x, abs_grad_y, gray, grad, binaryImg;

        cvtColor(image, gray, COLOR_RGB2GRAY);
        adaptiveThreshold(gray, binaryImg, 128, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 15);
        
        Sobel(gray, grad_x, CV_64F, 0, 1, 3);
        Sobel(gray, grad_y, CV_64F, 1, 0, 3);
        
    	convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
		addWeighted(abs_grad_x, 2, abs_grad_y, 2, 0, grad);

        Mat threshold_image;

        threshold(grad, threshold_image, 200, 255, THRESH_BINARY);
        dilate(threshold_image, threshold_image, getStructuringElement(MORPH_RECT, Size(3, 3)));
        erode(threshold_image, threshold_image, getStructuringElement(MORPH_RECT, Size(3, 3)));

        Mat edges, dst;
        filter2D(threshold_image, dst, -1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        /*Find the edges in the image using canny detector
        Canny(binaryImg, edges, 50, 200);
        // Create a vector to store lines of the image
        std::vector<Vec4i> lines;
        // Apply Hough Transform
        HoughLinesP(edges, lines, 1, CV_PI / 180, 1, 10, 250);

        for (size_t i = 0; i < lines.size(); i++)
        {
            Vec4i l = lines[i];
            line(threshold_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), 3, LINE_AA);
        }
        */
        imshow("Socacer Match", dst);

        if (waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }
    }

	return 0;
}
