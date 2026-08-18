#include "Luma/Math/Vector.h"
#include "Luma/Math/Vector2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Vector4.h"

template struct Luma::TVector<float, 2>;
template struct Luma::TVector<float, 3>;
template struct Luma::TVector<float, 4>;

template struct Luma::TVector<double, 2>;
template struct Luma::TVector<double, 3>;
template struct Luma::TVector<double, 4>;

template struct Luma::TVector<uint32_t, 2>;
template struct Luma::TVector<uint32_t, 3>;
template struct Luma::TVector<uint32_t, 4>;

template struct Luma::TVector<int32_t, 2>;
template struct Luma::TVector<int32_t, 3>;
template struct Luma::TVector<int32_t, 4>;