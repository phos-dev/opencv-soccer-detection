
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void detectLines(const Mat& image, vector<Vec4i>& lines)
{
    GaussianBlur(image, image, Size(3, 3), 0, 0);
    
    Mat edges;
    Canny(image, edges, 30, 90);

    HoughLinesP(edges, lines, 2, CV_PI / 180, 50, 60, 3);
}

void ApplySobel(const Mat& image, Mat& grad, Mat& gradX, Mat& gradY, Mat& gray)
{
    Mat abs_grad_x, abs_grad_y;

    cvtColor(image, gray, COLOR_BGR2GRAY);
    Sobel(gray, gradX, CV_32F, 1, 0);
    Sobel(gray, gradY, CV_32F, 0, 1);

    convertScaleAbs(gradX, abs_grad_x);
    convertScaleAbs(gradY, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
}

int main()
{
    VideoCapture cap("data.avi");

	if (!cap.isOpened())
	{
		std::cout << "Cannot open the video file. \n";
		return -1;
	}

    while (true)
    {

        Mat frame, src_gray, rgb[3];

        if (!cap.read(frame)) 
        {
	        std::cout << "\n Cannot read the video file. \n";
            break;
        }

        split(frame, rgb);
        Mat image = frame;
        Mat grad, grad_x, grad_y, gray;
		Mat morph_kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));

        // Contours
        ApplySobel(image, grad, grad_x, grad_y, gray);
        
        // Fill gaps
        magnitude(grad_x, grad_y, grad);
        normalize(grad, grad, 0, 255, NORM_MINMAX, CV_8U);

        adaptiveThreshold(gray, grad, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 3, 5);
        threshold(grad, grad, 50, 255, THRESH_BINARY);

        morphologyEx(grad, grad, MORPH_CLOSE, morph_kernel, Point(-1, -1), 2);

		// Remove ground		
        Mat threshold_image;
        Mat colorFilter(image.size(), CV_8UC1);
        for (int y = 0;y < image.rows;y++)
        {
            for (int x = 0;x < image.cols;x++)
            {
                Vec3b color = image.at<Vec3b>(y, x);
                uchar gradColor = grad.at<uchar>(y, x);

                if (color[1] > color[2] && color[2] > color[0] && gradColor == 0)
                {
                    colorFilter.at<uchar>(y, x) = 0;
                }
                else
                {
                    colorFilter.at<uchar>(y, x) = 255;
                }
            }
        }

		image.copyTo(grad, colorFilter);

        // Contours
        ApplySobel(grad, grad, grad_x, grad_y, gray);

        // Detect lines
        vector<Vec4i> lines;
        detectLines(grad, lines);

        for(size_t i = 0; i < lines.size(); i++)
        {
            Vec4i l = lines[i];
            line(grad, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0, 255), 3);
        }

        vector<vector<Point>> contours;
        
    	findContours(grad, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        

        vector<vector<Point> > players;

        Mat filteredImage = Mat::zeros(grad.size(), CV_8UC1);

        for (int i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            double perimeter = arcLength(contours[i], true);
            double ratio = area / perimeter;
            if (ratio > 1.3 && ratio <= 12)
            {
                players.push_back(contours[i]);
            }
        }


        Mat result = image.clone();
        drawContours(result, players, -1, Scalar(0, 0, 255), 2);
        
        imshow("Soccer Match", result);
        
        if (waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
        	break;
        }
    }

	return 0;
}
