module Screen;

public struct ScreenData
{
    public float2 fullResolution;
    public float2 halfResolution;
};

#ifdef DECLARE_SCREEN_BUFFER
[vk::binding(2, 2)] public ConstantBuffer<ScreenData> u_ScreenData : register(b2, space2);
#endif