#pragma once

#define LUMA_SQR(x) (x * x)
#define LUMA_CUBE(x) (LUMA_SQR(x) * x)
#define LUMA_QUART(x) (LUMA_CUBE(x) * x)
#define LUMA_QUINT(x) (LUMA_QUART(x) * x)