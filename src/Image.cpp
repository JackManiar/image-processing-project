// Image.cpp

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

// Default constructor
Image::Image() : Matrix(), filePath(""), numChannels(0), width(0), height(0) {}

Image::Image(const std::string& filePath) {
    // Load the image using stb_image
    int width, height, channels;
    uint8_t* imageData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    // Initialize the Matrix base class with the image data
    //keep each image row in one vector, channels laid out side by side
    //ex 2x2 image: data[0] = {R1, G1, B1, R2, G2, B2}
    //              data[1] = {R3, G3, B3, R4, G4, B4}
    data = Vector<Vector<uint8_t>>(height);
    for(size_t i = 0; i < height; i++){
        data[i] = Vector<uint8_t>(width * numChannels);
        for(size_t j = 0; j < width; j++){
            data[i][j] = imageData[i* width * numChannels + j]; //this is the formula for converting the row vecotr into layers
        }
    }

    // Set the Image properties
    this->filePath = filePath;
    this->numChannels = channels;
    this->width = width;
    this->height = height;

    // Free the loaded image data
    stbi_image_free(imageData);
}

// Constructor with file path, channels, width, and height
Image::Image(const std::string& filePath, int numChannels, int width, int height)
    : Matrix(height, width), filePath(filePath), numChannels(numChannels), width(width), height(height) {}

// Copy constructor
Image::Image(const Image& other)
    : Matrix(other.height, other.width), filePath(other.filePath), numChannels(other.numChannels), width(other.width), height(other.height){}

// Assignment operator
Image& Image::operator=(const Image& other) {
    if(this != &other){
        data = other.data;
        filePath = other.filePath;
        numChannels = other.numChannels;
        width = other.width;
        height = other.height;
    }
    return *this;
}

// Destructor
Image::~Image() {
    //YOUR CODE HERE//I don't think we need to have anything here, didn't use "new" anywhere
}

// Scaling an image
Image Image::operator*(double scalar) const {
    if(scalar < 0 || scalar > 1)
        throw std::invalid_argument("Scale must be between 0 and 1");
    
    Image result("", numChannels, width, height);
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width*numChannels; j++){
            int changed = static_cast<int>(data[i][j]) * scalar;
            result[i][j] = (changed > 255) ? static_cast<uint8_t>(255) : static_cast<uint8_t>(changed);
        }
    }
    return result;    
}

// Adding two images
Image Image::operator+(const Image& other) const {
   if( width != other.width || height != other.height || numChannels != other.numChannels)
    throw std::invalid_argument("Image dimensions or number of channels do not match (+)");

    Image result("", numChannels, width, height);
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            int sum = static_cast<int>(data[i][j]) + static_cast<int>(other.data[i][j]);
            result[i][j] = (sum>255) ? static_cast<uint8_t>(255) : static_cast<uint8_t>(sum);
        }
    }
    return result;
}

// Subtracting two images
Image Image::operator-(const Image& other) const {
    if( width != other.width || height != other.height || numChannels != other.numChannels)
    throw std::invalid_argument("Image dimensions or number of channels do not match (+)");

    Image result("", numChannels, width, height);
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            int diff = static_cast<int>(data[i][j]) - static_cast<int>(other.data[i][j]);
            result[i][j] = (diff < 0) ? static_cast<uint8_t>(0) : static_cast<uint8_t>(diff);
        }
    }
    return result;
}

// Multiplying two images
Image Image::operator*(const Image& other) const { //should this be 
    if(numChannels != other.numChannels)
        throw std::invalid_argument("Number of channels must match (*)");
    if(width != other.height)
        throw std::invalid_argument("Image1 width must match Image2 height (*)");
    
    //first split the matrix into three matrcies of each color, then use the * operator from matrix class to do the multiplication, then combine again at the end    
    Image result("", numChannels, other.width, height);
    for(size_t c = 0; c < numChannels; c++){
        Matrix A(height, width); //creating the two matricies
        Matrix B(other.height, other.width);

        //fill a
        for(size_t i = 0; i < height; i++){
            for(size_t j = 0; j < width; j++){
                A[i][j] = data[i][j * numChannels + c];
            }
        }
        //fill b
        for(size_t i = 0; i < other.height; i++){
            for(size_t j = 0; j < other.width; j++){
                B[i][j] = other.data[i][j * numChannels + c];
            }
        }

        Matrix seperate = A * B; //uses the operator overloading for matrix
    
        //combine the matricies again
    for(size_t i = 0; i < result.height; i++)
        for(size_t j = 0; j < result.width; j++)
            result[i][j * numChannels + c] = seperate[i][j];
        
    }
    return result;
}


int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

void Image::save(const std::string& filePath) const {
    // Convert the Matrix data into a 1D array suitable for saving as an image
    std::vector<uint8_t> imageData;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < numChannels; ++k) {
                imageData.push_back(data[i][j * numChannels + k]);
            }
        }
    }
    
    // Save the image data to the specified file using stb_image_write
    stbi_write_png(filePath.c_str(), width, height, numChannels, imageData.data(), width * numChannels);
}

void Image::resize(int newWidth, int newHeight) {
    if(newWidth <= 0 || newHeight <= 0)
        throw std::invalid_argument("Image dimensions must be positive");
    
    // This creates a buffer for original image and fills it with original image data.
    std::vector<uint8_t> originalBuffer(width * height * numChannels);
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width * numChannels; j++) {
            originalBuffer[i * (width * numChannels) + j] = data[i][j];
        }
    }
    
    // This makes buffer for resized image.
    std::vector<uint8_t> resizedBuffer(newWidth * newHeight * numChannels);
    
    // Now stb_image_resize library used for resizing
    int result = stbir_resize_uint8(
        originalBuffer.data(), width, height, width * numChannels,
        resizedBuffer.data(), newWidth, newHeight, newWidth * numChannels,
        numChannels
    );
    
    if(result == 0)
        throw std::runtime_error("Image resize operation failed");
    
    // This adjusts vector<vector<uint_8>> to match resized image's dimensions.
    data = Vector<Vector<uint8_t>>(newHeight);
    for(size_t i = 0; i < newHeight; i++) {
        data[i] = Vector<uint8_t>(newWidth * numChannels);
        for(size_t j = 0; j < newWidth * numChannels; j++) {
            data[i][j] = resizedBuffer[i * (newWidth * numChannels) + j];
        }
    }
    
    width = newWidth;
    height = newHeight;
}

