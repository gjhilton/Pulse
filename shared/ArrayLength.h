#if !defined (ARRAY_SIZE)
	#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))
#endif