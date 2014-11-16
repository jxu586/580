#ifndef _UTILITY_H_
#define _UTILITY_H_

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(ptr) \
	do { \
		if ((ptr) != NULL) {\
			delete[] (ptr);\
			(ptr) = NULL;\
		} \
	} while (0)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) \
	do { \
		if ((ptr) != NULL) {\
			delete (ptr);\
			(ptr) = NULL;\
		} \
	} while (0)
#endif

#ifndef SAFE_DO
#define SAFE_DO(ptr, proc)\
	do {\
		if (ptr) proc; \
	} while(0)
#endif

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

#endif
