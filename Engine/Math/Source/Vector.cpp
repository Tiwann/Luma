#include "Luma/Math/Vector.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Vector4.h"

template struct luma::TVector<float, 2>;
template struct luma::TVector<float, 3>;
template struct luma::TVector<float, 4>;

template struct luma::TVector<double, 2>;
template struct luma::TVector<double, 3>;
template struct luma::TVector<double, 4>;

template struct luma::TVector<uint32_t, 2>;
template struct luma::TVector<uint32_t, 3>;
template struct luma::TVector<uint32_t, 4>;

template struct luma::TVector<int32_t, 2>;
template struct luma::TVector<int32_t, 3>;
template struct luma::TVector<int32_t, 4>;