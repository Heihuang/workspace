/*
 * Collector.h
 *
 *  Created on: Jun 16, 2019
 *      Author: zhou
 */

#ifndef SRC_COLLECTOR_H_
#define SRC_COLLECTOR_H_

#include "Sender.h"

namespace lxh{
class Collector{
public:
	Collector();
	virtual ~Collector();
	virtual void Collect() = 0;
	void Ready();
	void Down();
protected:
	Sender* sender_;
};
}




#endif /* SRC_COLLECTOR_H_ */
