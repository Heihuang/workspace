/*
 * CltRedisInfo.h
 *
 *  Created on: Jun 16, 2019
 *      Author: zhou
 */

#ifndef SRC_CLTREDISINFO_H_
#define SRC_CLTREDISINFO_H_
#include "Collector.h"

namespace lxh{
class CltRedisInfo : public Collector{
public:
	CltRedisInfo();
	virtual ~CltRedisInfo();
	virtual void Collect();
};

}



#endif /* SRC_CLTREDISINFO_H_ */
