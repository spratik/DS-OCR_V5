/*
 * tdata.h
 *
 *  Created on: Mar 10, 2016
 *  Author: Soumyadeep Dhar
 */

#ifndef TDATA_H_
#define TDATA_H_

#include <stdio.h>
#include <string.h>

#include "enum.h"
#include "defines.h"
#include "datatype.h"

template < class T>
class TData
{
private:
	int _row;
	int _col;
	int _size;
	T *	_data;

	void sobel_operator_X(TData <int> &temp, TData <int> &out, Rectangle &rect)
	{
		int rIndex, cIndex, offset;
		int Val;
		int val1, val2, val3;

		int stCol = rect.left  + 1;
		int enCol = rect.right - 1;

		int stRow = rect.top    + 1;
		int enRow = rect.bottom - 1;

		T  	*iAddr = this->getdata();
		int	*tAddr = temp.getdata();
		int	*oAddr = out.getdata();

		// To calculate sobel X with the mask
		// -1 -2 -1
		//  0  0  0
		//  1  2  1

		for(rIndex = rect.top; rIndex <= rect.bottom; rIndex ++)
		{
			offset = rIndex * _col;
			tAddr[offset + rect.left] = 0;

			val1 = iAddr[offset + rect.left];
			val2 = iAddr[offset + stCol];

			for(cIndex = stCol; cIndex <= enCol; cIndex++)
			{
				val3 = iAddr[offset + cIndex + 1];

				Val = val1 + (val2 << 1) + val3;
				tAddr[offset + cIndex] = Val;

				val1 = val2;
				val2 = val3;
			}
			tAddr[offset + rect.right] = 0;
		}

		for(cIndex = rect.left; cIndex <= rect.right; cIndex ++)
		{
			offset = rect.top * _col;
			oAddr[offset + cIndex] = 0;

			val1 = tAddr[offset + cIndex];
			val2 = tAddr[offset + _col + cIndex];

			for(rIndex = stRow; rIndex <= enRow; rIndex++)
			{
				offset += _col;
				val3 = tAddr[offset + _col + cIndex];

				Val = val3 - val1;
				Val = ABS(Val);
				oAddr[offset + cIndex] = MIN(Val, _PMAX);

				val1 = val2;
				val2 = val3;

			}
			oAddr[offset + _col + cIndex] = 0;
		}
	}


	void sobel_operator_Y(TData <int> &temp, TData <int> &out, Rectangle &rect)
	{
		int rIndex, cIndex, offset;
		int Val;
		int val1, val2, val3;

		int stCol = rect.left  + 1;
		int enCol = rect.right - 1;

		int stRow = rect.top    + 1;
		int enRow = rect.bottom - 1;

		T   *iAddr = this->getdata();
		int *tAddr = temp.getdata();
		int *oAddr = out.getdata();

		// To calculate sobel Y with the mask
		// -1  0  1
		// -2  0  2
		// -1  0  1

		for(cIndex = rect.left; cIndex <= rect.right; cIndex ++)
		{
			offset = rect.top * _col;
			tAddr[offset + cIndex] = 0;
			val1 = iAddr[offset + cIndex];
			val2 = iAddr[offset + _col + cIndex];

			for(rIndex = stRow; rIndex <= enRow; rIndex++)
			{
				offset += _col;
				val3 = iAddr[offset + _col + cIndex];

				Val = val1 + (val2 << 1) + val3;
				tAddr[offset + cIndex] = Val;

				val1 = val2;
				val2 = val3;
			}
			tAddr[offset + _col + cIndex] = 0;
		}

		for(rIndex = rect.top; rIndex <= rect.bottom; rIndex ++)
		{
			offset = rIndex * _col;
			oAddr[offset + rect.left] = 0;

			val1 = tAddr[offset + rect.left];
			val2 = tAddr[offset + stCol];

			for(cIndex = stCol; cIndex <= enCol; cIndex++)
			{
				val3 = tAddr[offset + cIndex + 1];
				Val = val3 - val1;
				Val = ABS(Val);
				oAddr[offset + cIndex] = MIN(Val, _PMAX);

				val1 = val2;
				val2 = val3;
			}
			oAddr[offset + rect.right] = 0;
		}
	}

public:
	TData() {
		_row  = 0;
		_col  = 0;
		_size = 0;
		_data = NULL;
	}

	~TData() {}

	inline int getrows() {
		return _row;
	}

	inline int getcols() {
		return _col;
	}

	inline int getsize() {
		return _size;
	}

	inline T * getdata() {
		return _data;
	}

	int allocate(int r, int c) {
		_row  = r;
		_col  = c;
		_size = _row * _col;

		_data = new T[_size];
		if(!_data) {
			return TError;
		}

		memset(_data, 0, _size * sizeof(unsigned char));
		return Success;
	}

	int allocate(int r, int c, T val) {
		_row  = r;
		_col  = c;
		_size = _row * _col;

		_data = new T[_size];
		if(!_data) {
			return TError;
		}

		for(int i = 0 ; i < _size; i++){
			_data[i] = val;
		}
		return Success;
	}

	int allocate(int r, int c, unsigned char *data) {
		_row  = r;
		_col  = c;
		_size = _row * _col;

		_data = new T[_size];
		if(!_data) {
			return TError;
		}

		memcpy(_data, data, _size * sizeof(unsigned char));
		return Success;
	}

	int allocate(int r, int c, Rectangle rect, int shiftr, int shiftc, unsigned char *data) {
		int i, j, dataeffcol;

		_row  = r;
		_col  = c;
		_size = _row * _col;

		_data = new T[_size];
		if(!_data) {
			return TError;
		}

		dataeffcol = c - (shiftc << 1);

		/// Clear padded rows
		for (j = 0; j < _col; j++)	{
			for (i = 0; i < rect.top; i++)	{
				_data[i * _col + j] = 0;
			}

			for (i = rect.bottom + 1; i < _row; i++)	{
				_data[i * _col + j] = 0;
			}
		}

		/// Clear padded columns
		for (i = 0; i < _row; i++)	{
			for (j = 0; j < rect.left; j++)	{
				_data[i * _col + j] = 0;
			}

			for (j = rect.right + 1; j < _col; j++)	{
				_data[i * _col + j] = 0;
			}
		}

		/// Set data from buffer
		for (i = rect.top; i <= rect.bottom; i++)	{
			for (j = rect.left; j <= rect.right; j++)	{
				_data[i * _col + j] =
					data[(i - shiftr) * dataeffcol + (j - shiftc)];
			}
		}

		return Success;
	}

	int release() {
		if(_data) {
			delete _data;
			_data = NULL;
		}
		_row  = 0;
		_col  = 0;
		_size = 0;

		return Success;
	}

	template <typename R>
	TData& operator =(TData <R> &src) {

		/// If data not allocated do nothing
		if(!_data) {
			*this = src;
			return *this;
		}

		/// If size mismatch do nothing
		if(this->_size != src.getsize()){
			return *this;
		}

		_row    = src.getrows();
		_col    = src.getcols();
		_size   = _row * _col;
		R *data = src.getdata();

		for (int k = 0; k < _size; k++)	{
			_data[k] = (T)data[k];
		}
		return *this;
	}

	TData& operator =(T val) {
		/// If data not allocated do nothing
		if(!_data) {
			return *this;
		}

		for (int k = 0; k < _size; k++)	{
			_data[k] = val;
		}
		return *this;
	}

	template <typename R>
	TData& operator &=(TData <R> &src) {

		/// If data not allocated do nothing
		if(!_data) {
			return *this;
		}

		/// If size mismatch do nothing
		if(this->_size != src.getsize()){
			return *this;
		}

		_row    = src.getrows();
		_col    = src.getcols();
		_size   = _row * _col;
		R *data = src.getdata();

		for (int k = 0; k < _size; k++)	{
			_data[k] &= (T)data[k];
		}
		return *this;
	}

	TData& operator &=(T val) {
		/// If data not allocated do nothing
		if(!_data) {
			return *this;
		}

		for (int k = 0; k < _size; k++)	{
			_data[k] &= val;
		}
		return *this;
	}

	template <typename R>
	TData& operator |=(TData <R> &src) {

		/// If data not allocated do nothing
		if(!_data) {
			return *this;
		}

		/// If size mismatch do nothing
		if(this->_size != src.getsize()){
			return *this;
		}

		_row    = src.getrows();
		_col    = src.getcols();
		_size   = _row * _col;
		R *data = src.getdata();

		for (int k = 0; k < _size; k++)	{
			_data[k] |= (T)data[k];
		}
		return *this;
	}

	TData& operator |=(T val) {
		/// If data not allocated do nothing
		if(!_data) {
			return *this;
		}

		for (int k = 0; k < _size; k++)	{
			_data[k] |= val;
		}
		return *this;
	}

	template <typename R>
	int mask(TData<R> &src, Rectangle rect) {
		int i, j;
		int srcrow = src.getrows();
		int srccol = src.getcols();
		R *srcdata = src.getdata();

		/// If data not allocated do nothing
		if(!_data) {
			return MemoryNotAllocated;
		}

		/// If rectangle outside source boundary do nothing
		if(rect.top  < 0 || rect.bottom > (srcrow - 1) ||
		   rect.left < 0 || rect.right  > (srccol - 1)) {
			return OutOfBoundError;
		}

		/// If rectangle outside destination boundary do nothing
		if(rect.top  < 0 || rect.bottom > (this->_row - 1) ||
		   rect.left < 0 || rect.right  > (this->_col - 1)) {
			return OutOfBoundError;
		}

		for (i = rect.top; i <= rect.bottom; i++)	{
			for (j = rect.left; j <= rect.right; j++)	{
				_data[i * _col + j] &= srcdata[i * srccol + j];
			}
		}
		return Success;
	}


	template <typename R>
	int mask(R val, Rectangle rect) {
		int i, j;

		/// If data not allocated do nothing
		if(!_data) {
			return MemoryNotAllocated;
		}

		/// If rectangle outside destination boundary do nothing
		if(rect.top  < 0 || rect.bottom > (this->_row - 1) ||
		   rect.left < 0 || rect.right  > (this->_col - 1)) {
			return OutOfBoundError;
		}

		for (i = rect.top; i <= rect.bottom; i++)	{
			for (j = rect.left; j <= rect.right; j++)	{
				_data[i * _col + j] &= val;
			}
		}
		return Success;
	}

	template <typename R>
	int boundedSubtractFrom(TData<R> &src, Rectangle rect) {
		int i, j, value;
		int srcrow = src.getrows();
		int srccol = src.getcols();
		R *srcdata = src.getdata();

		/// If data not allocated do nothing
		if(!_data) {
			return MemoryNotAllocated;
		}

		/// If rectangle outside source boundary do nothing
		if(rect.top  < 0 || rect.bottom > (srcrow - 1) ||
		   rect.left < 0 || rect.right  > (srccol - 1)) {
			return OutOfBoundError;
		}

		/// If rectangle outside destination boundary do nothing
		if(rect.top  < 0 || rect.bottom > (this->_row - 1) ||
		   rect.left < 0 || rect.right  > (this->_col - 1)) {
			return OutOfBoundError;
		}

		for (i = rect.top; i <= rect.bottom; i++)	{
			for (j = rect.left; j <= rect.right; j++)	{
				value = srcdata[i * srccol + j] - _data[i * _col + j];
				_data[i * _col + j] = MAX(0, value);
			}
		}
		return Success;
	}

	template <typename R>
	int boundedSubtractFrom(R val, Rectangle rect) {
		int i, j, value;

		/// If data not allocated do nothing
		if(!_data) {
			return MemoryNotAllocated;
		}

		/// If rectangle outside destination boundary do nothing
		if(rect.top  < 0 || rect.bottom > (this->_row - 1) ||
		   rect.left < 0 || rect.right  > (this->_col - 1)) {
			return OutOfBoundError;
		}

		for (i = rect.top; i <= rect.bottom; i++)	{
			for (j = rect.left; j <= rect.right; j++)	{
				value = val - _data[i * _col + j];
				_data[i * _col + j] = MAX(0, value);
			}
		}
		return Success;
	}

	int writePBM(const char *fname) {

		int offset, i, j;
		FILE *fp = fopen(fname, "w+");
		if(!fp) {
			return FileWriteError;
		}

		fprintf(fp, "P1\n");
		fprintf(fp, "%d %d\n", _col, _row);

		for(i = 0 ; i < _row; i++){
			offset = i * _col;
			for(j = 0 ; j < _col; j++){
				fprintf(fp, "%d ", _data[offset + j]  > 0 ? _PFG : _PBG);
			}
			fprintf(fp, "\n");
		}
		fflush(fp);
		fclose(fp);

		return Success;
	}

	int writePGM(const char *fname) {

		int offset, i, j;
		FILE *fp = fopen(fname, "w+");
		if(!fp) {
			return FileWriteError;
		}

		fprintf(fp, "P2\n");
		fprintf(fp, "%d %d\n", _col, _row);
		fprintf(fp, "255\n");

		for(i = 0 ; i < _row; i++){
			offset = i * _col;
			for(j = 0 ; j < _col; j++){
				fprintf(fp, "%d ",
					_data[offset + j] < 0 ? 0 :
					_data[offset + j] > _PMAX ?
					_PMAX : _data[offset + j]);
			}
			fprintf(fp, "\n");
		}
		fflush(fp);
		fclose(fp);

		return Success;
	}

	int getOtsuValue() {
		Rectangle rect;
		rect.top		= 0;
		rect.bottom 	= _row - 1;
		rect.left 		= 0;
		rect.right  	= _col - 1;

		return getOtsuValue(rect);
	}

	int getOtsuValue(Rectangle rect, const int low = _PBG, const int high = _PMAX) {

		int _low  = BOUND(low,  _PBG, _PMAX);
		int _high = BOUND(high, _PBG, _PMAX);

		int histData[high + 1];
		int i, j, t, value, threshold;
		float sum, sumB, varMax, mB, mF, wB, wF, total, varBetween;


		/// Clear histogram buffer
		for(i = 0; i <= high; i++) {
			histData[i] = 0;
		}

		// Calculate histogram
		total = 0;
		for(i = rect.top; i <= rect.bottom; i++) {
			for(j = rect.left; j <= rect.right; j++) {
			   value = 0xFF & _data[i * _col + j];
			   histData[value] ++;
			   total ++;
			}
		}

		sum = 0;
		for (t = _low; t <= _high; t++) {
			sum += (t * histData[t]);
		}

		sumB = 0;
		wB = 0;
		wF = 0;

		varMax = 0;
		threshold = 0;

		for (t = _low; t <= _high ; t++) {
		   wB += histData[t];               // Weight Background
		   if (wB == 0) {
			   continue;
		   }

		   wF = total - wB;                 // Weight Foreground
		   if (wF == 0) {
			   break;
		   }

		   sumB += (float) (t * histData[t]);

		   mB = wB > 0 ? sumB / wB : 0.0F;            // Mean Background
		   mF = wF > 0 ? (sum - sumB) / wF : 0.0F;    // Mean Foreground

		   // Calculate Between Class Variance
		   varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

		   // Check if new maximum found
		   if (varBetween > varMax) {
		      varMax = varBetween;
		      threshold = t;
		   }
		}

		return threshold;
	}

	template <typename R>
	int getOtsuValue(TData<R> &map, const int size, const int low = _PBG, const int high = _PMAX, bool isHistogram = false) {

		int _low  = BOUND(low,  0, (map.getsize() - 1));
		int _high = BOUND(high, 0, (map.getsize() - 1));

		R *histData = map.getdata();
		int i, t, value, threshold;
		float sum, sumB, varMax, mB, mF, wB, wF, varBetween;


		if(isHistogram != true) {
			/// Clear histogram buffer
			for(i = _low; i <= _high; i++) {
				histData[i] = 0;
			}

			// Calculate histogram
			for(i = 0; i < size; i++) {
			   value = _data[i];
			   histData[value] ++;
			}
		}

		sum = 0;
		for (t = _low; t <= _high; t++) {
			sum += (t * histData[t]);
		}

		sumB = 0;
		wB = 0;
		wF = 0;

		varMax = 0;
		threshold = 0;

		for (t = _low; t <= _high ; t++) {
		   wB += histData[t];               // Weight Background
		   if (wB == 0) {
			   continue;
		   }

		   wF = size - wB;                 // Weight Foreground
		   if (wF == 0) {
			   break;
		   }

		   sumB += (float) (t * histData[t]);

		   mB = wB > 0 ? sumB / wB : 0.0F;            // Mean Background
		   mF = wF > 0 ? (sum - sumB) / wF : 0.0F;    // Mean Foreground

		   // Calculate Between Class Variance
		   varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

		   // Check if new maximum found
		   if (varBetween > varMax) {
		      varMax = varBetween;
		      threshold = t;
		   }
		}

		return threshold;
	}

	void setBinary(Rectangle rect, int type = NormalOtsu) {
		int threshold = _PBG;

		/// Get the threshold value
		switch(type) {
			case LowerOtsu : {
				/// Get the lower threshold value
				threshold = getOtsuValue(rect);
				threshold = getOtsuValue(rect, _PBG, threshold);
				break;
			}

			case UpperOtsu : {
				/// Get the lower threshold value
				threshold = getOtsuValue(rect);
				threshold = getOtsuValue(rect, threshold + 1, _PMAX);
				break;
			}

			default: {
				threshold = getOtsuValue(rect);
				break;
			}
		}

		int i, j;
		/// Update image based on otsu value
		for(i = rect.top; i <= rect.bottom; i++) {
			for(j = rect.left; j <= rect.right; j++) {
			   _data[i * _col + j] = _data[i * _col + j] > threshold ? _PBG : _PFG;
			}
		}
	}

	template <typename R>
	int setBinary(TData<R> &image, Rectangle rect, int type = NormalOtsu) {

		R * data = image.getdata();
		int i, j;
		int threshold = _PBG;

		/// Get the threshold value
		switch(type) {
			case LowerOtsu : {
				/// Get the lower threshold value
				threshold = image.getOtsuValue(rect);
				threshold = image.getOtsuValue(rect, _PBG, threshold);
				break;
			}

			case UpperOtsu : {
				/// Get the lower threshold value
				threshold = image.getOtsuValue(rect);
				threshold = image.getOtsuValue(rect, threshold + 1, _PMAX);
				break;
			}

			default: {
				threshold = image.getOtsuValue(rect);
				break;
			}
		}

		/// Update image based on otsu value
		for(i = rect.top; i <= rect.bottom; i++) {
			for(j = rect.left; j <= rect.right; j++) {
			   _data[i * _col + j] = data[i * _col + j] > threshold ? _PBG : _PFG;
			}
		}
		return Success;
	}

	void setBinary(int threshold, Rectangle rect) {
		int i, j;
		/// Update image based on otsu value
		for(i = rect.top; i <= rect.bottom; i++) {
			for(j = rect.left; j <= rect.right; j++) {
			   _data[i * _col + j] = _data[i * _col + j] > threshold ? _PBG : _PFG;
			}
		}
	}

	template <typename R>
	int setBinary(TData<R> &image, int threshold) {

		R * data = image.getdata();

		for(int i = 0 ; i < _size; i++){
				_data[i] = data[i] > threshold ? _PBG : _PFG;
		}
		return Success;
	}

	void setBinaryReverse(int type = NormalOtsu) {
		Rectangle imageRect;
		imageRect.top		= 0;
		imageRect.bottom 	= _row - 1;
		imageRect.left 		= 0;
		imageRect.right  	= _col - 1;

		setBinaryReverse(imageRect, type);
	}

	void setBinaryReverse(Rectangle rect, int type = NormalOtsu) {
		int i, j;
		int threshold = _PBG;

		/// Get the threshold value
		switch(type) {
			case LowerOtsu : {
				/// Get the lower threshold value
				threshold = getOtsuValue(rect);
				threshold = getOtsuValue(rect, _PBG, threshold);
				break;
			}

			case UpperOtsu : {
				/// Get the lower threshold value
				threshold = getOtsuValue(rect);
				threshold = getOtsuValue(rect, threshold + 1, _PMAX);
				break;
			}

			default: {
				threshold = getOtsuValue(rect);
				break;
			}
		}

		/// Update image based on otsu value
		for(i = rect.top; i <= rect.bottom; i++) {
			for(j = rect.left; j <= rect.right; j++) {
			   _data[i * _col + j] = _data[i * _col + j] <= threshold ? _PBG : _PFG;
			}
		}
	}

	template <typename R>
	int setBinaryReverse(TData<R> &image, Rectangle rect, int type = NormalOtsu) {
		int i, j;
		R * data = image.getdata();
		int threshold = _PBG;

		/// Get the threshold value
		switch(type) {
			case LowerOtsu : {
				/// Get the lower threshold value
				threshold = image.getOtsuValue(rect);
				threshold = image.getOtsuValue(rect, _PBG, threshold);
				break;
			}

			case UpperOtsu : {
				/// Get the lower threshold value
				threshold = image.getOtsuValue(rect);
				threshold = image.getOtsuValue(rect, threshold + 1, _PMAX);
				break;
			}

			default: {
				threshold = image.getOtsuValue(rect);
				break;
			}
		}

		/// Update image based on otsu value
		for(i = rect.top; i <= rect.bottom; i++) {
			for(j = rect.left; j <= rect.right; j++) {
			   _data[i * _col + j] = (data[i * _col + j] <= threshold ? _PBG : _PFG);
			}
		}

		return Success;
	}

	void setBinaryReverse(int threshold, Rectangle rect) {
		int i, j;
		/// Update image based on otsu value
		for(i = rect.top; i <= rect.bottom; i++) {
			for(j = rect.left; j <= rect.right; j++) {
			   _data[i * _col + j] = _data[i * _col + j] <= threshold ? _PBG : _PFG;
			}
		}
	}

	template <typename R>
	int setBinaryReverse(TData<R> &image, int threshold) {

		R * data = image.getdata();

		for(int i = 0 ; i < _size; i++){
				_data[i] = data[i] <= threshold ? _PBG : _PFG;
		}
		return Success;
	}


	void thiningZhangSuen(const int _fg, const int _bg,
				 const int x1, const int y1, const int x2, const int y2)
	{
		Rectangle rect;
		rect.top 	= MAX(x1, 1);
		rect.bottom = MIN(x2, (_row - 2));
		rect.left 	= MAX(y1, 1);
		rect.right  = MIN(y2, (_col - 2));

		thiningZhangSuen(_fg, _bg, rect);
	}

	void thiningZhangSuen(const int _fg, const int _bg, Rectangle rect)
	{
		int	p2, p3, p4, p5, p6, p7, p8, p9;
		int	cflag, tflag;
		int	r, c;
		int	totalNumOfFgNeighbour, transitionCount;
		int	condition1, condition2;
		int	flagedValue = 2;
		int	offset;


		cflag = 1;
		tflag = 0;

		while(cflag == 1)
		{
			cflag = 0;
			tflag = 1 - tflag;

			for(r = rect.top + 1; r < rect.bottom; r++)
			{
				for(c = rect.left + 1; c < rect.right; c++)
				{
				    offset = r * _col + c;
					if(_data[offset] == _fg)
					{

					    // Top
						p2 = _data[offset - _col] == _bg ? 0 : 1;

						// Top - right
						p3 = _data[offset - _col + 1] == _bg ? 0 : 1;

						// Right
						p4 = _data[offset + 1] == _bg ? 0 : 1;

						// Bottom - right
						p5 = _data[offset + _col + 1] == _bg ? 0 : 1;

						// Bottom
						p6 = _data[offset + _col] == _bg ? 0 : 1;

						// Bottom - left
						p7 = _data[offset + _col - 1] == _bg ? 0 : 1;

						// Left
						p8 = _data[offset - 1] == _bg ? 0 : 1;

						// Top - left
						p9 = _data[offset - _col - 1] == _bg ? 0 : 1;

						// Get total number of foreground neighbor pixels
						totalNumOfFgNeighbour =
							p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

						// Count total transition
						transitionCount =
							(p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
							(p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
							(p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
							(p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

						if(tflag == 1){
							condition1 = p2 * p4 * p6;
							condition2 = p4 * p6 * p8;
						}
						else{
							condition1 = p2 * p4 * p8;
							condition2 = p2 * p6 * p8;
						}

						if(transitionCount == 1 &&
						   totalNumOfFgNeighbour >= 2 &&
						   totalNumOfFgNeighbour <= 6 &&
						   condition1 == 0 &&
						   condition2 == 0)
						{
							_data[offset] = flagedValue;
							cflag = 1;
						}
					}
				}
			}

			for(r = rect.top + 1; r < rect.bottom; r++)
			{
				for(c = rect.left + 1; c < rect.right; c++)
				{
				    offset = r * _col + c;
					if(_data[offset] == flagedValue)
					{
						_data[offset] = _bg;
					}
				}
			}
		}
	}

	int findJunctionPoints(Rectangle rect)
	{
		int counter = 0;
		int i, j, index, sum;
		int p1, p2, p3, p4, p5, p6, p7, p8;

		//raster scanning the skeleton to search for redundant pixel
		for(i = rect.top; i <= rect.bottom; i++)
		{
			for(j = rect.left; j <= rect.right; j++)
			{
				index = i * _col + j;

				//if the skeleton pixel is ON
				if(_data[index] == _PFG)
				{
					p1 = _data[index - _col - 1];
					p2 = _data[index - _col];
					p3 = _data[index - _col + 1];

					p4 = _data[index + 1];

					p5 = _data[index + _col + 1];
					p6 = _data[index + _col];
					p7 = _data[index + _col - 1];

					p8 = _data[index - 1];

					/// Checks if the pixel has 120 degree connectivity
					/// with three neighboring pixels one in eight and two opposite
					/// in four directions
					sum = p1 * p3 * p5   + p1 * p3 * p6   + p1 * p3 * p7   + p1 * p4 * p6   + p1 * p4 * p7 + p1 * p5 * p7 +
						  p2 * p4 * p6   + p2 * p4 * p7   + p2 * p4 * p8   + p2 * p5 * p7   + p2 * p5 * p8 + p2 * p6 * p8 +
						  p3 * p5 * p7   + p3 * p5 * p8 /*+ p3 * p5 * p1*/ + p3 * p6 * p8 /*+ p3 * p6 * p1 + p3 * p7 * p1*/ +
						  p4 * p6 * p8 /*+ p4 * p6 * p1   + p4 * p6 * p2   + p4 * p7 * p1   + p4 * p7 * p2 + p4 * p8 * p2*/;
					if(sum > 0) {
						counter ++;
					}
				}
			}
		}

		return counter;
	}

	void deleteRedundantThiningPixels(Rectangle rect)
	{
		int i, j, index, sum, dia;
		int p1, p2, p3, p4, p5, p6, p7, p8;

		//raster scanning the skeleton to search for redundant pixel
		for(i = rect.top; i <= rect.bottom; i++)
		{
			for(j = rect.left; j <= rect.right; j++)
			{
				index = i * _col + j;

				//if the skeleton pixel is ON
				if(_data[index] == 1)
				{
					p1 = _data[index - _col - 1];
					p2 = _data[index - _col];
					p3 = _data[index - _col + 1];

					p4 = _data[index + 1];

					p5 = _data[index + _col + 1];
					p6 = _data[index + _col];
					p7 = _data[index + _col - 1];

					p8 = _data[index - 1];

					/// Checks if the pixel has 120 degree connectivity
					/// with three neighboring pixels one in eight and two opposite in four
					sum = p2 * p4 * p7 +
						  p4 * p6 * p1 +
						  p6 * p8 * p3 +
						  p8 * p2 * p5;

					/// Checks to see if the pixel diagonally connects any two pixels
					dia =  p2 * p4 +
						   p4 * p6 +
						   p6 * p8 +
						   p8 * p2;

					/// 'sum = 0' to see if deleting the pixel would break the connectivity or not
					/// 'dia != 0' to see redundant connection with the given pixel is present or not
					if(dia != 0 && sum == 0) {
						_data[index] = 0;
					}
				}
			}
		}
	}
	
	int drawBoundary(Boundary bound, const int label)
	{
		int rs = Success;
		rs = this->drawLine(bound.topleft.row, bound.topleft.col, bound.topright.row, bound.topright.col, label);
		if(rs != Success) return rs;

		rs = this->drawLine(bound.topright.row, bound.topright.col, bound.bottomright.row, bound.bottomright.col, label);
		if(rs != Success) return rs;

		rs = this->drawLine(bound.bottomright.row, bound.bottomright.col, bound.bottomleft.row, bound.bottomleft.col, label);
		if(rs != Success) return rs;

		rs = this->drawLine(bound.bottomleft.row, bound.bottomleft.col, bound.topleft.row, bound.topleft.col, label);
		if(rs != Success) return rs;

		return rs;
	}

	int drawRectangle(Rectangle rect, const int label, int thickness = 1, int continuity = 0)
	{
		thickness = thickness <= 0 ? 1 : thickness;
		int rs = Success;
		rs = this->drawLine(rect.top, rect.left, rect.top, rect.right, label, thickness, continuity);
		if(rs != Success) return rs;

		rs = this->drawLine(rect.top, rect.right - thickness + 1,rect.bottom, rect.right - thickness + 1, label, thickness, continuity);
		if(rs != Success) return rs;

		rs = this->drawLine(rect.bottom - thickness + 1, rect.right, rect.bottom - thickness + 1, rect.left, label, thickness, continuity);
		if(rs != Success) return rs;

		rs = this->drawLine(rect.bottom, rect.left, rect.top, rect.left, label, thickness, continuity);
		if(rs != Success) return rs;

		return rs;
	}

int fillBoundary(Boundary bound, const int label)
	{
		int rs = Success;
		int r, c, offset, st, en;

		rs = drawBoundary(bound, label);
		if(rs != Success) return rs;

		int top    = MIN(bound.topleft.row, bound.topright.row);
		int bottom = MAX(bound.bottomleft.row, bound.bottomright.row);
		int left   = MIN(bound.topleft.col, bound.bottomleft.col);
		int right  = MAX(bound.topright.col, bound.bottomright.col);

		for(r = top; r <= bottom; r++) {

			/// Initialize start, end marker
			st = right;
			en = left;

			/// Get start point
			for(c = left; c <= right; c++) {
			    offset = r * _col + c;
			    if(_data[offset] == label) {
			    	st = c;
			    	break;
			    }
			}

			/// Get end point
			for(c = right; c >= left; c--) {
			    offset = r * _col + c;
			    if(_data[offset] == label) {
			    	en = c;
			    	break;
			    }
			}

			/// mark data
			for(c = st; c <= en; c++) {
			    offset = r * _col + c;
			    _data[offset] = label;
			}
		}

		return rs;
	}

	int drawLine(const int x1, const int y1, const int x2, const int y2, const int label, int thickness = 1, int continuity = 0)
	{
		continuity = continuity ? MAX(continuity, 3) : continuity;

		int dx, dy, i, p;
		int dxabs, dyabs, sdx, sdy, xValue, yValue, px, py;

		// Check for boundary conditions
		if( x1 < 0 		|| y1 < 0 		||
			x1 >= _row 	|| y1 >= _col 	||
			x2 < 0 		|| y2 < 0 		||
			x2 >= _row 	|| y2 >= _col) {
			return OutOfBoundError;
		}

		// Horizontal line
		if(x1 == x2) {
			for (i = 0; i < thickness; i++) {
				if(y1 < y2) {
					for (p = y1; p <= y2; p++) {
						if(!continuity || (p % continuity && (p + 1) % continuity)) {
							_data[(x1 + i) * _col + p] = label;
						}
					}
				} else {
					for (p = y2; p <= y1; p++) {
						if(!continuity || (p % continuity && (p + 1) % continuity)) {
							_data[(x1 + i) * _col + p] = label;
						}
					}
				}
			}
		}
		// Vertical line
		else if(y1 == y2) {
			for (i = 0; i < thickness; i++) {
				if(x1 < x2) {
					for (p = x1; p <= x2; p++) {
						if(!continuity || (p % continuity && (p + 1) % continuity)) {
							_data[p * _col + (y1 + i)] = label;
						}
					}
				} else {
					for (p = x2; p <= x1; p++) {
						if(!continuity || (p % continuity && (p + 1) % continuity)) {
							_data[p * _col + (y1 + i)] = label;
						}
					}
				}
			}
		}
		/// Line with a slope
		else {
			dx = (x2 - x1);
			dy = (y2 - y1);

			dxabs = ABS(dx);
			dyabs = ABS(dy);

			sdx = dx >= 0 ? 1 : -1;
			sdy = dy >= 0 ? 1 : -1;

			xValue = dyabs >> 1;
			yValue = dxabs >> 1;

			px = x1;
			py = y1;

			_data[px * _col + py] = label;

			if (dxabs >= dyabs) {
				/// Line is more horizontal
				for(i = 0; i < dxabs; i++) {
					yValue += dyabs;

					if (yValue >= dxabs) {
						yValue -= dxabs;
						py += sdy;
					}
					px += sdx;
					for (p = 0; p < thickness; p ++) {
						if(!continuity || (p % continuity && (p + 1) % continuity)) {
							_data[px * _col + (py - p)] = label;
						}
					}
				}
			}
			else {
				/// Line is more vertical
				for (i = 0; i < dyabs; i++){
					xValue += dxabs;
					if (xValue >= dyabs) {
						xValue -= dyabs;
						px += sdx;
					}

					py += sdy;
					for(p = 0; p < thickness; p ++) {
						if(!continuity || (p % continuity && (p + 1) % continuity)) {
							_data[(px - p) * _col + py] = label;
						}
					}
				}
			}
		}
		return Success;
	}

	void sobelEdge(TData <int> &temp1, TData <int> &temp2, TData <int> &out, Rectangle rect)
	{
		/// Apply sobel operator
		sobel_operator_X(out, temp1, rect);
		sobel_operator_Y(out, temp2, rect);

		int stCol = rect.left  + 1;
		int enCol = rect.right - 1;

		int stRow = rect.top    + 1;
		int enRow = rect.bottom - 1;

		/// Get sobel gradient in X and Y direction
		int r, c, rIndex, cIndex, offset, offset1, offset2, val;

		int *sobel_X = temp1.getdata();
		int *sobel_Y = temp2.getdata();
		int *oAddr   = out.getdata();

		for(r = rect.top; r <= rect.bottom; r++)
		{
			offset = r * _col;
			oAddr[offset + rect.left] 	= 0;
			oAddr[offset + rect.right] 	= 0;
		}

		offset1 = rect.top    * _col;
		offset2 = rect.bottom * _col;
		for(c = rect.left; c <= rect.right; c++)
		{
			oAddr[offset1 + c] = 0;
			oAddr[offset2 + c] = 0;
		}


		for(rIndex = stRow; rIndex <= enRow; rIndex++)
		{
			offset1 = rIndex * _col;
			for(cIndex = stCol; cIndex <= enCol; cIndex++)
			{
				offset2 = offset1 + cIndex;
				val = (sobel_X[offset2] + sobel_Y[offset2]) >> 1;
				oAddr[offset2] = MIN(val, _PMAX);
			}
		}
	}

	template <typename R>
	void fill(TData<R> &image,
		const float sfactor = 1.0F, const int pagetype = _Normal,
	    const float tangle = 0.0F, const int shiftr = 0, const int shiftc = 0)
	{
		Rectangle rect;
		rect.top 	= 0;
		rect.bottom = _row - 1;
		rect.left 	= 0;
		rect.right  = _col - 1;

		fill(image, rect, sfactor, pagetype, tangle, shiftr, shiftc);
	}

	template <typename R>
	void fill(TData<R> &image, Rectangle rect,
		const float sfactor = 1.0F, const int pagetype = _Normal,
	    const float tangle = 0.0F, const int shiftr = 0, const int shiftc = 0)
	{
		int _refheight = image.getrows();
		int _refwidth  = image.getcols();

		int r, c, refr, refc, trefr, trefc, offset, index;
		int value1, value2, csx, csy;

		R *inAddr   = image.getdata();

		/// Get center position
		int heightby2 = (_refheight >> 1);
		int widthby2  = (_refwidth  >> 1);

		for(r = rect.top; r <= rect.bottom; r++)
		{
			for(c = rect.left; c <= rect.right; c++)
			{
			    offset = r * _col + c;

				/// Get scaled rectangle
			    trefr = r * sfactor;
			    trefc = c * sfactor;

				/// Adjust shift
			    trefr += (shiftr * sfactor + 0.5F);
			    trefc += (shiftc * sfactor + 0.5F);

				/// Reset orientation
				switch(pagetype) {
				case _Normal:
					/// Set the coordinates as normal
					refr = trefr;
					refc = trefc;
					break;
				case _UpSideDown:
					/// 180 rotation
					refr = _refheight - 1 - trefr;
					refc = _refwidth  - 1  - trefc;
					break;
				case _LeftRotated:
					/// 90 rotation clockwise
					refr = trefc;
					refc = _refwidth - 1 - trefr;
					break;
				case _RightRotated:
					/// 90 rotation anti clockwise
					refr = _refheight - 1 - trefc;
					refc = trefr;
					break;
				default:
					refr = trefr;
					refc = trefc;
				}

				/// Calculate new coordinates
				csx = refr   - heightby2;
				csy = refc  - widthby2;
				value1 = (csx * cos(tangle) - csy * sin(tangle)) + 0.5F;
				value2 = (csx * sin(tangle) + csy * cos(tangle)) + 0.5F;
				value1 += heightby2;
				value2 += widthby2;

				if( value1 >= 0 && value1 < _refheight &&
					value2 >= 0 && value2 < _refwidth) {
					index = value1 * _refwidth + value2;
					_data[offset] = inAddr[index];
				} else {
					_data[offset] = 0;
				}
			}
		}
	}
};

typedef TData < int > 					IntData;
typedef TData < unsigned char > 		UCharData;

#endif /* TDATA_H_ */
