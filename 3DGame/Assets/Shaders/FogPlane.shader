Shader "Custom/FogPlane"
{
    Properties
    {
        _Color ("Fog Color", Color) = (1,1,1,0.8)
        _FarFade ("Far Fade", Float) = 2.0
        _EdgeSoftness ("Edge Softness", Float) = 0.15
    }
    SubShader
    {
        Tags { "Queue"="Transparent" "RenderType"="Transparent" }
        Blend SrcAlpha OneMinusSrcAlpha
        ZWrite Off
        Cull Off

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"

            sampler2D _CameraDepthTexture;
            float4 _Color;
            float _FarFade;
            float _EdgeSoftness;

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float4 pos : SV_POSITION;
                float4 screenPos : TEXCOORD0;
                float2 uv : TEXCOORD1;
                float depth : TEXCOORD3;
            };

            v2f vert(appdata v)
            {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.screenPos = ComputeScreenPos(o.pos);
                o.uv = v.uv;

                // Calculamos la profundidad lineal del vértice (distancia a la cámara)
                // En ortográfica, esto es básicamente la distancia Z en espacio de vista
                o.depth = -UnityObjectToViewPos(v.vertex).z; 
                return o;
            }

            fixed4 frag(v2f i) : SV_Target
            {
                float2 screenUV = i.screenPos.xy / i.screenPos.w;
                float rawDepth = tex2D(_CameraDepthTexture, screenUV).r;

                // Profundidad de la escena (el fondo)
                #if defined(UNITY_REVERSED_Z)
                    // En plataformas modernas como DX11, el Z está invertido
                    float sceneDepth = lerp(_ProjectionParams.z, _ProjectionParams.y, rawDepth);
                #else
                    float sceneDepth = lerp(_ProjectionParams.y, _ProjectionParams.z, rawDepth);
                #endif

                // Profundidad del plano (el objeto actual)
                float planeDepth = i.depth;

                float depthDiff = sceneDepth - planeDepth;
                float depthAlpha = saturate(depthDiff / _FarFade);

                // Suavizado de bordes por UV
                float edgeX = min(i.uv.x, 1.0 - i.uv.x);
                float edgeY = min(i.uv.y, 1.0 - i.uv.y);
                float edgeAlpha = saturate(edgeX / _EdgeSoftness) * saturate(edgeY / _EdgeSoftness);

                return fixed4(_Color.rgb, depthAlpha * edgeAlpha * _Color.a);
            }
            ENDCG
        }
    }
}