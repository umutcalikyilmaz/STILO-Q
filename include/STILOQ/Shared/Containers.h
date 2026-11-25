#pragma once
#include <span>
#include <vector>
namespace STILOQ
{
    template <typename T>
    class Matrix
    {
    public:

        Matrix()
            : rowCount_(0),
              columnCount_(0) { }

        Matrix(const Matrix& original)
            : rowCount_(original.rowCount_),
              columnCount_(original.columnCount_),
              data_(original.data_) { }

        Matrix(int rowCount, int columnCount)
            : rowCount_(rowCount),
              columnCount_(columnCount),
              data_(rowCount * columnCount) { }

        std::span<T> getRow(int row)
        {
            return std::span<T>(data_).subspan(row * columnCount_, columnCount_);
        }

        std::span<T> operator[](int row)
        {
            return std::span<T>(data_).subspan(row * columnCount_, columnCount_);
        }

        T getElement(int row, int column)
        {
            return data_[row * columnCount_ + column];
        }

    private:

        std::vector<T> data_;
        std::size_t rowCount_;
        std::size_t columnCount_;
    };

    template <typename T>
    class Tensor
    {
    public:
    
        Tensor()
            : totalDepth_(0),
              totalHeight_(0),
              totalLength_(0) { }

        Tensor(const Tensor& original)
            : totalDepth_(original.totalDepth_),
              totalHeight_(original.totalHeight_),
              totalLength_(original.totalLength_),
              data_(original.data_) { }

        Tensor(int totalDepth, int totalHeight, int totalLength)
            : totalDepth_(totalDepth),
              totalHeight_(totalHeight),  
              totalLength_(totalLength),
              data_(totalDepth * totalHeight * totalLength) { }

        std::span<T> getRow(int depth, int height)
        {
            return std::span<T>(data_).subspan(depth * totalHeight_ * totalLength_ + height * totalLength_, totalLength_);
        }

        T getElement(int depth, int height, int length)
        {
            return data_[depth * totalHeight_ * totalLength_ + height * totalLength_ + length];
        }

    private:

        std::vector<T> data_;
        std::size_t totalDepth_;
        std::size_t totalHeight_;
        std::size_t totalLength_;        
    };
}