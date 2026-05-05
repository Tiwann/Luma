module Scene;
import LightTypes;

public struct SceneData
{
	public DirectionalLight directionalLight;
	public AmbientLight ambientLight;
}

#ifdef DECLARE_SCENE_BUFFER
[vk::binding(1, 2)] public ConstantBuffer<SceneData> u_SceneData : register(b1, space2);
#endif
