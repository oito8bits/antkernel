#ifndef _INCLUDE_ALIGN_
#define _INCLUDE_ALIGN_

#define ALIGNUP(value, align) ((value) + ((align) - 1) & ~((align) - 1))

#endif
