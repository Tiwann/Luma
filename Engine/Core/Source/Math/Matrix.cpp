#include "Luma/Math/Vector2.h"
#include "Luma/Math/Matrix2.h"
#include "Luma/Math/Vector3.h"
#include "Luma/Math/Matrix3.h"
#include "Luma/Math/Vector4.h"
#include "Luma/Math/Matrix4.h"

template struct Luma::TMatrix<float, 2, 2>;
template struct Luma::TMatrix<double, 2, 2>;
template struct Luma::TMatrix<uint32_t, 2, 2>;
template struct Luma::TMatrix<int32_t, 2, 2>;

template struct Luma::TMatrix<float, 3, 3>;
template struct Luma::TMatrix<double, 3, 3>;
template struct Luma::TMatrix<uint32_t, 3, 3>;
template struct Luma::TMatrix<int32_t, 3, 3>;

template struct Luma::TMatrix<float, 4, 4>;
template struct Luma::TMatrix<double, 4, 4>;
template struct Luma::TMatrix<uint32_t, 4, 4>;
template struct Luma::TMatrix<int32_t, 4, 4>;