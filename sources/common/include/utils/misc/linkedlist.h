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


#ifndef _LINKEDLIST_H
#define	_LINKEDLIST_H

#include <stdlib.h>

template<typename T>
struct LinkedListNode {
	LinkedListNode<T> *pPrev;
	LinkedListNode<T> *pNext;
	T info;
};

template<typename T>
LinkedListNode<T> *AddLinkedList(LinkedListNode<T> *pTo, T info, bool after) {
	LinkedListNode<T> *pResult = new LinkedListNode<T>;
	pResult->pNext = NULL;
	pResult->pPrev = NULL;
	pResult->info = info;

	if (pTo != NULL) {
		if (after) {
			pTo->pNext = pResult;
			pResult->pPrev = pTo;
		} else {
			pResult->pNext = pTo;
			pTo->pPrev = pResult;
		}
	}

	return pResult;
}

template<typename T>
LinkedListNode<T> *LastLinkedList(LinkedListNode<T> *pNode) {
	while (pNode != NULL) {
		if (pNode->pNext == NULL)
			return pNode;
		pNode = pNode->pNext;
	}
	return NULL;
}

template<typename T>
LinkedListNode<T> *RemoveLinkedList(LinkedListNode<T> *pNode) {
	LinkedListNode<T> *pPrev = pNode->pPrev;
	LinkedListNode<T> *pNext = pNode->pNext;
	if (pPrev != NULL)
		pPrev->pNext = pNext;
	if (pNext != NULL)
		pNext->pPrev = pPrev;
	delete pNode;
	if (pPrev != NULL)
		return LastLinkedList<T > (pPrev);
	return LastLinkedList<T > (pNext);
}


#endif	/* _LINKEDLIST_H */


