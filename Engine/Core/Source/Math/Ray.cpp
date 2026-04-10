#include "Luma/Math/Ray.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"

template struct luma::TRay<float, 2>;
template struct luma::TRay<double, 2>;
template struct luma::TRay<uint32_t, 2>;
template struct luma::TRay<int32_t, 2>;

template struct luma::TRay<float, 3>;
template struct luma::TRay<double, 3>;
template struct luma::TRay<uint32_t, 3>;
template struct luma::TRay<int32_t, 3>;