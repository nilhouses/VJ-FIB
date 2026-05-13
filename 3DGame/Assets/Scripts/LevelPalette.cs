using UnityEngine;

[CreateAssetMenu(fileName = "New Level Palette", menuName = "Level Palette")]
public class LevelPalette : ScriptableObject
{
    [Header("Floor Colors")]
    public Color floorColor;

    [Header("Wall Colors")]
    public Color wallColor;

    [Header("Environment Colors")]
    public Color fogColor;
    public Color skyboxColor;
    public Color ambientLightColor;
    public float ambientIntensity = 1f;
}
