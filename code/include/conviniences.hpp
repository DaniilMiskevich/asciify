#ifndef CONVINIENCES_HPP
#define CONVINIENCES_HPP

#define let    auto const
#define letmut auto

#define elsizeof(arr) (size_t(sizeof(arr[0])))
#define lenof(arr)    (size_t(sizeof(arr) / elsizeof(arr)))

#endif
