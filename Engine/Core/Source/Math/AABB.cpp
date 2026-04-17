#include "Luma/Math/AABB2.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/AABB3.h"
#include "Luma/Math/Vector3.h"

template struct Luma::TAABB<float, 2>;
template struct Luma::TAABB<double, 2>;
template struct Luma::TAABB<uint32_t, 2>;
template struct Luma::TAABB<int32_t, 2>;

template struct Luma::TAABB<float, 3>;
template struct Luma::TAABB<double, 3>;
template struct Luma::TAABB<uint32_t, 3>;
template struct Luma::TAABB<int32_t, 3>;