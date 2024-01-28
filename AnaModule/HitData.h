/*
 * dinupa3@gmail.com
 *
 */
#ifndef _HIT_DATA__H_
#define _HIT_DATA__H_

#include <vector>

struct HitData {
	int detID;
	int eleID_exp;
	int eleID_closest;

	HitData();
	virtual ~HitData() {;}

//	ClassDef(HitData, 1);
};

typedef std::vector<HitData> HitList;

#endif /* _HIT_DATA__H_*/
