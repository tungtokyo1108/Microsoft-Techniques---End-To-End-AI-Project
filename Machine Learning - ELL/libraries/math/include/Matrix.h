/**
 * Microsoft - Modern Information Technology
 * https://github.com/tungtokyo1108/ELL/blob/master/libraries/math/include/Matrix.h
 *
 *  Created on: Feb 07, 2019
 *  Student (MIG Virtual Developer): Tung Dang
 */

#pragma once

#include "Vector.h"
#include <utilities/include/IArchivable.h>
#include <cstddef>
#include <limits>

namespace ell
{
    namespace math
    {
        template <typename ElementType>
        class CommonMatrixBase
        {
            public: 
                size_t NumRows() const {return _numRows;}
                size_t NumColumns() const {return _numColumns;}
                size_t Size() const {return NumRows() * NumColumns();}
                size_t GetIncrement() const {return _increment;}

            protected: 
                CommonMatrixBase(const ElementType* pData, size_t numRows, size_t numColumns, size_t increment);
                void Swap(CommonMatrixBase<ElementType>& other);
                size_t _numRows;
                size_t _numColumns;
                size_t _increment;
        };

        enum class MatrixLayout
        {
            columnMajor,
            rowMajor
        };

        enum class MatrixTranspose
        {
            noTranspose,
            transpose
        };

        template <MatrixLayout>
        struct TransposeMatrixLayout;

        template <>
        struct TransposeMatrixLayout<MatrixLayout::columnMajor> 
        {
            static constexpr MatrixLayout value = MatrixLayout::rowMajor;
        };
        
        template <>
        struct TransposeMatrixLayout<MatrixLayout::rowMajor>
        {
            static constexpr MatrixLayout value = MatrixLayout::columnMajor;
        };

        template <typename ElementType, MatrixLayout layout>
        class MatrixBase;

        template <typename ElementType>
        class MatrixBase<ElementType, MatrixLayout::columnMajor> : public CommonMatrixBase<ElementType>
        {
            public: 
                size_t GetMinorSize() const 
                {
                    return this->NumColumns();
                }

                size_t GetMajorSize() const 
                {
                    return this->NumRows();
                }

                size_t GetRowIncrement() const 
                {
                    return 1;
                }

                size_t GetColumnIncrement() const 
                {
                    return this->GetIncrement();
                }

            protected:
                MatrixBase(const ElementType* pData, size_t numRows, size_t numColumns);
                MatrixBase(const ElementType* pData, size_t numRows. size_t numColumns, size_t increment);
                void Swap(MatrixBase<ElementType, MatrixLayout::columnMajor>& other);
                static constexpr VectorOrientation _intervalOrientation = VectorOrientation::column;
        };
        
        template <typename ElementType, MatrixLayout layout>
        class ConstMatrixReference : public MatrixBase<ElementType, layout>
        {
            public:
                ConstMatrixReference(const ElementType* pData, size_t numRows, size_t numColumns);
                ConstMatrixReference(const ElementType* pData, size_t numRows, size_t numColumns, size_t increment);
                MatrixLayout GetLayout() const 
                {
                    return layout;
                }
                inline ElementType operator()(size_t rowIndex, size_t columnIndex) const;
                const ElementType* GetConstDataPointer() const {return this->_pData;}
                std::vector<ElementType> ToArray() const;
                void Swap(ConstMatrixReference<ElementType, layout>& other);
                bool IsContigous() const 
                {
                    return this->GetIncrement() == this->GetMajorSize();
                }
                bool IsEqual(ConstMatrixReference<ElementType, layout> other, ElementType tolerance = static_cast<ElementType>(1.0e-8)) const;
                bool IsEqual(ConstMatrixReference<ElementType, TransposeMatrixLayout<layout>::value> other, 
                             ElementType tolerance = static_cast<ElementType>(1.0e-8)) const;
                bool operator==(const ConstMatrixReference<ElementType, layout>& other) const;
                bool operator==(const ConstMatrixReference<ElementType, TransposeMatrixLayout<layout>::value>& other) const;

                template <MatrixLayout otherLayout>
                bool operator!=(const ConstMatrixReference<ElementType, otherLayout>& other);

                ConstMatrixReference<ElementType, layout> GetConstReference() const {return *this;}
                ConstMatrixReference<ElementType, layout> GetSubMatrix(size_t firstRow, size_t firstColumn, 
                                                            size_t numRows, size_t numColumns) const;
                ConstColumnVectorReference<ElementType> GetColumns(size_t index) const;
                ConstRowVectorReference<ElementType> GetRow(size_t index) const;
                ConstColumnVectorReference<ElementType> GetDiagonal() const;
                
                auto GetMajorVector(size_t index) const
                {
                    return ConstVectorReference<ElementType, MatrixBase<ElementType, layout>::_intervalOrientation>(this->GetMajorVectorBegin(index), this->GetMajorSize(), 1);
                }
                auto Transpose() const -> ConstMatrixReference<ElementType, TransposeMatrixLayout<layout>::value>;
                ConstColumnVectorReference<ElementType> ReferenceAsVector() const;

            protected:
                friend class ConstMatrixReference<ElementType, TransposeMatrixLayout<layout>::value>;
                const ElementType* GetMajorVectorBegin(size_t index) const; 
        };

        template <typename ElementType, MatrixLayout layout>
        class MatrixReference : public ConstMatrixReference<ElementType, layout>
        {
            public:
                MatrixReference(ElementType* pData, size_t numRows, size_t numColumns, size_t increment);
                MatrixReference(ElementType* pData, size_t numRows, size_t numColumns);
                using ConstMatrixReference<ElementType, layout>::operator();
                inline ElementType& operator()(size_t rowIndex, size_t columnsIndex);
                ElementType* GetDataPointer() 
                {
                    return const_cast<ElementType*>(this->_pData);
                }
                using ConstMatrixReference<ElementType, layout>::IsContigous;
                void CopyFrom(ConstMatrixReference<ElementType, layout> other);
                void CopyFrom(ConstMatrixReference<ElementType, TransposeMatrixLayout<layout>::value> other);
                void Swap(MatrixReference<ElementType, layout>& other);
                void Reset() {Fill(0);}
                void Fill(ElementType value);
                
                template <typename GeneratorType>
                void Generate(GeneratorType generator);

                using ConstMatrixReference<ElementType, layout>::GetSubMatrix;
                using ConstMatrixReference<ElementType, layout>::GetColumns;
                using ConstMatrixReference<ElementType, layout>::GetRow;
                using ConstMatrixReference<ElementType, layout>::GetDiagonal;
                using ConstMatrixReference<ElementType, layout>::GetMajorVector;
                using ConstMatrixReference<ElementType, layout>::Transpose;
                using ConstMatrixReference<ElementType, layout>::ReferenceAsVector;

                MatrixReference<ElementType, layout> GetReference() 
                {
                    return *this;
                }

                auto Transpose() -> MatrixReference<ElementType, TransposeMatrixLayout<layout>::value>;
                MatrixReference<ElementType, layout> GetSubMatrix(size_t firstRow, size_t firstColumn, size_t numRows, size_t numColumns);
                ColumnVectorReference<ElementType> GetColumn(size_t index);
                RowVectorReference<ElementType> GetRow(size_t index);
                ColumnVectorReference<ElementType> GetDiagonal();
                ColumnVectorReference<ElementType> ReferenceAsVector();
                auto GetMajorVector(size_t index)
                {
                    return VectorReference<ElementType, MatrixBase<ElementType, layout>::_internalOrientation>(this->GetMajorVectorBegin(index), this->GetMajorSize(),1);
                }
            protected:
                friend MatrixReference<ElementType, TransposeMatrixLayout<layout>::value>;
        };

        template <typename ElementType, MatrixLayout layout>
        class Matrix : public MatrixReference<ElementType, layout>
        {
            public: 
                Matrix(size_t numRows, size_t numColumns);
                Matrix(std::initializer_list<std::initializer_list<ElementType>> list);
                Matrix(size_t numRows, size_t numColumns, const std::vector<ElementType>& data);
                Matrix(size_t numRows, size_t numColumns, std::vector<ElementType>&& data);
                Matrix(Matrix<ElementType, layout>&& other);
                Matrix(const Matrix<ElementType, layout>& other);
                Matrix(ConstMatrixReference<ElementType, TransposeMatrixLayout<layout>::value> other);
                Matrix<ElementType, layout>& operator=(Matrix<ElementType, layout> other);
                std::vector<ElementType> TaArray() const 
                {
                    return _data;
                }
                void Swap(Matrix<ElementType, layout>& other);
            
            private:
                std::vector<ElementType> _data;
        };

        class MatrixArchiver
        {
            public: 
                template<typename ElementType, MatrixLayout layout>
                static void Write(const Matrix<ElementType, layout>& matrix, const std::string& name, utilities::Archiver& archiver);

                template<typename ElementType, MatrixLayout layout>
                static void Read(Matrix<ElementType, layout>& matrix, const std::string& name, utilities::Unarchiver& archiver);

            private:
                static std::string GetRowsName(const std::string& name) {return name + "_rows";}
                static std::string GetColumnsName(const std::string& name) {return name + "_columns";}
                static std::string GetValuesName(const std::string& name) {return name + "_values";}
        };

        template <typename ElementType>
        using ColumnMatrix = Matrix<ElementType, MatrixLayout::columnMajor>;

        template <typename ElementType>
        using ColumnMatrixReference = MatrixReference<ElementType, MatrixLayout::columnMajor>;

        template <typename ElementType>
        using RowMatrix = Matrix<ElementType, MatrixLayout::rowMajor>;

        template <typename ElementType>
        using RowMatrixReference = MatrixReference<ElementType, MatrixLayout::rowMajor>;

        template <typename ElementType>
        using ConstRowMatrixReference = ConstMatrixReference<ElementType, MatrixLayout::rowMajor>;
    }
} 
