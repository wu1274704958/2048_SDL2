//
// Created by Administrator on 17-11-13.
//
#pragma once
#include <iostream>
//#include "drawChar.h"
#include <windows.h>
//#include "Console.h"


template <typename T>
class Mat {
protected:
    T* data_;
    int row_;
    int col_;
public:
    Mat(int row,int col,T&& a) : row_(row) ,col_(col)
    {
        data_ = new T[row*col];
        for(int i = 0;i < row * col;i++)
        {
            data_[i] = a;
        }
    }
	Mat()
	{
		data_ = NULL;
		row_ = 0;
		col_ = 0;
	}
    Mat(const Mat& m)
    {
        if(m.data_ != nullptr)
        {
            row_ = m.row_;
            col_ = m.col_;
            data_ = new T[m.row_*m.col_];
            setData(m.data_,m.row_,m.col_);
        }
    }
    Mat(Mat &&m)
    {
        if(m.data_ != nullptr)
        {
            row_ = m.row_;
            col_ = m.col_;
            data_ = m.data_;
            m.data_ = nullptr;
            m.row_ = m.col_ = 0;
        }
    }

	Mat &operator=(const Mat &m)
	{
		if (!(m.data_))
		{
			return *this;
		}
		if (data_)
		{
			if (row_ * col_ != m.row_ * m.col_)
			{
				delete[] data_;
				data_ = new T[m.col_ * m.row_];
			}
			
		}
		else {
			data_ = new T[m.col_ * m.row_];
		}
		memcpy(data_, m.data_, sizeof(T) * m.col_ * m.row_);
		row_ = m.row_;
		col_ = m.col_;

		return *this;
	}

	Mat &operator=(Mat &&m)
	{
		if (!(m.data_))
		{
			return *this;
		}
		if (data_)
		{
			delete[] data_;
		}
		
		data_ = m.data_;
		col_ = m.col_;
		row_ = m.row_;

		m.data_ = nullptr;
		m.col_ = 0;
		m.row_ = 0;

		return *this;
	}

	virtual ~Mat()
    {
        if(data_ != nullptr)
        {
            delete[] data_;
        }
    }

	T& operator[](int i)
	{
		return data_[i];
	}

    virtual void print() const
    {
        for(int i = 0; i < row_ * col_;i++)
        {
            std::cout << data_[i] <<" " ;
            if((i+1) % row_ == 0 && i+1 != row_ * col_)
                std::cout<<std::endl;
        }
    }
    void setData(const T *d,int row,int col)
    {
        int len = row * col > row_ * col_ ?  row_ * col_ : row * col;
        for(int i = 0;i < len;i++)
        {
            data_[i] = d[i];
        }
    }
	const T* getData()
	{
		return data_;
	}
	virtual void cover(const Mat& mat,int x,int y,int row,int col,int px,int py)
    {
		while (px < 0)
		{
			px++;
			x++;
		}
		while (py < 0)
		{
			py++;
			y++;
		}
		for (int i = px,m = x; i < row_ && m < row; i++,m++)
		{
			for (int j = py,n = y; j < col_ && n < col; j++,n++)
			{
				data_[i + (j * row_)] = mat.data_[m + (n * mat.row_)];
			}
		}
    }

    void clear(T &&a)
    {
        for(int i = 0;i < row_ * col_;i++)
        {
            data_[i] = a;
        }
    }
	
};


class MatChar : public Mat<char>
{

public:
	MatChar(int row, int col, char&& v) : Mat<char>(row, col, std::forward<char>(v))
	{

	}
	MatChar(const Mat<char> &m) : Mat<char>(m)
	{

	}
	MatChar( Mat<char> &&m) : Mat<char>(m)
	{

	}
	MatChar() : Mat<char>()
	{
	}
	virtual ~MatChar()
	{
	}
	
	/*virtual void print() const
	{
		for (int i = 0; i < row_; i++)
		{
			for (int j = 0; j < col_; j++)
			{
				if (data_[i + (j * row_)] != ' ')
				{
					WWS::Console::gotoxy(i, j);
					std::cout << data_[i + (j * row_)];
				}
			}
		}
	}*/

	/*void drawText(WCHAR a,char unit = '#')
	{
		int size = col_ > row_ ? row_ : col_;
		if (DrawChar::drawChar(data_, size, a,unit) != 0)
		{
			std::cout << "draw char failed" << std::endl;
		}
	}*/

	virtual void cover(const MatChar& mat, int x, int y, int row, int col, int px, int py)
	{
		while (px < 0)
		{
			px++;
			x++;
		}
		while (py < 0)
		{
			py++;
			y++;
		}
		for (int i = px, m = x; i < row_ && m < row; i++, m++)
		{
			for (int j = py, n = y; j < col_ && n < col; j++, n++)
			{
				if (mat.data_[m + (n * mat.row_)] != ' ')
				{
					data_[i + (j * row_)] = mat.data_[m + (n * mat.row_)];
				}
			}
		}
	}

};


