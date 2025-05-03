// Vector.h

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>

template<typename T> 
class Vector {
    /*currently debating on making some of these function 
    virtual bc the instructions say to make them virtual 
    if we think we need to but at the same time, it does 
    not seem that we are ever making a derived class*/
    protected:
        T* data;
        size_t size;

    public:
        //all constructors
        //default
        Vector(): data(nullptr), size(0){}

        //parameterized constructor
        Vector(int s): size(s){
            if(s<0){ 
                throw std::invalid_argument("Size must be a non-negative number");
            }
            data = new T[size]();
        }

        //copy constructor
        Vector(const Vector& other): size(other.size){
            data = new T[size]; 
            for(size_t i = 0; i<size; i++){
                data[i] = other.data[i];
            }
        }

        //destructor
        ~Vector(){
            delete[] data;
        }

        //all methods
        //get size
        int getsize()const{
            return size;
        }

        //operators
        //assignment
        Vector& operator=(const Vector& other){
            if(this != &other){
                delete[] data;
                size = other.size;
                data = new T[size];
                for(size_t i = 0; i < size; i++){
                    data[i] = other.data[i];
                }
            }
            return *this;
        }

        //output stream
        friend std::ostream& operator<<(std::ostream& out, const Vector& vector){
            out << "[";
            for(size_t i = 0; i < vector.size; i++){
                out << vector.data[i];
                if(i != vector.size-1)
                    out <<", ";
            }
            out <<"]";
            return out;
        }
        //input stream
        friend std ::istream& operator>>(std::istream& in,  Vector& vector){
            for(size_t i = 0; i <vector.size; i++){
                in >> vector.data[i];
            }
            return in;
        }

        //addition
        Vector operator+(const Vector& other){
            if(size != other.size)
                throw std::invalid_argument("Vector sizes must match (+)");
            Vector result(size);
            for(size_t i = 0; i < size; i++){
                result[i] = data[i] + other.data[i];
            }
            return result;
        }
        //subtraction
        Vector operator-(const Vector& other){
            if(size != other.size)
                throw std::invalid_argument("Vector sizes must match (-)");
            Vector result(size);
            for(size_t i = 0; i < size; i++){
                result[i] = data[i] - other.data[i];
            }
            return result;
        }
        //multiplication
        Vector operator*(const Vector& other){
            if(size != other.size)
                throw std::invalid_argument("Vector sizes must match (*)");
            Vector result(size);
            for(size_t i = 0; i < size; i++){
                result[i] = data[i] * other.data[i];
            }
            return result;
        }
        
        //subscript operator[]
        //const function for when x = arr[1]
        const T& operator[](int index) const{
            if(index < 0 || index >=size)
                throw std::out_of_range("Index is out of range");
            return data[index];
        }
        //non const function for when arr[1] = x
        T& operator[](int index) {
            if(index < 0 || index >= size){
                throw std::out_of_range("Vector index " + std::to_string(index) + " out of range (size=" + std::to_string(size) + ")");
            }
            return data[index];
        }
};      

#endif // VECTOR_H

