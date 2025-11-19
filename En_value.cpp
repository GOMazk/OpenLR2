#include "En_value.h"

//401ec0
int LoopInRange(int min, int max, int add, int *num){
	*num = *num + add;
	if (max < *num) {
		*num = min;
	}
	else if (*num < min) {
		*num = max;
	}
	return 1;
}

//for convenience (DSTDbyTime())
double ByTime(double v1, double v2, double t1, double t2, double tO) {
	double ratio;

	if (v2 == v1){
		return v1;
	}
	else if (t1 <= tO && tO <= t2 && t1 < t2) {
		ratio = (tO - t1) / (t2 - t1);
		return (1 - ratio) * v1 + ratio * v2;
	}
	else {
		return (tO <= t1)? v1 : v2;
	}
}

//49a790
double ChangeValueByTime(double val1, double val2, double time1, double time2, double timenow, int type){
	double ratio;

	if(val1 == val2){
		return val1;
	}
	else if (timenow <= time2 && time1 <= timenow && time1 < time2){
		if (type == 0) {
			ratio = (timenow - time1) / (time2 - time1);
			return (1.0 - ratio) * val1 + ratio * val2;
		}
		if (type == 1) {
			ratio = (timenow - time1) / (time2 - time1);
			ratio = ratio * ratio * ratio;
			return (1.0 - ratio) * val1 + ratio * val2;
		}
		if (type == 2) {
			ratio = 1.0 - (timenow - time1) / (time2 - time1);
			ratio = 1.0 - ratio * ratio * ratio;
			return (1.0 - ratio) * val1 + ratio * val2;
		}
		return val1;
	}
	else{
		return (time1 < timenow)? val2 : val1;
	}
}