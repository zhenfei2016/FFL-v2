#include "TimestampUtils.hpp"

namespace player {	
	//
	//  ʱ�����λת��usֵ
	//
	int64_t timestampToUs(int64_t timestamp, const FFL::TimeBase& tb) {
		if (tb.mDen == 0) {
			FFL_ASSERT_LOG(0, "timestampToUs tb.den=0");
			return timestamp;
		}

		//av_rescale_q()
		//
		//  ʱ���һ���̶ȶ�Ӧ���ٺ���
		//		
		double us = ((double)(1000 * 1000 * tb.mNum)) / tb.mDen;
		return (int64_t)(timestamp* us);
	}
	//
	//  ʱ�����λת��usֵ
	//
	int64_t timestampToUs(int64_t timestamp, const FFL::TimeBase& tb, FFL::Clock* clock) {
		if (tb.mDen == 0) {
			FFL_ASSERT_LOG(0, "timestampToUs tb.den=0");
			return timestamp;
		}

		//av_rescale_q()
		//
		//  ʱ���һ���̶ȶ�Ӧ���ٺ���
		//		
		double us = ((double)(1000 * 1000 * tb.mNum)) / tb.mDen;
		return (int64_t)(timestamp* us);
	}
	//
	//  usתʱ�����λ
	//
	int64_t usToTimestamp(int64_t us, const FFL::TimeBase& tb) {
		if (tb.mDen == 0|| tb.mNum==0) {
			FFL_ASSERT_LOG(0, "usToTimestamp tb.den=0");
			return us;
		}

		//
		//  ʱ���һ���̶ȶ�Ӧ���ٺ���
		//		
		double units = ((double)(1000 * 1000 * tb.mNum)) / tb.mDen;
		return (int64_t)(us/units);
	}
}