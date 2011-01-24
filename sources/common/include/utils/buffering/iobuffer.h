/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _IOBUFFER_H
#define _IOBUFFER_H

#include "platform/platform.h"
#include "utils/misc/file.h"
#include "utils/misc/mmapfile.h"
#include "utils/misc/crypto.h"

#define GETAVAILABLEBYTESCOUNT(x)     ((x)._published - (x)._consumed)
#define GETIBPOINTER(x)     ((uint8_t *)((x)._pBuffer + (x)._consumed))

/*!
	@class IOBuffer
	@brief Base class which contains all the necessary functions involving input/output buffering.
 */
class DLLEXP IOBuffer {
public:
	uint8_t *_pBuffer;
	uint32_t _size;
	uint32_t _published;
	uint32_t _consumed;
	uint32_t _minChunkSize;
	socklen_t _dummy;
public:
	//constructor/destructor and initialization
	IOBuffer();
	virtual ~IOBuffer();

	/*!
		@brief initializes the buffer
		@param expected - The expected size of this buffer
	 */
	void Initialize(uint32_t expected);

	//Read from a source and put to this buffer
	/*!
		@brief Read from Pipe and saves it.
		@param fd
		@param expected - Expected size of the buffer
		@param recvAmount - Size of data read
	 */
	bool ReadFromPipe(int32_t fd, uint32_t expected, int32_t &recvAmount);

	/*!
		@brief Read from TCP File Descriptor and saves it. This function is advisable for connection-oriented sockets.
		@param fd - Descriptor that contains the data
		@param expected - Expected size of the receiving buffer
		@param recvAmount - Size of data received
	 */
	bool ReadFromTCPFd(int32_t fd, uint32_t expected, int32_t &recvAmount);

	/*!
		@brief Read from UDP File Descriptor and saves it. This function is advisable for connectionless-oriented sockets.
		@param fd - Descriptor that contains the data
		@param recvAmount - Size of data received
		@param peerAddress - The source's address
	 */
	bool ReadFromUDPFd(int32_t fd, int32_t &recvAmount, sockaddr_in &peerAddress);

	/*!
		@brief Read from  Standard IO and saves it.
		@param fd - Descriptor that contains the data
		@param expected - Expected size of the receiving buffer
		@param recvAmount - Size of data received
	 */
	bool ReadFromStdio(int32_t fd, uint32_t expected, int32_t &recvAmount);

	/*!
		@brief Read from File Stream and saves it.
		@param fs - Descriptor that contains the data
		@param size - Size of the receiving buffer
	 */
	bool ReadFromFs(fstream &fs, uint32_t size);

	/*!
		@brief Read from File Stream and saves it
		@param fs - Descriptor that contains the data
		@param size - Size of the receiving buffer
	 */
	bool ReadFromFs(File &file, uint32_t size);
#ifdef HAS_MMAP
	bool ReadFromFs(MmapFile &file, uint32_t size);
#endif /* HAS_MMAP */
	/*!
		@brief Read data from buffer and copy it.
		@param pBuffer - Buffer that contains the data to be read.
		@param size - Size of data to read.
	 */
	bool ReadFromBuffer(const uint8_t *pBuffer, const uint32_t size);

	/*!
		@brief Read data from an input buffer starting at a designated point
		@param pInputBuffer - Pointer to the input buffer
		@param start - The point where copying starts
		@param size - Size of data to read
	 */
	void ReadFromInputBuffer(IOBuffer *pInputBuffer, uint32_t start, uint32_t size);

	/*!
		@brief Read data from an input buffer
		@param buffer - Buffer that contains the data to be read.
		@param size - Size of data to read.
	 */
	bool ReadFromInputBuffer(const IOBuffer &buffer, uint32_t size);

	/*!
		@brief Read data from buffer from a string
		@param binary - The string that will be read
	 */
	bool ReadFromString(string binary);

	/*!
		@brief Read data from buffer byte data
		@param byte
	 */
	void ReadFromByte(uint8_t byte);

	/*!
		@brief Read data from a memory BIO
		@param pBIO
	 */
	bool ReadFromBIO(BIO *pBIO);

	/*!
		@brief
	 */
	void ReadFromRepeat(uint8_t byte, uint32_t size);

	//Read from this buffer and put to a destination
	/*!
		@brief Read data from buffer and store it.
		@param fd
		@param size - Size of buffer
		@param sentAmount - Size of data sent
	 */
	bool WriteToTCPFd(int32_t fd, uint32_t size, int32_t &sentAmount);

	/*!
		@brief Read data from standard IO and store it.
		@param fd
		@param size
	 */
	bool WriteToStdio(int32_t fd, uint32_t size);

	//Utility functions

	/*!
		@brief Returns the minimum chunk size. This value is initialized in the constructor.
	 */
	uint32_t GetMinChunkSize();
	/*!
		@brief Sets the value of the minimum chunk size
		@param minChunkSize
	 */
	void SetMinChunkSize(uint32_t minChunkSize);

	/*!
		@brief Returns the size of the buffer that has already been written on.
	 */
	uint32_t GetCurrentWritePosition();

	/*!
		@brief Returns the pointer to the buffer
	 */
	uint8_t *GetPointer();

	//memory management

	/*!
		@brief Ignores a specified size of data in the buffer
		@param size
	 */
	bool Ignore(uint32_t size);

	/*!
		@brief Ignores all data in the buffer
	 */
	bool IgnoreAll();

	/*!
		@brief Moves the data in a buffer to optimize memory space
	 */
	bool MoveData();

	/*!
		@brief Makes sure that there is enough memory space in the buffer. If space is not enough, a new buffer is created.
		@param expected - This function makes sure that the size indicated by this parameter is accommodated.
		@discussion In the case that a new buffer is created and the current buffer has data, the data is copied to the new buffer.
	 */
	bool EnsureSize(uint32_t expected);

	/*!
		@brief
	 */

	static string DumpBuffer(uint8_t *pBuffer, uint32_t length);
	string ToString(uint32_t startIndex = 0, uint32_t limit = 0);
	operator string();

protected:
	void Cleanup();
	void Recycle();
};
#endif //_IOBUFFER_H

