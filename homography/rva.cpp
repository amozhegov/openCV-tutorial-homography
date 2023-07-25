#include <iostream>
#include <opencv2/opencv.hpp>

#include "rva.h"

// CREATE YOUR FUNCTIONS HERE
cv::Mat rva_compute_homography(std::vector<cv::Point2f> points_image1, std::vector<cv::Point2f> points_image2) {
    // Check if the input vectors have exactly 4 points
    if (points_image1.size() != 4 || points_image2.size() != 4) {
        std::cerr << "Error: Both input point vectors must have exactly 4 points." << std::endl;
        exit(1);
    }

    // Compute homography using OpenCV's findHomography function
    cv::Mat homography = cv::findHomography(points_image1, points_image2);

    // Return the resulting homography matrix
    return homography;
}

void rva_draw_contour(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color, int thickness) {
    // Create a vector of points to represent the polygon enclosing the marked region
    std::vector<std::vector<cv::Point>> contours;
    contours.push_back(std::vector<cv::Point>(points.begin(), points.end()));

    // Draw the polygon on the input image
    cv::drawContours(image, contours, 0, color, thickness);

    // Display the image with the polygon drawn on it
    cv::imshow("Input Image", image);
}

void rva_deform_image(const cv::Mat& im_input, cv::Mat& im_output, cv::Mat homography)
{
    // Get the transformed corners
    std::vector<cv::Point2f> corners(4);
    corners[0] = cv::Point2f(0, 0);
    corners[1] = cv::Point2f(im_input.cols, 0);
    corners[2] = cv::Point2f(im_input.cols, im_input.rows);
    corners[3] = cv::Point2f(0, im_input.rows);
    std::vector<cv::Point2f> corners_transformed(4);
    cv::perspectiveTransform(corners, corners_transformed, homography);

    // Debugging: print out the corners and transformed corners
    std::cout << "Corners: " << corners << std::endl;
    std::cout << "Transformed corners: " << corners_transformed << std::endl;

    // Find the bounding box
    cv::Rect bbox = cv::boundingRect(corners_transformed);

    // Debugging: print out the bounding box
    std::cout << "Bounding box: " << bbox << std::endl;

    // Warp the input image
    cv::warpPerspective(im_input, im_output, homography, im_output.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());

    // Debugging: print out the output image size
    std::cout << "Output image size: " << im_output.size() << std::endl;

//    im_output = im_output(bbox);
    // Show the output image
    cv::imshow("Output Image", im_output);
}
