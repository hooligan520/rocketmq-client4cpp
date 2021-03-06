/**
* Copyright (C) 2013 kangliqiang ,kangliq@163.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#if!defined __MESSAGEQUEUELOCK_H__
#define __MESSAGEQUEUELOCK_H__

#include <map>
#include "Mutex.h"
#include "ScopedLock.h"
#include "MessageQueue.h"

/**
* 严格保证单个队列同一时刻只有一个线程消费
* 
*/
class MessageQueueLock
{
public:
	MessageQueueLock()
	{

	}

	~MessageQueueLock()
	{
		std::map<MessageQueue, kpr::Mutex*>::iterator it = m_mqLockTable.begin();

		for (;it!=m_mqLockTable.end();it++)
		{
			delete it->second;
		}
	}

	kpr::Mutex* fetchLockObject(MessageQueue& mq)
	{
		kpr::ScopedLock<kpr::Mutex> lock(m_lock);
		std::map<MessageQueue, kpr::Mutex*>::iterator it = m_mqLockTable.find(mq);
		kpr::Mutex* objLock;
		if (it==m_mqLockTable.end())
		{
			objLock = new kpr::Mutex();
			m_mqLockTable[mq]=objLock;
		}
		else
		{
			objLock = it->second;
		}
		
		return objLock;
	}

private:
	std::map<MessageQueue, kpr::Mutex*> m_mqLockTable;
	kpr::Mutex m_lock;
};

#endif
