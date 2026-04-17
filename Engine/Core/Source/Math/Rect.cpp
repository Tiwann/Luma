#include "Luma/Math/Vector2.h"
#include "Luma/Math/Rect2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Rect3.h"

template struct Luma::TRect<float, 2>;
template struct Luma::TRect<double, 2>;
template struct Luma::TRect<uint32_t, 2>;
template struct Luma::TRect<int32_t, 2>;

template struct Luma::TRect<float, 3>;
template struct Luma::TRect<double, 3>;
template struct Luma::TRect<uint32_t, 3>;
template struct Luma::TRect<int32_t, 3>;