#include "Luma/Math/AABB2.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/AABB3.h"
#include "Luma/Math/Vector3.h"

template struct luma::TAABB<float, 2>;
template struct luma::TAABB<double, 2>;
template struct luma::TAABB<uint32_t, 2>;
template struct luma::TAABB<int32_t, 2>;

template struct luma::TAABB<float, 3>;
template struct luma::TAABB<double, 3>;
template struct luma::TAABB<uint32_t, 3>;
template struct luma::TAABB<int32_t, 3>;