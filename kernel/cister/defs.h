#ifndef _CISTER_DEFS_H__
#define _CISTER_DEFS_H__

#define CONSOLE_DEBUGGING


#ifdef CONSOLE_DEBUGGING
#include <linux/kernel.h>

#define PRINT_DEBUG_MESSAGE(msg) { printk(KERN_DEBUG "%s|||%s:%d called: " msg, __FILE__, __FUNCTION__, __LINE__); }

#define PRINT_DEBUG_MESSAGE_WITH_ARGS(msg, ...) { printk(KERN_DEBUG "%s|||%s:%d called: " msg, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); }


#else

#define PRINT_DEBUG_MESSAGE(msg) {}
#define PRINT_DEBUG_MESSAGE_WITH_ARGS(msg, ...) {}

#endif


















#endif