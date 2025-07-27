#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <sstream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <video_file>" << std::endl;
        return 1;
    }

    std::string video_path = argv[1];

    // Open the video file
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open video file: " << video_path << std::endl;
        return 1;
    }

    // Create an output directory
    std::string output_dir = "frames";
    std::filesystem::create_directory(output_dir);

    int frame_number = 0;
    cv::Mat frame;
    while (cap.read(frame))
    {
        std::ostringstream filename;
        filename << output_dir << "/frame_" << std::setfill('0') << std::setw(5) << frame_number << ".jpg";

        // Save the frame
        if (!cv::imwrite(filename.str(), frame))
        {
            std::cerr << "Error: Failed to write " << filename.str() << std::endl;
            break;
        }

        std::cout << "Saved " << filename.str() << std::endl;
        frame_number++;
    }

    std::cout << "Done! " << frame_number << " frames extracted." << std::endl;
    return 0;
}
