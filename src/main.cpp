#include <iostream>
#include <string>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include "Image.h"

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: ./program <function> <input file 1> <input file 2 (only needed for add, subtract, or dot)> <output directory>" << std::endl;
        return 1;
    }
    std::string function = argv[1];
    std::string input_file_1 = argv[2];
    std::string input_file_2 = argc > 4 ? argv[3] : "";
    std::string output_directory = argv[argc - 1];
    std::cout << "image 1: " << input_file_1 << " image 2: " << input_file_2 << " output directory: " << output_directory << "   function: " << function << std::endl;

    // Load input image 1
    Image input_image_1(input_file_1);

    // Load input image 2 (if applicable)
    Image input_image_2;
    if (!input_file_2.empty() && function != "scale") {
        input_image_2 = Image(input_file_2);
    }

    // Output image
    Image output_image;

    // Perform the specified function
    if (function == "add" && !input_file_2.empty()) {
        output_image = input_image_1 + input_image_2;
    }
    else if (function == "subtract" && !input_file_2.empty()) {
        output_image = input_image_1 - input_image_2;
    }
    else if (function == "dot" && !input_file_2.empty()) {
        output_image = input_image_1 * input_image_2;
    }
    else if (function == "scale") {
        float alpha = 1.0f; // default value
        if (argc > 4) {
            alpha = std::stof(input_file_2);
        }
        int newWidth = static_cast<int>(input_image_1.getWidth() * alpha);
        int newHeight = static_cast<int>(input_image_1.getHeight() * alpha);
        input_image_1.resize(newWidth, newHeight);
        output_image = input_image_1;
    }
    else {
        std::cout << "Invalid function name or insufficient number of arguments" << std::endl;
        return 1;
    }

    // Write output image
    std::filesystem::create_directories(output_directory);
    std::string output_filename = output_directory + "/output.png";
    output_image.save(output_filename);

    return 0;
}

