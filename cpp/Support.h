#ifndef SUPPORT_H
#define SUPPORT_H

#define LIKELY(expr)    __builtin_expect(!!(expr), true)
#define UNLIKELY(expr)  __builtin_expect(!!(expr), false)

#endif // SUPPORT_H
