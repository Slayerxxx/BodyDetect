#ifndef _RABBIT_DEF_H_
#define _RABBIT_DEF_H_


// 用于将数字转换为字符串的形式输出
#define NUM2STR(x) static_cast< std::ostringstream &>(\
           (std::ostringstream() << std::dec << x )).str()\


// 用于循环处理HSV图片，用以找到特定的某种颜色
#define HSV_LOOP(low_hue, high_hue, low_satur, high_satur, low_value, high_value, result){  \
	for (int r = 0; r < num_row; r++) {														\
		const uchar* curr_r_hue = channels[0].ptr<const uchar>(r);							\
		const uchar* curr_r_satur = channels[1].ptr<const uchar>(r);						\
		const uchar* curr_r_value = channels[2].ptr<const uchar>(r);						\
		uchar* curr_r_result = result.ptr<uchar>(r);										\
		for (int c = 0; c < num_col; c++) {													\
			if ((curr_r_hue[c] <= high_hue && curr_r_hue[c] >= low_hue) &&					\
				(curr_r_satur[c] <= high_satur && curr_r_satur[c] >= low_satur) &&			\
				(curr_r_value[c] <= high_value && curr_r_value[c] >= low_value)				\
				) {																			\
				curr_r_result[c] = 255;														\
			}																				\
			else {																			\
				curr_r_result[c] = 0;														\
			}																				\
		}																					\
	}																						\
}																							\

	enum remove_region_type {
		UNCHECKED = 0,
		CHECKED = 1,
		UNQUALIFIED = 2,
		QUALIFIED = 3
	};
	enum animal_category {
		RABBIT = 0,
		RAT = 1
		// ....
	};


#endif
