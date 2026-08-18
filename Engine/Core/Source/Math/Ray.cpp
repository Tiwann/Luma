#include "Luma/Math/Ray.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"

template struct Luma::TRay<float, 2>;
template struct Luma::TRay<double, 2>;
template struct Luma::TRay<uint32_t, 2>;
template struct Luma::TRay<int32_t, 2>;

template struct Luma::TRay<float, 3>;
template struct Luma::TRay<double, 3>;
template struct Luma::TRay<uint32_t, 3>;
template struct Luma::TRay<int32_t, 3>;