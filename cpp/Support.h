#ifndef SUPPORT_H
#define SUPPORT_H


#define DEBUG_VIEW false /// makes window smaller
#define DEBUG_MODE false /// add some comments
#define MORE_DEBUG false /// add even more comments

#define SAFE_MODE true /// if true then functions might check if argument is correct

#define LIKELY(expr)    __builtin_expect(!!(expr), true)
#define UNLIKELY(expr)  __builtin_expect(!!(expr), false)


#endif // SUPPORT_H
