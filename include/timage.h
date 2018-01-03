/*
 * timage.h
 *
 *  Created on: Apr 15, 2016
 *      Author: root
 */

#ifndef TIMAGE_H_
#define TIMAGE_H_

#include "tdata.h"

template < class T>
class TImage
{
private:
	int _row;
	int _col;
	int _size;
	TData < T > _channelR;
	TData < T > _channelG;
	TData < T > _channelB;

public:
	TImage() {
		_row  = 0;
		_col  = 0;
		_size = 0;
	}

	~TImage() {}

	inline int getrows() {
		return _row;
	}

	inline int getcols() {
		return _col;
	}

	inline int getsize() {
		return _size;
	}

	inline TData < T >& getchannelR() {
		return _channelR;
	}

	inline TData < T >& getchannelG() {
		return _channelG;
	}

	inline TData < T >& getchannelB() {
		return _channelB;
	}

	int allocate(int r, int c) {
		int rs = Success;

		_row  = r;
		_col  = c;
		_size = _row * _col;

		// Allocate and get the data from buffer
		rs = this->_channelR.allocate(r, c);
		if(rs != Success) return rs;

		rs = this->_channelG.allocate(r, c);
		if(rs != Success) return rs;

		rs = this->_channelB.allocate(r, c);
		if(rs != Success) return rs;

		return rs;
	}

	int allocate(int r, int c, T val) {
		int rs = Success;
		_row  = r;
		_col  = c;
		_size = _row * _col;

		// Allocate and get the data from buffer
		rs = this->_channelR.allocate(r, c, val);
		if(rs != Success) return rs;

		rs = this->_channelG.allocate(r, c, val);
		if(rs != Success) return rs;

		rs = this->_channelB.allocate(r, c, val);
		if(rs != Success) return rs;

		return rs;
	}

	int allocate(int r, int c, unsigned char *data) {
		int rs = Success;
		_row  = r;
		_col  = c;
		_size = _row * _col;

		int offset1 = 0;
		int offset2 = r * c;
		int offset3 = 2 * offset2;

		// Allocate and get the data from buffer
		rs = this->_channelR.allocate(r, c, (data + offset1));
		if(rs != Success) return rs;

		rs = this->_channelG.allocate(r, c, (data + offset2));
		if(rs != Success) return rs;

		rs = this->_channelB.allocate(r, c, (data + offset3));
		if(rs != Success) return rs;

		return rs;
	}

	int release() {
		int rs = Success;

		rs = this->_channelR.release();
		if(rs != Success) return rs;

		rs = this->_channelG.release();
		if(rs != Success) return rs;

		rs =this->_channelB.release();
		if(rs != Success) return rs;

		_row  = 0;
		_col  = 0;
		_size = 0;

		return rs;
	}

	int getGray(TData<T> &image) {

		T * data = image.getdata();

		T * dataR = _channelR.getdata();
		T * dataG = _channelG.getdata();
		T * dataB = _channelB.getdata();

		for(int i = 0 ; i < _size; i++){
			data[i] = (dataR[i] + dataG[i] + dataB[i]) / 3;
		}
		return Success;
	}

	template <typename R>
	TImage& operator =(TImage <R> &src) {

		/// If size mismatch do nothing
		if(this->_size != src.getsize()){
			return *this;
		}

		_row    = src.getrows();
		_col    = src.getcols();
		_size   = _row * _col;

		this->_channelR = src.getchannelR();
		this->_channelG = src.getchannelG();
		this->_channelB = src.getchannelB();

		return *this;
	}

	template <typename R>
	TImage& operator =(TData <R> &src) {

		/// If size mismatch do nothing
		if(this->_size != src.getsize()){
			return *this;
		}

		_row    = src.getrows();
		_col    = src.getcols();
		_size   = _row * _col;

		this->_channelR = src;
		this->_channelG = src;
		this->_channelB = src;

		return *this;
	}

	TImage& operator =(T val) {
		this->_channelR = val;
		this->_channelG = val;
		this->_channelB = val;

		return *this;
	}

	template <typename R>
	TImage& operator &=(TImage <R> &src) {

		/// If size mismatch do nothing
		if(this->_size != src.getsize()){
			return *this;
		}

		_row    = src.getrows();
		_col    = src.getcols();
		_size   = _row * _col;

		this->_channelR &= src.getchannelR();
		this->_channelG &= src.getchannelG();
		this->_channelB &= src.getchannelB();

		return *this;
	}

	template <typename R>
	TImage& operator &=(TData <R> &src) {

		/// If size mismatch do nothing
		if(this->_size != src.getsize()){
			return *this;
		}

		_row    = src.getrows();
		_col    = src.getcols();
		_size   = _row * _col;

		this->_channelR &= src;
		this->_channelG &= src;
		this->_channelB &= src;

		return *this;
	}

	TImage& operator &=(T val) {
		this->_channelR &= val;
		this->_channelG &= val;
		this->_channelB &= val;

		return *this;
	}

	template <typename R>
	int mask(TImage<R> &src, Rectangle rect) {
		int rs = Success;

		rs = _channelR.mask(src.getchannelR(), rect);
		if(rs != Success) return rs;

		rs = _channelG.mask(src.getchannelG(), rect);
		if(rs != Success) return rs;

		rs = _channelB.mask(src.getchannelB(), rect);
		if(rs != Success) return rs;

		return rs;
	}

	template <typename R>
	int mask(TData<R> &src, Rectangle rect) {
		int rs = Success;

		rs = _channelR.mask(src, rect);
		if(rs != Success) return rs;

		rs = _channelG.mask(src, rect);
		if(rs != Success) return rs;

		rs = _channelB.mask(src, rect);
		if(rs != Success) return rs;

		return rs;
	}

	template <typename R>
	int mask(R val, Rectangle rect) {
		int rs = Success;

		rs = _channelR.mask(val, rect);
		if(rs != Success) return rs;

		rs = _channelG.mask(val, rect);
		if(rs != Success) return rs;

		rs = _channelB.mask(val, rect);
		if(rs != Success) return rs;

		return rs;
	}

	template <typename R>
	int boundedSubtractFrom(TImage<R> &src, Rectangle rect) {
		int rs = Success;

		rs = _channelR.boundedSubtractFrom(src.getchannelR(), rect);
		if(rs != Success) return rs;

		rs = _channelG.boundedSubtractFrom(src.getchannelG(), rect);
		if(rs != Success) return rs;

		rs = _channelB.boundedSubtractFrom(src.getchannelB(), rect);
		if(rs != Success) return rs;

		return rs;
	}

	template <typename R>
	int boundedSubtractFrom(TData<R> &src, Rectangle rect) {
		int rs = Success;

		rs = _channelR.boundedSubtractFrom(src, rect);
		if(rs != Success) return rs;

		rs = _channelG.boundedSubtractFrom(src, rect);
		if(rs != Success) return rs;

		rs = _channelB.boundedSubtractFrom(src, rect);
		if(rs != Success) return rs;

		return rs;
	}

	template <typename R>
	int boundedSubtractFrom(R val, Rectangle rect) {
		int rs = Success;

		rs = _channelR.boundedSubtractFrom(val, rect);
		if(rs != Success) return rs;

		rs = _channelG.boundedSubtractFrom(val, rect);
		if(rs != Success) return rs;

		rs = _channelB.boundedSubtractFrom(val, rect);
		if(rs != Success) return rs;

		return rs;
	}

	int writePPM(const char *fname) {

		int offset, i, j;
		T * dataR = _channelR.getdata();
		T * dataG = _channelG.getdata();
		T * dataB = _channelB.getdata();

		FILE *fp = fopen(fname, "w+");
		if(!fp) {
			return FileWriteError;
		}

		fprintf(fp, "P3\n");
		fprintf(fp, "%d %d\n", _col, _row);
		fprintf(fp, "255\n");

		for(i = 0 ; i < _row; i++){
			offset = i * _col;
			for(j = 0 ; j < _col; j++){
				fprintf(fp, "%d %d %d ",
						dataR[offset + j] < 0 ? 0 :
						dataR[offset + j] > _PMAX ?
						_PMAX : dataR[offset + j],

						dataG[offset + j] < 0 ? 0 :
						dataG[offset + j] > _PMAX ?
						_PMAX : dataG[offset + j],

						dataB[offset + j] < 0 ? 0 :
						dataB[offset + j] > _PMAX ?
						_PMAX : dataB[offset + j]);
			}
			fprintf(fp, "\n");
		}
		fflush(fp);
		fclose(fp);

		return Success;
	}
};

typedef TImage < int > 					IntImage;
typedef TImage < unsigned char > 		UCharImage;

#endif /* TIMAGE_H_ */
