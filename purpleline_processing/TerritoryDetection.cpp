﻿
#include "TerritoryDetection.h"

// things this needs to to do

// determine purple line

struct vecClass {
    bool operator() (cv::Vec4i v1, cv::Vec4i v2) {return (TerritoryDetection::findLength(v1) > TerritoryDetection::findLength(v2));}
} vecObjectWow;

namespace TerritoryDetection {

void goThroughPixels(cv::Mat& im_purp_contour, GridMap& local_map) {
    int num_rows = im_purp_contour.rows;
    int num_cols = im_purp_contour.cols;//FRAME_SIZE_X;
    for (int i=0; i<num_rows; i++) { // from bottom to top
        for (int j=0; j<num_cols; j++) {
            //std::cout<<i<<" "<<j;
            uchar tmp = im_purp_contour.at<uchar>(i,j); // get gray value
            if(tmp == COLOR_WHITE) {
                Eigen::Vector2d pt_im;
                pt_im << j, i; // col, row --> x, y
                Eigen::Vector2d pt_c = CameraMath::reconstructPoint2D(pt_im, 0); // on the floor
                int x = floor(30+pt_c[0]); // hacking for now
                int y = floor(30+pt_c[1]);
                local_map.setVal(x,y,255);
            }
        }
    }
}


// should i first do find contour to find area of feature?
void detectPurpleLine(cv::Mat& frame, GridMap& local_map) {

    // get purple line
    cv::Mat im_purp = ColorDetection::detectColor(frame, ColorDetection::COLOR_LINE_PURPLE);
    // do a canny edge detection
    cv::Mat purp_canny = ImageUtils::cannyEdge(im_purp);

    ImageUtils::ContourData purp_contour_data = ImageUtils::getContours(im_purp);
    ImageUtils::cleanContour(purp_contour_data, FEATURE_AREA_THRESH_PURP_LINE);
    cv::Mat purp_contour = ImageUtils::drawContoursFilled(purp_contour_data, im_purp);

    goThroughPixels(purp_contour, local_map);

    // do a hough line fit

    // check line properties


}

// groups a vector of lines into a vector of vector of lines
// used to separate them based on threshold of sin0

void groupPurpleLines(std::vector<cv::Vec4i>& inpVec, std::vector<std::vector<cv::Vec4i>>& outVec, double threshold) {
    if (inpVec.size() == 1) {
        outVec = {inpVec};
    }
    else if (inpVec.size() > 1) {
        std::vector<double> cpVec;
        cv::Vec4i tempVec;
        double cp, cpMin, cpAverage;
        int minGroup;
//        std::sort(inpVec.begin(),inpVec.end(),vecObjectWow);
        outVec.push_back({inpVec[0]});
        inpVec.erase(inpVec.begin());
        while (inpVec.size() > 0) {
            tempVec = inpVec[0];
            inpVec.erase(inpVec.begin());
            cpVec.clear();
            for (int i = 0; i < outVec.size(); i++) {
                cp = 0.0;
                cpAverage = 0.0;
                int badcount = 0;
                for (int j = 0; j < outVec[j].size(); j++) {
                    cp = findCrossProduct(tempVec,outVec[i][j]);
                    if (cp == 0.0) {
                        badcount += 1;
                    }
                    else {
                        cpAverage += cp;
                    }
                }
                if (badcount < outVec[i].size()) {
                    cpAverage = cpAverage / (outVec[i].size()-badcount);
                    cpVec.push_back(cpAverage);
                }
            }
            if (!cpVec.empty()) {
                cpMin = cpVec[0];
                minGroup = 0;
                for (int i = 1; i < cpVec.size(); i++) {
                    if (cpVec[i] < cpMin) {
                        cpMin = cpVec[i];
                        minGroup = i;
                    }
                }
                std::cout << cpMin << std::endl;
                if (cpMin < threshold) {
                    outVec[minGroup].push_back(inpVec[minGroup]);
                }
                else {
                    outVec.push_back({inpVec[minGroup]});
                }
            }
        }
    }
}

void findAverageOfLines(std::vector<std::vector<cv::Vec4i>>& inpVec, std::vector<cv::Vec4i>& outVec) {
    std::vector<cv::Vec4i> tempVec;
    double x1avg, x2avg, y1avg, y2avg;
    for (int i = 0; i < inpVec.size(); i++) {
        tempVec = inpVec[i];
        x1avg = 0.0;
        x2avg = 0.0;
        y1avg = 0.0;
        y2avg = 0.0;
        for (int j = 0; j < tempVec.size(); j++) {
            x1avg += tempVec[j][0];
            y1avg += tempVec[j][1];
            x2avg += tempVec[j][2];
            y2avg += tempVec[j][3];
        }
        int x1 = (int) round(x1avg/tempVec.size());
        int y1 = (int) round(y1avg/tempVec.size());
        int x2 = (int) round(x2avg/tempVec.size());
        int y2 = (int) round(y2avg/tempVec.size());
        outVec.push_back({x1,y1,x2,y2});
    }
}

void detectPurpleLineTest2(cv::Mat& frame, GridMap& local_map) {

    cv::Mat purp_canny_color;
    cv::Mat purp_contour_color;

    cv::Mat im_purp = ColorDetection::detectColor(frame, ColorDetection::COLOR_LINE_PURPLE);
    cv::Mat purp_canny = ImageUtils::cannyEdge(im_purp);

    ImageUtils::ContourData purp_contour_data = ImageUtils::getContours(im_purp);
    ImageUtils::cleanContour(purp_contour_data, FEATURE_AREA_THRESH_PURP_LINE);
    cv::Mat purp_contour = ImageUtils::drawContoursFilled(purp_contour_data, im_purp);

    goThroughPixels(purp_contour, local_map);

    ImageUtils::HoughDataP purp_contour_hough = ImageUtils::houghLinesP(purp_contour);
    cv::Mat hough_drawing = ImageUtils::drawHoughLinesP(purp_contour_hough, frame);

    std::cout << purp_contour_hough.lines.size() << " lines originally" << std::endl;

    //purp_contour.copyTo(frame);

    for (size_t i = 0; i < purp_contour_hough.lines.size(); i++) {
        cv::line(frame, cv::Point(purp_contour_hough.lines[i][0], purp_contour_hough.lines[i][1]),
                cv::Point(purp_contour_hough.lines[i][2],purp_contour_hough.lines[i][3]), cv::Scalar(0,0,255));
    }
//    hough_drawing.copyTo(frame);

    std::vector<std::vector<cv::Vec4i>> purp_contour_grouped_lines;
    std::vector<cv::Vec4i> purp_contour_averaged_lines;

    groupPurpleLines(purp_contour_hough.lines,purp_contour_grouped_lines,0.7);
    findAverageOfLines(purp_contour_grouped_lines,purp_contour_averaged_lines);

    for (size_t i = 0; i < purp_contour_averaged_lines.size(); i++) {
        cv::line(frame, cv::Point(purp_contour_averaged_lines[i][0], purp_contour_averaged_lines[i][1]),
                cv::Point(purp_contour_averaged_lines[i][2], purp_contour_averaged_lines[i][3]), cv::Scalar(0,255,0), 3, 8);
    }
    std::cout << purp_contour_averaged_lines.size() << " lines after reduction" << std::endl;

    for (size_t i = 0; i < purp_contour_averaged_lines.size(); i++) {
        cv::circle(frame,cv::Point((purp_contour_averaged_lines[i][0]+purp_contour_averaged_lines[i][2])/2,
                (purp_contour_averaged_lines[i][1]+purp_contour_averaged_lines[i][3])/2),5,cv::Scalar(255,0,0),3,8);
    }

    cv::waitKey(1000);

/*

    // probablistic hough line detection

    cv::cvtColor(purp_canny, purp_canny_color, CV_GRAY2BGR);
    cv::cvtColor(purp_contour, purp_contour_color, CV_GRAY2BGR);
//    purp_contour.copyTo(frame);

    cv::Mat dst (70,70,CV_8UC3,cv::Scalar(1,3));
    std::vector<cv::Vec4i> purp_lines,purp_lines_unique;
    cv::HoughLinesP(purp_contour, purp_lines, HOUGH_P_RHO,
                    HOUGH_P_THETA, HOUGH_P_THRESHOLD,
                    HOUGH_P_MIN_LINE_LENGTH, HOUGH_P_MAX_LINE_GAP);

    std::sort(purp_lines.begin(),purp_lines.end(),vecObjectWow);
    purp_lines_unique = removeSimilarLines(purp_lines,0.4);

    for (size_t i = 0; i < purp_lines.size(); i++ ) {
        cv::line(frame, cv::Point(purp_lines[i][0], purp_lines[i][1]),
                cv::Point(purp_lines[i][2], purp_lines[i][3]), cv::Scalar(0,0,255), 3, 8);
    }
    for (size_t i = 0; i < purp_lines_unique.size(); i++ ) {
        cv::line(frame, cv::Point(purp_lines_unique[i][0], purp_lines_unique[i][1]),
                cv::Point(purp_lines_unique[i][2], purp_lines_unique[i][3]), cv::Scalar(0,255,0), 3, 8);
    }

//    cv::namedWindow("lines",CV_WINDOW_NORMAL);
//    cv::imshow("lines",dst);
//    cv::waitKey(100);

    //local_map_mat.copyTo(frame);
*/
    //std::cout << "number of lines: " << purp_lines.size() << std::endl;
    //std::cout << "number of unique lines: " << purp_lines_unique.size() << std::endl;
}


void detectPurpleLineTest(cv::Mat& frame, GridMap& local_map) {

    cv::Mat purp_canny_color;
    cv::Mat purp_contour_color;

    // get purple line
    cv::Mat im_purp = ColorDetection::detectColor(frame, ColorDetection::COLOR_LINE_PURPLE);
    // do a canny edge detection
    cv::Mat purp_canny = ImageUtils::cannyEdge(im_purp);

    ImageUtils::ContourData purp_contour_data = ImageUtils::getContours(im_purp);
    ImageUtils::cleanContour(purp_contour_data, FEATURE_AREA_THRESH_PURP_LINE);
    cv::Mat purp_contour = ImageUtils::drawContoursFilled(purp_contour_data, im_purp);

    goThroughPixels(purp_contour, local_map);

    cv::Mat local_map_mat = local_map.cvtImage();

    ImageUtils::HoughDataNonP local_map_hough = ImageUtils::houghLinesNonP(local_map_mat);
    cv::Mat hough_drawing = ImageUtils::drawHoughLinesNonP(local_map_hough,local_map_mat);
    cv::namedWindow("w",CV_WINDOW_NORMAL);
    cv::imshow("w",hough_drawing);

    ImageUtils::HoughDataNonP purp_contour_hough = ImageUtils::houghLinesNonP(purp_contour);
    cv::Mat hough_contour_drawing = ImageUtils::drawHoughLinesNonP(purp_contour_hough, purp_contour);
    cv::namedWindow("r",CV_WINDOW_NORMAL);
    cv::imshow("r",hough_contour_drawing);

    cv::waitKey(100);



/*

    // probablistic hough line detection

    cv::cvtColor(purp_canny, purp_canny_color, CV_GRAY2BGR);
    cv::cvtColor(purp_contour, purp_contour_color, CV_GRAY2BGR);
//    purp_contour.copyTo(frame);

    cv::Mat dst (70,70,CV_8UC3,cv::Scalar(1,3));
    std::vector<cv::Vec4i> purp_lines,purp_lines_unique;
    cv::HoughLinesP(purp_contour, purp_lines, HOUGH_P_RHO,
                    HOUGH_P_THETA, HOUGH_P_THRESHOLD,
                    HOUGH_P_MIN_LINE_LENGTH, HOUGH_P_MAX_LINE_GAP);

    std::sort(purp_lines.begin(),purp_lines.end(),vecObjectWow);
    purp_lines_unique = removeSimilarLines(purp_lines,0.4);

    for (size_t i = 0; i < purp_lines.size(); i++ ) {
        cv::line(frame, cv::Point(purp_lines[i][0], purp_lines[i][1]),
                cv::Point(purp_lines[i][2], purp_lines[i][3]), cv::Scalar(0,0,255), 3, 8);
    }
    for (size_t i = 0; i < purp_lines_unique.size(); i++ ) {
        cv::line(frame, cv::Point(purp_lines_unique[i][0], purp_lines_unique[i][1]),
                cv::Point(purp_lines_unique[i][2], purp_lines_unique[i][3]), cv::Scalar(0,255,0), 3, 8);
    }

//    cv::namedWindow("lines",CV_WINDOW_NORMAL);
//    cv::imshow("lines",dst);
//    cv::waitKey(100);

    //local_map_mat.copyTo(frame);
*/
    //std::cout << "number of lines: " << purp_lines.size() << std::endl;
    //std::cout << "number of unique lines: " << purp_lines_unique.size() << std::endl;
}

//double findCrossProduct(cv::Vec4i v1, cv::Vec4i v2) {
//    double dx1, dx2, dy1, dy2, norm1, norm2, mag;
//    dx1 = fabs(v1[2]-v1[0]);
//    dy1 = fabs(v1[3]-v1[1]);
//    norm1 = sqrt(pow(dx1,2.0)+pow(dy1,2.0));
//    dx1 /= norm1;
//    dy1 /= norm1;
//    dx2 = fabs(v2[2]-v2[0]);
//    dy2 = fabs(v2[3]-v2[1]);
//    norm2 = sqrt(pow(dx2,2.0)+pow(dy2,2.0));
//    dx2 /= norm2;
//    dy2 /= norm2;
//    mag = fabs(dx1*dy2 - dy1*dx2);
//    return mag;
//}

double findCrossProduct(cv::Vec4i v1, cv::Vec4i v2) {
    double dx1, dx2, dy1, dy2, norm1, norm2, mag;
    dx1 = fabs(v1[2]-v1[0]);
    dy1 = fabs(v1[3]-v1[1]);
    norm1 = sqrt(pow(dx1,2.0)+pow(dy1,2.0));
    //dx1 /= norm1;
    //dy1 /= norm1;
    dx2 = fabs(v2[2]-v2[0]);
    dy2 = fabs(v2[3]-v2[1]);
    norm2 = sqrt(pow(dx2,2.0)+pow(dy2,2.0));
    //dx2 /= norm2;
    //dy2 /= norm2;
    //calc angle between them
    mag = fabs(dx1*dy2 - dy1*dx2);
    mag /= norm1;
    mag /= norm2;
    if (mag >= 1 || isnan(mag) || mag <= 0.01) {
        mag = 0.0;
    }
    std::cout << mag << std::endl;
    return mag;
}

double findLength(cv::Vec4i v1) {
    double dx1, dy1, mag;
    dx1 = fabs(v1[2]-v1[0]);
    dy1 = fabs(v1[3]-v1[1]);
    mag = sqrt(pow(dx1,2)+pow(dy1,2));
    return mag;
}

std::vector<cv::Vec4i> removeSimilarLines(std::vector<cv::Vec4i> lines, double threshold) {
    double crossProduct;
    cv::Vec4i currentLine;
    std::vector<cv::Vec4i> unsimilarLines;
    bool isUnsimilar;
    std::vector<cv::Vec4i>::iterator maxIt;
    double maxLength = 0;
    double tempLength;
    for (std::vector<cv::Vec4i>::iterator it=lines.begin(); it!=lines.end(); it++) {
        tempLength = findLength(*it);
        if (tempLength > maxLength) {
            maxLength = tempLength;
            maxIt = it;
        }
    }
    if (lines.size() > 0) {
        unsimilarLines.push_back(*maxIt);
        lines.erase(maxIt);
    }
    for (int i = 0; i < lines.size(); i++) {
        isUnsimilar = true;
        for (int j = 0; j < unsimilarLines.size(); j++) {
            crossProduct = findCrossProduct(lines[i],unsimilarLines[j]);
            std::cout << "cp: " << crossProduct << std::endl;
            if (crossProduct < threshold) {
                isUnsimilar = false;
            }
        }
        if (isUnsimilar) {
            unsimilarLines.push_back(lines[i]);
        }
    }
    return unsimilarLines;
}

    // args= image, lines, rho, theta, threshold, minlinelength, maxlinegap

    // do a hough line fit

    // check line properties


}
