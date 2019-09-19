#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;

const cv::String options = "{inputfile i |  | name of file to be processed}"
                           "{help h      |  | show help}";

int main(int argc, char **argv)
{
    cv::CommandLineParser cmdparser(argc,argv,options);
    cmdparser.about("Application to research image proc algos for husk area calcualtion");
    if(cmdparser.has("help")) {
        cmdparser.printMessage();
    }
    if(cmdparser.has("inputfile") == false) {
        std::cout << "You have not provide inputfile! Abort..." << std::endl;
        return 1;
    }

    cv::Mat image = cv::imread(cmdparser.get<cv::String>("inputfile"),cv::IMREAD_GRAYSCALE);
    if(image.empty() == false) {

        // Global thresholding
        cv::Mat binimage;
        cv::threshold(image,binimage,0,255,cv::THRESH_OTSU);
        cv::imshow("Global binarized",binimage);

        // Morphological filtration
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
        cv::dilate(binimage,binimage,kernel,cv::Point(-1,-1),6);
        cv::erode(binimage,binimage,kernel,cv::Point(-1,-1),3);
        cv::imshow("Morphological filtrarion",binimage);

        // Blob detection
        cv::SimpleBlobDetector::Params params;
        params.filterByCircularity = false;
        params.filterByInertia = false;
        params.filterByConvexity = false;
        cv::Ptr<cv::SimpleBlobDetector> ptr = cv::SimpleBlobDetector::create(params);
        std::vector<cv::KeyPoint> keypoints;
        ptr->detect(binimage, keypoints);
        cout << "Blobs have been found: " << keypoints.size() << std::endl;

        // Result visualization
        cv::Mat rgbimage;
        cv::cvtColor(image,rgbimage,cv::COLOR_GRAY2BGR);
        for(const auto &keypoint: keypoints)
            cv::circle(rgbimage,keypoint.pt,keypoint.size/1.5f,cv::Scalar(0,0,255),1,cv::LINE_AA);
        cv::imshow("Blobs",rgbimage);

        cv::waitKey(0); // Delay exit untill user will press any key

    } else {
        cout << "Can not read image '" << cmdparser.get<cv::String>("inputfile").c_str() << "'!" << std::endl;
    }
    return 0;
}
