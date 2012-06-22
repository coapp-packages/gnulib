#ifndef _GL_COMMON_INC
#define _GL_COMMON_INC

#ifdef _WIN64
typedef signed long long ssize_t;
#else
typedef signed long ssize_t;
#endif

#define _GL_UNUSED

#define inline __inline

#endif