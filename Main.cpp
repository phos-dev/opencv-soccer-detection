
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
        Mat grad_x, grad_y, abs_grad_x, abs_grad_y, gray, grad;

        cvtColor(image, gray, COLOR_RGB2GRAY);
        
        
        Sobel(gray, grad_x, CV_64F, 0, 1);
        Sobel(gray, grad_y, CV_64F, 1, 0);
        
    	convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

        Mat threshold_image = grad;

        for (int y = 0;y < grad.rows;y++)
        {
            for (int x = 0;x < grad.cols;x++)
            {
                Vec3b& color = threshold_image.at<Vec3b>(y, x);
                if (color[1] > color[2] > color[0])
                {
                    color[0] = color[1] = color[2] = 0;
                }
                else
                {
                    color[0] = color[1] = color[2] = 255;
                }

                threshold_image.at<Vec3b>(Point(x, y)) = color;
            }
        }


    	imshow("Soccer Match", threshold_image);

        if (waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }
    }

	return 0;
}
