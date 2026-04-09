#include "Luma/Math/Vector2.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Rect3.h"

template struct luma::TRect<float, 2>;
template struct luma::TRect<double, 2>;
template struct luma::TRect<uint32_t, 2>;
template struct luma::TRect<int32_t, 2>;

template struct luma::TRect<float, 3>;
template struct luma::TRect<double, 3>;
template struct luma::TRect<uint32_t, 3>;
template struct luma::TRect<int32_t, 3>;