/*
 * CHashProcessor.h
 *
 *  Created on: 2015-12-6
 *      Author: root
 */

#ifndef CHASHPROCESSOR_H_
#define CHASHPROCESSOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HNode
{
	int data;
	struct HNode *pNext;
}HashNode;

// 比较器
struct HashCompare
{
	bool operator () (const HashNode &key1, const HashNode &key2)
	{
		if (key1.data == key2.data)
		{
			return true;
		}
		return false;
	}
};

// 获取hash索引
class HaskKey
{
public:
	HaskKey()
	{
		m_HashHeight = 200;
	}

	uint operator()(HashNode &pNode)
	{
		int hashkey = pNode.data % m_HashHeight;
		return hashkey;
	}
private:
	uint m_HashHeight;
};

template <typename Hash_key, typename Hash_type, typename Compare>
class CHashProcessor
{
public:
	typedef Hash_key hashkey;
	typedef Hash_type hashtype;
	typedef Compare compare;

public:
	CHashProcessor(uint hashHeight, uint arrayLen)
	{
		m_HashHeight = hashHeight;
		m_ArrayLen = arrayLen;
		m_pHash = NULL;
		m_pArray = NULL;
		m_pFreeHeader = NULL;
	}

	bool Create()
	{
		m_pArray = new hashtype[m_ArrayLen];
		memset(m_pArray, 0x00, sizeof(hashtype) * m_ArrayLen);
		m_pHash = new hashtype*[m_HashHeight];
		memset(m_pHash, 0x00, sizeof(hashtype*) * m_HashHeight);

		m_pFreeHeader = &m_pArray[0];
		for (uint i = 1; i < m_ArrayLen; ++i)
		{
			m_pArray[i-1].pNext = &m_pArray[i];
		}
//		hashtype *p = m_pFreeHeader;
//		while (p)
//		{
//			printf("p[%p], p->pNext=[%p]\n", p, p->pNext);
//			p = p->pNext;
//		}
		return true;
	}

	bool Find(const hashtype &key, hashtype &pNode, bool &isFind)
	{
		uint hash = GetHashKey(key);
		hashtype *pTemp = m_pHash[hash];
		if (!pTemp)
		{
			pTemp = GetFreeNode();
			if (!pTemp)
			{
				printf("error pTemp = NULL");
				return isFind;
			}
			*pNode = pTemp;
			isFind = true;
			return isFind;
		}

		while (pTemp)
		{
			if (m_compare(key, pTemp))
			{
				isFind = true;
				*pNode = pTemp;
				break;
			}
			pTemp = pTemp->pNext;
		}

		// 没找到，新增节点追加到最后
		if (!isFind)
		{
			hashtype* pNew = GetFreeNode();
			pTemp->pNext = pNew;
			pNew->pNext = NULL;
			isFind = true;
		}
		return isFind;
	}

	uint GetHashKey(const hashtype &key)
	{
		uint hash = m_hashkey(key);
		if (hash > m_HashHeight)
		{
			hash = hash % m_HashHeight;
		}
		return hash;
	}

	hashtype* GetFreeNode()
	{
		hashtype *pNode = m_pFreeHeader;
		m_pFreeHeader = m_pFreeHeader->pNext;
		return pNode;
	}

	bool DeleteNode(hashtype* pNode)
	{
		return true;
	}

	bool AddFreeNode(hashtype* pNode)
	{
		if (!pNode)
		{
			return false;
		}
		memset(pNode, 0x00, sizeof(hashtype));
		pNode->pNext = m_pFreeHeader;
		m_pFreeHeader = pNode;
		return true;
	}

private:
	hashkey m_hashkey;
	compare m_compare;
	uint m_HashHeight;
	uint m_ArrayLen;
	Hash_type **m_pHash;
	Hash_type *m_pArray;
	Hash_type *m_pFreeHeader;
};

typedef CHashProcessor<HaskKey, HashNode, HashCompare> CHashMap;

#endif /* CHASHPROCESSOR_H_ */
