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

#ifdef USE_MEM_POOL

#include "mempool/mempool.h"

MemoryPoolManager &MemoryPoolManager::GetInstance() {
	static MemoryPoolManager oInstance;
	return oInstance;
}

MemoryPoolManager::~MemoryPoolManager() {
	info();
	Cleanup();
	info();
	Shutdown();
	info();
}

void MemoryPoolManager::Register(MemoryPool* pMemoryPool) {
	_memoryPools[pMemoryPool->GetSize()] = pMemoryPool;
}

void MemoryPoolManager::UnRegister(MemoryPool* pMemoryPool) {
	_memoryPools.erase(pMemoryPool->GetSize());
}

void MemoryPoolManager::Cleanup() {
	for (map<size_t, MemoryPool*>::iterator i = _memoryPools.begin();
			i != _memoryPools.end(); i++) {
		(i->second)->Cleanup();
	}
}

void MemoryPoolManager::info() {
	printf("---------------\n");
	for (map<size_t, MemoryPool*>::iterator i = _memoryPools.begin();
			i != _memoryPools.end(); i++) {
		(i->second)->info();
	}
	printf("---------------\n");
}

MemoryPoolManager::MemoryPoolManager() {

}

void MemoryPoolManager::Shutdown() {
	while (_memoryPools.size() > 0) {
		delete _memoryPools.begin()->second;
	}
}

MemoryPool::MemoryPool(size_t size) {
	_pEntries = NULL;
	_created = 0;
	_used = 0;
	_size = size >= sizeof (MemPoolEntry) ? size : sizeof (MemPoolEntry);
	MemoryPoolManager::GetInstance().Register(this);
}

MemoryPool::~MemoryPool() {
	Cleanup();
	assert(_used == 0);
	MemoryPoolManager::GetInstance().UnRegister(this);
}

void * MemoryPool::Allocate() {
	_used++;
	void* pResult = _pEntries;
	if (pResult == NULL) {
		_created++;
		return ::operator new(_size, std::nothrow);
	} else {
		_pEntries = _pEntries->pNext;
		return pResult;
	}
}

void MemoryPool::Deallocate(void *p) {
	_used--;
	assert(p != NULL);
	MemPoolEntry* pEntry = (MemPoolEntry *) (p);
	pEntry->pNext = _pEntries;
	_pEntries = pEntry;
}

void MemoryPool::Cleanup() {
	MemPoolEntry *pCurrent = _pEntries;
	while (pCurrent != NULL) {
		_pEntries = _pEntries->pNext;
		::operator delete((void *) pCurrent);
		pCurrent = _pEntries;
		_created--;
	}
}

size_t MemoryPool::GetSize() {
	return _size;
}

void MemoryPool::info() {
	LOG_STATS('I');
}

#endif /* USE_MEM_POOL */


