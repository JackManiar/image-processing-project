// Image.h

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include "Matrix.h"

class Image : public Matrix {
private:
    std::string filePath;
    int numChannels;
    int width;
    int height;

public:
    // Default constructor
    Image();

    explicit Image(const std::string& filePath);

    // Constructor with file path, channels, width, and height
    Image(const std::string& filePath, int numChannels, int width, int height);

    // Copy constructor
    Image(const Image& other);

    // Assignment operator
    Image& operator=(const Image& other);

    // Destructor
    ~Image();

    // Scaling an image
    Image operator*(double scalar) const;

    // Adding two images
    Image operator+(const Image& other) const;

    // Subtracting two images
    Image operator-(const Image& other) const;

    // Multiplying two images
    Image operator*(const Image& other) const;

    // Resize function
    void resize(int newWidth, int newHeight);

    // Get width of the image
    int getWidth() const;

    // Get height of the image
    int getHeight() const;

    // Save image to a file
    void save(const std::string& filePath) const;
};

#endif // IMAGE_H

